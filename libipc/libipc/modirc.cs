using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace libipc
{
	public class modirc
	{
		// 
		private CommunicationConnector connector;
		// 
		public modirc(String hash, String first_message)
		{
            //ConnunicationConnectoy always connect to "::1:6669 ie. IPV6 localhost to "CommunicationHub", It can be used to do connections to other remóte services too.
            //CommunicationServer then maps connections with randomized hash to create the CommunicaionContoller tubes.
	        // One alwaus could use OOP use for passing data but this is my networkinh demo.
            //
            // Handshake with CommunicationServer =>
			connector = new CommunicationConnector ("::1", 6669);
			connector.__write (String.Join("", "IPCH :", hash));
            // 
			Console.WriteLine ("modirc :: alive !!");
		}
		public void thread()
		{
			Console.WriteLine ("modirc :: looping !!");
			string data_cache;
			while (true) {
				data_cache = connector.receive (); // blockxzs
				// parse
				// send back
			}
		}
	}
}

