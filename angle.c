#include "angle.h"

const double PI = 3.1415;
const float filter_coef = 0.96;
volatile double pitches[2];
volatile double rolls[2];

double FastArcTan(double x)
{
    return M_PI_4*x - x*(fabs(x) - 1)*(0.2447 + 0.0663*fabs(x));
}

double cal_pitch(acc_data_double* data){									// n :  0 - old state 1 - newstate
	double pitch = 0.0;
	double xy_atan = (sqrt(data->Y_AXIS * data->Y_AXIS + data->Z_AXIS * data->Z_AXIS));
	if(xy_atan<0.01 && xy_atan > -0.01){
		return 0.0;
	}
	pitch = -1*(FastArcTan(data->X_AXIS/xy_atan)* 180.0 / PI);
	//pitch = atan(-1 * X_AXIS / sqrt(pow(Y_AXIS, 2) + pow(Z_AXIS,2)))* 180.0 / PI;
	return pitch;
}

double cal_roll(acc_data_double* data){
	double roll;
	if( data->Z_AXIS < 0.01 && data->Z_AXIS > -0.01){
		return 0.0;
	}
	//roll = atan(Y_AXIS / sqrt(pow(X_AXIS,2) + pow(Z_AXIS,2))) * 180 /PI;
	roll = FastArcTan(data->Y_AXIS /data->Z_AXIS) * 180.0 /PI;	
	return roll;
}

void filter(acc_data_double* present_data, acc_data_double* previous_data, acc_data_double* result){    										
																			// usrednia wartosc pomiaru pitch lub roll za pomoca low pass filter 
	result->X_AXIS = filter_coef*(previous_data->X_AXIS) + (1.0-filter_coef)*(present_data->X_AXIS);
	result->Y_AXIS = filter_coef*(previous_data->Y_AXIS) + (1.0-filter_coef)*(present_data->Y_AXIS);
	result->Z_AXIS = filter_coef*(previous_data->Z_AXIS) + (1.0-filter_coef)*(present_data->Z_AXIS);

}

void normalize(acc_data_int* raw_data, acc_data_double* normalized_data){
	int16_t x_temp = ((int16_t)(raw_data->X_MSB<< 6) | (int16_t)(raw_data->X_LSB >> 2));
	int16_t y_temp = ((int16_t)(raw_data->Y_MSB<< 6) | (int16_t)(raw_data->Y_LSB >> 2));
	int16_t z_temp = ((int16_t)(raw_data->Z_MSB<< 6) | (int16_t)(raw_data->Z_LSB >> 2));
	
	normalized_data->X_AXIS = (double)(x_temp - ((x_temp & (1ul << 13)) ? 16384 : 0))/4096.0;
	normalized_data->Y_AXIS = (double)(y_temp - ((y_temp & 1ul << 13) ? 16384 : 0))/4096.0;
	normalized_data->Z_AXIS = (double)(z_temp - ((z_temp & 1ul << 13) ? 16384 : 0))/4096.0;
}

// pseudo "sample code" poniewaz nie do konca wiem jak rozmieszczone sa silnik 1,2 plus jak wygladaja argumenty przyjmowane przez fkcje rotateAngle
//double pitches[2];
//pitches[0]  = cal_pitch(...,0)
//pitches[1]  = cal_pitch(...,1)
//f_pitch = filter(pitches)

/*
double f_pitch;

void balance_Y(f_pitch){
int direction;
if (f_pitch < 0){
direction = 1; 				//przekazujemy wartosc odwrotna do obecnego odchylenia
}
else{
direction = -1;
}
float angle;
angle = -1* f_pitch;
rotateAngle(MOTOR1, direction, angle)
}*/

