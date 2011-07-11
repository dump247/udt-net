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
			string path = GetFile("The quick brown fox jumped over the lazy dog");

			StdFileStream fs = new StdFileStream(path, FileMode.Open);

			Assert.IsTrue(fs.CanRead);
			Assert.IsTrue(fs.CanSeek);
			Assert.IsFalse(fs.CanTimeout);
			Assert.IsTrue(fs.CanWrite);

			Assert.AreEqual(new FileInfo(path).Length, fs.Length);
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

		[Test]
		public void Write()
		{
			string path = GetFile();
			StdFileStream fs = new StdFileStream(path, FileMode.Open);

			Assert.AreEqual(0, fs.Length);
			Assert.AreEqual(0, fs.Position);

			fs.Write(new byte[] { 100, 103, 103, 104, 105 }, 0, 5);
			Assert.AreEqual(5, fs.Length);
			Assert.AreEqual(5, fs.Position);

			fs.Position = 2;
			Assert.AreEqual(2, fs.Position);

			fs.Write(new byte[] { 100, 103, 103, 104, 105 }, 1, 2);
			Assert.AreEqual(5, fs.Length);
			Assert.AreEqual(4, fs.Position);

			fs.Write(new byte[] { 100, 103, 103, 104, 105 }, 1, 2);
			Assert.AreEqual(6, fs.Length);
			Assert.AreEqual(6, fs.Position);

			fs.Close();
		}

		[Test]
		public void Read()
		{
			byte[] buffer = new byte[1024];
			string path = GetFile();
			StdFileStream fs = new StdFileStream(path, FileMode.Open);

			fs.Write(new byte[] { 100, 102, 103, 104, 105, 106, 107, 108 }, 0, 8);
			fs.Position = 0;

			Assert.AreEqual(2, fs.Read(buffer, 0, 2));
			Assert.AreEqual(new byte[] { 100, 102 }, buffer.Take(2));

			Assert.AreEqual(5, fs.Read(buffer, 2, 5));
			Assert.AreEqual(new byte[] { 100, 102, 103, 104, 105, 106, 107 }, buffer.Take(7));

			Assert.AreEqual(1, fs.Read(buffer, 1, 5));
			Assert.AreEqual(new byte[] { 100, 108, 103, 104, 105, 106, 107 }, buffer.Take(7));

			Assert.AreEqual(0, fs.Read(buffer, 0, 5));

			fs.Close();
		}

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
	}
}
