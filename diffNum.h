

//Behold! All of highschool derivatives rules in one class heirarchy
//A system of classes to stress my understanding of classes, inheritance, and operator overloading.
//templates to come soon.

#include <math.h>
class Parameter;
class NegResult;
class NegResult;
class BinaryOperator;
class Variable;
class SumResult;
class MulResult;
class DivResult;
class PowResult;


//abstract class
class Parameter
{
public:
	NegResult operator-();
	SumResult operator+(const Parameter &B);
	MulResult operator*(const Parameter &B);
	DivResult operator/(const Parameter &B);
	SumResult operator-(const Parameter &B);
	PowResult operator^(const Parameter &B);

	virtual double D(const Parameter &B) const =0;
	virtual double value() const =0;
};


//Unary negation operator
class NegResult : public Parameter
{
public:
	const Parameter &arg;
	NegResult(const Parameter &_arg):
	arg(_arg)
	{}

	double D(const Parameter &B) const
	{
		return -arg.D(B);
	}

	double value() const
	{
		return -arg.value(); 	
	}
};

//create a generic binary constructor
class BinaryOperator : public Parameter
{
public:
	const Parameter &argLHS, &argRHS;

	BinaryOperator(const Parameter &inLHS, const Parameter &inRHS):
	argLHS(inLHS),
	argRHS(inRHS)
	{}
};


class SumResult : public BinaryOperator
{	
public:
	using BinaryOperator::BinaryOperator;
	double D(const Parameter &B) const 	//Sum Rule
	{
		return argLHS.D(B) + argRHS.D(B);
	}

	double value() const
	{
		return argLHS.value() + argRHS.value(); 
	}
};


class MulResult : public BinaryOperator
{
public:
	using BinaryOperator::BinaryOperator;
	double D(const Parameter &B) const	//Product Rule
	{
		return (argLHS.value() * argRHS.D(B)) + (argRHS.value() * argLHS.D(B));
	}

	double value() const
	{
		return argLHS.value() * argRHS.value(); 
	}
};


class DivResult : public BinaryOperator
{
public:
	using BinaryOperator::BinaryOperator;
	double D(const Parameter &B) const	//Quotient rule
	{
		return ((argLHS.D(B) * argRHS.value()) - (argRHS.D(B) * argLHS.value()))/(argRHS.value() * argRHS.value());
	}

	double value() const
	{
		return argLHS.value() / argRHS.value();
	}
};


class PowResult : public BinaryOperator
{
public:
	using BinaryOperator::BinaryOperator;

	double D(const Parameter &B) const
	{
		return pow(argLHS.value(), argRHS.value()) *
			(
				argLHS.D(B) * (argRHS.value()/argLHS.value()) +
				argRHS.D(B) * log(argLHS.value())
			);
	}
	double value() const
	{
		return pow(argLHS.value(), argRHS.value());
	}
};

//loop through params, adjust each to reduce energy.

//encode constants as the class "Variable", but simply exclude it from the gradient descent




class Variable : public Parameter
{
public:
	double val;
	Variable(double _val):
	val(_val)
	{}

	double D(const Parameter &B) const
	{
		if(&B == this)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	double value() const
	{
		return val;
	}

};



NegResult Parameter::operator-()
{
	return NegResult(*this);
}
SumResult Parameter::operator+(const Parameter &B)
{
	return SumResult(*this, B);
}
MulResult Parameter::operator*(const Parameter &B)
{
	return MulResult(*this, B);
}
DivResult Parameter::operator/(const Parameter &B)
{
	return DivResult(*this, B);
}
SumResult Parameter::operator-(const Parameter &B)
{
	//const NegResult &negB = -B;
	return SumResult(*this, NegResult(B));
}
PowResult Parameter::operator^(const Parameter &B)
{
	return PowResult(*this, B);
}