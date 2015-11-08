using System;
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
			CommunicationConnector CommunicationServer = new CommunicationConnector ();
			Thread CommunicationConnectorThread = new Thread(CommunicationServer.loop);
			CommunicationConnectorThread.Start ();
		}
	}
}
