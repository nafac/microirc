using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace libipc
{
	public class CommunicationConnector
	{
		// 
		private static Socket connector;
		// 
		public CommunicationConnector ()
		{
			try {
				connector = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
				try {
					connector.Connect(new IPEndPoint(IPAddress.Parse ("::1"), 6669));
					Console.WriteLine("CommunicationConnector connected to remote endpoint.");
				} catch (ArgumentNullException ane) {
					Console.WriteLine ("ArgumentNullException : {0}", ane.ToString());
				} catch (SocketException se) {
					Console.WriteLine ("SocketException : {0}", se.ToString());
				} catch (Exception e) {
					Console.WriteLine ("Unexpected exception : {0}", e.ToString());
				}
			} catch (Exception e) {
				Console.WriteLine (e.ToString ());
			}
			Console.WriteLine ("CommunicationConnector :: up and running !!");
		}
		// Standalone CommunicationConnector
		/*
		public void main_loop()
		{
			// connect to remote server
			try {
				// create a TCP/IP socket
				connector = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);

				// connect the socket to remote endpoint, catch errors
				try {
					connector.Connect(new IPEndPoint(IPAddress.Parse ("::1"), 6669));
					Console.WriteLine("CommunicationConnector connected to remote endpoint.");
					while(true) {
						int bytesReceived = 0, bytesTotal = 0;
						// encode the data into a byte array
						byte[] msg = Encoding.UTF8.GetBytes("This is a test<EOF>");
						// send
						int bytesSent = connector.Send(msg);
						// receive
						connector.Receive(bytes);
						Console.WriteLine ("Echoed test = {0}", Encoding.UTF8.GetString(bytes, 0, bytesReceived));
						// sleep
						Thread.Sleep(60);
					}
				} catch (ArgumentNullException ane) {
					Console.WriteLine ("ArgumentNullException : {0}", ane.ToString());
				} catch (SocketException se) {
					Console.WriteLine ("SocketException : {0}", se.ToString());
				} catch (Exception e) {
					Console.WriteLine ("Unexpected exception : {0}", e.ToString());
				}
			} catch (Exception e) {
				Console.WriteLine (e.ToString ());
			}
		}
		*/
		// blocking receive
		public string receive()
		{
			byte[] bytes = new byte[1024];
			int bytesRec = connector.Receive(bytes);
			string ret   = Encoding.UTF8.GetString (bytes, 0, bytesRec);
			return ret;
		}
		// non-blocking send
		public void __write(String message)
		{
			String mesg = String.Join ("", message, "\n\r<EOF>");
			byte[] msg = Encoding.UTF8.GetBytes(mesg);
			int bytes_sent = connector.Send(msg);
			Console.WriteLine ("CommunicationConnector::__write data={0} bytes={1}", message, bytes_sent);
			return;
		}
	}
}

