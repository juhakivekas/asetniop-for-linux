#!/usr/bin/env python3
# This script will be used to extract data from the JSON keymap whenever
# needed. At the moment a minimal keymap is being filtered, basically just
# an array of characters that can be indexed by the integer that is the 
# binary representation of an 8-touch chord.

import json

#load the json file in its entirety
keymap = json.loads(open('test-keymap.json', 'r').read(-1))

#flatten the dict into an array
keymap = [mapping[1] for mapping in sorted(list(keymap.items()), key=lambda k: int(k[0]))]

#filter and write a keymap
minimal = open('test-keymap-minimal.dat', 'w')
minimal.write('\x00')
for mapping in keymap[1:256]:
	if list(mapping.keys()).count('base') != 0:
		minimal.write(mapping['base'])
	else:
		minimal.write('\x00')
