using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NUnit.Framework;

namespace UdtProtocol_Test
{
    /// <summary>
    /// Test fixture for <see cref="Udt.Message"/>.
    /// </summary>
    [TestFixture]
    public class MessageTest
    {
        /// <summary>
        /// Test for <see cref="Udt.Message(byte[])"/>.
        /// </summary>
        [Test]
        public void Constructor_byteArray()
        {
            byte[] b = new byte[10];
            Udt.Message message = new Udt.Message(b);

            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(0, message.Buffer.Offset);
            Assert.AreEqual(10, message.Buffer.Count);

            Assert.AreEqual(false, message.InOrder);
            Assert.AreEqual(Udt.Message.Infinite, message.TimeToLive);

            // Empty array
            b = new byte[0];
            message = new Udt.Message(b);
            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(0, message.Buffer.Offset);
            Assert.AreEqual(0, message.Buffer.Count);
        }
        
        /// <summary>
        /// Test for <see cref="Udt.Message(byte[])"/> with invalid argument
        /// values.
        /// </summary>
        [Test]
        public void Constructor_byteArray__InvalidArgs()
        {
            ArgumentException argEx = Assert.Throws<ArgumentNullException>(() => new Udt.Message((byte[])null));
            Assert.AreEqual("array", argEx.ParamName);
        }

        /// <summary>
        /// Test for <see cref="Udt.Message(byte[],int,int)"/>.
        /// </summary>
        [Test]
        public void Constructor_byteArray_int_int()
        {
            byte[] b = new byte[10];
            Udt.Message message = new Udt.Message(b, 0, 10);

            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(0, message.Buffer.Offset);
            Assert.AreEqual(10, message.Buffer.Count);

            Assert.AreEqual(false, message.InOrder);
            Assert.AreEqual(Udt.Message.Infinite, message.TimeToLive);

            // Slice
            b = new byte[10];
            message = new Udt.Message(b, 5, 2);

            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(5, message.Buffer.Offset);
            Assert.AreEqual(2, message.Buffer.Count);

            Assert.AreEqual(false, message.InOrder);
            Assert.AreEqual(Udt.Message.Infinite, message.TimeToLive);

            // Empty slice
            b = new byte[10];
            message = new Udt.Message(b, 3, 0);
            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(3, message.Buffer.Offset);
            Assert.AreEqual(0, message.Buffer.Count);

            // Empty array
            b = new byte[0];
            message = new Udt.Message(b, 0, 0);
            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(0, message.Buffer.Offset);
            Assert.AreEqual(0, message.Buffer.Count);
        }

        /// <summary>
        /// Test for <see cref="Udt.Message(byte[],int,int)"/> with invalid argument
        /// values.
        /// </summary>
        [Test]
        public void Constructor_byteArray_int_int__InvalidArgs()
        {
            // null array
            ArgumentException argEx = Assert.Throws<ArgumentNullException>(() => new Udt.Message((byte[])null, 0, 0));
            Assert.AreEqual("array", argEx.ParamName);

            // negative offset
            argEx = Assert.Throws<ArgumentOutOfRangeException>(() => new Udt.Message(new byte[5], -1, 2));
            Assert.AreEqual("offset", argEx.ParamName);

            // invalid range
            argEx = Assert.Throws<ArgumentException>(() => new Udt.Message(new byte[5], 4, 2));
            Assert.IsNull(argEx.ParamName);

            // invalid range
            argEx = Assert.Throws<ArgumentException>(() => new Udt.Message(new byte[5], 5, 1));
            Assert.IsNull(argEx.ParamName);
        }

        /// <summary>
        /// Test for <see cref="Udt.Message(ArraySegment{byte})"/>.
        /// </summary>
        [Test]
        public void Constructor_ArraySegment()
        {
            byte[] b = new byte[10];
            Udt.Message message = new Udt.Message(new ArraySegment<byte>(b, 0, 10));

            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(0, message.Buffer.Offset);
            Assert.AreEqual(10, message.Buffer.Count);

            Assert.AreEqual(false, message.InOrder);
            Assert.AreEqual(Udt.Message.Infinite, message.TimeToLive);

            // Slice
            b = new byte[10];
            message = new Udt.Message(new ArraySegment<byte>(b, 5, 2));

            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(5, message.Buffer.Offset);
            Assert.AreEqual(2, message.Buffer.Count);

            Assert.AreEqual(false, message.InOrder);
            Assert.AreEqual(Udt.Message.Infinite, message.TimeToLive);

            // Empty slice
            b = new byte[10];
            message = new Udt.Message(new ArraySegment<byte>(b, 3, 0));
            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(3, message.Buffer.Offset);
            Assert.AreEqual(0, message.Buffer.Count);

            // Empty array
            b = new byte[0];
            message = new Udt.Message(new ArraySegment<byte>(b, 0, 0));
            Assert.AreSame(b, message.Buffer.Array);
            Assert.AreEqual(0, message.Buffer.Offset);
            Assert.AreEqual(0, message.Buffer.Count);
        }

        /// <summary>
        /// Test for <see cref="Udt.Message(ArraySegment{byte})"/> with invalid argument
        /// values.
        /// </summary>
        [Test]
        public void Constructor_ArraySegment__InvalidArgs()
        {
            // null array
            ArgumentException argEx = Assert.Throws<ArgumentException>(() => new Udt.Message(new ArraySegment<byte>()));
            Assert.AreEqual("buffer", argEx.ParamName);
        }

        /// <summary>
        /// Test for <see cref="Udt.Message.InOrder"/>.
        /// </summary>
        [Test]
        public void InOrder()
        {
            Udt.Message message = new Udt.Message(new byte[5]);
            Assert.IsFalse(message.InOrder);
            message.InOrder = true;
            Assert.IsTrue(message.InOrder);
        }

        /// <summary>
        /// Test for <see cref="Udt.Message.TimeToLive"/>.
        /// </summary>
        [Test]
        public void TimeToLive()
        {
            Udt.Message message = new Udt.Message(new byte[5]);
            Assert.AreEqual(Udt.Message.Infinite, message.TimeToLive);
            message.TimeToLive = TimeSpan.Zero;
            Assert.AreEqual(TimeSpan.Zero, message.TimeToLive);
            message.TimeToLive = new TimeSpan(0, 0, 0, 0, int.MaxValue);
            Assert.AreEqual(Udt.Message.Max, message.TimeToLive);
            message.TimeToLive = new TimeSpan(0, 0, 0, 0, -1);
            Assert.AreEqual(Udt.Message.Infinite, message.TimeToLive);
        }

        /// <summary>
        /// Test for <see cref="Udt.Message.TimeToLive"/> with invalid argument
        /// values.
        /// </summary>
        [Test]
        public void TimeToLive__InvalidArgs()
        {
            byte[] b = new byte[10];
            
            // low
            ArgumentOutOfRangeException argEx = Assert.Throws<ArgumentOutOfRangeException>(() => new Udt.Message(b).TimeToLive = new TimeSpan(-1));
            Assert.AreEqual("value", argEx.ParamName);
            Assert.AreEqual(new TimeSpan(-1), argEx.ActualValue);

            // high
            TimeSpan ts = new TimeSpan(0, 0, 0, 0, -1).Add(TimeSpan.FromTicks(1));
            argEx = Assert.Throws<ArgumentOutOfRangeException>(() => new Udt.Message(b).TimeToLive = ts);
            Assert.AreEqual("value", argEx.ParamName);
            Assert.AreEqual(ts, argEx.ActualValue);
        }
    }
}
