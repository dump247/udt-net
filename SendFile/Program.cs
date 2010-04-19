using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Reflection;
using System.Threading;

namespace SendFile
{
	class Program
	{
		static int Main(string[] args)
		{
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Loopback, 10000);
            ManualResetEvent serverEvent = new ManualResetEvent(false);

            ThreadPool.QueueUserWorkItem((object state) =>
            {
                using (Udt.Socket server = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
                {
                    server.Bind(IPAddress.Loopback, 0);
                    serverEndPoint = server.LocalEndPoint;
                    server.Listen(1);

                    serverEvent.Set();

                    using (Udt.Socket serverClient = server.Accept())
                    {
                        serverClient.Receive(new byte[100]);
                    }
                }
            });

            serverEvent.WaitOne();

            using (Udt.Socket client = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                client.Connect(serverEndPoint);
                client.Send(new byte[100]);

                Udt.TraceInfo perf = client.GetPerformanceInfo();
            }

            return 0;

            //if ((1 < args.Length) || ((1 == args.Length) && (0 == int.Parse(args[0]))))
            //{
            //    Console.WriteLine("Usage: SendFile [ServerPort]");
            //    return 1;
            //}

            //try
            //{
            //    using (Udt.Socket server = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            //    {
            //        int port = 9000;
            //        if (1 == args.Length)
            //            port = int.Parse(args[0]);

            //        server.Bind(IPAddress.Any, port);
            //        Console.WriteLine("Server is ready at port: {0}", port);
            //        server.Listen(1);

            //        using (Udt.Socket client = server.Accept())
            //        {
            //            server.Close();

            //            // Receive file name from client
            //            byte[] file = new byte[1024];
            //            int length;
            //            string name;

            //            client.Receive(file, 0, sizeof(int));
            //            length = BitConverter.ToInt32(file, 0);

            //            client.Receive(file, 0, length);
            //            name = Encoding.UTF8.GetString(file, 0, length);

            //            // Send file size information
            //            client.Send(BitConverter.GetBytes(new FileInfo(name).Length), 0, sizeof(long));

            //            Udt.TraceInfo trace = client.GetPerformanceInfo();

            //            // Send the file
            //            client.SendFile(name);

            //            trace = client.GetPerformanceInfo();

            //            PrintProps("Total", trace.Total);
            //            PrintProps("Local", trace.Local);
            //            PrintProps("Probe", trace.Probe);

            //            client.Close();
            //        }
            //    }

            //    Console.ReadKey(true);
            //    return 0;
            //}
            //catch (Exception ex)
            //{
            //    Console.Error.WriteLine("Error sending file: {0}", ex.Message);
            //    Console.ReadKey(true);
            //    return 2;
            //}
		}

        static void PrintProps(string name, object obj)
        {
            Console.WriteLine("---- {0} ----", name);
            foreach (PropertyInfo prop in obj.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance))
            {
                Console.WriteLine("{0} = {1}", prop.Name, prop.GetValue(obj, null));
            }
        }
	}
}
