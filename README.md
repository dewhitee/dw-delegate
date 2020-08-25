# dw-delegate
## Table of contents
* [General info](#general-info)
* [API](#api)
* [Examples](#examples)
  - [Initialization](#initialization)
  - [Subscribing](#subscribing)
  - [Calling](#calling)
  - [Duplicating](#duplicating)
  - [Removing](#removing)
  - [Combining](#combining)
  - [Shifting](#shifting)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
Small header-only templates library for C#-like delegate.

# API
###### Note: All classes are templates.
### DelegateBase
Abstract base (parent) class of (almost) all delegates.

```cpp
template <typename ReturnType, typename... Params>
class DelegateBase
...
```

#### Fields:
Field name:  | Type:                                               | Description
-------------|-----------------------------------------------------|------------
subscribers  | std::vector<DelegateType>                           | Vector of functions subscribed to this delegate
parameters   | std::vector<DelegateParams<Params...>>              | Vector of parameters passed with the Subscribe() method.

#### Methods:
Method name: | Return Type: | Parameters:                                                            | Description
-------------|--------------|------------------------------------------------------------------------|------------
Combine      | void         | const DelegateBase& other                                              | Subscribes all functions (subscribers) from other delegate to this delegate
Subscribe    | void         | const DelegateType& delegate, Params... params                         |
Subscribe    | void         | const std::initializer_list<DelegateType>& delegates, Params... params |
Subscribe    | void         | const DelegateType& delegate, std::vector<std::tuple<Params...>> params| 
Invoke       | void         | *none*                                                                 | Call all subscribed functions of this delegate that have parameters saved on Subscribe() method.
Remove       | void         | int count = 1, bool fromBack = true                                    | Remove *count* functions from the back (fromBack == true) or front (fromBack == false).
Clear        | void         | *none*                                                                 | Removes all subscribed functions and parameters from this delegate.
operator+=   | DelegateBase&| const DelegateType& rhs                                                | Subscribes function to this delegate.
operator+=   | DelegateBase&| const std::initializer_list<DelegateType>& rhs                         | Subscribes multiple functions to this delegate.
operator-=   | DelegateBase&| const DelegateType& rhs                                                | Unsubscribes choosen function from this delegate.
operator++   | DelegateBase&| *none*                                                                 | Prefix version for duplicating delegate's last subscribed function.
operator++   | DelegateBase&| int                                                                    | Postfix version for duplication delegate's last subscribed function.
operator--   | DelegateBase&| *none*                                                                 | Prefix version for removing delegate's last subscribed function.
operator--   | DelegateBase&| int                                                                    | Postfix version for removing delegate's last subscribed function.
operator==   | const bool   | const DelegateBase& rhs                                                | Compares subscribers of other delegate to subscribers of this delegate.
operator<    | const bool   | const DelegateBase& rhs                                                | Compares the size of subscribers vector of both delegates.
operator>    | const bool   | const DelegateBase& rhs                                                | Compares the size of subscribers vector of both delegates.
operator<<   | DelegateBase&| const DelegateBase& rhs                                                | Transfer all subscribers of other delegate to this delegate. Will clear subscribers from other delegate.
operator>>   | DelegateBase&| const DelegateBase& rhs                                                | Transfer all subscribers of this delegate to other delegate. Will clear subscribers from this delegate.
  
### Delegate
Main delegate class.

```cpp
template <typename... Params>
class Delegate : public DelegateBase<void, Params...>
...
```
### RetDelegate
Same as [Delegate](#delegate), but can have a custom *ReturnType* specified as template parameter.
```cpp
template <typename ReturnType, typename... Params>
class RetDelegate : public DelegateBase<ReturnType, Params...>
...
```

### SimpleDelegate
Type of delegate that don't have ability to save parameters through Subscribe() method. Is more memory efficient than Delegate or RetDelegate.
```cpp
template <typename... Params>
class SimpleDelegate
...
```

### MemberDelegate
Delegate that holds the subscribed member functions.
```cpp
template <typename ReturnType, typename ObjType, typename... Params>
class MemberDelegate
...
```

## Examples
```cpp
#include "Delegate\Delegate.h"

using namespace dw;
```
### Initialization
```cpp
// Initializing delegates:

// Initialization of delegate with void return type and no parameters.
Delegate del1;

// Initialization of delegate with one parameter returning nothing.
Delegate<int&> del2;

// Initialization of delegate with one parameter returning integer.
RetDelegate<int, int&> del3;

RetDelegate<void, int&> badDelegate; // Note that you can't initialize RetDelegate returning void.

// Initialization of SimpleDelegate with one parameter.
SimpleDelegate<int> del4;
```

### Subscribing
```cpp
// Subscribing lambda to the delegate without specifying parameters for later evaluation:

Delegate<int&> del;
auto lambda = [](int& x) { x++; };

del += lambda;
```

```cpp
// Subscribing lambda to the delegate with specified parameters:

Delegate<int&> del;
auto lambda = [](int& x) { x++; };

int y = 0;

del.Subscribe(lambda, y);
// Then Invoke() method will call this lambda function with the 'y' as parameter.
```

```cpp
// Subscribe several different functions using initializer list:

Delegate<int> del;

auto lambda1 = [](int x) { std::cout << "First lambda x = " << x << std::endl; };
auto lambda2 = [](int x) { std::cout << "Second lambda x = " << x << std::endl; };
auto lambda3 = [](int x) { std::cout << "Third lambda x = " << x << std::endl; };
```

```cpp
// Subscribing with initializer list
del += {lambda1, lambda2, lambda3};

del(4);
```
###### Result
```cpp
First lambda x = 4
Second lambda x = 4
Third lambda x = 4
```
---
Subscribing with Subscribe() method, passing parameters to be evaluated by choosen lambda function when the Invoke() method will be called:
```cpp
del.Subscribe(lambda1, {10, 13, 15});

del.Invoke();
```
###### Result
```cpp
First lambda x = 10
First lambda x = 13
First lambda x = 15
```
---
Subscribing several lambdas with Subscribe() method, passing one integer parameter to be evaluated later on Invoke() method:
```cpp
del.Subscribe({lambda1, lambda2, lambda3}, 21);

del.Invoke();
```
###### Result
```cpp
First lambda x = 21
Second lambda x = 21
Third lambda x = 21
```
---
Subscribing lambda with multiple parameters:
```cpp
Delegate<int, float, std::string> del;

auto lambda = [](int x1, float x2, std::string x3)
{
    std::cout << "x1 = " << x1 << ", x2 = " << x2 << ", x3 = " << x3 << std::endl;
};

// Subscribing one lambda with two tuples of parameters for later evaluation.
del.Subscribe(lambda, {{1, 3.5f, "foo"}, {2, 5.74f, "bar"}});

del.Invoke();
```
###### Result
```cpp
x1 = 1, x2 = 3.5, x3 = foo
x1 = 2, x2 = 5.74, x3 = bar
```


### Calling
Calling subscribed functions of delegate with one parameter:
```cpp
Delegate<int&> del;
auto lambda = [](int& x) { x++; };

int y = 0;
del.Subscribe(lambda, y);

// Calling only those functions that were subscribed to the delegate with Subscribe() method:
del.Invoke();

// Calling all subscribed functions (saved parameters will not be used):
int z = 0;
del(z);
```

Calling subscribed functions of delegate returning integer with one parameter:
```cpp
RetDelegate<int, int&> del;
auto lambda = [](int& x) -> int { return x; };

int y = 0;
del.Subscribe(lambda, y);

// Calling only those functions that were subscribed to the delegate with Subscribe() method:
int a = del.Invoke();

// Calling all subscribed functions without saved parameters:
int b = del(a);
```

### Duplicating
```cpp
Delegate<int> del;
auto lambda = [](int x) { std::cout << "x = " << x << std::endl; };

del.Subscribe(lambda, {4, 6, 8});

// Duplicating the last subscribed function with the specified integer parameter 8:
// Note that both  postfix and prefix operators are valid.
del++;

del.Invoke();
```
###### Result
```cpp
x = 4
x = 6
x = 8
x = 8
```

### Removing
```cpp
Delegate<int> del;
auto lambda = [](int x) { std::cout << "x = " << x << std::endl; };

del.Subscribe(lambda, {4, 6, 8});

// Removing last subscribed function from the delegate.
del--;

del.Invoke();
```
###### Result
```cpp
x = 4
x = 6
```
---
Removing n functions from the delegate from end or beginning.
```cpp
Delegate<int> del;
auto lambda1 = [](int x) { std::cout << "x = " << x << std::endl; };
auto lambda2 = [](int y) { std::cout << "y = " << y << std::endl; };

del.Subscribe(lambda1, {4, 6, 8});
del.Subscribe(lambda2, {-5, -7});

std::cout << "Before:" << std::endl;
del.Invoke();

// Removing 1 function from the end:
del.Remove(1);

std::cout << "\nAfter removing 1 function from the end:" << std::endl;
del.Invoke();

// Removing 2 functions from the beginning:
del.Remove(2, false);

std::cout << "\nAfter removing 2 functions from the beginning: " << std::endl;
del.Invoke();
```
###### Result
```cpp
Before:
x = 4
x = 6
x = 8
y = -5
y = -7

After removing 1 function from the end:
x = 4
x = 6
x = 8
y = -5

After removing 2 functions from the beginning:
x = 8
y = -5
```

### Combining
You can combine two delegates subscribed functions by using Combine() method.

Note: Combining is performed for the delegate that calls the Combine() method only, so other (second) delegate will not be updated.
```cpp
Delegate<int> del1;
Delegate<int> del2;

auto lambda1 = [](int x) { std::cout << "x = " << x << std::endl; };
auto lambda2 = [](int y) { std::cout << "y = " << y << std::endl; };

// Subscribing first lambda with two different parameters for later evaluation to the first delegate.
del1.Subscribe(lambda1, {10, 15});

// Subscribing second lambda with two different parameters for later evaluation to the second delegate.
del2.Subscribe(lambda2, {-2, -6});

// Combining first delegate with the second delegate.
del1.Combine(del2);

del1.Invoke();
```
###### Result
```cpp
x = 10
x = 15
y = -2
y = -6
```

### Shifting
You can transfer all subscribed functions, with the parameters that were saved, from one delegate to another by using the overloaded right and left shift operators. All subscribed methods of delegate from which shift was applied will be cleared. For example, considering that delegates have matching signature, this line of code: ```delegate1 >> delegate2;```, will transfer all methods of ```delegate1``` to ```delegate2```, and clear all the subscribed methods of ```delegate1```.

Note: Both delegates must have the same signature.
```cpp
Delegate<int> del1;
Delegate<int> del2;

auto lambda1 = [](int x) { std::cout << "x = " << x << std::endl; };
auto lambda2 = [](int y) { std::cout << "y = " << y << std::endl; };

del1.Subscribe(lambda1, {10, 15});
del2.Subscribe(lambda2, {-2, -6});

del1 << del2;

std::cout << "\nFirst delegate: " << std::endl;
del1.Invoke();

std::cout << "\nSecond delegate: None" << std::endl;
del2.Invoke();
```
###### Result
```cpp
First delegate:
x = 10
x = 15
y = -2
y = -6

Second delegate: None
```

```cpp
// ...

del1 >> del2;

std::cout << "\nFirst delegate: None" << std::endl;
del1.Invoke();

std::cout << "\nSecond delegate: " << std::endl;
del2.Invoke();
```
###### Result
```cpp
First delegate: None

Second delegate:
x = 10
x = 15
y = -2
y = -6
```

## Technologies
Project is created with:
* C++ Standard: 14 (or later)

## Setup
Just put **Delegate** folder into the project.
