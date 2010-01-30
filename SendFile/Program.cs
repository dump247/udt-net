using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.IO;

namespace SendFile
{
	class Program
	{
		static int Main(string[] args)
		{
			if ((1 < args.Length) || ((1 == args.Length) && (0 == int.Parse(args[0]))))
			{
				Console.WriteLine("Usage: SendFile [ServerPort]");
				return 1;
			}

			try
			{
				using (Udt.Socket server = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
				{
					int port = 9000;
					if (1 == args.Length)
						port = int.Parse(args[0]);

					server.Bind(IPAddress.Any, port);
					Console.WriteLine("Server is ready at port: {0}", port);
					server.Listen(1);

					using (Udt.Socket client = server.Accept())
					{
						server.Close();

						// Receive file name from client
						byte[] file = new byte[1024];
						int length;
						string name;

						client.Receive(file, 0, sizeof(int));
						length = BitConverter.ToInt32(file, 0);

						client.Receive(file, 0, length);
						name = Encoding.UTF8.GetString(file, 0, length);

						// Send file size information
						client.Send(BitConverter.GetBytes(new FileInfo(name).Length), 0, sizeof(long));

						Udt.TraceInfo trace = client.GetPerformanceInfo();

						// Send the file
						client.SendFile(name);

						trace = client.GetPerformanceInfo();
						Console.WriteLine("Speed = {0}Mbits/sec", trace.SendMbps);

						client.Close();
					}
				}

				Console.ReadKey(true);
				return 0;
			}
			catch (Exception ex)
			{
				Console.Error.WriteLine("Error sending file: {0}", ex.Message);
				Console.ReadKey(true);
				return 2;
			}
		}
	}
}
