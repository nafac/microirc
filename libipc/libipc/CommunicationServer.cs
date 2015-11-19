using System;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;
using System.Collections.Generic;
using System.Threading;

namespace libipc
{

	public class CommunicationServer
	{
		// Advanced Micro Devices
		private ThreadDispatcher ThreadPoolManager;
		// Thread signal handler.
		public static ManualResetEvent allDone = new ManualResetEvent(false);
		public CommunicationServer ()
		{
			ThreadPoolManager = new ThreadDispatcher ();
			Console.WriteLine ("CommunicationServer :: up and running !!");
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
							//#Alpha6 :: module detection
							if(s.Contains("NOTICE AUTH")) {
								/*
								// new IRC server connection
								String hash = HashModule();
								CreateModule("Module_IRC", hash);		// spawn fingerprinted module.
								// CreateModuleBridge(handler, hash);	// connectable object.
								return;									// detach from IPC-Hub.
								*/
								ThreadDispatcher.ModuleCreate(handler);	// bridge builder part 1 ..
								return; // detach from IPCHub
							}
							//#Alpha6 :: process pairing
							if(s.Contains("IPCH")) {
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
		/*
		private static void ProcessControl(String command)
		{
			// I do this like, always..
			String[] deli = new string[] { " " };
			String[] args = command.Split(deli, StringSplitOptions.RemoveEmptyEntries);
			if(args[0].Contains("SPAWN")) {
				if (args [1].Contains ("IRCLIB")) {
					
				}
				// pair their networks
			}
		}
		*/
	}
	public class ThreadDispatcher
	{
		// ModuleManager = ConnectorBridge
		/// <summary>
		/// 
		/// </summary>
		static List<Module> ModuleManager = new List<Module>();
		// 
		public ThreadDispatcher()
		{
			Console.WriteLine ("ThreadDispatcher :: dummy :: up and running !!");
		}
		// 
		public static int ModuleCreate(Socket handler)
		{
			// create module with unique hash
			string hash = RandomHash();
			foreach (Module m in ModuleManager) {
				do {
					hash = RandomHash();
				} while(m.CheckFingerprint(hash));
			}
			// add module to manager list
			ModuleManager.Add (new Module (handler, hash));
			return 0;
		}
		public int ModulePairing(String hash)
		{
			return 0;
		}
		public class Module
		{
			// 
			private String HashedFingerprint;
			private Socket Endpoint1;
			private Socket Endpoint2;
			// 
			public Module(Socket handler, string hash)
			{
				// store data
				HashedFingerprint	= hash;
				Endpoint1			= handler;

				// TESTING THE BRIDGE
				String message = String.Join ("", "bridge builder\n\r<EOF>");
				byte[] msg = Encoding.UTF8.GetBytes(message);
				int bytes_sent = handler.Send(msg);
				//Console.WriteLine ("CommunicationConnector::__write data={0} bytes={1}", message, bytes_sent);

				// CreateBridgeStub()
				//
				// module	= "Module_IRC";
				// hash		= unique random hash

				// create the module
				Module_IRC IRC = new Module_IRC (hash);
				Thread mod = new Thread (IRC.main);
				mod.Start ();

				// Grab socket and pass it as a variable, or just do it again and again and again
				// pair them
				// CreateModuleBridge(); ie. CreateBridgeStub();

				Console.WriteLine ("Module({0}) :: up and running !!", HashedFingerprint);
			}
			//
			public void ModulePair(String hash)
			{
				// ConnectBridge
				Console.WriteLine ("ModulePair :: up and running !!");
			}
			public bool CheckFingerprint(String hash)
			{
				if (HashedFingerprint.Contains (hash)) {
					Console.WriteLine ("unique fingerprint found in db, pair it !");
					return true;
				} else {
					Console.WriteLine ("unique fingerprint created.");
					return false;
				}
			}
		}
		// ======================================================================================
		public static string RandomHash()
		{
			Random rnd = new Random();
			int rnd_n = rnd.Next(Int32.MinValue, Int32.MaxValue);
			using (MD5 md5Hash = MD5.Create()) {
				string hash = GetMD5Hash(md5Hash, rnd_n.ToString());
				Console.WriteLine("The MD5 hash=" + hash +".");
				return hash;
			}
		}
		private static string GetMD5Hash(MD5 md5Hash, string input)
		{
			byte[] data = md5Hash.ComputeHash(Encoding.UTF8.GetBytes(input));
			StringBuilder sBuilder = new StringBuilder ();
			for (int i = 0; i < data.Length; i++) {
				sBuilder.Append (data [i].ToString ("x2"));
			}
			return sBuilder.ToString ();
		}
		// ======================================================================================
	}
}

