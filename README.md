# diffNum
A number format that implements partial differentiation on the stack

Arithmetic operators become nodes in a directed acyclic graph describing an equation.

The result of an equation calls a full re-evaluation back to the original variables when you request its value().\
You can interrogate the result of any equation and recover its partial derivative with respect to a given variable.\
The number format is fully templated, so it should compile with any number format that is well behaved with the normal arithemtic ops.

This is useful (but slow) for solving numerical equations using gradient descent methods.\
It is especially useful when you cannot know the structure of the equation you need to solve until runtime.
