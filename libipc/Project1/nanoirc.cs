using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nanoirc
{
    class nanoirc
    {
        // 
        private static Socket ConnectionRemote;
        private static Socket ConnectionBridge;
        // 
        public static void Main()
        {
            try {
                // IRC
                ConnectionRemote = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPHostEntry host = Dns.GetHostEntry("dreamhack.se.quakenet.org");
                foreach(IPAddress ip in host.AddressList)
                {
                    ConnectionRemote.Connect(new IPEndPoint(ip, 6667));
                    Console.WriteLine("Connection to {0} established.", ip);
                };
                // IPC
                ConnectionBridge = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
                ConnectionBridge.Connect(new IPEndPoint(IPAddress.Parse("::1"), 6669));
                Console.WriteLine("CommunicationConnector connected to remote endpoint.");
                // bridge
                while(true) {
                    Thread.Sleep(60);
                }
            // 
            } catch(Exception ex) {
                Console.WriteLine(ex.ToString());
            }
            ConnectionRemote.Shutdown(SocketShutdown.Both);
            ConnectionRemote.Close();
            ConnectionBridge.Shutdown(SocketShutdown.Both);
            ConnectionBridge.Close();
        }
    }
}
