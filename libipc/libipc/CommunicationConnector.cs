using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace libipc
{
	public class CommunicationConnector
	{
		// 
        private GenericNetworking network = new GenericNetworking();
		private static Socket connector;
		// 
		public CommunicationConnector (string address, int port)
		{
            try {
				connector = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
				try {
                    if(address.Contains("::1")) {
                        connector.Connect(new IPEndPoint(IPAddress.Parse("::1"), 6669));
                    } else {
                        IPHostEntry host = Dns.GetHostEntry(address);
                        foreach (IPAddress ip in host.AddressList)
                        {
                            connector.Connect(new IPEndPoint(ip, port));
                            Console.WriteLine("CommunicationConnector :: connection to remote endpoint {0} established.", ip);
                        };
                    }
                    // ^^
                    //connector.Connect(new IPEndPoint(IPAddress.Parse ("::1"), 6669));
					//Console.WriteLine("CommunicationConnector connected to remote endpoint.");
				} catch (ArgumentNullException ane) {
					Console.WriteLine ("ArgumentNullException : {0}", ane.ToString());
				} catch (SocketException se) {
					Console.WriteLine ("SocketException : {0}", se.ToString());
				} catch (Exception e) {
					Console.WriteLine ("Unexpected exception : {0}", e.ToString());
				}
			} catch (Exception e) {
				Console.WriteLine (e.ToString ());
			}
			Console.WriteLine ("CommunicationConnector :: up and running !!");
		}
        // GenericNetworking wrapz
        public string read()
        {
            // GenericNetworking *blocking read*
            String data = network.__read(connector);
            return data;
        }
        public int write(String message)
        {
            // GenericNetworking *non-blocking write*
            int b = network.__write(connector, message);
            Console.WriteLine("CommunicationConnector::write data={0} bytes={1}", message, b);
            return b;
        }
    }
}

