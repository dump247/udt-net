using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NUnit.Framework;
using System.Net.Sockets;
using System.IO;

namespace UdtProtocol_Test
{
    /// <summary>
    /// Test fixture for <see cref="Udt.NetworkStream"/>.
    /// </summary>
    [TestFixture]
    public class NetworkStreamTest
    {
        /// <summary>
        /// Test for <see cref="Udt.NetworkStream.CanRead"/>, <see cref="Udt.NetworkStream.CanWrite"/>,
        /// and <see cref="Udt.NetworkStream.CanSeek"/>.
        /// </summary>
        [Test]
        public void CanRead_CanWrite_CanSeek()
        {
            Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream);
            Udt.NetworkStream ns = new Udt.NetworkStream(socket, FileAccess.Read, false);
            Assert.IsTrue(ns.CanRead);
            Assert.IsFalse(ns.CanSeek);
            Assert.IsFalse(ns.CanWrite);

            ns = new Udt.NetworkStream(socket, FileAccess.Write, false);
            Assert.IsFalse(ns.CanRead);
            Assert.IsFalse(ns.CanSeek);
            Assert.IsTrue(ns.CanWrite);

            ns = new Udt.NetworkStream(socket, FileAccess.ReadWrite, false);
            Assert.IsTrue(ns.CanRead);
            Assert.IsFalse(ns.CanSeek);
            Assert.IsTrue(ns.CanWrite);

            ns.Dispose();
            Assert.IsFalse(ns.CanRead);
            Assert.IsFalse(ns.CanSeek);
            Assert.IsFalse(ns.CanWrite);
        }
    }
}
