using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using Udt;

namespace UdtProtocol_Test
{
	/// <summary>
	/// Test fixture for <see cref="DataPacket"/>.
	/// </summary>
	[TestFixture]
	public class DataPacketTest
	{
		[Test]
		public void Constructor()
		{
			DataPacket packet = new DataPacket();

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();
		}

		[Test]
		public void Read_write()
		{
			DataPacket packet = new DataPacket();

			packet.Write(0, new byte[] { 0x01 }, 0, 1);
			Assert.AreEqual(1, packet.DataLength);

			byte[] buffer = new byte[2];
			Assert.AreEqual(1, packet.Read(0, buffer, 0, 2));
			CollectionAssert.AreEqual(new byte[] { 0x01, 0x00 }, buffer);

			packet.Dispose();
		}

		[Test]
		public void Write_will_reallocate()
		{
			DataPacket packet = new DataPacket();
			
			packet.Write(0, new byte[] { 0x01 }, 0, 1);
			int capacity = packet.DataCapacity;

			byte[] data = new byte[500];
			for (int i = 0; i < data.Length; ++i) data[i] = (byte)(i % 255);
			packet.Write(1, data, 0, data.Length);

			Assert.That(packet.DataCapacity, Is.GreaterThan(capacity));

			byte[] buffer = new byte[1000];
			Assert.AreEqual(501, packet.Read(0, buffer, 0, 1000));
			CollectionAssert.AreEqual(new byte[] { 0x01 }.Concat(data).ToArray(), buffer.Take(501).ToArray());

			packet.Dispose();
		}

		[Test]
		public void DataLength()
		{
			DataPacket packet = new DataPacket();

			packet.DataLength = 100;
			Assert.AreEqual(100, packet.DataLength);

			packet.DataLength = 200;
			Assert.AreEqual(200, packet.DataLength);

			packet.DataLength = 500;
			Assert.AreEqual(500, packet.DataLength);

			packet.DataLength = 1;
			Assert.AreEqual(1, packet.DataLength);

			packet.DataLength = 0;
			Assert.AreEqual(0, packet.DataLength);

			packet.Dispose();
		}

		[Test]
		public void DataLength_invalid_args()
		{
			DataPacket packet = new DataPacket();

			ArgumentException ex = Assert.Throws<ArgumentOutOfRangeException>(() => packet.DataLength = -1);
			Assert.AreEqual("value", ex.ParamName);

			packet.Dispose();
		}

		[Test]
		public void TimeStamp()
		{
			DataPacket packet = new DataPacket();

			packet.TimeStamp = DataPacket.MaxTimeStamp;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(DataPacket.MaxTimeStamp, packet.TimeStamp);

			packet.TimeStamp = new TimeSpan(10L);

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(new TimeSpan(10L), packet.TimeStamp);

			// TimeStamp resolution is 1 microsecond (1000 nanoseconds)
			// The property value gets rounded down to the nearest microsecond.
			packet.TimeStamp = new TimeSpan(9L);

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.TimeStamp = TimeSpan.Zero;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();
		}

		[Test]
		public void Invalid_TimeStamp()
		{
			DataPacket packet = new DataPacket();
			Assert.Throws<ArgumentOutOfRangeException>(() => packet.TimeStamp = TimeSpan.Zero.Subtract(new TimeSpan(1L)));
			Assert.Throws<ArgumentOutOfRangeException>(() => packet.TimeStamp = DataPacket.MaxTimeStamp.Add(new TimeSpan(1L)));
			packet.Dispose();
		}

		[Test]
		public void MessageBoundaryTest()
		{
			DataPacket packet = new DataPacket();

			packet.MessageBoundary = Udt.MessageBoundary.First;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(Udt.MessageBoundary.First, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.MessageBoundary = Udt.MessageBoundary.Last;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(Udt.MessageBoundary.Last, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.MessageBoundary = Udt.MessageBoundary.Solo;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(Udt.MessageBoundary.Solo, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.MessageBoundary = Udt.MessageBoundary.None;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(Udt.MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();
		}

		[Test]
		public void Invalid_MessageBoundary()
		{
			DataPacket packet = new DataPacket();
			Assert.Throws<ArgumentOutOfRangeException>(() => packet.MessageBoundary = (Udt.MessageBoundary)5);
			packet.Dispose();
		}

		[Test]
		public void MessageNumber()
		{
			DataPacket packet = new DataPacket();

			packet.MessageNumber = DataPacket.MaxMessageNumber;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(DataPacket.MaxMessageNumber, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.MessageNumber = 1;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(1, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.MessageNumber = 0;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();
		}

		[Test]
		public void Invalid_MessageNumber()
		{
			DataPacket packet = new DataPacket();
			Assert.Throws<ArgumentOutOfRangeException>(() => packet.MessageNumber = DataPacket.MaxMessageNumber + 1);
			Assert.Throws<ArgumentOutOfRangeException>(() => packet.MessageNumber = -1);
			packet.Dispose();
		}

		[Test]
		public void PacketNumber()
		{
			DataPacket packet = new DataPacket();

			packet.PacketNumber = Int32.MaxValue;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(Int32.MaxValue, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.PacketNumber = 1;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(1, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.PacketNumber = 0;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();
		}

		[Test]
		public void Invalid_PacketNumber()
		{
			DataPacket packet = new DataPacket();
			Assert.Throws<ArgumentOutOfRangeException>(() => packet.PacketNumber = -1);
			packet.Dispose();
		}

		[Test]
		public void InOrder()
		{
			DataPacket packet = new DataPacket();

			packet.InOrder = true;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsTrue(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.InOrder = false;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();
		}

		[Test]
		public void DestinationId()
		{
			DataPacket packet = new DataPacket();

			packet.DestinationId = Int32.MaxValue;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(Int32.MaxValue, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.DestinationId = 1;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(1, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.DestinationId = 0;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(0, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.DestinationId = -1;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(-1, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.DestinationId = Int32.MinValue;

			Assert.AreEqual(0, packet.DataLength);
			Assert.AreEqual(Int32.MinValue, packet.DestinationId);
			Assert.IsFalse(packet.InOrder);
			Assert.AreEqual(MessageBoundary.None, packet.MessageBoundary);
			Assert.AreEqual(0, packet.MessageNumber);
			Assert.AreEqual(0, packet.PacketNumber);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();
		}
	}
}
