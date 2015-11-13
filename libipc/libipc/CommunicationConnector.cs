using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace libipc
{
	public class CommunicationConnector
	{
		// instance signal completion
		private static ManualResetEvent connectDone = new ManualResetEvent(false);
		private static ManualResetEvent sendDone = new ManualResetEvent(false);
		private static ManualResetEvent receiveDone = new ManualResetEvent(false);
		// response from the remote device
		private static String response = String.Empty;

		public CommunicationConnector ()
		{
			Console.WriteLine ("CommunicationConnector :: up and running !!");
		}
		public void main_loop()
		{
			Thread.Sleep (600);
			// data buffer for incoming data
			byte[] bytes = new byte[1024];
			// connect to remote server
			try {
				// create a TCP/IP socket
				Socket connector = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
				// socket options
				//connector.Blocking = false;
				//connector.ReceiveTimeout = 16;

				// Asynchronous connector
				/*
				connector.BeginConnect(new IPEndPoint(IPAddress.Parse ("::1"), 6669), new AsyncCallback(ConnectCallback), connector);
				connectDone.WaitOne();
				// send data to remote device
				Send(connector, "This is a text.<EOF>");
				sendDone.WaitOne();
				*/
				/*
				// receive data from the remote device
				Receive(connector);
				receiveDone.WaitOne();
				// write response to console
				Console.WriteLine("Response received : {0}", response);
				// flush buffer
				response = String.Empty;
				*/

				// connect the socket to remote endpoint, catch errors
				try {
					connector.Connect(new IPEndPoint(IPAddress.Parse ("::1"), 6669));
					Console.WriteLine("CommunicationConnector connected to remote endpoint.");
					while(true) {
						/*
						int bytesReceived = 0, bytesTotal = 0;
						// encode the data into a byte array
						byte[] msg = Encoding.UTF8.GetBytes("This is a test<EOF>");
						// send
						int bytesSent = connector.Send(msg);
						// receive
						connector.Receive(bytes);
						Console.WriteLine ("Echoed test = {0}", Encoding.UTF8.GetString(bytes, 0, bytesReceived));
						// sleep
						*/
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
		private static void ConnectCallback(IAsyncResult ar) 
		{
			try {
				// asyncstate
				Socket connector = (Socket) ar.AsyncState;
				// complete the connection
				connector.EndConnect(ar);
				Console.WriteLine("CommunicationConnector connected to '{0}'.", connector.RemoteEndPoint.ToString());
				// signal management
				connectDone.Set();
			} catch (Exception e) {
				Console.WriteLine (e.ToString ());
			}
		}
		private static void Send(Socket client, String data)
		{
			byte[] byteData = Encoding.UTF8.GetBytes (data);
			client.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), client);
		}
		private static void SendCallback(IAsyncResult ar)
		{
			try {
				Socket client = (Socket) ar.AsyncState;						// retrieve the socket from the state object
				int bytes_sent = client.EndSend(ar);						// complete sending data to the remote device
				Console.WriteLine("Sent {0} bytes to server.", bytes_sent);
				sendDone.Set();												// signal all sent
			} catch (Exception e) {
				Console.WriteLine (e.ToString ());
			}
		}
	}
}

