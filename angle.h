#ifndef angle_h
#define angle_h

#include <math.h>
#include "stepper.h"
#include "MKL25Z4.h"
#include "mma8451.h"
#include "atan_lut.h"

#define PREVIOUS 0
#define PRESENT 1
#define FILTERED 2
#define M_PI_4 0.78539816339744830962 

extern volatile double pitches[2];
extern volatile double rolls[2];

double lut_atan(double y, double x);
double FastArcTan(double x);
double cal_pitch(acc_data_double* data);
double cal_roll(acc_data_double* data);
void filter(acc_data_double* present_data, acc_data_double* previous_data, acc_data_double* result);
void normalize(acc_data_int* raw_data, acc_data_double* normalized_data);
#endif
