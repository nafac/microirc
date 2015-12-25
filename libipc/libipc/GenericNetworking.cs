using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace libipc
{
    // Asynchronous sockets data.
    public class StateObject
    {
        public Socket WorkSocket = null;
        public const int buffersize = 1024;
        public byte[] buffer = new byte[buffersize];
        public StringBuilder sb = new StringBuilder();
    }
    // GenericNetworking
    class GenericNetworking
    {
        public GenericNetworking()
        {
            // foo foo
        }
        public void CommunicationBridge(Socket Endpoint1, Socket Endpoint2)
        {
            Console.WriteLine("CommunicationBridge: Endpoint1={0} Endpoint2={1}", Endpoint1, Endpoint2);
            while (true)
            {
                __write(Endpoint2, __read(Endpoint1));
                __write(Endpoint1, __read(Endpoint2));
            }
        }
        // A blocking read, implement another with select.
        public String __read(Socket s)
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
        public int __write(Socket s, String message)
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
                Console.WriteLine("GenericNetworking::__write data={0} bytes={1}", formatted_message, bytes_sent);
                return bytes_sent;
                // 
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                return -1;
            }
        }
    }
}
