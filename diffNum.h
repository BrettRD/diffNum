

//Behold! All of highschool derivatives rules in one class heirarchy
//A system of classes to stress my understanding of classes, inheritance, and operator overloading.


#include <memory>
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


template <typename T>
class diffNumBase
{
public:
	virtual T D(std::shared_ptr<diffNumBase<T> > x) = 0;
	virtual T value() = 0;
};

template <typename T>
using diffNum = std::shared_ptr<diffNumBase<T> >;


//Unary negation operator
template <typename T>
class NegResult : public diffNumBase<T>
{
public:
	diffNum<T> arg;
	NegResult(diffNum<T> inarg) : arg(inarg) {}

	T D(diffNum<T> x)
	{
		return -arg->D(x);
	}

	T value()
	{
		return -arg->value(); 	
	}
};

//create a generic binary constructor
template <typename T>
class BinOp : public diffNumBase<T>
{
public:
	diffNum<T> argLHS;
	diffNum<T> argRHS;

	BinOp(diffNum<T> inLHS, diffNum<T> inRHS):
	argLHS(inLHS),
	argRHS(inRHS)
	{}
};

template <typename T>
class SumResult : public BinOp<T>
{	
public:
	using BinOp<T>::BinOp;
	T D(diffNum<T> x) 	//Sum Rule
	{
		return BinOp<T>::argLHS->D(x) + BinOp<T>::argRHS->D(x);
	}

	T value()
	{
		return BinOp<T>::argLHS->value() + BinOp<T>::argRHS->value(); 
	}
};

template <typename T>
class MulResult : public BinOp<T>
{
public:
	using BinOp<T>::BinOp;
	T D(diffNum<T> x)	//Product Rule
	{
		return (BinOp<T>::argLHS->value() * BinOp<T>::argRHS->D(x)) +
		       (BinOp<T>::argRHS->value() * BinOp<T>::argLHS->D(x));
	}

	T value()
	{
		return BinOp<T>::argLHS->value() * BinOp<T>::argRHS->value(); 
	}
};

template <typename T>
class DivResult : public BinOp<T>
{
public:
	using BinOp<T>::BinOp;
	T D(diffNum<T> x)	//Quotient rule
	{
		return ((BinOp<T>::argLHS->D(x) * BinOp<T>::argRHS->value()) -
		        (BinOp<T>::argRHS->D(x) * BinOp<T>::argLHS->value())) /
		       (BinOp<T>::argRHS->value() * BinOp<T>::argRHS->value());
	}

	T value()
	{
		return BinOp<T>::argLHS->value() / BinOp<T>::argRHS->value();
	}
};

template <typename T>
class PowResult : public BinOp<T>
{
public:
	using BinOp<T>::BinOp;

	T D(diffNum<T> x)
	{
		return pow(BinOp<T>::argLHS->value(), BinOp<T>::argRHS->value()) *
			(
				BinOp<T>::argLHS->D(x) * (BinOp<T>::argRHS->value()/BinOp<T>::argLHS->value()) +
				BinOp<T>::argRHS->D(x) * log(BinOp<T>::argLHS->value())
			);
	}

	T value()
	{
		return pow(BinOp<T>::argLHS->value(), BinOp<T>::argRHS->value());
	}
};


template <typename T>
class Variable : public diffNumBase<T>
{
public:
	T val;
	Variable(){}
	Variable(T initValue):
	val(initValue)
	{}

	T D(diffNum<T> x)
	{
		if(x.get() == this)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	T value()
	{
		return val;
	}

	void set(T newValue)
	{
		val = newValue;
	}

};


template <typename T>
diffNum<T> operator-(diffNum<T> inRHS)
{
	return std::make_shared<NegResult<T> >(inRHS);
}
template <typename T>
diffNum<T> operator+(diffNum<T> inLHS, diffNum<T> inRHS)
{
	return std::make_shared<SumResult<T> >(inLHS, inRHS);
}
template <typename T>
diffNum<T> operator*(diffNum<T> inLHS, diffNum<T> inRHS)
{
	return std::make_shared<MulResult<T> >(inLHS, inRHS);
}
template <typename T>
diffNum<T> operator/(diffNum<T> inLHS, diffNum<T> inRHS)
{
	return std::make_shared<DivResult<T> >(inLHS, inRHS);
}
template <typename T>
diffNum<T> operator-(diffNum<T> inLHS, diffNum<T> inRHS)
{
	return inLHS + (-inRHS);
}
template <typename T>
diffNum<T> operator^(diffNum<T> inLHS, diffNum<T> inRHS)
{
	return std::make_shared<PowResult<T> >(inLHS, inRHS);
}
