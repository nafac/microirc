#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys, signal, platform, array, traceback, time
import threading
import re, logging, curses
from datetime import datetime
from network import *
from ui_ncurses import *

class microirc:
	def __init__(self):
		# independant threads
		__network = network();
		__ncurses = ui_ncurses();
		# simple message queuing
		while True:
			__ncurses.process_message(__network.queue());
		return;
		'''
		logging_format = '%(asctime)-15s: %(message)s'
		logging.basicConfig(filename='debug.log', format=logging_format, level=logging.DEBUG)
		logging.info('Welcome to new µIRC session.')
		'''

if __name__ == '__main__':
	microirc()
	
