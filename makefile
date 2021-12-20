# eenvoudige makefile
all: hoofd.o koffiebord.o
	g++ -Wall -Wextra -o hetspel koffiebord.o hoofd.o
koffiebord.o: koffiebord.cc koffiebord.h
	g++ -Wall -Wextra -c koffiebord.cc
hoofd.o: hoofd.cc koffiebord.h
	g++ -Wall -Wextra -c hoofd.cc
# eenvoudige makefile
# all: hoofd.o koffiebord.o stapel.o
# 	g++ -Wall -Wextra -o hetspel koffiebord.o hoofd.o stapel.o
# koffiebord.o: koffiebord.cc koffiebord.h
# 	g++ -Wall -Wextra -c koffiebord.cc
# hoofd.o: hoofd.cc koffiebord.h stapel.h
# 	g++ -Wall -Wextra -c hoofd.cc
# stapel.o: stapel.cc stapel.h koffiebord.h
# 	g++ -Wall -Wextra -c stapel.cc