# all: output
# 	./output

# output: Assignment1.o
# 	g++ Assignment1.o -o output -std=c++11 `pkg-config --cflags --libs opencv`

# Assignment1.o: Assignment1.cpp
# 	g++ -c Assignment1.cpp -std=c++11 `pkg-config --cflags --libs opencv`

# clean: 
# 	rm -rf *.o Assignment1.o outputs/Grayscale_empty.jpg output

run_queue:
	make queue_baseline
	make queue_sub_sampling
	make queue_resolution
	make queue_spatial
	make queue_tempora

run_dynamic:
	make dynamic_baseline
	make dynamic_sparse
	make dynamic_sub_sampling
	make dynamic_resolution
	make dynamic_spatial
	make dynamic_temporal
	
allpart2:
	g++ assignment1_2.cpp -o part2.exe -std=c++11 `pkg-config --cflags --libs opencv`

allpart1: 
	g++ Assignment1.cpp -o part1.exe -std=c++11 `pkg-config --cflags --libs opencv`

queue_baseline:
	g++ queue_baseline.cpp -o queue_baseline.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_baseline.exe

queue_spatial:
	g++ queue_spatial.cpp -o queue_spatial.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_spatial.exe 1 >> queue_runtimes.txt
	./queue_spatial.exe 2 >> queue_runtimes.txt
	./queue_spatial.exe 3 >> queue_runtimes.txt
	./queue_spatial.exe 4 >> queue_runtimes.txt
	./queue_spatial.exe 5 >> queue_runtimes.txt
	./queue_spatial.exe 6 >> queue_runtimes.txt

queue_temporal:
	g++ queue_temporal.cpp -o queue_temporal.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv` 
	./queue_temporal.exe 1 >> queue_runtimes.txt
	./queue_temporal.exe 2 >> queue_runtimes.txt
	./queue_temporal.exe 3 >> queue_runtimes.txt
	./queue_temporal.exe 4 >> queue_runtimes.txt
	./queue_temporal.exe 5 >> queue_runtimes.txt
	./queue_temporal.exe 6 >> queue_runtimes.txt

queue_sub_sampling:
	g++ queue_sub_sampling.cpp -o queue_sub_sampling.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_sub_sampling.exe 0 >> queue_runtimes.txt
	./queue_sub_sampling.exe 1 >> queue_runtimes.txt
	./queue_sub_sampling.exe 2 >> queue_runtimes.txt
	./queue_sub_sampling.exe 3 >> queue_runtimes.txt
	./queue_sub_sampling.exe 4 >> queue_runtimes.txt
	./queue_sub_sampling.exe 5 >> queue_runtimes.txt

queue_resolution:
	g++ queue_resolution.cpp -o queue_resolution.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_resolution.exe 1 >> queue_runtimes.txt
	./queue_resolution.exe 2 >> queue_runtimes.txt
	./queue_resolution.exe 3 >> queue_runtimes.txt
	./queue_resolution.exe 4 >> queue_runtimes.txt
	./queue_resolution.exe 5 >> queue_runtimes.txt
	./queue_resolution.exe 6 >> queue_runtimes.txt

dynamic_baseline:
	g++ dynamic_baseline.cpp -o dynamic_baseline.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_baseline.exe >> dynamic_runtimes.txt

dynamic_sparse:
	g++ dynamic_sparse.cpp -o dynamic_sparse.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_sparse.exe >> dynamic_runtimes.txt

dynamic_sub_sampling:
	g++ dynamic_sub_sampling.cpp -o dynamic_sub_sampling.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_sub_sampling.exe 0 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe 1 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe 2 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe 3 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe 4 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe 5 >> dynamic_runtimes.txt

dynamic_resolution:
	g++ dynamic_resolution.cpp -o dynamic_resolution.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_resolution.exe 1 >> dynamic_runtimes.txt
	./dynamic_resolution.exe 2 >> dynamic_runtimes.txt
	./dynamic_resolution.exe 3 >> dynamic_runtimes.txt
	./dynamic_resolution.exe 4 >> dynamic_runtimes.txt
	./dynamic_resolution.exe 5 >> dynamic_runtimes.txt
	./dynamic_resolution.exe 6 >> dynamic_runtimes.txt

dynamic_spatial:
	g++ dynamic_spatial.cpp -o dynamic_spatial.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_spatial.exe 1 >> dynamic_runtimes.txt
	./dynamic_spatial.exe 2 >> dynamic_runtimes.txt
	./dynamic_spatial.exe 3 >> dynamic_runtimes.txt
	./dynamic_spatial.exe 4 >> dynamic_runtimes.txt
	./dynamic_spatial.exe 5 >> dynamic_runtimes.txt
	./dynamic_spatial.exe 6 >> dynamic_runtimes.txt

dynamic_temporal:
	g++ dynamic_temporal.cpp -o dynamic_temporal.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv` 
	./dynamic_temporal.exe 1 >> dynamic_runtimes.txt
	./dynamic_temporal.exe 2 >> dynamic_runtimes.txt
	./dynamic_temporal.exe 3 >> dynamic_runtimes.txt
	./dynamic_temporal.exe 4 >> dynamic_runtimes.txt
	./dynamic_temporal.exe 5 >> dynamic_runtimes.txt
	./dynamic_temporal.exe 6 >> dynamic_runtimes.txt
	
clean:
	rm -rf *.exe