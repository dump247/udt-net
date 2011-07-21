using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using Udt;

namespace UdtProtocol_Test
{
	/// <summary>
	/// Test fixture for <see cref="Ack2Packet"/>.
	/// </summary>
	[TestFixture]
	public class Ack2PacketTest
	{
		[Test]
		public void Create_and_dispose()
		{
			Ack2Packet packet = new Ack2Packet();
			
			Assert.AreEqual(0, packet.DestinationId);
			Assert.AreEqual(0, packet.SequenceNumber);
			Assert.IsFalse(packet.IsDisposed);
			Assert.IsTrue(packet.IsEditable);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();

			Assert.Throws<ObjectDisposedException>(() => { var x = packet.DestinationId; });
			Assert.Throws<ObjectDisposedException>(() => { var x = packet.SequenceNumber; });
			Assert.IsTrue(packet.IsDisposed);
			Assert.IsFalse(packet.IsEditable);
			Assert.Throws<ObjectDisposedException>(() => { var x = packet.TimeStamp; });
		}

		[Test]
		public void ErrorCode()
		{
			Ack2Packet packet = new Ack2Packet();

			foreach (int value in new[] { 1, Int32.MaxValue, -1, Int32.MinValue, 0 })
			{
				packet.SequenceNumber = value;
				Assert.AreEqual(value, packet.SequenceNumber);
			}

			packet.Dispose();
		}

		[Test]
		public void Set_ErrorCode_after_disposed()
		{
			Ack2Packet packet = new Ack2Packet();
			packet.Dispose();

			Assert.Throws<ObjectDisposedException>(() => packet.SequenceNumber = 1);
		}
	}
}
