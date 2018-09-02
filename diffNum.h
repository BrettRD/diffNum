

//Behold! All of highschool derivatives rules in one class heirarchy
//A system of classes to stress my understanding of classes, inheritance, and operator overloading.


#include <math.h>


template <typename T>
class Parameter;
template <typename T>
class NegResult;
template <typename T>
class NegResult;
template <typename T>
class BinOp;
template <typename T>
class Variable;
template <typename T>
class SumResult;
template <typename T>
class MulResult;
template <typename T>
class DivResult;
template <typename T>
class PowResult;


//abstract class
template <typename T>
class Parameter
{
public:
	NegResult<T> operator-();
	SumResult<T> operator+(const Parameter &B);
	MulResult<T> operator*(const Parameter &B);
	DivResult<T> operator/(const Parameter &B);
	SumResult<T> operator-(const Parameter &B);
	PowResult<T> operator^(const Parameter &B);

	virtual T D(const Parameter &B) const =0;
	virtual T value() const =0;
};


//Unary negation operator
template <typename T>
class NegResult : public Parameter<T>
{
public:
	const Parameter<T> &arg;
	NegResult(const Parameter<T> &_arg):
	arg(_arg)
	{}

	T D(const Parameter<T> &B) const
	{
		return -arg.D(B);
	}

	T value() const
	{
		return -arg.value(); 	
	}
};

//create a generic binary constructor
template <typename T>
class BinOp : public Parameter<T>
{
public:
	const Parameter<T> &argLHS;
	const Parameter<T> &argRHS;

	BinOp(const Parameter<T> &inLHS, const Parameter<T> &inRHS):
	argLHS(inLHS),
	argRHS(inRHS)
	{}
};

template <typename T>
class SumResult : public BinOp<T>
{	
public:
	using BinOp<T>::BinOp;
	T D(const Parameter<T> &B) const 	//Sum Rule
	{
		return BinOp<T>::argLHS.D(B) + BinOp<T>::argRHS.D(B);
	}

	T value() const
	{
		return BinOp<T>::argLHS.value() + BinOp<T>::argRHS.value(); 
	}
};

template <typename T>
class MulResult : public BinOp<T>
{
public:
	using BinOp<T>::BinOp;
	T D(const Parameter<T> &B) const	//Product Rule
	{
		return (BinOp<T>::argLHS.value() * BinOp<T>::argRHS.D(B)) +
		       (BinOp<T>::argRHS.value() * BinOp<T>::argLHS.D(B));
	}

	T value() const
	{
		return BinOp<T>::argLHS.value() * BinOp<T>::argRHS.value(); 
	}
};

template <typename T>
class DivResult : public BinOp<T>
{
public:
	using BinOp<T>::BinOp;
	T D(const Parameter<T> &B) const	//Quotient rule
	{
		return ((BinOp<T>::argLHS.D(B) * BinOp<T>::argRHS.value()) -
		        (BinOp<T>::argRHS.D(B) * BinOp<T>::argLHS.value())) /
		       (BinOp<T>::argRHS.value() * BinOp<T>::argRHS.value());
	}

	T value() const
	{
		return BinOp<T>::argLHS.value() / BinOp<T>::argRHS.value();
	}
};

template <typename T>
class PowResult : public BinOp<T>
{
public:
	using BinOp<T>::BinOp;

	T D(const Parameter<T> &B) const
	{
		return pow(BinOp<T>::argLHS.value(), BinOp<T>::argRHS.value()) *
			(
				BinOp<T>::argLHS.D(B) * (BinOp<T>::argRHS.value()/BinOp<T>::argLHS.value()) +
				BinOp<T>::argRHS.D(B) * log(BinOp<T>::argLHS.value())
			);
	}
	T value() const
	{
		return pow(BinOp<T>::argLHS.value(), BinOp<T>::argRHS.value());
	}
};


template <typename T>
class Variable : public Parameter<T>
{
public:
	T val;
	Variable(T initValue):
	val(initValue)
	{}

	T D(const Parameter<T> &B) const
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

	T value() const
	{
		return val;
	}

	void set(T newValue)
	{
		val = newValue;
	}

};


template <typename T>
NegResult<T> Parameter<T>::operator-()
{
	return NegResult<T>(*this);
}
template <typename T>
SumResult<T> Parameter<T>::operator+(const Parameter<T> &B)
{
	return SumResult<T>(*this, B);
}
template <typename T>
MulResult<T> Parameter<T>::operator*(const Parameter<T> &B)
{
	return MulResult<T>(*this, B);
}
template <typename T>
DivResult<T> Parameter<T>::operator/(const Parameter<T> &B)
{
	return DivResult<T>(*this, B);
}
template <typename T>
SumResult<T> Parameter<T>::operator-(const Parameter<T> &B)
{
	return SumResult<T>(*this, NegResult<T>(B));
}
template <typename T>
PowResult<T> Parameter<T>::operator^(const Parameter<T> &B)
{
	return PowResult<T>(*this, B);
}

