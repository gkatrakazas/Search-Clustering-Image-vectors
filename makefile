compile: main_lsh.cpp lsh_function.cpp main_cube.cpp hashtable.cpp math.cpp main_clustering.cpp
		 g++ -o lsh main_lsh.cpp lsh_function.cpp math.cpp hashtable.cpp ReadMNIST.cpp
		 g++ -o cube main_cube.cpp lsh_function.cpp math.cpp hashtable.cpp ReadMNIST.cpp
		 g++ -o clustering main_clustering.cpp clustering.cpp lsh_function.cpp math.cpp hashtable.cpp ReadMNIST.cpp
		 
clean:
		 rm lsh 
		 rm cube 
		 rm clustering 
