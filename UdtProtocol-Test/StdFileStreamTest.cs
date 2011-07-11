using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using System.IO;
using Udt;

namespace UdtProtocol_Test
{
	[TestFixture]
	public class StdFileStreamTest
	{
		[Test]
		public void Constructor()
		{
			StdFileStream fs = new StdFileStream(_path, FileMode.Open);

			Assert.IsTrue(fs.CanRead);
			Assert.IsTrue(fs.CanSeek);
			Assert.IsFalse(fs.CanTimeout);
			Assert.IsTrue(fs.CanWrite);

			Assert.AreEqual(new FileInfo(_path).Length, fs.Length);
			Assert.AreEqual(0, fs.Position);
			Assert.Throws<InvalidOperationException>(() => { var x = fs.ReadTimeout; });
			Assert.Throws<InvalidOperationException>(() => { var x = fs.WriteTimeout; });

			fs.Close();

			Assert.IsFalse(fs.CanRead);
			Assert.IsFalse(fs.CanSeek);
			Assert.IsFalse(fs.CanTimeout);
			Assert.IsFalse(fs.CanWrite);

			Assert.Throws<ObjectDisposedException>(() => { var x = fs.Length; });
			Assert.Throws<ObjectDisposedException>(() => { var x = fs.Position; });
			Assert.Throws<InvalidOperationException>(() => { var x = fs.ReadTimeout; });
			Assert.Throws<InvalidOperationException>(() => { var x = fs.WriteTimeout; });
		}

		private string _path;

		[SetUp]
		public void SetUp()
		{
			_path = Path.GetTempFileName();
			File.WriteAllText(_path, "The quick brown fox jumped over the lazy dog");
		}

		[TearDown]
		public void TearDown()
		{
			try { File.Delete(_path); }
			catch { }
		}
	}
}
