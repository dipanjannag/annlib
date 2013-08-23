#include "stdafx.h"
#include "Feature.h"

// in this file traits will be provided
Feature<int>::Feature(int dim,int def=0)
{
	this->dimension = dim;
	fet = new int[dimension];
}


