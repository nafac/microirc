using System;
using System.Threading;

namespace libipc
{
	public class CommunicationConnector
	{
		public CommunicationConnector ()
		{
			Console.WriteLine ("CommunicationConnector :: up and running !!");
		}
		public void loop()
		{
			Thread.Sleep (60);
		}
	}
}

