using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace libipc
{
    class GenericNetworking
    {
        public GenericNetworking()
        {
            // foo foo
        }
        // A blocking read, implement another with select.
        public string __read(Socket s)
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
        public void __write(Socket s, String message)
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
