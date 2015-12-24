using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;
using System.Threading;

namespace libipc
{

	public class CommunicationServer
	{
		// Advanced Micro Devices
		private static ThreadDispatcher ThreadManager;
		// Thread signal handler.
		public static ManualResetEvent allDone = new ManualResetEvent(false);
		public CommunicationServer ()
		{
            ThreadManager = new ThreadDispatcher ();
			//Console.WriteLine ("CommunicationServer :: up and running !!");
		}
		public void loop()
		{
			Socket listener = new Socket (AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
			try {
				// 
				listener.Bind (new IPEndPoint(IPAddress.Parse ("::1"), 6669));
				listener.Listen (128);
				// 
				while (true)
				{
					//Console.WriteLine ("CommunicationServer :: Waiting for a connection ..");
					allDone.Reset ();
					listener.BeginAccept (new AsyncCallback (AcceptCallback), listener);
					allDone.WaitOne ();
					//#Alpha6TODO :: maybe spawn IRCLib here, process control
				}
			} catch (Exception e) {
				Console.WriteLine (e.ToString ());
			}
		}
		private static void AcceptCallback(IAsyncResult ar)
		{
			allDone.Set (); // thread signal handler
			Socket listener	= (Socket)ar.AsyncState;
			Socket handler = listener.EndAccept (ar);
			StateObject state = new StateObject ();
			state.WorkSocket = handler;

			Console.WriteLine ("CommunicationServer :: connection accepted ..");
			Send (handler, "Welcome to nafac.co IPC-Hub !! \n\r<EOF>");

			// IPC-Hub handshake
			/*
			byte[] bytes = new byte[1024];
			int bytesRec = handler.Receive(bytes);
			string ret   = Encoding.UTF8.GetString (bytes, 0, bytesRec);
			*/
			// handshake
			// 	!!
			// AsyncRead
			handler.BeginReceive (state.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), state);

			//#Alpha6TODO ::
			//	0. get module type and hash.
			//	1. the highly secret handshake.
			//	2. pair hashed modules.
			//	3. bridge hashed modules.
		}
		// HEY DUDE, USE PIPES C++, CONTAIN DATA, series of tubes, etc.
		private static void ReadCallback(IAsyncResult ar)
		{
			String content = String.Empty;
			//Console.WriteLine ("CommunicationServer :: reading ..");
			StateObject ConnectorState = (StateObject)ar.AsyncState;
			Socket handler = ConnectorState.WorkSocket;
			try {
				int bytes_read = handler.EndReceive (ar);
				if (bytes_read > 0) {
					// There might be more fata, so store the data received so far.
					ConnectorState.sb.Append (Encoding.UTF8.GetString (ConnectorState.buffer, 0, bytes_read));
					// Check for end-of-line tag. If it is not there => read more data.
					content = ConnectorState.sb.ToString ();
					//Console.WriteLine ("CommunicationServer :: received {0}", content);
					//if (content.IndexOf ("\n\r") > -1) {
					if (content.IndexOf ("<EOF>") > -1) {
						//Console.WriteLine ("CommunicationServer :: received {0}", content);

						// I still smoke hack every day ..
						//Array.Clear(ConnectorState.buffer, 0, ConnectorState.buffer.Length);
						ConnectorState.sb.Clear();

						// .. echo ..
						//Send (handler, content);

						// SPLIT
						// these strings are dumb (or dead if you say)..
						string[] delim		= new string[] {"\n"};
						string[] commands	= content.Split(delim, StringSplitOptions.RemoveEmptyEntries);
						// parser
						// Just skip all nonsense,
						foreach (string s in commands) {
							if(s.Length < 3 || s.Contains("<EOF>"))
								continue;
							Console.WriteLine("CommunicationConnector :: {0}", String.IsNullOrEmpty(s) ? "<>" : s);
                            // module detection
                            // 
                            // module_irc
							if(s.Contains("NOTICE AUTH")) {
                                ThreadManager.CreateModule(handler, s);     // bridge builder part 1 ..
								return;                                     //#Alpha6 WIP => bridged rail
							}
                            // module_pairing
							if(s.Contains("IPCH")) {
                                // synchronize threads
                                Thread.Sleep(30);
                                // pair modules
                                string[] i_deli = new string[] { ":", "\n", "\r" };
                                string[] i_hash = content.Split(i_deli, StringSplitOptions.RemoveEmptyEntries);
                                ThreadManager.AddEndpoint(handler, i_hash[1]);
																		// bridge builder part 2 ..
								// ConnectModuleBridge(handler, hash);	// hashed module can serve any amount of connections.
								// 
								// EnableModuleBridge(handler, hash);	// new disposable container to pass data P-to-P in two directions.
								//// new disposable container to pass the data
								//// one container ONLY transfers data in TWO DIRECTIONS !!
								//// 1 <=> 2
								//// 1 <=> 3
								//// 1 <=> 4
								//// each in own "disposable" thread
								//// but remember to manage object lifetime !! ServerLifetime monitor.
							}
							// detect irc
							// spawn thread / object
							// detach from CommunicationServer

							/*
							if(s.Contains("IPC_IDENT")) {
								Console.WriteLine("initializing module pairing ..");
							}
							if(s.Contains("IPC_SPAWN")) {
								// 
								Console.WriteLine("initializing module spawning ..");
								// 
							}
							*/
							// each pipe has own objects (with Super Secret MicroSoft AI intelligence and memory)
							//ProcessControl(s);
							// It is a server though, LOL!!
							//ConnectorState.WorkSocket
							// pair sockets

							// defer asyncsockets

							// enter blocking mode
						}
						// WRITE REPLY

						handler.BeginReceive (ConnectorState.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), ConnectorState); // read more data when finished
					} else {
						handler.BeginReceive (ConnectorState.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), ConnectorState);
					}
				}
			} catch(SocketException) {
				//handler.Shutdown (SocketShutdown.Both);
				handler.Close ();
				Console.WriteLine ("CommunicationConnector disconnected");
			}
			// I smoke hack every day.
			//handler.BeginReceive (ConnectorState.buffer, 0, StateObject.buffersize, 0, new AsyncCallback (ReadCallback), ConnectorState);
		}
		private static void Send(Socket handler, String data)
		{
			byte[] byte_data = Encoding.UTF8.GetBytes (data);
			handler.BeginSend(byte_data, 0, byte_data.Length, 0, 
				new AsyncCallback(SendCallback), handler);
		}
		private static void SendCallback(IAsyncResult ar)
		{
			try {
				Socket handler = (Socket) ar.AsyncState;
				handler.EndSend(ar);
				//Console.WriteLine("Sent bytes to client.");
			} catch(Exception e) {
				Console.WriteLine (e.ToString ());
			}
		}
	}
    public class ThreadDispatcher
    {
        // 
        private List<Module> ModuleManager = new List<Module>();
        // 
        public ThreadDispatcher()
        {
            //Console.WriteLine("ThreadDispatcher :: up and running !!");
        }
        // create new hashed module, connect it later
        public string CreateModule(Socket handler, String passed_message)
        {
            // 
            DisposableUtilities disposable = new DisposableUtilities();
            String hash;
            // 
            generate_fresh_hash:
            hash = disposable.GetHash();
            // 
            foreach (var m in ModuleManager)
                if (m.CheckFingerprint(hash) == false) { continue; /* fingerprint is suitable for use */ } else { goto generate_fresh_hash; /* fingerprint found, generate new */ }
            // 
            Console.WriteLine("CreateModule: hash={0}", hash);
            // 
            ModuleManager.Add(new Module(hash, handler, passed_message));
            // debug
            foreach (var author in ModuleManager)
            {
                Console.WriteLine("Author: {0}", author.HashedFingerprint);
            }
            // unused
            return hash;
        }
        public bool AddEndpoint(Socket handler, String hash)
        {
            Console.WriteLine("AddEndpoint: hash={0}", hash);
            foreach (var m in ModuleManager)
            {
                Console.WriteLine("AddEndpoint: hash={0}", hash);
                if (m.CheckFingerprint(hash))
                {
                    m.AddEndpoint(handler);
                    return true;
                }
            }
            return false;
        }
        public class Module
        {
            // 
            public String HashedFingerprint;   // hash
            private Socket Endpoint1;           // module 1
            private Socket Endpoint2;           // module 2
            //
            public Module(String hash, Socket handler, String first_message)
            {
                // 
                HashedFingerprint   = hash;     // hash
                Endpoint1           = handler;  // module 1
                // detect modules type
                // 
                // create module
                modirc mod      = new modirc(hash, first_message);
                Thread mod_t    = new Thread(mod.thread);
                       mod_t.Start();
                // 
            }
            public bool AddEndpoint(Socket handler)
            {
                // 
                Endpoint2 = handler;  // module 2
                // 
                if(Endpoint1 != null && Endpoint2 != null)
                {
                    Console.WriteLine("AddEndpoint is valid !!");
                    return true;
                } else {
                    Console.WriteLine("AddEndpoint is invalid !!");
                    return false;
                }
            }
            public bool CheckFingerprint(String hash)
            {
                Console.WriteLine("CheckFingerprint called");
                if (HashedFingerprint.Contains(hash)) { return true; } else { return false; }
            }
        }
    }
