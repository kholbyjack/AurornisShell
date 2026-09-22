# object creation
aurornisbash: aurornisbash.c
	gcc -o aurornisbash.c

# change to install to /bin/, may want to make this a variable 
install: aurornisbash
	install -m 755 aurornisbash /home/osc/assignments/luckynumber/aurornisbash

# uninstall from the bin
uninstall: 
	rm -f /home/osc/assignments/luckynumber/aurornisbash
	
# clean
clean: 
	rm -f aurornisbash