using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace ReceiveFile
{
	class Program
	{
		static int Main(string[] args)
		{
			if ((args.Length != 4) || (0 == int.Parse(args[1])))
			{
				Console.WriteLine("Usage: ReceiveFile server_ip server_port remote_filename local_filename");
				return 1;
			}

			try
			{
				using (Udt.Socket client = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
				{
					client.Connect(IPAddress.Parse(args[0]), int.Parse(args[1]));

					// Send name information of the requested file
					string name = args[2];
					byte[] nameBytes = Encoding.UTF8.GetBytes(name);

					client.Send(BitConverter.GetBytes(nameBytes.Length), 0, sizeof(int));
					client.Send(nameBytes);

					// Get size information
					long size;
					byte[] file = new byte[1024];

					client.Receive(file, 0, sizeof(long));
					size = BitConverter.ToInt64(file, 0);

					// Receive the file
					string localName = args[3];
					client.ReceiveFile(localName, size);

					client.Close();
				}

				Console.ReadKey(true);
				return 0;
			}
			catch (Exception ex)
			{
				Console.Error.WriteLine("Error receiving file: {0}", ex.Message);
				Console.ReadKey(true);
				return 2;
			}
		}
	}
}
