INCLUDE_DIRS = -I/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/include/
LIB_DIRS = -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib/debug -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib
MPICC = mpicc
CC = gcc

CDEFS=
CFLAGS=-g -fopenmp -Wall -O2 $(INCLUDE_DIRS) $(CDEFS)
MPIFLAGS= -g -Wall -O2 $(INCLUDE_DIRS) $(CDEFS)
LIBS=

PRODUCT= page_rank_ompv2 page_rank_omp page_rank_omp_simple

HFILES= 
MPIFILES= page_rank_ompv2.c page_rank_omp.c page_rank_omp_simple.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~
	-rm -f ${PRODUCT} ${DERIVED} ${GARBAGE}

page_rank_ompv2:	page_rank_ompv2.c
	$(CC) $(CFLAGS) -o $@ page_rank_ompv2.c $(LIB_DIRS) -lm

page_rank_omp:	page_rank_omp.c
	$(CC) $(CFLAGS) -o $@ page_rank_omp.c $(LIB_DIRS) -lm

page_rank_omp_simple:	page_rank_omp_simple.c
	$(CC) $(CFLAGS) -o $@ page_rank_omp_simple.c $(LIB_DIRS) -lm