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
	        // One always could use OOP use for passing data but this is my networkinh demo.
            //
            // Handshake with CommunicationServer =>
			connector = new CommunicationConnector ("::1", 6669);
			connector.write (String.Join("", "IPCH :", hash));
            // do not lose data
            feed(first_message);
            // 
			Console.WriteLine ("modirc :: running !!");
		}
		public void thread()
		{
            DisposableUtilities utilities = new DisposableUtilities();
			Console.WriteLine ("modirc :: looping !!");
			while (true)
            {
                String   cache = connector.read();
                String[] lines = utilities.explode(new string[] { "\n", "\r", "<EOF>" }, cache);
                foreach(String line in lines) { feed(line); }
                feed(connector.read());
            }
		}
        private void feed(String message)
        {
            DisposableUtilities utilities = new DisposableUtilities();
            // feed
            // parse
            String[] args = utilities.explode(new string[] { " " }, message);
            // reply
            // The IRC Hanshake =>
            if (message.Contains("AUTH NOTICE"))
            {
                connector.write("USER micro +xi micro :microirc-A6");
                connector.write("NICK microirc");
            }
            if (args[0].Contains("PING"))
            {
                connector.write(String.Join(" ", "PONG", args[1]));
            }
            Console.WriteLine("modirc::feed message={0}", message);
        }
	}
}

