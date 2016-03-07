#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import socket, ssl, traceback, logging

class network():
	def __init__(self):
		# libipc client 
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
		self.sock.connect(("127.0.0.1", 6669));
		while True:
			data = self.sock.recv(8192).decode();
			if not data: continue;
			print(data);
		return;
		# standalone client
		'''
		logging_format = '%(asctime)-15s: %(message)s'
		logging.basicConfig(filename='debug.log', format=logging_format, level=logging.DEBUG)
		
		self.address = address
		self.port = port
		self.nickname = nickname
		
		#self.bufferi = [] #Alpha1
		self.buffero = []
		
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		if port == 6697 or port == 7001:
			sock = ssl.wrap_socket(sock, ciphers="HIGH:-aNULL:-eNULL:-PSK:RC4-SHA:RC4-MD5", ssl_version=ssl.PROTOCOL_SSLv23, cert_reqs=ssl.CERT_NONE, ca_certs='/etc/ssl/certs/ca-bundle.crt')
		sock.connect((self.address, self.port))
		
		self.sock = sock
		
		self.send('USER microirc +xi µirc: µirc\n\r')
		self.send('NICK %s\n\r' % (self.nickname))
		
		return
		'''
	
	def loop(self):
		while True:
			try:
				# Write buffer:
				''' #Alpha1
				for message in self.bufferi:
					self.sock.sendall(('%s\n\r' % (message)).encode())
					self.bufferi.remove(message)
					logging.info('sent: %s to %s:%s.' % (line.rstrip(), self.address, self.port))
				'''
				# Read buffer:
				data = self.sock.recv(8192).decode()
				if not data: break
				for line in data.split('\n'):
					msg = line.split(' ')
					if len(msg) <= 1: continue
					if msg[0] == 'PING':
						self.sock.sendall(('PONG %s' % msg[1]).encode())
						continue #Alpha2todo Move to IRClib or microirc.
					self.buffero.append(line.rstrip())
					logging.info('received: %s from %s:%s.' % (line.rstrip(), self.address, self.port))
			except:
				logging.debug(traceback.format_exc())
		sock.close()
		return
	def send(self, message):
		self.sock.sendall('%s\n\r' % (message))
		#logging.info('Sent %s to %s:%i.' % (message, self.address, self.port)) #Alpha2 TODO
		return
