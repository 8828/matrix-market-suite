CC=			gcc
CFLAGS=			-g -Wall -Wno-unused-function -O2
WRAP_MALLOC=		-DUSE_MALLOC_WRAPPERS
DFLAGS=			-DHAVE_PTHREAD $(WRAP_MALLOC)

PROG=			MM-Suite

MPIDIR= MPI
SUBDIRS= io operations utils solvers lib/CBLAS

SUBCLEAN = $(addsuffix .clean,$(SUBDIRS))

OBJECTS= io/CreateDenseMatrixSymmetric.o io/CreateDenseVector.o \
	utils/utils.o utils/mmio.o \
	operations/DMxV.o operations/VectorOperations.o \
	solvers/ConjugateGradient.o solvers/ConjugateGradientSolver.o

CBLAS_DIR = ./lib/CBLAS/lib/

LIBS = -L$(CBLAS_DIR) -lcblas -lm -lblas
INCLUDES =

.PHONY: all

.SUFFIXES:.c .o .cc

.c.o:
	$(CC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

all:$(PROG)
	

.PHONY: subdirs $(SUBDIRS)
.PHONY: clean $(SUBCLEAN)

subdirs: $(SUBDIRS)


$(SUBDIRS):
	$(MAKE) -C $@

MPISuite:
	$(MAKE) -C $(MPIDIR)

cleanMPI:
	$(MAKE) -C $(MPIDIR) clean

MM-Suite: MM-Suite.o subdirs
	$(CC) $(CFLAGS) $(DFLAGS) $(AOBJS) MM-Suite.o $(OBJECTS) -o $@  $(LIBS)

clean: $(SUBCLEAN)
	rm $(PROG) *.o

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean
