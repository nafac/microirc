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
        //
        // 
        public static void Main()
        {
            // 
            Socket ConnectionRemote;
            Socket ConnectionBridge;
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
                Console.WriteLine("nanoirc connected to remote endpoint.");
                // handshake

                // blocking bridge
                static_route_io(ConnectionRemote, ConnectionBridge);
                // 
                ConnectionRemote.Shutdown(SocketShutdown.Both);
                ConnectionRemote.Close();
                ConnectionBridge.Shutdown(SocketShutdown.Both);
                ConnectionBridge.Close();
            // 
            } catch(Exception ex) {
                Console.WriteLine(ex.ToString());

            }
        }
        private static void static_route_io(Socket one, Socket two)
        {
            while (true) {
                __write(two, __read(one));
                __write(one, __read(two));
            }
        }
        // A blocking read, implement another with select.
        public static string __read(Socket s)
        {
            // 
            byte[] bytes = new byte[1024];
            int bytes_received = 0;
            // 
            try
            {
                bytes_received = s.Receive(bytes);
                return Encoding.UTF8.GetString(bytes, 0, bytes_received);
                // 
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                return null;
            }
        }
        // Writes are always non-blocking.
        public static void __write(Socket s, String message)
        {
            //
            string formatted_message;
            byte[] formatted_message_byte;
            int bytes_sent = 0;
            // 
            try
            {
                formatted_message = String.Join("", message, "\n\r<EOF>");
                formatted_message_byte = Encoding.UTF8.GetBytes(formatted_message);
                bytes_sent = s.Send(formatted_message_byte);
                Console.WriteLine("CommunicationConnector::__write data={0} bytes={1}", formatted_message, bytes_sent);
                return;
                // 
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                return;
            }
        }
    }
}
