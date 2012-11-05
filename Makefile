all: compile run-1 run-1

compile:
	gcc -o rgb-test-1 SFRGBPiTest1.c -L /usr/local/lib -l wiringPi -Wall
	gcc -o rgb-test-2 SFRGBPiTest2.c -L /usr/local/lib -l wiringPi -Wall

run-1:
	sudo ./rgb-test-1

run-2:
	sudo ./rgb-test-2
