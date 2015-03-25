# Introduction #

This example shows how to use the UDT library to send and receive a file.

Sending a file directly is equivalent to reading the file's contents and sending the bytes over the socket. Therefore, the file contents must be prefixed with the file length so the receiver can determine the data size. Using SendFile does not necessitate using ReceiveFile at the peer side. The receiver could use Receive calls to receive the file data instead.

# Client Send File #
```
using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
using (Udt.StdFileStream fs = new Udt.StdFileStream(path, FileMode.Open))
{
  socket.Connect(IPAddress.Loopback, 10000);

  // Send the file length, in bytes
  socket.Send(BitConverter.ToBytes(fs.Length), 0, sizeof(long));

  // Send the file contents
  socket.SendFile(fs);
}
```

# Server Receive File #
```
using (Udt.Socket socket = new Udt.Socket(AddressFamily.InterNetwork, SocketType.Stream))
{
  socket.Bind(IPAddress.Loopback, 10000);
  socket.Listen(10);

  using (Udt.Socket client = socket.Accept())
  {
    // Receive the file length, in bytes
    byte[] buffer = new byte[8];
    client.Receive(buffer, 0, sizeof(long));

    // Receive the file contents (path is where to store the file)
    client.ReceiveFile(path, BitConverter.ToInt64(buffer, 0));
  }
}
```