using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nano_irc
{
    class Communicator
    {
        //
        private MainPage UIControl;
        //
        public Communicator(MainPage UIControl)
        {
            this.UIControl = UIControl;
        }
        public async void CommunicationHub()
        {
            this.UIControl.ConsoleAppendDaemon("CommunicationHub :: initializing");
        }
        public async void CommunicationConnector()
        {
            this.UIControl.ConsoleAppendClient("CommunicationConnector :: initializing");
        }
        public async void CommunicationBridge(string EndPoint1_address, string EndPoint1_port, string EndPoint2_address, string EndPoint2_port)
        {
            this.UIControl.ConsoleAppendClient("CommunicationBridge :: initializing");
        }
    }
}