/*
		public class Module
		{
			// 
			private String HashedFingerprint;
			private Socket Endpoint1;
			private Socket Endpoint2;
            // 
            public Module(Socket handler, string hash)
            {
                // TESTING THE BRIDGE
                String message = String.Join("", "bridge builder\n\r<EOF>");
                byte[] msg = Encoding.UTF8.GetBytes(message);
                int bytes_sent = handler.Send(msg);
                //Console.WriteLine ("CommunicationConnector::__write data={0} bytes={1}", message, bytes_sent);

                // CreateBridgeStub()
                //
                // module	= "Module_IRC";
                // hash		= unique random hash

                // create the module

                // Grab socket and pass it as a variable, or just do it again and again and again
                // pair them
                // CreateModuleBridge(); ie. CreateBridgeStub();

                Console.WriteLine("Module({0}) :: up and running !!", HashedFingerprint);
            }
			//
			public void AddEndpoint(Socket s) {
                // ConnectBridge
                Endpoint2 = s;
                Console.WriteLine ("CommunicationServer paired up the connection !! ");
			}
		}
	}
    */
    // CommunicationServer::CommunicationRouter
    public class CommunicationBridge
    {
        GenericNetworking net;
        Socket one, two;
        public CommunicationBridge(Socket fd_one, Socket fd_two)
        {
            // 
            net = new GenericNetworking();
            // 
            one = fd_one;
            two = fd_two;
            // 
            return;
        }
        public void __transfer_io()
        {
            while (true)
            {
                net.__write(two, net.__read(one));
                net.__write(one, net.__read(two));
            }
        }
    }
}

