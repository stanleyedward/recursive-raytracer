// Readfile definitions 
#pragma once
#include "stdafx.h"

void matransform (stack<mat4> &transfstack, float * values) ;
void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, float * values) ;
string readfile (const char * filename) ;
