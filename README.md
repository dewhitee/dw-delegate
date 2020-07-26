# dw-delegate
## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
C#-like delegate for C++.

All of this project are templates.
#### Contains
* ***DelegateBase < ReturnType, Params... >***          -   Base (parent) class of all delegates.
* ***Delegate < void, Params... >***                    -   Main delegate class.
* ***RetDelegate < ReturnType, Params... >***           -   Same as ***Delegate***, but can have a custom *ReturnType* specified as template parameter.
* ***DelegateVisualizer < ReturnType, Params... >***    -   Utility class that can visualize delegate's data.

## Technologies
Project is created with:
* C++ Standard: 14 (or later)

## Setup
Just put **Delegate** folder into the project.
