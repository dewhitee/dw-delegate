# dw-delegate
## Table of contents
* [General info](#general-info)
* [Contains](#contains)
* [Examples](#examples)
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

#### Examples

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

```cpp
// Subscribing lambda to the delegate without specifying - saving any arguments - for lazy evaluation:

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
```

```cpp
// Calling subscribed functions of delegate:

Delegate<int&> del;

// Calling only those functions that were subscribed to the delegate with Subscribe() method:
del.Invoke();

// Calling all subscribed functions (saved parameters will not be used):
int y = 0;
del(y);

```

```cpp
// Calling

```

## Technologies
Project is created with:
* C++ Standard: 14 (or later)

## Setup
Just put **Delegate** folder into the project.
