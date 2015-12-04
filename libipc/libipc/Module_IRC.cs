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
            //ConnunicationConnectoy always connect to "::1:6669 ie. IPV6 localhost to "CommunicationHub", It can be used to do connections to other remóte services too.
            //CommunicationServer then maps connections with randomized hash to create the CommunicaionContoller tubes.
	        // One alwaus could use OOP use for passing data but this is my networkinh demo.
			connector = new CommunicationConnector ("::1", 6669);
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

