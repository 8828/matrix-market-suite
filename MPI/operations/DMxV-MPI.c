/**
  * Copyright 2016 José Manuel Abuín Mosquera <josemanuel.abuin@usc.es>
  * 
  * This file is part of Matrix Market Suite.
  *
  * Matrix Market Suite is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * Matrix Market Suite is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with Matrix Market Suite. If not, see <http://www.gnu.org/licenses/>.
  */

#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>

#include "cblas.h"

#include "DMxV-MPI.h"


int DMxVMPI(int argc, char *argv[], int numProcs, int myid) {
//Options: InputMatrixFile InputVectorFile

	
	//int i;
	int ret_code = 1;
	
	unsigned long *I;
	unsigned long *J;
	double *values;
	
	unsigned long M;
	unsigned long local_M;
	unsigned long N;
	unsigned long long nz;
	
	
	double *vectorValues;
	unsigned long M_Vector;
	unsigned long N_Vector;
	unsigned long long nz_vector;
	
	int write2file = 0;
	
	if (argc < 3)
	{
		fprintf(stderr, "[%s] Usage: %s input-matrix-file input-vector-file [output-file]\n",__func__, argv[0]);
		return 0;
	}
	
	if(argc == 4){
		write2file = 1;
	}
	
	//Read matrix
	if(!readDenseCoordinateMatrixMPI(argv[1],&I,&J,&values,&M,&local_M,&N,&nz,myid, numProcs)){
		fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
		return 0;
	}

	//Read vector
	if(!readDenseVector(argv[2], &vectorValues,&M_Vector,&N_Vector,&nz_vector)){
		fprintf(stderr, "[%s] Can not read Vector\n",__func__);
		return 0;
	}

	
	/*
	void cblas_dgemv(const enum CBLAS_ORDER order,
                 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
                 const double alpha, const double *A, const int lda,
                 const double *X, const int incX, const double beta,
                 double *Y, const int incY);
                 */
        
        double *partial_result=(double *) malloc(local_M * sizeof(double));
        
        
        
	//cblas_dgemv(CblasColMajor,CblasNoTrans,local_M,N,1.0,values,N,vectorValues,1,0.0,result,1);
	cblas_dgemv(CblasRowMajor,CblasNoTrans,local_M,N,1.0,values,N,vectorValues,1,0.0,partial_result,1);
	
	
	
	double* final_result = (double*)calloc(M,sizeof(double));
	
	if(final_result == NULL){
		fprintf("[%s] Error reserving memory for final result vector in processor %d\n",__func__,myid);
		return 0;
	}
	
	MPI_Allgather (partial_result,local_M,MPI_DOUBLE,final_result,local_M,MPI_DOUBLE,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (myid == 0){
	
		if(write2file){
			writeDenseVector(argv[3], final_result,M_Vector,N_Vector,nz_vector);
		}
		else{
			writeDenseVector("stdout", final_result,M_Vector,N_Vector,nz_vector);
		}
	}
	return ret_code;
}

