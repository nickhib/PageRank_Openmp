INCLUDE_DIRS = -I/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/include/
LIB_DIRS = -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib/debug -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib
MPICC = mpicc
CC = gcc

CDEFS=
CFLAGS=-g -fopenmp -Wall -O2 $(INCLUDE_DIRS) $(CDEFS)
MPIFLAGS= -g -Wall -O2 $(INCLUDE_DIRS) $(CDEFS)
LIBS=

PRODUCT= sequential page_rank_omp_function 

HFILES= 
MPIFILES= sequential.c  page_rank_omp_function.c 

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~
	-rm -f ${PRODUCT} ${DERIVED} ${GARBAGE}

sequential:	sequential.c
	$(CC) $(CFLAGS) -o $@ sequential.c $(LIB_DIRS) -lm

page_rank_omp_function:	page_rank_omp_function.c
	$(CC) $(CFLAGS) -o $@ page_rank_omp_function.c $(LIB_DIRS) -lm