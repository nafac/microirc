using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace libipc
{
	public class Module_IRC
	{
		// 
		private CommunicationConnector connector;
		// 
		public Module_IRC(Socket handler, String hash)
		{
			// CommunicationConnector always connects to "::1:6669" ie. IPv6 localhost ie. CommunicationHub
				// One could, or could not just use variables for pairing.
			connector = new CommunicationConnector ();
			connector.__write (String.Join("", "IPCH :", hash));
			Console.WriteLine ("mod(IRC) :: alive !!");
		}
		public void main()
		{
			Console.WriteLine ("mod(IRC) :: looping !!");
			string data_cache;
			while (true) {
				data_cache = connector.receive (); // blockxzs
				// parse
				// send back
			}
		}
	}
}

