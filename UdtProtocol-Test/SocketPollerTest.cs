using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;

using NUnit.Framework;
using System.Net;
using System.Threading;
using System.Threading.Tasks;

namespace UdtProtocol_Test
{
    [TestFixture]
    public class SocketPollerTest
    {
        [Test]
        public void Constructor()
        {
            Udt.SocketPoller poller = new Udt.SocketPoller();
            CollectionAssert.IsEmpty(poller.ReadSockets);
            CollectionAssert.IsEmpty(poller.WriteSockets);
            poller.Dispose();
            CollectionAssert.IsEmpty(poller.ReadSockets);
            CollectionAssert.IsEmpty(poller.WriteSockets);
        }

        [Test]
        public void Add_closed_socket()
        {
            using (Udt.SocketPoller poller = new Udt.SocketPoller())
            {
                Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
                socket.Dispose();
                Assert.Throws<Udt.SocketException>(() => poller.AddSocket(socket));
            }
        }

        [Test]
        public void Remove_closed_socket()
        {
            using (Udt.SocketPoller poller = new Udt.SocketPoller())
            {
                Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
                poller.AddSocket(socket);
                socket.Dispose();
                Assert.Throws<Udt.SocketException>(() => poller.RemoveSocket(socket));
            }
        }

        [Test]
        public void Add_socket_to_disposed_poller()
        {
            Udt.SocketPoller poller = new Udt.SocketPoller();
            poller.Dispose();
            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
            Assert.Throws<ObjectDisposedException>(() => poller.AddSocket(socket));
            socket.Dispose();
        }

        [Test]
        public void Wait_with_no_sockets()
        {
            using (Udt.SocketPoller poller = new Udt.SocketPoller())
            {
                Assert.Throws<InvalidOperationException>(() => poller.Wait());
            }
        }

        [Test]
        public void Wait_when_disposed()
        {
            Udt.SocketPoller poller = new Udt.SocketPoller();
            poller.Dispose();
            Assert.Throws<ObjectDisposedException>(() => poller.Wait());
        }

        [Test]
        public void Wait_for_accept()
        {
            using (Udt.SocketPoller poller = new Udt.SocketPoller())
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                socket.Bind(IPAddress.Loopback, 0);
                socket.Listen(100);
                ManualResetEvent doneEvent = new ManualResetEvent(false);

                poller.AddSocket(socket);

                Task.Factory.StartNew(() =>
                {
                    using (Udt.Socket client = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
                    {
                        client.Connect(IPAddress.Loopback, socket.LocalEndPoint.Port);
                        doneEvent.WaitOne(1000);
                    }
                });

                Assert.IsTrue(poller.Wait(TimeSpan.FromSeconds(1)));
                CollectionAssert.IsEmpty(poller.WriteSockets);
                CollectionAssert.AreEqual(new[] { socket }, poller.ReadSockets);
                doneEvent.Set();

                Assert.IsFalse(poller.Wait(TimeSpan.Zero));
                CollectionAssert.IsEmpty(poller.WriteSockets);
                CollectionAssert.IsEmpty(poller.ReadSockets);
            }
        }

        [Test]
        public void Remove_socket()
        {
            using (Udt.SocketPoller poller = new Udt.SocketPoller())
            using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
            {
                socket.Bind(IPAddress.Loopback, 0);
                socket.Listen(100);
                ManualResetEvent doneEvent = new ManualResetEvent(false);

                poller.AddSocket(socket);
                Assert.IsFalse(poller.Wait(TimeSpan.Zero));
                poller.RemoveSocket(socket);

                Assert.Throws<InvalidOperationException>(() => poller.Wait(TimeSpan.Zero));
            }
        }
    }
}
