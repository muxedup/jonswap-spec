CC = clang++ 
CFLAGS = -stdlib=libc++ -std=gnu++11 -Wall

debug: jonswapTest_dbg.o jonswapSpec_dbg.o
	$(CC) $(CFLAGS) -g -o jonswap_dbg jonswapTest_dbg.o jonswapSpec_dbg.o
	
jonswapTest_dbg.o: jonswapTest.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c -g -o jonswapTest_dbg.o jonswapTest.cpp 

jonswapSpec_dbg.o: jonswapSpec.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c -g -o jonswapSpec_dbg.o jonswapSpec.cpp 

jonswap: jonswapTest.o jonswapSpec.o
	$(CC) $(CFLAGS) -o jonswap jonswapTest.o jonswapSpec.o
	
jonswapSpec.o:  jonswapSpec.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c jonswapSpec.cpp
	
jonswapTest.o: jonswapTest.cpp jonswapSpec.h
	$(CC) $(CFLAGS) -c jonswapTest.cpp 
	
clean:
	rm *.o
	rm *.txt