#!/usr/bin/python3
# -*- coding: UTF-8 -*-
def text2chat(self, text):
	if not text: return
	chat = []
	for line in text.split('\n'):
		msg = line.split(' ')
		if len(msg) <= 1: continue
		# NOPE #
		# if msg[0] == 'PING':
		#	self.sock.sendall(('PONG %s' % msg[1]).encode())
		#	continue #Alpha2todo Move to IRClib or microirc.
		chat.append(line.rstrip())
	return chat;

# Beginning of Internet Relay Chat (µirc -> world) commands:
def message(self, pad_num, message):
	self.bufferi += 'PRIVMSG %s :%s\r\n' % (str(self.windowmanager.network[pad_num][1]).strip(), str(message).strip())
	self.handle_write() # instant communication please
	#self.windowmanager.updatepad(pad_num, '%s <%s> %s' % (datetime.now().strftime("%H:%M:%S"), self.nickname, message.strip()))
	return
def join(self, channel):
	self.bufferi += 'JOIN %s\r\n' % channel.strip()
	self.handle_write() # instant communication please
	return True
def part(self, pad_num, message):
	channel = self.windowmanager.removeid(pad_num)
	if channel != False:
		self.bufferi += 'PART %s :%s\r\n' % (channel.strip(), message.strip())
		self.handle_write() # instant communication please
		return True
	return False
def quit(self, message):
	self.bufferi += 'QUIT :%s\r\n' % message.strip()
	self.handle_write() # instant communication please
	return True
def nick(self, nickname):
	self.nickname = nickname
	self.bufferi += 'NICK %s\r\n' % nickname.strip()
	self.handle_write() # instant communication please
	return True

# Auxiliary Internet Relay Chat channel methods:
def broadcast(self, message): # "Broadcast" message into all pads.
	for i in range(1, len(self.windowmanager.network)):
		self.windowmanager.updatepad(i, message)
	return

# Experimental SPEECH_RECOGNITION #Alpha2 modularize!
#threading.Thread(target=self.speech).start()
def speech(self):
	while True:
		try:
			import pyaudio
			import speech_recognition as sr
			r = sr.Recognizer()
			with sr.Microphone() as source:
				audio = r.listen(source)
			try:
				logging.info('User said: "%s"' % (r.recognize(audio)))
			except LookupError:
				logging.info('Could not understand audio')
		except:
			logging.info('Install SpeechRecognition + PyAudio to get speech recognition module to work.')
			logging.debug(traceback.format_exc())
		time.sleep(1)
	return
'''
# TEXT_TO_SPEECH, #Alpha2 MODULARIZE! ie. write hooks.
	try:
		from espeak import espeak
		espeak.synth(message)
		except:
			logging.info('Install python3-espeak to get text-to-speech module to work.')
'''

## Auxiliary methods.
def kill(self): # Portable os.kill
	self.cmdwin.getch()
	curses.endwin() # Clear curses window to make terminal work again.
	os.kill(os.getpid(), signal.SIGKILL)
	return os.getpid()	
def implode(self, implodable, begin, end): # Make a custom string out of list. #Alpha2todo
	try:
		message = ''
		for i in range(begin, end):
				message = str.join(' ', (message, implodable[i]))
		return message.strip(' ').strip(':')
	except:
		logging.debug(traceback.format_exc())
	return

