using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace libipc
{
	public class StateObject
	{
		public Socket			WorkSocket = null;
		public const int		buffersize = 1024;
		public byte[]			buffer = new byte[buffersize];
		public StringBuilder	sb = new StringBuilder();
	}
	public class CommunicationServer
	{
		// Thread signal handler.
		public static ManualResetEvent allDone = new ManualResetEvent(false);
		public CommunicationServer ()
		{
			// 
			Socket listener = new Socket (AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
			listener.Bind (new IPEndPoint(IPAddress.Parse ("::1"), 6669));
			listener.Listen (128);
			// 
			while (true) {
				allDone.Reset (); // thread signal handler
				// 
				Console.WriteLine ("CommunicationServer :: Waiting for a connection ..");
				listener.BeginAccept (new AsyncCallback (AcceptCallback), listener);
				// 
				allDone.WaitOne (); // thread signal handler
			}
			Console.WriteLine ("CommunicationServer dead !");
		}
		private static void AcceptCallback(IAsyncResult ar)
		{
			allDone.Set ();
			Socket listener	= (Socket)ar.AsyncState;
			Socket handler = listener.EndAccept (ar);
			StateObject state = new StateObject ();
			state.WorkSocket = handler;
//			handler.BeginReceive (state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback (ReadCallback), state);
		}
	}
}

