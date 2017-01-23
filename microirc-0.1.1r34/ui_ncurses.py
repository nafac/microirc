#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import curses, threading, time
from datetime import datetime
class ui_ncurses:
	def __init__(self):
		threading.Thread(target=self.__loop).start()
		return;
	def __loop(self):
		self.stdscr = curses.initscr()

		curses.raw()						# Allow CTRL + ALT

		(height, width) = self.stdscr.getmaxyx()

		self.cmdwin = curses.newwin(1, 0, height - 1, 0)
		self.cmdwin.keypad(True)
		self.padwin = curses.newwin(1, 0, height - 2, 0)	#Alpha2TODO
		
		self.nickname = 'microirc'
		self.pads = [] # List of pads.
		self.networks = [] # List of networks.
		pad = self.createpad('default', '')
		
		self.defaultpad = pad[3]
		self.defaultpad.append('begin with /connect <nickname> <address>:<port>.')
		
		# required threads
		threading.Thread(target=self.update_io).start()
		# optional threads; for modules and suchs #Alpha2todo HOOK!
		#
		
		# Accept user keystrokes.
		c = 0
		while c != 17:
			try:
				c = self.cmdwin.getch()
				#self.defaultpad.append('debug: %s' % (str(c)))
				
				if c == 47: # /
					self.command(self.getinput())
				elif (c >= 47 and c <= 57) or (c >= 65 and c <=122): # 0-9 and A-z
					self.usr_privmsg(self.getinput())
				elif (c == 260): # Change pad left.
					self.switchpad(-1)
				elif (c == 261): # Change pad right.
					self.switchpad( 1)
			except:
				logging.debug(traceback.format_exc())
		self.kill() # Kill!
		return
# Window management:
## Window management variables:
	# self.pad_id = 0 # Identification number of the current pad.

## Window management;
	def createpad(self, server, channel): # Create a switchable scrolling pad for each server, channel and user.
		(height, width) = self.stdscr.getmaxyx() # Loop it!
		
		pad = curses.newwin(height - 2, width, 0, 0)
		pad.scrollok(True)
		pad.refresh()
		
		linebuffer = [ ]
		
		self.pads.append( [ server, channel, pad, linebuffer ] )
		self.pad_id = len(self.pads) - 1
		
		return self.pads[self.pad_id]
		
	def switchpad(self, direction): # Change pad; We use this to refresh a pad too!
		pad = self.findpad_by_id(self.pad_id + direction)
		pad[2].clear()
		pad[2].addstr("\n\r".join(pad[3]))
		pad[2].refresh()
		
		#Alpha2todo - Resize pads without breaking input.
		
		self.pad_id = self.pads.index(pad)
		
		self.updatelistpad()
		
		return
	
	def updatelistpad(self):
		self.padwin.clear()
		
		for i in range(0, len(self.pads)):
			if i == self.pad_id:
				self.padwin.addstr(' %i' % (i + 1), curses.A_BOLD)
				continue
			self.padwin.addstr(' %i' % (i + 1))
			
		self.padwin.addstr(' [ %s@%s ]' % (self.pads[self.pad_id][1], self.pads[self.pad_id][0]))
		self.padwin.refresh()
		return
	
## Window management; Auxiliary methods.
	def findpad_by_id(self, pad_id): # Find a pad by identifier.
		if pad_id < 0 or pad_id > (len(self.pads) - 1):
			return self.pads[0]
		return self.pads[pad_id]
	def findpad_by_name(self, server, channel): # Find a pad by server and channel names.
		for pad in self.pads:
			if pad.count(server) and pad.count(channel):
				return pad
		return self.pads[0]

## Window management; User input.
	def getinput(self): # Use command window to accept user input.
		curses.echo()
		
		text = self.cmdwin.getstr() # Get user input
		text = text.decode() # and make it unicode.
		
		curses.noecho()
		
		self.cmdwin.clear()
		#self.cmdwin.refresh()
		
		return text
	def command(self, cmd):
		command = cmd.strip('/').split(' ')
		if len(command) <= 0:
			return
		
		argument = self.implode(command, 1, len(command))
		
		# This is a nice'ish list of all evaluable commands.
		commands = [
		[ 'connect', 'nickname server:port', 2 ],
		[ 'disconnect', '[message]', 0 ],
		[ 'join', '#channel[,#channel]', 1 ],
		[ 'part', '[message]', 0 ],
		[ 'quit', '[message]', 0 ], #Alpha2todo
		[ 'nick', 'nickname', 1 ], #Alpha2todo
		[ 'clear', '[#channel]', 0], #Alpha2todo
		[ 'broadcast', 'message', 1 ]] #Alpha2todo

		# Determine command and advice user to use them.
		for i in commands:
			if i.count(command[0]): # Find matching command.
				if len(command) <= i[2]: # Check if command has sufficient parameters.
					self.defaultpad.append('Use "/%s %s"' % (i[0], i[1]))
				else:
					try:
						# Process messages from user; Evaluate.
						eval('self.usr_%s(argument)' % (i[0]))
						logging.debug('usr_%s("%s") evaluated.' % (i[0], argument))
					#except (AttributeError, SyntaxError):
						#logging.debug('usr_%s() is not implemented.' % i[0]) #Alpha3todo - /msg this to nafac when ready enough.
					except:
						logging.debug(traceback.format_exc())
		return
		
