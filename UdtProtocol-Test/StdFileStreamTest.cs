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
		public void Constructor_with_invalid_args()
		{
			string path = GetFile();

			// Null path
			ArgumentException ex =  Assert.Throws<ArgumentNullException>(() => new StdFileStream(null, FileMode.Open));
			Assert.AreEqual("path", ex.ParamName);

			// Empty path
			ex = Assert.Throws<ArgumentException>(() => new StdFileStream("", FileMode.Open));
			Assert.AreEqual("path", ex.ParamName);

			// Whitespace path
			ex = Assert.Throws<ArgumentException>(() => new StdFileStream(" \t\r\n", FileMode.Open));
			Assert.AreEqual("path", ex.ParamName);

			// Invalid character in path
			ex = Assert.Throws<ArgumentException>(() => new StdFileStream("file|name", FileMode.Open));
			Assert.AreEqual("path", ex.ParamName);

			// FileShare.Delete
			ex = Assert.Throws<ArgumentOutOfRangeException>(() => new StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Delete));
			Assert.AreEqual("share", ex.ParamName);
			
			// FileShare.Inheritable
			ex = Assert.Throws<ArgumentOutOfRangeException>(() => new StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Inheritable));
			Assert.AreEqual("share", ex.ParamName);
		}

		[Test]
		public void File_not_found()
		{
			var ex = Assert.Throws<FileNotFoundException>(() => new StdFileStream(@"file_that_does_not_exist", FileMode.Open));
			Assert.AreEqual("file_that_does_not_exist", ex.FileName);
		}

		[Test, Ignore("Not sure how to detect max path error. _wfsopen always seems to return ENOENT (file not found). Also, using MAX_PATH is not correct is we are using Unicode API (should be 32K or so).")]
		public void Path_too_long()
		{
			// See: http://msdn.microsoft.com/en-us/library/aa365247.aspx#maxpath
			var ex = Assert.Throws<PathTooLongException>(() => new StdFileStream(@"C:\" + new string('a', 50000), FileMode.Open));
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

		[Test]
		public void Exclusive_access()
		{
			string path = GetFile();

			using (StdFileStream fs = new StdFileStream(path, FileMode.Open))
			{
				Assert.Throws<IOException>(() => new StdFileStream(path, FileMode.Open));
				fs.Close();
			}

			using (FileStream fs = new FileStream(path, FileMode.Open))
			{
				Assert.Throws<IOException>(() => new FileStream(path, FileMode.Open));
				fs.Close();
			}
		}

		[Test]
		public void Shared_readwrite_access_followed_by_shared_read()
		{
			string path = GetFile();

			using (StdFileStream fs = new StdFileStream(path, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite))
			{
				Assert.Throws<IOException>(() => new StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read));
				fs.Close();
			}

			using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite))
			{
				Assert.Throws<IOException>(() => new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read));
				fs.Close();
			}
		}

		[Test]
		public void Shared_read_with_readwrite_access_followed_by_shared_read()
		{
			string path = GetFile();

			using (StdFileStream fs = new StdFileStream(path, FileMode.Open, FileAccess.ReadWrite, FileShare.Read))
			{
				Assert.Throws<IOException>(() => new StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read));
				fs.Close();
			}

			using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.ReadWrite, FileShare.Read))
			{
				Assert.Throws<IOException>(() => new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read));
				fs.Close();
			}
		}

		[Test]
		public void Shared_read_access_followed_by_shared_readwrite()
		{
			string path = GetFile();

			using (StdFileStream fs = new StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
			{
				Assert.Throws<IOException>(() => new StdFileStream(path, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite));
				fs.Close();
			}

			using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
			{
				Assert.Throws<IOException>(() => new FileStream(path, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite));
				fs.Close();
			}
		}

		[Test]
		public void Shared_read_access()
		{
			string path = GetFile();

			File.WriteAllBytes(path, new byte[] { 0x01, 0x02, 0x03 });

			using (StdFileStream fs1 = new StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
			using (StdFileStream fs2 = new StdFileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
			{
				byte[] buffer = new byte[4];

				Assert.AreEqual(3, fs1.Read(buffer, 0, 4));
				CollectionAssert.AreEqual(new byte[] { 0x01, 0x02, 0x03, 0x00 }, buffer);

				Assert.AreEqual(3, fs2.Read(buffer, 0, 4));
				CollectionAssert.AreEqual(new byte[] { 0x01, 0x02, 0x03, 0x00 }, buffer);
			}

			using (FileStream fs1 = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
			using (FileStream fs2 = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read))
			{
				byte[] buffer = new byte[4];

				Assert.AreEqual(3, fs1.Read(buffer, 0, 4));
				CollectionAssert.AreEqual(new byte[] { 0x01, 0x02, 0x03, 0x00 }, buffer);

				Assert.AreEqual(3, fs2.Read(buffer, 0, 4));
				CollectionAssert.AreEqual(new byte[] { 0x01, 0x02, 0x03, 0x00 }, buffer);
			}
		}

        /// <summary>
        /// Addressing 'Udt.StdFileStream.Length is not correct when the file size is 2G+'
        /// http://code.google.com/p/udt-net/issues/detail?id=7
        /// </summary>
        [Test]
        public void Issue7_Length_of_long_file()
        {
            long size = (1024L * 1024L * 1024L * 2L) - 1;

            using (StdFileStream fs = BuildFile(size))
            {
                Assert.AreEqual(size, fs.Position);
                Assert.AreEqual(size, fs.Length);

                fs.WriteByte(0);

                // This was failing with a negative value
                Assert.AreEqual(size + 1, fs.Position);
                Assert.AreEqual(size + 1, fs.Length);
            }
        }

        public StdFileStream BuildFile(long length)
        {
            string path = GetFile();
            StdFileStream fs = new StdFileStream(path, FileMode.Open);

            try
            {
                byte[] buffer = new byte[4096];
                long remaining = length;

                while (remaining > 0)
                {
                    fs.Write(buffer, 0, (int)Math.Min(remaining, buffer.Length));
                    remaining -= buffer.Length;
                }

                return fs;
            }
            catch
            {
                fs.Close();
                throw;
            }
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
