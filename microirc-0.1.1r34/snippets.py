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

# TEXT_TO_SPEECH, #Alpha2 MODULARIZE! ie. write hooks.
		try:
			from espeak import espeak
			espeak.synth(message)
		except:
			logging.info('Install python3-espeak to get text-to-speech module to work.')

