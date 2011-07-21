using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using Udt;

namespace UdtProtocol_Test
{
	/// <summary>
	/// Test fixture for <see cref="ErrorPacket"/>.
	/// </summary>
	[TestFixture]
	public class ErrorPacketTest
	{
		[Test]
		public void Create_and_dispose()
		{
			ErrorPacket packet = new ErrorPacket();
			
			Assert.AreEqual(0, packet.DestinationId);
			Assert.AreEqual(0, packet.ErrorCode);
			Assert.IsFalse(packet.IsDisposed);
			Assert.IsTrue(packet.IsEditable);
			Assert.AreEqual(TimeSpan.Zero, packet.TimeStamp);

			packet.Dispose();

			Assert.Throws<ObjectDisposedException>(() => { var x = packet.DestinationId; });
			Assert.Throws<ObjectDisposedException>(() => { var x = packet.ErrorCode; });
			Assert.IsTrue(packet.IsDisposed);
			Assert.IsFalse(packet.IsEditable);
			Assert.Throws<ObjectDisposedException>(() => { var x = packet.TimeStamp; });
		}

		[Test]
		public void ErrorCode_constructor()
		{
			foreach (int value in new[] { 1, Int32.MaxValue, -1, Int32.MinValue, 0 })
			{
				ErrorPacket packet = new ErrorPacket(value);
				Assert.AreEqual(value, packet.ErrorCode);
				packet.Dispose();
			}
		}

		[Test]
		public void ErrorCode()
		{
			ErrorPacket packet = new ErrorPacket();

			foreach (int value in new[] { 1, Int32.MaxValue, -1, Int32.MinValue, 0 })
			{
				packet.ErrorCode = value;
				Assert.AreEqual(value, packet.ErrorCode);
			}

			packet.Dispose();
		}

		[Test]
		public void Set_ErrorCode_after_disposed()
		{
			ErrorPacket packet = new ErrorPacket();
			packet.Dispose();

			Assert.Throws<ObjectDisposedException>(() => packet.ErrorCode = 1);
		}
	}
}
