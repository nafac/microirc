#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import sys, string, traceback
import socket, asyncore
import logging, threading

class network(asyncore.dispatcher):
	def __init__(self, iobuffer, nick, port):
		logging_format = '%(asctime)-15s: %(message)s'
		logging.basicConfig(filename='debug.log', format=logging_format, level=logging.DEBUG)
		
		self.iobuffer = iobuffer # These locate in microirc now.
		self.address = iobuffer[2]
	
		asyncore.dispatcher.__init__(self)
		self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
		self.connect((self.address, port))
		
		self.iobuffer[0].append('USER microirc +xi µirc: µirc')
		self.iobuffer[0].append('NICK %s' % nick)
		
		threading.Thread(target=asyncore.loop).start()
		
		logging.info('Connected to %s:%s.' % (address, str(port)))
		return
		
# overwritten asyncore methods:
	def handle_connect(self):
		pass
	
	def handle_close(self):
		self.close()
	
	def handle_read(self):
		try: # This is the second one we try..
			rcv = self.recv(8192).decode().split('\n')
			
			for lmsg in rcv:
				rmsg = lmsg.split(' ')
				if len(rmsg) <= 1:
					return
			
				if rmsg[0] == 'PING':
					self.iobuffer[0].append('PONG %s' % rmsg[1]) #Alpha2todo - Functionalize direct write.
			
				self.iobuffer[1].append(lmsg.rstrip())
				logging.debug('Received "%s" from server.' % lmsg.rstrip())			
				continue

			return
		except:
			logging.debug(traceback.format_exc())
			return
		
	def writable(self):
		return (len(self.iobuffer[0]) > 0)
	
	def handle_write(self):
		for i in self.iobuffer[0]:
			logging.debug('Sent "%s" to server.' % (i.rstrip()))
			n = '%s\n\r' % i.rstrip()
			self.send(n.encode('utf-8'))
			self.iobuffer[0].pop(self.iobuffer[0].index(i))
		return
