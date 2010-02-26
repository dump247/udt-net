using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;

using NUnit.Framework;
using System.Net;

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
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                socket.Bind(IPAddress.Any, 10000);

                IPEndPoint localEP = socket.LocalEndPoint;
                Assert.AreEqual(IPAddress.Any, localEP.Address);
                Assert.AreEqual(10000, localEP.Port);
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

            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Stream))
            {
                socket.Bind(IPAddress.IPv6Any, 10000);

                IPEndPoint localEP = socket.LocalEndPoint;
                Assert.AreEqual(IPAddress.IPv6Any, localEP.Address);
                Assert.AreEqual(10000, localEP.Port);
            }
        }

        /// <summary>
        /// Test for multiple calls to <see cref="Udt.Socket.Bind(IPAddress,int)"/>.
        /// </summary>
        [Test]
        public void Bind_IPAddress_int__BindTwice()
        {
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                socket.Bind(IPAddress.Any, 10000);

                Udt.SocketException error = Assert.Throws<Udt.SocketException>(() =>
                {
                    socket.Bind(IPAddress.Any, 10000);
                });

                Assert.AreEqual(Udt.SocketError.InvalidOperation, error.SocketErrorCode);
            }

            if (Socket.OSSupportsIPv6)
            {
                using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Stream))
                {
                    socket.Bind(IPAddress.IPv6Any, 10000);

                    Udt.SocketException error = Assert.Throws<Udt.SocketException>(() =>
                    {
                        socket.Bind(IPAddress.IPv6Any, 10000);
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
            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
            socket.Dispose();

            Udt.SocketException error = Assert.Throws<Udt.SocketException>(() =>
            {
                socket.Bind(IPAddress.Any, 10000);
            });
            Assert.AreEqual(Udt.SocketError.InvalidSocket, error.SocketErrorCode);
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
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                socket.Bind(new IPEndPoint(IPAddress.Any, 10000));

                IPEndPoint localEP = socket.LocalEndPoint;
                Assert.AreEqual(IPAddress.Any, localEP.Address);
                Assert.AreEqual(10000, localEP.Port);
            }

            if (Socket.OSSupportsIPv6)
            {
                using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetworkV6, SocketType.Stream))
                {
                    socket.Bind(new IPEndPoint(IPAddress.IPv6Any, 10000));

                    IPEndPoint localEP = socket.LocalEndPoint;
                    Assert.AreEqual(IPAddress.IPv6Any, localEP.Address);
                    Assert.AreEqual(10000, localEP.Port);
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
            ArgumentException ex = Assert.Throws<ArgumentNullException>(() => socket.Bind(null));
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
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                socket.Bind(IPAddress.Any, 10000);
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
    }
}
