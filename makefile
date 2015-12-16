commutor control : main.o control.o
	g++ -D_REENTRANT main.o -o commutor -lpthread
	g++ -D_REENTRANT control.o -o control -lpthread

main.o : main.cpp udp_sender.h udp_receiver.h

control.o : control.cpp udp_sender.h

clean :
	-rm *.o
	-rm *.bin
	-rm *.gch
	-rm *.img
	-rm commutor
	-rm control

check :
	commutor -version
	commutor -help
	commutor -start

install :
	echo ""
	echo ========================================
	echo          INSTALLING COMMUTOR
	echo ========================================
	echo *** COPY PROGRAM TO /bin
	sudo cp commutor /usr/bin/
	echo *** ADD PROGRAM TO START WITH BOOT LIST
	sudo echo "\r\n \r\n "
	echo *** DONE
	echo ========================================
	echo ""
