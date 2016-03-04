using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

namespace libipc
{
    class nanoirc
    {
        [STAThread]
        public static void Main(string[] args)
        {
            //Application.EnableVisualStyles();

            // 
            Console.WriteLine("Chaos Server #Alpha6");
            // libipc 
            CommunicationServer CommunicationHub = new CommunicationServer();
            Thread CommunicationHubThread = new Thread(CommunicationHub.loop);
                   CommunicationHubThread.Start();
            // nanoirc

        }
    }
}
