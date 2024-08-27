#pragma once
#include <cmath>

class TurretParamCalculate
{
public:
	static int DegreeToSteps(int degree, int micro_steps, float reduction_ratio);
	static float Freq(float rotation_speed, int micro_steps, float reduction_ratio);
	static float PeriodSignal(float rotation_speed, int micro_steps, float reduction_ratio);
};

