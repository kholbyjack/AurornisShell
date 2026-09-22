# object creation
all:aurornisbash

aurornisbash: aurornisbash.o
	gcc -o aurornisbash aurornisbash.o

aurornisbash.o: aurornisbash.c
	gcc -c aurornisbash.c

# change to install to /bin/, may want to make this a variable 
install: aurornisbash
	install -m 755 aurornisbash /bin/aurornisbash

# uninstall from the bin
uninstall: 
	rm -f /bin/aurornisbash
	
# clean
clean: 
	rm -f aurornisbash aurornisbash.o