using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Networking;
using Windows.Networking.Sockets;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace nano_irc
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        // 
        public MainPage()
        {
            this.InitializeComponent();
            //
            Communicator CommunicationHub = new Communicator(this);
            CommunicationHub.CommunicationHub();
            //
            Communicator CommunicatorIRC = new Communicator(this);
            CommunicatorIRC.CommunicationBridge("irc.se.quakenet.org", "6667", "localhost", "6669");
            //
            ConsoleAppendDaemon("");
            ConsoleAppendClient("");
        }
        // mirccd
        public async void CreateListener()
        {
            try
            {
                StreamSocketListener socketListener = new StreamSocketListener();
                socketListener.ConnectionReceived += SocketListener_ConnectionReceived;
                await socketListener.BindServiceNameAsync("6669");
            }
            catch (Exception e) { this.ConsoleAppendDaemon(e.ToString()); }
        }
        private async void SocketListener_ConnectionReceived(StreamSocketListener sender, StreamSocketListenerConnectionReceivedEventArgs args)
        {
            try {
                this.ConsoleAppendDaemon("Connection accepted..\n");
                //
                Stream inputStream = args.Socket.InputStream.AsStreamForRead();
                StreamReader inputReader = new StreamReader(inputStream);
                //
                Stream outputStream = args.Socket.OutputStream.AsStreamForWrite();
                StreamWriter outputWriter = new StreamWriter(outputStream);
                //
                String inputBuffer;
                // 
                while (true)
                {
                    // print CommunicationServer passed messages.
                    inputBuffer = String.Join("", await inputReader.ReadLineAsync(), "\n");
                    this.ConsoleAppendDaemon(inputBuffer);
                    // module pairing
                    /*
                    await outputWriter.WriteAsync(inputBuffer);
                    await outputWriter.FlushAsync();
                    */
                }
            } catch (Exception e) { this.ConsoleAppendClient(e.ToString()); }
        }

        // nanoirc
        public async void CreateConnector(string address, string port)
        {
            try
            {
                StreamSocket connectorSocket = new StreamSocket();
                await connectorSocket.ConnectAsync(new HostName(address), port);
                //
                Stream outputStream = connectorSocket.OutputStream.AsStreamForWrite();
                StreamWriter outputWriter = new StreamWriter(outputStream);
                await outputWriter.WriteAsync("Hello World\n");
                await outputWriter.FlushAsync();
                //
                Stream inputStream = connectorSocket.InputStream.AsStreamForRead();
                StreamReader inputReader = new StreamReader(inputStream);
                String inputBuffer = await inputReader.ReadLineAsync();
                //
                this.ConsoleAppendClient(String.Join("", "data=", inputBuffer, "\n\r"));
            } catch (Exception e) { this.ConsoleAppendClient(e.ToString()); }
        }
        private async void CommunicationBridge(string address1, string port1, string address2, string port2)
        {
            try
            {
                // socket one
                StreamSocket connectorSocket1 = new StreamSocket();
                await connectorSocket1.ConnectAsync(new HostName(address1), port1);
                // stream one
                Stream inputStream1 = connectorSocket1.InputStream.AsStreamForRead();
                StreamReader inputReader1 = new StreamReader(inputStream1);
                Stream outputStream1 = connectorSocket1.OutputStream.AsStreamForWrite();
                StreamWriter outputWriter1 = new StreamWriter(outputStream1);
                // socket two
                StreamSocket connectorSocket2 = new StreamSocket();
                await connectorSocket2.ConnectAsync(new HostName(address2), port2);
                // stream two
                Stream inputStream2 = connectorSocket2.InputStream.AsStreamForRead();
                StreamReader inputReader2 = new StreamReader(inputStream2);
                Stream outputStream2 = connectorSocket2.OutputStream.AsStreamForWrite();
                StreamWriter outputWriter2 = new StreamWriter(outputStream2);

                String ConsoleMessage;
                while (true)
                {
                    // display messages
                    ConsoleMessage = await inputReader1.ReadLineAsync();
                    ConsoleAppendClient(String.Join("", ConsoleMessage, "\n"));
                    // Connector => Server
                    await outputWriter2.WriteAsync(String.Join("", ConsoleMessage, "\n"));
                    await outputWriter2.FlushAsync();
                    // Server => Connector
                    /*
                    await outputWriter1.WriteAsync(String.Join("", await inputReader2.ReadLineAsync(), "\n"));
                    await outputWriter1.FlushAsync();
                    */
                }
            }
            catch (Exception e) { this.ConsoleAppendClient(e.ToString()); }
        }

        // UI-manipulation
        public async void ConsoleAppendDaemon(String text)
        {
            await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                this.TextBlockDaemon.Text = this.TextBlockDaemon.Text + text + "\n";
            });
        }
        public async void ConsoleAppendClient(String text)
        {
            await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                this.TextBlockClient.Text = this.TextBlockClient.Text + text + "\n"; ;
            });
        }
    }
}
