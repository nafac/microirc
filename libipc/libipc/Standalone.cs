using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace libipc
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Chaos Server #Alpha5");
			// 
			CommunicationServer CommunicationHub = new CommunicationServer ();
			Thread CommunicationHubThread = new Thread (CommunicationHub.loop);
			CommunicationHubThread.Start ();
			// 
			//	embed this in server module!!
			CommunicationConnector CommunicationServer = new CommunicationConnector ();
			Thread CommunicationConnectorThread = new Thread(CommunicationServer.main_loop);
			CommunicationConnectorThread.Start ();
		}
	}
	public class StateObject
	{
		public Socket			WorkSocket = null;
		public const int		buffersize = 1024;
		public byte[]			buffer = new byte[buffersize];
		public StringBuilder	sb = new StringBuilder();
	}
}
