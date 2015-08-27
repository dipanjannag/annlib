annlib [![Build status](https://ci.appveyor.com/api/projects/status/mo5ucovr7f43y58?svg=true)](https://ci.appveyor.com/project/medeep/annlib)
======
annlib aims to create c++ library for designing neural network. Currently this project has a main file to test the functionality without unittest ( though I'm trying to add more unittest ). The target is also
set to build a executable not a shared library . this project is under construction. It uses cpu thread to handle brunched network and to calculate the result in each layer it uses C++AMP

Description:
=========================================================================================================================
Firstly, I'm going to tell what aims.
  1) say, I need to create a neural network. I'll firstly create a layer, with a activation function by providing a enumerated value if I want a standard activation function, or I can supply my own custom activation function as lambda argument.
  2) to train this network I need to create a trainer object and pass the whole network as argument ( Specifying the training method as enumerated value).
  3) it will provide functions for standard tasks, like linearClassifier

What it currently has?
=========================================================================================================================

  1) It has the ability to create a network. Each brunch in network is handled in seperate thread. For computation in each layer it uses GPU ( But currently GPU is limited to windows with directx11+, as we are using c++ amp, though I personally
  want the experimental amp implementation over opencl comes into light).
  2) Some very common predefined network for something e.g. linearClassifier
   
=========================================================================================================================
What's more we need help. If you find anything here is cool please consider adding ( or maybe removing too) something, we will be incredibly happy to have you with us.
