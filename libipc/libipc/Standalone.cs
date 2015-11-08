using System;

namespace libipc
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Chaos Server #Alpha5");
			CommunicationServer communicationhub = new CommunicationServer ();
		}
	}
}
