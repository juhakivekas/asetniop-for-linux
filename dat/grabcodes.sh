#this does some processing to get some magic numbers into the filtering script

cat /usr/include/linux/input.h \
|grep 'KEY_[A-Z]\s' \
|sed 's/.*KEY_\([A-Z]\)\s*\([0-9]*\)/\t"\1": \2,/' \
|tr '[:upper:]' '[:lower:]'