## Buffered I/O between network and user-interface.
	def process_message(self, message):
		self.defaultpad.append(message);
		return;
	def update_io(self): # Process message queue and update pads.
		while True:
			try:
				for network in self.networks:
					for message in network.buffero:
						self.findpad_by_name(network.address, '')[3].append(message)
						network.buffero.remove(message)
						
						# Process messages from server.
						command_array = message.split(' ')
						if len(command_array) < 2:
							continue
					
						# hop, skip, jump
						if len(command_array[1]) < 2 or command_array[1].startswith(':') or command_array[1][0].isdigit():
							#logging.debug('net_%s() is not implemented. MOTD or incompatible server messages.' % command_array[1].lower())
							continue
					
						# Process messages from server; Evaluate.
						try:
							eval('self.net_%s(network, command_array)' % (command_array[1].lower()))
							logging.debug('net_%s(self, %s, %s) evaluated.' % (command_array[1].lower(), network.address, command_array))
						except (AttributeError, SyntaxError):
							logging.debug('net_%s() is not implemented.' % command_array[1].lower()) #Alpha3todo - /msg this to nafac when ready enough.
						except:
							logging.debug(traceback.format_exc())
							
					continue
			except:
				logging.debug(traceback.format_exc())
				
			self.switchpad(0)
			time.sleep(1)
		return
		
	def return_network(self, address): # Find network by name; Return object.
		for net in self.networks:
			if net.address.count(address):
				return net
		return
	
	''' #Alpha2todo
	def remove_network(self, address): # Remove network and all related pads.
		# Remove all related pads.
		for i in range(0, len(self.pads)-1):
			if self.pads[i].count(address):
				self.pads.pop(i)
		# Remove all related networks.
		for i in range(0, len(self.networks)-1):
			if self.networks[i].count(address):
				self.networks.pop(i)
		# Change and refresh pad.
		self.switchpad(0)
		return
	'''
		
## Evaluated user commands:
	def usr_connect(self, args):
		args = re.split(':| ', args)
		
		interface = network(args[1], int(args[2]), args[0])
		threading.Thread(target=interface.loop).start()
		self.networks.append(interface) # Store network interface and dump rest of the evidence. #Alpha2todo Isolate!
		
		self.createpad(args[1], '')
		
		return
	def usr_join(self, args):
		args = re.split(',', args)
		for arg in args:
			pad = self.createpad(self.pads[self.pad_id][0], arg)

			net = self.return_network(pad[0])
			net.send('JOIN %s' % (arg))

		return
	def usr_part(self, args):
		pad = self.findpad_by_id(self.pad_id)
		net = self.return_network(pad[0])
		
		if len(args) > 1:
			net.send('PART %s :%s' % (pad[1], args))
		else:
			net.send('PART %s :%s' % (pad[1], 'µirc is a minimalistic and secure irc client, check it out http://is.gd/1Cb3Oc'))
		
		self.pads.pop(self.pad_id)
		self.switchpad(-1)
			
		return
	def usr_disconnect(self, args): #Alpha2todo
		pad = self.findpad_by_id(self.pad_id)
		net = self.return_network(pad[0])
		
		if len(args) > 1:
			net.send('QUIT :%s' % (args))
		else:
			net.send('QUIT :%s' % ('µirc is a minimalistic and secure irc client, check it out http://is.gd/1Cb3Oc'))
		
		self.remove_network(pad[0])
			
		return
	def usr_privmsg(self, args):
		args = re.split(' ', args)
		message = self.implode(args, 0, len(args))
		
		pad = self.findpad_by_id(self.pad_id)
			
		net = self.return_network(pad[0])
		net.send('PRIVMSG %s :%s' % (pad[1], message))
		
		pad[3].append('%s <%s> %s' % (datetime.now().strftime("%H:%M:%S"), net.nickname, message))
		
		return
		
## Evaluated network commands:
	def net_privmsg(self, net, args):
		user_id = re.split('!', args[0].lstrip(':')) # Functionalize this if necessary.
		message = '%s <%s> %s' % (datetime.now().strftime("%H:%M:%S"), user_id[0], self.implode(args, 3, len(args)))
		
		pad = self.findpad_by_name(net.address, args[2])
		pad[3].append(message)
		
		#Alpha2todo : Modularize text hooks.

		return
