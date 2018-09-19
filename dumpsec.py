import sys
try:
	import pefile
except ImportError:
	print 'You have to install pefile (pip install pefile)'
	sys.exit()

def main():
	if len(sys.argv) < 2:
		print 'usage: dumpsec.py program.exe'
		return

	pe = pefile.PE(sys.argv[1], fast_load=True)
	data = pe.get_memory_mapped_image()
	i = 0
	for section in pe.sections:
		print (section.Name, hex(section.VirtualAddress), hex(section.Misc_VirtualSize), section.SizeOfRawData )
		secdata = data[section.VirtualAddress:section.VirtualAddress+section.Misc_VirtualSize]
		f = open('section%02d%s.bin' % (i, section.Name.replace('\0', '').replace('\x0a', '')), 'wb')
		f.write(secdata)
		f.close()
		i += 1

if __name__ == "__main__":
	main()