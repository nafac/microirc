using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace libipc
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Chaos Server #Alpha6");
			// CommunicationHub
			CommunicationServer CommunicationHub = new CommunicationServer ();
			Thread CommunicationHubThread = new Thread (CommunicationHub.loop);
			CommunicationHubThread.Start ();
		/*	// CommunicationConnectors are embedded in fault proof modules.
		 *	CommunicationConnector CommunicationServer = new CommunicationConnector ();
		 *	Thread CommunicationConnectorThread = new Thread(CommunicationServer.main_loop);
		 *	CommunicationConnectorThread.Start ();
		 */
		}
	}
	/*
	public class ThreadDispatcher
	{
		public ThreadDispatcher()
		{
			Console.WriteLine ("ThreadDispatcher :: up and running !!");
		}
		public void ThreadCreate (String module)
		{
			
		}
		private void ThreadManager ()
		{
			
		}
		private class ConnectorBridge
		{
			public ConnectorBridge ()
			{
				Console.WriteLine ("ConnectorBridge :: up and running !!");
			}
			private void TransferData()
			{
				
			}
		}
	}
	*/
	/*
	// CommunicationServer IPC-Hub <=> CommunicationConnector module static router bridge.
	public class ConnectorBridge
	{
		// Variables
		private Socket Hub;
		private Socket Module;
		private String IO;
		// Functions
		public ConnectorBridge (Socket hub, Socket connector)
		{
			Hub = hub;
			Module = connector;
			Console.WriteLine ("ConnectorStaticRouter :: up and running !!");
			Console.WriteLine ("Connection should be paired now !!");
			// Thread-i-fy.
			while (true) { __connector_static_router (); }
		}
		// C++ Functions
		private int __connector_static_router()
		{
			if (__flush_read (Module) > 2)
				__flush_write (Hub);
			if (__flush_read (Hub) > 2)
				__flush_write (Module);
			return 0;
		}
		private int __flush_read(Socket fd)
		{
			IO = "";
			byte[] bytes = new byte[1024];
			int bytes_received = fd.Receive (bytes);
			IO = Encoding.UTF8.GetString (bytes, 0, bytes_received);
			return bytes_received;
		}
		private int __flush_write(Socket fd)
		{
			byte[] msg = Encoding.UTF8.GetBytes(IO);
			int bytes_sent = fd.Send(msg);
			IO = "";
			return bytes_sent;
		}
	}
	*/

	// Asynchronous sockets data.
	public class StateObject
	{
		public Socket			WorkSocket = null;
		public const int		buffersize = 1024;
		public byte[]			buffer = new byte[buffersize];
		public StringBuilder	sb = new StringBuilder();
	}
}
