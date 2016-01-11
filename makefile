CC = clang++ 
CFLAGS = -stdlib=libc++ -std=gnu++11 -Wall

jonswap: jonswapTest.o jonswapSpec.o
	$(CC) $(CFLAGS) -o jonswap jonswapTest.o jonswapSpec.o
	
jonswapSpec.o:  jonswapSpec.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c jonswapSpec.cpp
	
jonswapTest.o: jonswapTest.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c jonswapTest.cpp 
	