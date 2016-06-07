#!/usr/bin/env python3
# This script will be used to extract data from the JSON keymap whenever
# needed. At the moment a minimal keymap is being filtered, basically just
# an array of characters that can be indexed by the integer that is the 
# binary representation of an 8-touch chord.

import json

#these are not magic numbers, they come from <linux/input.h>
#check grabcodes.sh for details
keycode = {
	"q": 16,
	"w": 17,
	"e": 18,
	"r": 19,
	"t": 20,
	"y": 21,
	"u": 22,
	"i": 23,
	"o": 24,
	"p": 25,
	"a": 30,
	"s": 31,
	"d": 32,
	"f": 33,
	"g": 34,
	"h": 35,
	"j": 36,
	"k": 37,
	"l": 38,
	"z": 44,
	"x": 45,
	"c": 46,
	"v": 47,
	"b": 48,
	"n": 49,
	"m": 50,
}

#load the json file in its entirety
keymap = json.loads(open('test-keymap.json', 'r').read(-1))

#flatten the dict into an array
keymap = [mapping[1] for mapping in sorted(list(keymap.items()), key=lambda k: int(k[0]))]

table = [0]* 256
#filter and write a keymap


print('chord',end='\t')
print('UTF-8', end='\t')
print('keycode',  end='\t')
print('chord touches')
print('-----------------------------------')

for mapping in keymap[1:]:
	chord = sum([1<<(i-1) for i in mapping['input']])
	if list(mapping.keys()).count('base') != 0:
		if len(mapping['base']) == 1 and mapping['base'].islower():
			print(hex(chord),end='\t')
			print(mapping['base'], end='\t')
			print(keycode[mapping['base']],  end='\t')
			print(mapping['input'])
			table[chord] = keycode[mapping['base']]
		elif len(mapping['base']) == 1:
			print(hex(chord),end='\t')
			print(mapping['base'], end='\t')
			print('null',  end='\t')
			print(mapping['input'])
			

minimal = open('test-keymap-minimal.dat', 'w')
#minimal.write('\x00')
for char in table:
	minimal.write(chr(char))
