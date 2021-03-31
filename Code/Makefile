prepare_queue_exe:
	g++ queue_baseline.cpp -o queue_baseline.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ queue_spatial.cpp -o queue_spatial.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ queue_temporal.cpp -o queue_temporal.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ queue_sub_sampling.cpp -o queue_sub_sampling.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ queue_resolution.cpp -o queue_resolution.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`

run_queue:
	make queue_baseline
	make queue_sub_sampling
	make queue_resolution
	make queue_spatial
	make queue_temporal

run_dynamic:
	make dynamic_baseline
	make dynamic_sparse
	make dynamic_sub_sampling
	make dynamic_resolution
	make dynamic_spatial
	make dynamic_temporal

prepare_dynamic_exe:
	g++ dynamic_baseline.cpp -o dynamic_baseline.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ dynamic_sparse.cpp -o dynamic_sparse.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ dynamic_sub_sampling.cpp -o dynamic_sub_sampling.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ dynamic_resolution.cpp -o dynamic_resolution.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ dynamic_spatial.cpp -o dynamic_spatial.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	g++ dynamic_temporal.cpp -o dynamic_temporal.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv` 

queue_baseline:
	g++ queue_baseline.cpp -o queue_baseline.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_baseline.exe trafficvideo.mp4 

queue_spatial:
	g++ queue_spatial.cpp -o queue_spatial.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_spatial.exe trafficvideo.mp4  1 >> queue_runtimes.txt
	./queue_spatial.exe trafficvideo.mp4  2 >> queue_runtimes.txt
	./queue_spatial.exe trafficvideo.mp4  3 >> queue_runtimes.txt
	./queue_spatial.exe trafficvideo.mp4  4 >> queue_runtimes.txt
	./queue_spatial.exe trafficvideo.mp4  5 >> queue_runtimes.txt
	./queue_spatial.exe trafficvideo.mp4  6 >> queue_runtimes.txt

queue_temporal:
	g++ queue_temporal.cpp -o queue_temporal.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv` 
	./queue_temporal.exe trafficvideo.mp4  1 >> queue_runtimes.txt
	./queue_temporal.exe trafficvideo.mp4  2 >> queue_runtimes.txt
	./queue_temporal.exe trafficvideo.mp4  3 >> queue_runtimes.txt
	./queue_temporal.exe trafficvideo.mp4  4 >> queue_runtimes.txt
	./queue_temporal.exe trafficvideo.mp4  5 >> queue_runtimes.txt
	./queue_temporal.exe trafficvideo.mp4  6 >> queue_runtimes.txt

queue_sub_sampling:
	g++ queue_sub_sampling.cpp -o queue_sub_sampling.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_sub_sampling.exe trafficvideo.mp4  0 >> queue_runtimes.txt
	./queue_sub_sampling.exe trafficvideo.mp4  1 >> queue_runtimes.txt
	./queue_sub_sampling.exe trafficvideo.mp4  2 >> queue_runtimes.txt
	./queue_sub_sampling.exe trafficvideo.mp4  3 >> queue_runtimes.txt
	./queue_sub_sampling.exe trafficvideo.mp4  4 >> queue_runtimes.txt
	./queue_sub_sampling.exe trafficvideo.mp4  5 >> queue_runtimes.txt

queue_resolution:
	g++ queue_resolution.cpp -o queue_resolution.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./queue_resolution.exe trafficvideo.mp4  1 >> queue_runtimes.txt
	./queue_resolution.exe trafficvideo.mp4  2 >> queue_runtimes.txt
	./queue_resolution.exe trafficvideo.mp4  3 >> queue_runtimes.txt
	./queue_resolution.exe trafficvideo.mp4  4 >> queue_runtimes.txt
	./queue_resolution.exe trafficvideo.mp4  5 >> queue_runtimes.txt
	./queue_resolution.exe trafficvideo.mp4  6 >> queue_runtimes.txt

dynamic_baseline:
	g++ dynamic_baseline.cpp -o dynamic_baseline.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_baseline.exe trafficvideo.mp4  >> dynamic_runtimes.txt

dynamic_sparse:
	g++ dynamic_sparse.cpp -o dynamic_sparse.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_sparse.exe trafficvideo.mp4  >> dynamic_runtimes.txt

dynamic_sub_sampling:
	g++ dynamic_sub_sampling.cpp -o dynamic_sub_sampling.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_sub_sampling.exe trafficvideo.mp4  0 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe trafficvideo.mp4  1 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe trafficvideo.mp4  2 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe trafficvideo.mp4  3 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe trafficvideo.mp4  4 >> dynamic_runtimes.txt
	./dynamic_sub_sampling.exe trafficvideo.mp4  5 >> dynamic_runtimes.txt

dynamic_resolution:
	g++ dynamic_resolution.cpp -o dynamic_resolution.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_resolution.exe trafficvideo.mp4  1 >> dynamic_runtimes.txt
	./dynamic_resolution.exe trafficvideo.mp4  2 >> dynamic_runtimes.txt
	./dynamic_resolution.exe trafficvideo.mp4  3 >> dynamic_runtimes.txt
	./dynamic_resolution.exe trafficvideo.mp4  4 >> dynamic_runtimes.txt
	./dynamic_resolution.exe trafficvideo.mp4  5 >> dynamic_runtimes.txt
	./dynamic_resolution.exe trafficvideo.mp4  6 >> dynamic_runtimes.txt

dynamic_spatial:
	g++ dynamic_spatial.cpp -o dynamic_spatial.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv`
	./dynamic_spatial.exe trafficvideo.mp4  1 >> dynamic_runtimes.txt
	./dynamic_spatial.exe trafficvideo.mp4  2 >> dynamic_runtimes.txt
	./dynamic_spatial.exe trafficvideo.mp4  3 >> dynamic_runtimes.txt
	./dynamic_spatial.exe trafficvideo.mp4  4 >> dynamic_runtimes.txt
	./dynamic_spatial.exe trafficvideo.mp4  5 >> dynamic_runtimes.txt
	./dynamic_spatial.exe trafficvideo.mp4  6 >> dynamic_runtimes.txt

dynamic_temporal:
	g++ dynamic_temporal.cpp -o dynamic_temporal.exe -lpthread -std=c++11 `pkg-config --cflags --libs opencv` 
	./dynamic_temporal.exe trafficvideo.mp4  1 >> dynamic_runtimes.txt
	./dynamic_temporal.exe trafficvideo.mp4  2 >> dynamic_runtimes.txt
	./dynamic_temporal.exe trafficvideo.mp4  3 >> dynamic_runtimes.txt
	./dynamic_temporal.exe trafficvideo.mp4  4 >> dynamic_runtimes.txt
	./dynamic_temporal.exe trafficvideo.mp4  5 >> dynamic_runtimes.txt
	./dynamic_temporal.exe trafficvideo.mp4  6 >> dynamic_runtimes.txt
	
clean:
	rm -rf *.exe