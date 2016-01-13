CC = clang++ 
CFLAGS = -stdlib=libc++ -std=gnu++11 -Wall
BINNAME = jonswap

ifeq ($(DEBUG), 1)
    CFLAGS += -g
    BINNAME = jonswap_dbg
endif

jonswap: jonswapTest.o jonswapSpec.o
	$(CC) $(CFLAGS) -o $(BINNAME) jonswapTest.o jonswapSpec.o
	
jonswapSpec.o:  jonswapSpec.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c jonswapSpec.cpp
	
jonswapTest.o: jonswapTest.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c jonswapTest.cpp 
	
clean:
	rm *.o
#rm *.txt