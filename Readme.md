This is a small library for defining units. It is intended to facilitate the writing of type-safe API's. It includes 4 unit template classes (Amplitude, Decibel, Q Coefficient, and resonance 
coefficient), as well as a base unit template class that you can inherit from to define your own units. The base unit template overloads all of the standard arithmetic, bitshift, and assignment 
operators, and so by default any subclass will have these operations defined for it (as long as both operands are of the same type, or in the case of multiplication and addition, one of the operands is 
the unit, and the other has the same type as the unit's underlying type). 

This base class has no concept of dimensional analysis- it is up to you to define any and all conversions between different units, as well as any operator overloads that use units of different types and
the unit types that may result from those operations. If you need dimensional analysis, or you need a units library for doing calculations with physical quantites, you will probably be better served by
a different library. A typical unit will inherit from the Unit class via crtp, and then define several converting constructors- the decibel and amplitude classes are a good example of how this works.