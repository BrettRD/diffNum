#include "diffNum.h"
#include <stdio.h>

int main(){

	Variable A = Variable((double)3);
	Variable B = Variable((double)4);
	Variable C = Variable((double)2);

	//need to find a way of using the abstract class name instead without slicing
	SumResult E = (A^C)+(B^C);
	
	printf("A = %f  \n", A.value());
	printf("B = %f  \n", B.value());
	printf("C = %f  \n", C.value());
	printf("E = (A^C)+(B^C) is: %f  \n", E.value());
	printf("dE/dA is: %f  \n", E.D(A));
	printf("dE/dB is: %f  \n", E.D(B));
	printf("dE/dC is: %f  \n", E.D(C));

	return 0;
}