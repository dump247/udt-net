using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Threading.Tasks;

using NUnit.Framework;
using System.IO;
using Moq;

namespace UdtProtocol_Test
{
    /// <summary>
    /// Test fixture for <see cref="Udt.Socket"/>.
    /// </summary>
    [TestFixture]
    public class SocketTest
    {
        /// <summary>
        /// Test for <see cref="Udt.Socket(AddressFamily,SocketType)"/> constructor.
        /// </summary>
        [Test]
        public void Constructor()
        {
            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
            Assert.AreEqual(AddressFamily.InterNetwork, socket.AddressFamily);
            Assert.IsTrue(socket.BlockingReceive);
            Assert.IsTrue(socket.BlockingSend);
            Assert.IsNull(socket.CongestionControl);
            LingerOption opt = socket.LingerState;
            Assert.IsTrue(opt.Enabled);
            Assert.AreEqual(180, opt.LingerTime);
            Assert.Throws<Udt.SocketException>(() => { IPEndPoint ep = socket.LocalEndPoint; });
            Assert.AreEqual(-1, socket.MaxBandwidth);
            Assert.AreEqual(1052, socket.MaxPacketSize);
            Assert.AreEqual(25600, socket.MaxWindowSize);
            Assert.AreEqual(8388608, socket.ReceiveBufferSize);
            Assert.AreEqual(-1, socket.ReceiveTimeout);
            Assert.Throws<Udt.SocketException>(() => { IPEndPoint ep = socket.RemoteEndPoint; });
            Assert.IsFalse(socket.Rendezvous);
            Assert.IsTrue(socket.ReuseAddress);
            Assert.AreEqual(8388608, socket.SendBufferSize);
            Assert.AreEqual(-1, socket.SendTimeout);
            Assert.AreEqual(SocketType.Stream, socket.SocketType);
            Assert.AreEqual(12288000, socket.UdpReceiveBufferSize);
            Assert.AreEqual(65536, socket.UdpSendBufferSize);
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket(AddressFamily,SocketType)"/> constructor
        /// with all the valid argument combinations.
        /// </summary>
        [Test]
        public void Constructor__ValidArgs()
        {
            // InterNetwork
            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Dgram);
            Assert.AreEqual(AddressFamily.InterNetwork, socket.AddressFamily);
            Assert.AreEqual(SocketType.Dgram, socket.SocketType);

            socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
            Assert.AreEqual(AddressFamily.InterNetwork, socket.AddressFamily);
            Assert.AreEqual(SocketType.Stream, socket.SocketType);

            // InterNetworkV6
            socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Dgram);
            Assert.AreEqual(AddressFamily.InterNetworkV6, socket.AddressFamily);
            Assert.AreEqual(SocketType.Dgram, socket.SocketType);

            socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Stream);
            Assert.AreEqual(AddressFamily.InterNetworkV6, socket.AddressFamily);
            Assert.AreEqual(SocketType.Stream, socket.SocketType);
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket(AddressFamily,SocketType)"/> constructor
        /// with invalid argument values.
        /// </summary>
        [Test]
        public void Constructor__InvalidArgs()
        {
            // family
            ArgumentException ex = Assert.Throws<ArgumentException>(() => new Udt.Socket(AddressFamily.ImpLink, SocketType.Stream));
            Assert.AreEqual("family", ex.ParamName);

            ex = Assert.Throws<ArgumentException>(() => new Udt.Socket(AddressFamily.Ipx, SocketType.Stream));
            Assert.AreEqual("family", ex.ParamName);

            // type
            ex = Assert.Throws<ArgumentException>(() => new Udt.Socket(AddressFamily.InterNetwork, SocketType.Raw));
            Assert.AreEqual("type", ex.ParamName);

            ex = Assert.Throws<ArgumentException>(() => new Udt.Socket(AddressFamily.InterNetwork, SocketType.Rdm));
            Assert.AreEqual("type", ex.ParamName);

            ex = Assert.Throws<ArgumentException>(() => new Udt.Socket(AddressFamily.InterNetwork, SocketType.Seqpacket));
            Assert.AreEqual("type", ex.ParamName);

            ex = Assert.Throws<ArgumentException>(() => new Udt.Socket(AddressFamily.InterNetwork, SocketType.Unknown));
            Assert.AreEqual("type", ex.ParamName);
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Bind(IPAddress,int)"/>.
        /// </summary>
        [Test]
        public void Bind_IPAddress_int()
        {
			int port = _portNum++;

            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
				socket.Bind(IPAddress.Any, port);

                IPEndPoint localEP = socket.LocalEndPoint;
                Assert.AreEqual(IPAddress.Any, localEP.Address);
				Assert.AreEqual(port, localEP.Port);
            }
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Bind(IPAddress,int)"/> with IPv6 address.
        /// </summary>
        [Test]
        public void Bind_IPAddress_int__IPV6()
        {
            if (!Socket.OSSupportsIPv6)
            {
                Assert.Ignore("OS does not support IPv6");
            }

			int port = _portNum++;

            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Stream))
            {
				socket.Bind(IPAddress.IPv6Any, port);

                IPEndPoint localEP = socket.LocalEndPoint;
                Assert.AreEqual(IPAddress.IPv6Any, localEP.Address);
				Assert.AreEqual(port, localEP.Port);
            }
        }

        /// <summary>
        /// Test for multiple calls to <see cref="Udt.Socket.Bind(IPAddress,int)"/>.
        /// </summary>
        [Test]
        public void Bind_IPAddress_int__BindTwice()
		{
			int port = _portNum++;

            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
				socket.Bind(IPAddress.Any, port);

                Udt.SocketException error = Assert.Throws<Udt.SocketException>(() =>
                {
					socket.Bind(IPAddress.Any, port);
                });

                Assert.AreEqual(Udt.SocketError.InvalidOperation, error.SocketErrorCode);
            }

			port = _portNum++;

            if (Socket.OSSupportsIPv6)
            {
                using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Stream))
                {
					socket.Bind(IPAddress.IPv6Any, port);

                    Udt.SocketException error = Assert.Throws<Udt.SocketException>(() =>
                    {
						socket.Bind(IPAddress.IPv6Any, port);
                    });

                    Assert.AreEqual(Udt.SocketError.InvalidOperation, error.SocketErrorCode);
                }
            }
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Bind(IPAddress,int)"/> after the
        /// socket has been closed.
        /// </summary>
        [Test]
        public void Bind_IPAddress_int__AfterClosed()
		{
			int port = _portNum++;

            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
            socket.Dispose();

            Assert.Throws<ObjectDisposedException>(() =>
            {
				socket.Bind(IPAddress.Any, port);
            });
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Bind(IPAddress,int)"/> with invalid
        /// argument values.
        /// </summary>
        [Test]
        public void Bind_IPAddress_int__InvalidArgs()
        {
            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);

            // address
            ArgumentException ex = Assert.Throws<ArgumentNullException>(() => socket.Bind(null, 1));
            Assert.AreEqual("address", ex.ParamName);

            // address type different from family passed to constructor
            ex = Assert.Throws<ArgumentException>(() => socket.Bind(IPAddress.IPv6Any, 10000));
            Assert.AreEqual("address", ex.ParamName);

            // port
            ArgumentOutOfRangeException argEx = Assert.Throws<ArgumentOutOfRangeException>(() => socket.Bind(IPAddress.Any, -1));
            Assert.AreEqual("port", argEx.ParamName);
            Assert.AreEqual(-1, argEx.ActualValue);

            argEx = Assert.Throws<ArgumentOutOfRangeException>(() => socket.Bind(IPAddress.Any, 65536));
            Assert.AreEqual("port", argEx.ParamName);
            Assert.AreEqual(65536, argEx.ActualValue);
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Bind(IPEndPoint)"/>.
        /// </summary>
        [Test]
        public void Bind_IPEndPoint()
		{
			int port = _portNum++;

            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
				socket.Bind(new IPEndPoint(IPAddress.Any, port));

                IPEndPoint localEP = socket.LocalEndPoint;
                Assert.AreEqual(IPAddress.Any, localEP.Address);
				Assert.AreEqual(port, localEP.Port);
            }

            if (Socket.OSSupportsIPv6)
			{
				port = _portNum++;

                using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Stream))
                {
					socket.Bind(new IPEndPoint(IPAddress.IPv6Any, port));

                    IPEndPoint localEP = socket.LocalEndPoint;
                    Assert.AreEqual(IPAddress.IPv6Any, localEP.Address);
					Assert.AreEqual(port, localEP.Port);
                }
            }
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Bind(IPEndPoint)"/> with invalid
        /// argument values.
        /// </summary>
        [Test]
        public void Bind_IPEndPoint__InvalidArgs()
        {
            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);

            // endPoint
            ArgumentException ex = Assert.Throws<ArgumentNullException>(() => socket.Bind((IPEndPoint)null));
            Assert.AreEqual("endPoint", ex.ParamName);

            // address type different from family passed to constructor
            ex = Assert.Throws<ArgumentException>(() => socket.Bind(new IPEndPoint(IPAddress.IPv6Any, 10000)));
            Assert.AreEqual("endPoint", ex.ParamName);
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Listen(int)"/>.
        /// </summary>
        [Test]
        public void Listen()
		{
			int port = _portNum++;

            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
				socket.Bind(IPAddress.Any, port);
                socket.Listen(1);
                // What condition to assert here?
            }
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Listen(int)"/> with invalid argument values.
        /// </summary>
        [Test]
        public void Listen__InvalidArgs()
        {
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                ArgumentOutOfRangeException error = Assert.Throws<ArgumentOutOfRangeException>(() =>
                {
                    socket.Listen(0);
                });

                Assert.AreEqual("backlog", error.ParamName);
                Assert.AreEqual(0, error.ActualValue);
            }
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.Listen(int)"/> when the socket is not bound.
        /// </summary>
        [Test]
        public void Listen__NotBound()
        {
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                Udt.SocketException error = Assert.Throws<Udt.SocketException>(() =>
                {
                    socket.Listen(1);
                });

                Assert.AreEqual(Udt.SocketError.UnboundSocket, error.SocketErrorCode);
            }
        }

        /// <summary>
        /// Test for <see cref="Udt.Socket.GetPerformanceInfo()"/> when the
        /// connection is not open.
        /// </summary>
        [Test]
        public void GetPerformanceInfo__NotConnected()
        {
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                Udt.SocketException error = Assert.Throws<Udt.SocketException>(() => socket.GetPerformanceInfo());
                Assert.AreEqual(Udt.SocketError.NoConnection, error.SocketErrorCode);
            }
        }

        [Test]
        public void Get_set_CongestionControl()
        {
            var mock1 = new Mock<Udt.ICongestionControlFactory>();
            var mock2 = new Mock<Udt.ICongestionControlFactory>();

            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                Assert.IsNull(socket.CongestionControl);
                socket.CongestionControl = mock1.Object;
                Assert.AreSame(mock1.Object, socket.CongestionControl);
                Assert.AreSame(mock1.Object, socket.GetSocketOption(Udt.SocketOptionName.CongestionControl));

                socket.SetSocketOption(Udt.SocketOptionName.CongestionControl, mock2.Object);
                Assert.AreSame(mock2.Object, socket.CongestionControl);
                socket.SetSocketOption(Udt.SocketOptionName.CongestionControl, mock2.Object);
                Assert.AreSame(mock2.Object, socket.CongestionControl);
            }
        }

        [Test]
        public void Set_CongestionControl_to_invalid_values()
        {
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                ArgumentException argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.CongestionControl, "string value"));
                Assert.AreEqual("value", argEx.ParamName);

                argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.CongestionControl, 10));
                Assert.AreEqual("value", argEx.ParamName);

                argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.CongestionControl, 100L));
                Assert.AreEqual("value", argEx.ParamName);

                argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.CongestionControl, true));
                Assert.AreEqual("value", argEx.ParamName);
            }
        }

		[Test]
		public void Get_set_BlockingReceive()
		{
			using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				Assert.IsTrue(socket.BlockingReceive);
				socket.BlockingReceive = false;
				Assert.IsFalse(socket.BlockingReceive);

				Assert.IsFalse((bool)socket.GetSocketOption(Udt.SocketOptionName.BlockingReceive));
				socket.SetSocketOption(Udt.SocketOptionName.BlockingReceive, true);
				Assert.IsTrue((bool)socket.GetSocketOption(Udt.SocketOptionName.BlockingReceive));
				socket.SetSocketOption(Udt.SocketOptionName.BlockingReceive, 0);
				Assert.IsFalse(socket.BlockingReceive);
				socket.SetSocketOption(Udt.SocketOptionName.BlockingReceive, 10L);
				Assert.IsTrue(socket.BlockingReceive);
				socket.SetSocketOption(Udt.SocketOptionName.BlockingReceive, (object)false);
				Assert.IsFalse((bool)socket.GetSocketOption(Udt.SocketOptionName.BlockingReceive));
			}
		}

		[Test]
		public void Get_set_LingerState()
		{
			using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				LingerOption opt = socket.LingerState;
				Assert.IsTrue(opt.Enabled);
				Assert.AreEqual(180, opt.LingerTime);

				socket.LingerState = new LingerOption(false, 500);
				opt = socket.LingerState;
				Assert.IsFalse(opt.Enabled);
				Assert.AreEqual(500, opt.LingerTime);

				opt = (LingerOption)socket.GetSocketOption(Udt.SocketOptionName.Linger);
				Assert.IsFalse(opt.Enabled);
				Assert.AreEqual(500, opt.LingerTime);

				socket.SetSocketOption(Udt.SocketOptionName.Linger, new LingerOption(true, 180));
				opt = (LingerOption)socket.GetSocketOption(Udt.SocketOptionName.Linger);
				Assert.IsTrue(opt.Enabled);
				Assert.AreEqual(180, opt.LingerTime);
			}
		}

		[Test]
		public void Set_LingerState_to_invalid_values()
		{
			using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				ArgumentException argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.Linger, 1));
				Assert.AreEqual("value", argEx.ParamName);
				argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.Linger, -10L));
				Assert.AreEqual("value", argEx.ParamName);
				argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.Linger, true));
				Assert.AreEqual("value", argEx.ParamName);
				argEx = Assert.Throws<ArgumentNullException>(() => socket.SetSocketOption(Udt.SocketOptionName.Linger, null));
				Assert.AreEqual("value", argEx.ParamName);
				argEx = Assert.Throws<ArgumentException>(() => socket.SetSocketOption(Udt.SocketOptionName.Linger, "string value"));
				Assert.AreEqual("value", argEx.ParamName);
			}
		}

		/// <summary>
		/// Test setting and retrieving the various socket optins.
		/// </summary>
		[Test]
		public void Get_set_ReceiveTimeout()
		{
			using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				Assert.AreEqual(-1, socket.ReceiveTimeout);
				socket.ReceiveTimeout = 50;
				Assert.AreEqual(50, socket.ReceiveTimeout);

				Assert.AreEqual(50, socket.GetSocketOption(Udt.SocketOptionName.ReceiveTimeout));

				socket.SetSocketOption(Udt.SocketOptionName.ReceiveTimeout, -1);
				Assert.AreEqual(-1, socket.GetSocketOption(Udt.SocketOptionName.ReceiveTimeout));

				socket.SetSocketOption(Udt.SocketOptionName.ReceiveTimeout, 5L);
				Assert.AreEqual(5, socket.GetSocketOption(Udt.SocketOptionName.ReceiveTimeout));

				socket.SetSocketOption(Udt.SocketOptionName.ReceiveTimeout, true);
				Assert.AreEqual(1, socket.GetSocketOption(Udt.SocketOptionName.ReceiveTimeout));

				socket.SetSocketOption(Udt.SocketOptionName.ReceiveTimeout, (object)10);
				Assert.AreEqual(10, socket.GetSocketOption(Udt.SocketOptionName.ReceiveTimeout));
			}
		}

        [Test]
		public void Get_set_MaxBandwidth()
        {
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                // MaxBandwidth
                Assert.AreEqual(-1L, socket.MaxBandwidth);
                socket.MaxBandwidth = 50L;
                Assert.AreEqual(50L, socket.MaxBandwidth);

                Assert.AreEqual(50L, socket.GetSocketOption(Udt.SocketOptionName.MaxBandwidth));

                socket.SetSocketOption(Udt.SocketOptionName.MaxBandwidth, (object)1);
                Assert.AreEqual(1L, socket.GetSocketOption(Udt.SocketOptionName.MaxBandwidth));

                socket.SetSocketOption(Udt.SocketOptionName.MaxBandwidth, 2);
                Assert.AreEqual(2L, socket.GetSocketOption(Udt.SocketOptionName.MaxBandwidth));

                socket.SetSocketOption(Udt.SocketOptionName.MaxBandwidth, -1L);
                Assert.AreEqual(-1L, socket.GetSocketOption(Udt.SocketOptionName.MaxBandwidth));

                socket.SetSocketOption(Udt.SocketOptionName.MaxBandwidth, false);
                Assert.AreEqual(0L, socket.GetSocketOption(Udt.SocketOptionName.MaxBandwidth));

                socket.SetSocketOption(Udt.SocketOptionName.MaxBandwidth, (object)10L);
                Assert.AreEqual(10L, socket.GetSocketOption(Udt.SocketOptionName.MaxBandwidth));
            }
        }

		[Test]
		public void Set_MaxBandwidth_to_invalid_values()
		{
			using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				ArgumentException argEx = Assert.Throws<ArgumentNullException>(() => socket.SetSocketOption(Udt.SocketOptionName.MaxBandwidth, null));
				Assert.AreEqual("value", argEx.ParamName);
			}
		}

		[Test]
		public void Accept()
		{
			ManualResetEvent acceptedEvent = new ManualResetEvent(false);

			int port = _portNum++;

			var serverTask = Task.Factory.StartNew(() =>
			{
				using (Udt.Socket server = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
				{
					server.Bind(IPAddress.Loopback, port);
					server.Listen(1);

					using (Udt.Socket accept = server.Accept())
					{
						acceptedEvent.Set();
					}
				}
			});

			using (Udt.Socket client = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				client.Connect(IPAddress.Loopback, port);
				acceptedEvent.WaitOne();
			}

			serverTask.Wait();
		}

		[Test]
		public void Send_receive()
		{
			ManualResetEvent serverDoneEvent = new ManualResetEvent(false);
			ManualResetEvent clientDoneEvent = new ManualResetEvent(false);
			int port = _portNum++;

			var serverTask = Task.Factory.StartNew(() =>
			{
				using (Udt.Socket server = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
				{
					server.Bind(IPAddress.Loopback, port);
					server.Listen(1);

					using (Udt.Socket accept = server.Accept())
					{
						accept.Send(new byte[] { 1, 2, 3 });

						byte[] buffer = new byte[1024];
						Assert.AreEqual(3, accept.Receive(buffer));

						serverDoneEvent.Set();
						Assert.IsTrue(clientDoneEvent.WaitOne(1000));
					}
				}
			});

			using (Udt.Socket client = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				client.Connect(IPAddress.Loopback, port);

				byte[] buffer = new byte[1024];
				Assert.AreEqual(3, client.Receive(buffer));
				CollectionAssert.AreEqual(new byte[] { 1, 2, 3 }, buffer.Take(3));

				client.Send(new byte[] { 1, 2, 3 });

				clientDoneEvent.Set();
				Assert.IsTrue(serverDoneEvent.WaitOne(1000));
			}

			serverTask.Wait();
		}

		[Test]
		public void SendFile_stream()
		{
			ManualResetEvent serverDoneEvent = new ManualResetEvent(false);
			ManualResetEvent clientDoneEvent = new ManualResetEvent(false);
			int port = _portNum++;
			string path = GetFile("The quick brown fox jumped over the lazy dog");

			var serverTask = Task.Factory.StartNew(() =>
			{
				using (Udt.Socket server = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
				{
					server.Bind(IPAddress.Loopback, port);
					server.Listen(1);

					using (Udt.Socket accept = server.Accept())
					using (Udt.StdFileStream file = new Udt.StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
					{
						accept.SendFile(file);

						serverDoneEvent.Set();
						Assert.IsTrue(clientDoneEvent.WaitOne(1000));
					}
				}
			});

			using (Udt.Socket client = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			{
				byte[] buffer = new byte[1024];

				client.Connect(IPAddress.Loopback, port);
				Assert.AreEqual(44, client.Receive(buffer));

				CollectionAssert.AreEqual(File.ReadAllBytes(path), buffer.Take(44));
	
				clientDoneEvent.Set();
				Assert.IsTrue(serverDoneEvent.WaitOne(1000));
			}

			serverTask.Wait();
		}

		[Test]
		public void ReceiveFile_stream()
		{
			ManualResetEvent serverDoneEvent = new ManualResetEvent(false);
			ManualResetEvent clientDoneEvent = new ManualResetEvent(false);
			int port = _portNum++;
			string path = GetFile("The quick brown fox jumped over the lazy dog");
			string receivePath = GetFile();

			var serverTask = Task.Factory.StartNew(() =>
			{
				using (Udt.Socket server = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
				{
					server.Bind(IPAddress.Loopback, port);
					server.Listen(1);

					using (Udt.Socket accept = server.Accept())
					using (Udt.StdFileStream file = new Udt.StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
					{
						accept.SendFile(file);

						serverDoneEvent.Set();
						Assert.IsTrue(clientDoneEvent.WaitOne(1000));
					}
				}
			});

			using (Udt.Socket client = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
			using (Udt.StdFileStream file = new Udt.StdFileStream(receivePath, FileMode.Open, FileAccess.ReadWrite))
			{
				client.Connect(IPAddress.Loopback, port);
				Assert.AreEqual(44, client.ReceiveFile(file, 44));

				file.Close();

				CollectionAssert.AreEqual(File.ReadAllBytes(path), File.ReadAllBytes(receivePath));

				clientDoneEvent.Set();
				Assert.IsTrue(serverDoneEvent.WaitOne(1000));
			}

			serverTask.Wait();
		}

		private int _portNum = 10000;

		private string GetFile(string content = "")
		{
			string path = Path.GetTempFileName();
			File.WriteAllText(path, content);
			_paths.Add(path);
			return path;
		}

		private List<string> _paths;

		[SetUp]
		public void SetUp()
		{
			_paths = new List<string>();
		}

		[TearDown]
		public void TearDown()
		{
			foreach (string path in _paths)
			{
				try
				{
					File.Delete(path);
				}
				catch (Exception ex)
				{
					Console.WriteLine("Error deleting file {0}", path);
					Console.WriteLine(ex);
				}
			}
		}

        private class CongestionControlFactoryTester : Udt.ICongestionControlFactory
        {
            public Udt.CongestionControl CreateCongestionControl()
            {
                return new CongestionControlTester();
            }
        }

        private class CongestionControlTester : Udt.CongestionControl
        {
        }
    }
}
