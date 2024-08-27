#include "TurretParamCalculate.h"

int TurretParamCalculate::DegreeToSteps(int degree, int micro_steps, float reduction_ratio)
{
	int steps;
	float steps_360 = ((float)micro_steps * reduction_ratio);
	float degree_in_step = 360.0 / steps_360;

	steps = steps_360 * (float)degree / 360.0;

	return steps;
}

float TurretParamCalculate::Freq(float rotation_speed, int micro_steps, float reduction_ratio)
{
	float freq;
	float steps_360 = ((float)micro_steps * reduction_ratio);
	float degree_in_step = 360.0 / steps_360;

	freq = rotation_speed / degree_in_step;

	return freq;
}

float TurretParamCalculate::PeriodSignal(float rotation_speed, int micro_steps, float reduction_ratio)
{
	float period;
	float steps_360 = ((float)micro_steps * reduction_ratio);
	float degree_in_step = 360.0 / steps_360;

	period = degree_in_step / rotation_speed;

	return period;
}