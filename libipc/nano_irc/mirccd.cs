using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Windows.UI.Xaml.Controls;

namespace nano_irc
{
    class mirccd
    {
        // 
        private TextBlock UITB;
        private String Console;
        // 
        public mirccd(TextBlock UIControl)
        {
            //
            this.UITB = UITB;
            //
            this.ConsoleAppendLine("nanoIRC daemon #A7");
            // 
            CommunicationServer IPC = new CommunicationServer(UIControl);
        }
        public void ConsoleAppend(String message)
        {
            this.Console = this.Console + message;
            this.ConsoleRefresh();
        }
        public void ConsoleAppendLine(String message)
        {
            this.Console = this.Console + message + "\n\r";
            this.ConsoleRefresh();
        }
        public void ConsoleRefresh()
        {
            this.UITB.Text = this.Console;
        }
    }
}
