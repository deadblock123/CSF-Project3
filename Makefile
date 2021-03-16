CC = g++
CFLAGS = -Wall -g

#TARGETS FOR THE MAKEFILES
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
csim: cppCacheMain.o
	$(CC) $(CFLAGS) -o csim cppCacheMain.o

cppCacheMain.o: cppCacheMain.cpp
	$(CC) $(CFLAGS) -cpp cppCacheMain.cpp

clean:
	$(RM) count *.o *~

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

