CC = g++
CFLAGS = -Wall -g

#TARGETS FOR THE MAKEFILES
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
csim: cppCacheMain.cpp
	$(CC) $(CFLAGS) -o csim cppCacheMain.cpp

clean:
	$(RM) count *.o *~

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

