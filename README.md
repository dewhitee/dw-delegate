# dw-delegate
## Table of contents
* [General info](#general-info)
* [Contains](#contains)
* [Examples](#examples)
  - [Initialization](#initialization)
  - [Subscribing](#subscribing)
  - [Calling](#calling)
  - [Duplicating](#duplicating)
  - [Removing](#removing)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
C#-like delegate for C++.

#### Contains
###### Note: All classes are templates.
* ***DelegateBase < ReturnType, Params... >***          -   Abstract base (parent) class of (almost) all delegates.
* ***Delegate < void, Params... >***                    -   Main delegate class.
* ***RetDelegate < ReturnType, Params... >***           -   Same as ***Delegate***, but can have a custom *ReturnType* specified as template parameter.
* ***SimpleDelegate < Params... >***                    -   Type of delegate that can't have subscribed functions with saved arguments for lazy evaluation. Is more memory efficient than Delegate or RetDelegate.
* ***DelegateVisualizer < ReturnType, Params... >***    -   Utility class that can visualize delegate's data.

### Examples
```cpp
#include "Delegate\Delegate.h"

using namespace dw;
```
#### Initialization
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

#### Subscribing
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
```cpp
// Subscribing with Subscribe() method, passing parameters to be evaluated by choosen lambda function when the Invoke() method will be called.
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
```cpp
// Subscribing several lambdas with Subscribe() method, passing one integer parameter to be evaluated later on Invoke() method.
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
```cpp
// Subscribing of lambda with multiple parameters:

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


#### Calling
```cpp
// Calling subscribed functions of delegate with one parameter:

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

```cpp
// Calling subscribed functions of delegate returning integer with one parameter:

RetDelegate<int, int&> del;
auto lambda = [](int& x) -> int { return x; };

int y = 0;
del.Subscribe(lambda, y);

// Calling only those functions that were subscribed to the delegate with Subscribe() method:
int a = del.Invoke();

// Calling all subscribed functions without saved parameters:
int b = del(a);
```

#### Duplicating
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

#### Removing
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
```cpp
// Removing n functions from the delegate from end or beginning.

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
del.Remove(1, false);

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

## Technologies
Project is created with:
* C++ Standard: 14 (or later)

## Setup
Just put **Delegate** folder into the project.
