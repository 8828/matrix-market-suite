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
#include <string.h>

#include "CreateDenseMatrixGeneralRowLine.h"

int CreateDenseMatrixGeneralRowLine(char *fileName, unsigned long int numRows, unsigned long int numCols, unsigned int seed) {
//Options: numRows numCols fileName seed

	FILE *output;
	//long nz;   
	unsigned long i, j;
	

	MM_typecode outputmatcode;
	
	mm_initialize_typecode(&outputmatcode);
	mm_set_matrix(&outputmatcode);
	//mm_set_coordinate(&outputmatcode);
	mm_set_dense(&outputmatcode);
	mm_set_real(&outputmatcode);
	mm_set_general(&outputmatcode);


	if(strcmp(fileName,"stdout")==0){
		output = stdout;
	}
	else{
		if ((output = fopen(fileName, "w")) == NULL){
			fprintf(stderr,"[%s] Unable to open file for writing\n",__func__);
			return 0;
		}
	}

	
	mm_write_banner(output, outputmatcode);
	mm_write_mtx_crd_size(output, numRows, numCols, numRows*numCols);
	
	
	double value = 0.0;
	
	srand (seed);
	
	for(i = 0;i < numRows; i++){
	
		fprintf(output, "%lu:",i);
	
		for(j = 0; j< numCols; j++){

			value = ((double)rand() / (double)RAND_MAX)/100;
			
			fprintf(output, "%lg,",value);
			
			
		}

		fprintf(output,"\n");

	}
	
	fclose(output);

	return 1;
}

