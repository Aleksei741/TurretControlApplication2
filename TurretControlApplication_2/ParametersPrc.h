#pragma once
#include "TurretParamCalculate.h"
#include <libconfig.h++>
#include "main.h"

#define CONFIG_FILE_NAME "config.cfg"
#define ROOT_NAME_VIDOE_OPTION "Video"
#define ROOT_NAME_CONTROL_OPTION "Control"
#define ROOT_NAME_CONTROL_MOTOR1_OPTION "motor1"
#define ROOT_NAME_CONTROL_MOTOR2_OPTION "motor2"
#define ROOT_NAME_CONTROL_MOUSE_OPTION "mouse"
#define ROOT_NAME_AIM_OPTION "Aim"
#define ROOT_NAME_DAMAGE_OPTION "Damage"

class ParametersPrc
{
	void defaultVideoOption(VideoOption_DType* opt);
	void defaultMotorOption(MotorOption_DType* opt);
	void defaultAimOption(AimOption_DType* opt);
	void defaultAimTextOption(AimOption_DType* opt);
	void defaultDamageOption(DamageOption_DType* opt);

public:
	void saveConfig(void* opt_, const char* file_path = CONFIG_FILE_NAME);
	int loadConfig(void *opt, const char* file_path = CONFIG_FILE_NAME);
};

