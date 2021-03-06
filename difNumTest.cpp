#include "diffNum.h"
#include <stdio.h>

int main(){

	//encode constants as the class "Variable", but simply exclude it from the gradient descent

	diffNum<double> A = std::make_shared<Variable<double> >(3);
	diffNum<double> B = std::make_shared<Variable<double> >(4);
	diffNum<double> C = std::make_shared<Variable<double> >(2);

	//need to find a clean way of using one class name instead of specialising per last used operator
	diffNum<double> E = A*(B+C);
	
	double errorMargin=1;	//solve to 0.1 absolute error
	double stepSize = 0.1;	//only move by 10% of the distance at a time

	int nFreeVars = 2;	//vary three variables in the attempt
	diffNum<double> freeVars[nFreeVars] = {A, C};

	while (E->value() > errorMargin)
	{

		double gradients[nFreeVars] = {0}; 

		for(int i=0; i<nFreeVars; i++)
		{
			gradients[i] = E->D(freeVars[i]);
		}

		for(int i=0; i<nFreeVars; i++)
		{
			//dereference of doom
			((Variable<double>*)freeVars[i].get())->set(
				(1 - (stepSize * gradients[i])) * ((Variable<double>*)freeVars[i].get())->value()
			);
		}

		printf("A = %f  \n", A->value());
		printf("B = %f  \n", B->value());
		printf("C = %f  \n", C->value());
		printf("E = A*(B+C) is: %f  \n", E->value());
		printf("dE/dA is: %f  \n", E->D(A));
		printf("dE/dB is: %f  \n", E->D(B));
		printf("dE/dC is: %f  \n", E->D(C));
	}


	return 0;
}
