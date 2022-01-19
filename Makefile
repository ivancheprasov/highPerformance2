compile:
	mpicc *.c *.h -lm -o lab2
run:
	mpiexec -n 2 ./lab2 --time 20 --src /home/subhuman/CLionProjects/highPerformance2/src.txt --dst /home/subhuman/CLionProjects/highPerformance2/out --mpi