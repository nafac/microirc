using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace libipc
{
	public class CommunicationServer
	{
		// Thread signal handler.
		public static ManualResetEvent allDone = new ManualResetEvent(false);
		public CommunicationServer ()
		{
			Console.WriteLine ("CommunicationServer :: up and running !!");
		}
		public void loop()
		{
			Socket listener = new Socket (AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
			try {
				// 
				listener.Bind (new IPEndPoint(IPAddress.Parse ("::1"), 6669));
				listener.Listen (128);
				// 
				while (true)
				{
					Console.WriteLine ("CommunicationServer :: Waiting for a connection ..");
					allDone.Reset ();
					listener.BeginAccept (new AsyncCallback (AcceptCallback), listener);
					allDone.WaitOne ();
				}
			} catch (Exception e) {
				Console.WriteLine (e.ToString ());
			}
		}
		private static void AcceptCallback(IAsyncResult ar)
		{
			allDone.Set (); // thread signal handler
			Socket listener	= (Socket)ar.AsyncState;
			Socket handler = listener.EndAccept (ar);
			StateObject state = new StateObject ();
			state.WorkSocket = handler;
			handler.BeginReceive (state.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), state);
			Console.WriteLine ("CommunicationServer :: connection accepted ..");
		}
		private static void ReadCallback(IAsyncResult ar)
		{
			String content = String.Empty;
			Console.WriteLine ("CommunicationServer :: reading ..");
			StateObject ConnectorState = (StateObject)ar.AsyncState;
			Socket handler = ConnectorState.WorkSocket;

			int bytes_read = handler.EndReceive (ar);
			if (bytes_read > 0) {
				// There might be more fata, so store the data received so far.
				ConnectorState.sb.Append (Encoding.UTF8.GetString (ConnectorState.buffer, 0, bytes_read));
				// Check for end-of-line tag. If it is not there => read more data.
				content = ConnectorState.sb.ToString ();
				//Console.WriteLine ("CommunicationServer :: received {0}", content);
				//if (content.IndexOf ("\n\r") > -1) {
				if (content.IndexOf ("<EOF>") > -1) {
					Console.WriteLine ("CommunicationServer :: received {0}", content);

					// I still smoke hack every day ..
					//Array.Clear(ConnectorState.buffer, 0, ConnectorState.buffer.Length);
					ConnectorState.sb.Clear();
					// .. echo ..
					Send (handler, content);
					// receive more data
					handler.BeginReceive (ConnectorState.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), ConnectorState); // read more data when finished
				} else {
					handler.BeginReceive (ConnectorState.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), ConnectorState);
				}
			}
			// I smoke hack every day.
			//handler.BeginReceive (ConnectorState.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), ConnectorState);
		}
		private static void Send(Socket handler, String data)
		{
			byte[] byte_data = Encoding.UTF8.GetBytes (data);
			handler.BeginSend(byte_data, 0, byte_data.Length, 0, 
				new AsyncCallback(SendCallback), handler);
		}
		private static void SendCallback(IAsyncResult ar)
		{
			try {
				Socket handler = (Socket) ar.AsyncState;
				//
				int bytes_sent = handler.EndSend(ar);
				Console.WriteLine("Sent {0} bytes to client.", bytes_sent);
				//
				//handler.Shutdown(SocketShutdown.Both);
				//handler.Close();
			} catch(Exception e) {
				Console.WriteLine (e.ToString ());
			}
		}
	}
}

