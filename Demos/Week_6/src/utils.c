#include "utils.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

