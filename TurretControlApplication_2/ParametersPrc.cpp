#include "ParametersPrc.h"

#pragma comment(lib,"libconfig++.lib")
#pragma comment(lib,"libconfig++d.lib")



void ParametersPrc::saveConfig(void* opt_, const char* file_path)
{
    int cnt;
    std::string group_name;
    UserParameters_DType* opt = (UserParameters_DType*)opt_;

	libconfig::Config cfg;

    libconfig::Setting& root = cfg.getRoot();

    if (!root.exists(ROOT_NAME_VIDOE_OPTION))
        root.add(ROOT_NAME_VIDOE_OPTION, libconfig::Setting::TypeGroup);

    libconfig::Setting& video_options = root[ROOT_NAME_VIDOE_OPTION];

    video_options.add("width", libconfig::Setting::TypeInt) = opt->Video.width;
    video_options.add("height", libconfig::Setting::TypeInt) = opt->Video.height;
    video_options.add("port", libconfig::Setting::TypeInt) = opt->Video.port;
    video_options.add("bitrate", libconfig::Setting::TypeInt) = opt->Video.bitrate;
    video_options.add("framerate", libconfig::Setting::TypeInt) = opt->Video.framerate;
    video_options.add("H26X", libconfig::Setting::TypeBoolean) = static_cast<bool>(opt->Video.H26X);

    if (!root.exists(ROOT_NAME_CONTROL_OPTION))
        root.add(ROOT_NAME_CONTROL_OPTION, libconfig::Setting::TypeGroup);

    libconfig::Setting& ControlOptions = root[ROOT_NAME_CONTROL_OPTION];

    if (!ControlOptions.exists(ROOT_NAME_CONTROL_MOTOR1_OPTION))
        ControlOptions.add(ROOT_NAME_CONTROL_MOTOR1_OPTION, libconfig::Setting::TypeGroup);

    libconfig::Setting& m1_option = ControlOptions[ROOT_NAME_CONTROL_MOTOR1_OPTION];

    m1_option.add("MaxDegreePosition", libconfig::Setting::TypeInt) = opt->M1.MaxDegreePosition;
    m1_option.add("MinDegreePosition", libconfig::Setting::TypeInt) = opt->M1.MinDegreePosition;
    m1_option.add("MicroStepsStepperMotor", libconfig::Setting::TypeInt) = opt->M1.MicroStepsStepperMotor;
    m1_option.add("ReductionRatioStepperMotor", libconfig::Setting::TypeFloat) = opt->M1.ReductionRatioStepperMotor;
    m1_option.add("RotationSpeed", libconfig::Setting::TypeFloat) = opt->M1.RotationSpeed;

    if (!ControlOptions.exists(ROOT_NAME_CONTROL_MOTOR2_OPTION))
        ControlOptions.add(ROOT_NAME_CONTROL_MOTOR2_OPTION, libconfig::Setting::TypeGroup);

    libconfig::Setting& m2_option = ControlOptions[ROOT_NAME_CONTROL_MOTOR2_OPTION];

    m2_option.add("MaxDegreePosition", libconfig::Setting::TypeInt) = opt->M2.MaxDegreePosition;
    m2_option.add("MinDegreePosition", libconfig::Setting::TypeInt) = opt->M2.MinDegreePosition;
    m2_option.add("MicroStepsStepperMotor", libconfig::Setting::TypeInt) = opt->M2.MicroStepsStepperMotor;
    m2_option.add("ReductionRatioStepperMotor", libconfig::Setting::TypeFloat) = opt->M2.ReductionRatioStepperMotor;
    m2_option.add("RotationSpeed", libconfig::Setting::TypeFloat) = opt->M2.RotationSpeed;

    if (!ControlOptions.exists(ROOT_NAME_CONTROL_MOUSE_OPTION))
        ControlOptions.add(ROOT_NAME_CONTROL_MOUSE_OPTION, libconfig::Setting::TypeGroup);

    libconfig::Setting& mouse_option = ControlOptions[ROOT_NAME_CONTROL_MOUSE_OPTION];
    mouse_option.add("Sensitivity", libconfig::Setting::TypeFloat) = opt->Mouse.Sensitivity;

    //Aim
    if (!root.exists(ROOT_NAME_AIM_OPTION))
        root.add(ROOT_NAME_AIM_OPTION, libconfig::Setting::TypeGroup);

    libconfig::Setting& aim_option = root[ROOT_NAME_AIM_OPTION];

    aim_option.add("Color", libconfig::Setting::TypeInt) = (int)opt->Aim.Color;
    aim_option.add("FontInfo", libconfig::Setting::TypeString) = opt->Aim.FontInfo;
    aim_option.add("height", libconfig::Setting::TypeInt) = opt->Aim.height;
    aim_option.add("width", libconfig::Setting::TypeInt) = opt->Aim.width;
    aim_option.add("PixelLine", libconfig::Setting::TypeInt) = opt->Aim.PixelLine;
    aim_option.add("X", libconfig::Setting::TypeInt) = opt->Aim.X;
    aim_option.add("Y", libconfig::Setting::TypeInt) = opt->Aim.Y;

    for (cnt = 0; cnt < 3; cnt++)
    {
        group_name = "Text" + std::to_string(cnt);

        if (!aim_option.exists(group_name.c_str()))
            aim_option.add(group_name.c_str(), libconfig::Setting::TypeGroup);

        libconfig::Setting& aim_text_option = aim_option[group_name.c_str()];

        aim_text_option.add("Active", libconfig::Setting::TypeBoolean) = opt->Aim.AimText[cnt].Active;
        aim_text_option.add("LenLine", libconfig::Setting::TypeInt) = opt->Aim.AimText[cnt].LenLine;
        aim_text_option.add("Offset", libconfig::Setting::TypeInt) = opt->Aim.AimText[cnt].Offset;
        aim_text_option.add("Text", libconfig::Setting::TypeString) = opt->Aim.AimText[cnt].Text;
    }
    
    //Damage
    if (!root.exists(ROOT_NAME_DAMAGE_OPTION))
        root.add(ROOT_NAME_DAMAGE_OPTION, libconfig::Setting::TypeGroup);

    libconfig::Setting& damage_option = root[ROOT_NAME_DAMAGE_OPTION];

    damage_option.add("HP", libconfig::Setting::TypeInt) = (int)opt->Damage.HP;
    damage_option.add("DelaySensor_ms", libconfig::Setting::TypeInt) = opt->Damage.DelaySensor_ms;
    damage_option.add("DamageDelay_min", libconfig::Setting::TypeInt) = opt->Damage.DamageDelay_min;
    damage_option.add("DamageDelay_sec", libconfig::Setting::TypeInt) = opt->Damage.DamageDelay_sec;
    damage_option.add("MovementOff", libconfig::Setting::TypeBoolean) = opt->Damage.MovementOff;
    damage_option.add("VideoOff", libconfig::Setting::TypeBoolean) = opt->Damage.VideoOff;
    damage_option.add("WeaponOff", libconfig::Setting::TypeBoolean) = opt->Damage.WeaponOff;

    cfg.writeFile(file_path);
}

int ParametersPrc::loadConfig(void* opt_, const char* file_path)
{
    int cnt;
	libconfig::Config cfg;

    UserParameters_DType* opt = (UserParameters_DType*)opt_;

    try
    {
        cfg.readFile(file_path);
    }
    catch (const libconfig::FileIOException& fioex)
    {
        defaultVideoOption(&opt->Video);
        defaultMotorOption(&opt->M1);
        defaultMotorOption(&opt->M2);
        opt->Mouse.Sensitivity = MOUSE_OPT_DEFAULT_SENS;
        defaultAimOption(&opt->Aim);
        defaultAimTextOption(&opt->Aim);
        return(EXIT_FAILURE);
    }
    catch (const libconfig::ParseException& pex)
    {
        defaultVideoOption(&opt->Video);
        defaultMotorOption(&opt->M1);
        defaultMotorOption(&opt->M2);
        opt->Mouse.Sensitivity = MOUSE_OPT_DEFAULT_SENS;
        defaultAimOption(&opt->Aim);
        defaultAimTextOption(&opt->Aim);
        return(EXIT_FAILURE);
    }

    const libconfig::Setting& root = cfg.getRoot();

    //Video
    try
    {
        const libconfig::Setting& video_options = root[ROOT_NAME_VIDOE_OPTION];

        if (!(video_options.lookupValue("width", opt->Video.width) &&
            video_options.lookupValue("height", opt->Video.height) &&
            video_options.lookupValue("port", opt->Video.port) &&
            video_options.lookupValue("bitrate", opt->Video.bitrate) &&
            video_options.lookupValue("framerate", opt->Video.framerate) &&
            video_options.lookupValue("H26X", (bool&)opt->Video.H26X)))
        {
            defaultVideoOption(&opt->Video);
        }
    }
    catch (const  libconfig::SettingNotFoundException& nfex)
    {
        defaultVideoOption(&opt->Video);
    }

    //Control
    try
    {
        const libconfig::Setting& control = root[ROOT_NAME_CONTROL_OPTION];

        const libconfig::Setting& m1_options = control[ROOT_NAME_CONTROL_MOTOR1_OPTION];

        if (!(m1_options.lookupValue("MaxDegreePosition", opt->M1.MaxDegreePosition) &&
            m1_options.lookupValue("MinDegreePosition", opt->M1.MinDegreePosition) &&
            m1_options.lookupValue("MicroStepsStepperMotor", opt->M1.MicroStepsStepperMotor) &&
            m1_options.lookupValue("ReductionRatioStepperMotor", opt->M1.ReductionRatioStepperMotor) &&
            m1_options.lookupValue("RotationSpeed", opt->M1.RotationSpeed)))
        {
            defaultMotorOption(&opt->M1);
        }

        opt->M1.MaxStepsPosition = TurretParamCalculate::DegreeToSteps(opt->M1.MaxDegreePosition, opt->M1.MicroStepsStepperMotor, opt->M1.ReductionRatioStepperMotor);
        opt->M1.MinStepsPosition = TurretParamCalculate::DegreeToSteps(opt->M1.MinDegreePosition, opt->M1.MicroStepsStepperMotor, opt->M1.ReductionRatioStepperMotor);
        opt->M1.Freq = TurretParamCalculate::Freq(opt->M1.RotationSpeed, opt->M1.MicroStepsStepperMotor, opt->M1.ReductionRatioStepperMotor);
        opt->M1.Period = TurretParamCalculate::PeriodSignal(opt->M1.RotationSpeed, opt->M1.MicroStepsStepperMotor, opt->M1.ReductionRatioStepperMotor);

        const libconfig::Setting& m2_options = control[ROOT_NAME_CONTROL_MOTOR2_OPTION];

        if (!(m2_options.lookupValue("MaxDegreePosition", opt->M2.MaxDegreePosition) &&
            m2_options.lookupValue("MinDegreePosition", opt->M2.MinDegreePosition) &&
            m2_options.lookupValue("MicroStepsStepperMotor", opt->M2.MicroStepsStepperMotor) &&
            m2_options.lookupValue("ReductionRatioStepperMotor", opt->M2.ReductionRatioStepperMotor) &&
            m2_options.lookupValue("RotationSpeed", opt->M2.RotationSpeed)))
        {
            defaultMotorOption(&opt->M2);
        }

        opt->M2.MaxStepsPosition = TurretParamCalculate::DegreeToSteps(opt->M2.MaxDegreePosition, opt->M2.MicroStepsStepperMotor, opt->M2.ReductionRatioStepperMotor);
        opt->M2.MinStepsPosition = TurretParamCalculate::DegreeToSteps(opt->M2.MinDegreePosition, opt->M2.MicroStepsStepperMotor, opt->M2.ReductionRatioStepperMotor);
        opt->M2.Freq = TurretParamCalculate::Freq(opt->M2.RotationSpeed, opt->M2.MicroStepsStepperMotor, opt->M2.ReductionRatioStepperMotor);
        opt->M2.Period = TurretParamCalculate::PeriodSignal(opt->M2.RotationSpeed, opt->M2.MicroStepsStepperMotor, opt->M2.ReductionRatioStepperMotor);

        const libconfig::Setting& mouse_options = control[ROOT_NAME_CONTROL_MOUSE_OPTION];

        if (!mouse_options.lookupValue("Sensitivity", opt->Mouse.Sensitivity))
        {
            opt->Mouse.Sensitivity = MOUSE_OPT_DEFAULT_SENS;
        }
    }
    catch (const  libconfig::SettingNotFoundException& nfex)
    {
        defaultMotorOption(&opt->M1);
        defaultMotorOption(&opt->M2);
        opt->Mouse.Sensitivity = MOUSE_OPT_DEFAULT_SENS;
    }

    //aim
    try
    {
        const libconfig::Setting& aim = root[ROOT_NAME_AIM_OPTION];
        
        std::strcpy(opt->Aim.FontInfo, aim.lookup("FontInfo").c_str());
        
        if (!(aim.lookupValue("X", opt->Aim.X) &&
            aim.lookupValue("Y", opt->Aim.Y) &&
            aim.lookupValue("height", opt->Aim.height) &&
            aim.lookupValue("width", opt->Aim.width) &&
            //aim.lookupValue("FontInfo", font) &&
            aim.lookupValue("Color", (int&)opt->Aim.Color) &&
            aim.lookupValue("PixelLine", opt->Aim.PixelLine)            
            ))
        {
            defaultAimOption(&opt->Aim);
        }        
    }
    catch (const  libconfig::SettingNotFoundException& nfex)
    {
        defaultAimOption(&opt->Aim);
    }

    try
    {
        const libconfig::Setting& aim = root[ROOT_NAME_AIM_OPTION];

        for (cnt = 0; cnt < 3; cnt++)
        {
            std::string group_name = "Text" + std::to_string(cnt);
            const libconfig::Setting& aim_text = aim[group_name.c_str()];

            if (aim_text.lookupValue("Active", opt->Aim.AimText[cnt].Active) &&
                aim_text.lookupValue("LenLine", opt->Aim.AimText[cnt].LenLine) &&
                //aim_text.lookupValue("Text", text) &&
                aim_text.lookupValue("Offset", opt->Aim.AimText[cnt].Offset)
                )
            {
                std::strcpy(opt->Aim.AimText[cnt].Text, aim_text.lookup("Text").c_str());
            }
            else
            {
                defaultAimTextOption(&opt->Aim);
            }
        }
    }
    catch (const  libconfig::SettingNotFoundException& nfex)
    {
        defaultAimTextOption(&opt->Aim);
    }

    //Damage
    try
    {
        const libconfig::Setting& damage = root[ROOT_NAME_DAMAGE_OPTION];

        if (!(damage.lookupValue("HP", opt->Damage.HP) &&
            damage.lookupValue("DelaySensor_ms", opt->Damage.DelaySensor_ms) &&
            damage.lookupValue("DamageDelay_min", opt->Damage.DamageDelay_min) &&
            damage.lookupValue("DamageDelay_sec", opt->Damage.DamageDelay_sec) &&
            damage.lookupValue("MovementOff", opt->Damage.MovementOff) &&
            damage.lookupValue("VideoOff", opt->Damage.VideoOff) &&
            damage.lookupValue("WeaponOff", opt->Damage.WeaponOff)
            ))
        {
            defaultDamageOption(&opt->Damage);
        }
    }
    catch (const  libconfig::SettingNotFoundException& nfex)
    {
        defaultDamageOption(&opt->Damage);
    }

    return EXIT_SUCCESS;
}

void ParametersPrc::defaultVideoOption(VideoOption_DType* opt)
{
    opt->width = VIDEO_OPT_DEFAULT_WIDTH;
    opt->height = VIDEO_OPT_DEFAULT_HEIGHT;
    opt->port = VIDEO_OPT_DEFAULT_PORT;
    opt->bitrate = VIDEO_OPT_DEFAULT_BITRATE_K;
    opt->framerate = VIDEO_OPT_DEFAULT_FRAMERATE;
    opt->H26X = static_cast<CodecH26X_DType>(VIDEO_OPT_DEFAULT_H265);
}

void ParametersPrc::defaultMotorOption(MotorOption_DType* opt)
{
    opt->MaxDegreePosition = MOTOR_OPT_DEFAULT_MAX_DEGREE_POSITION;
    opt->MinDegreePosition = MOTOR_OPT_DEFAULT_MIN_DEGREE_POSITION;
    opt->MicroStepsStepperMotor = MOTOR_OPT_DEFAULT_MICRO_STEPS;
    opt->ReductionRatioStepperMotor = MOTOR_OPT_DEFAULT_REDUCTION_RATIO;
    opt->RotationSpeed = MOTOR_OPT_DEFAULT_ROTATION_SPEED;

    opt->MinStepsPosition = TurretParamCalculate::DegreeToSteps(opt->MinDegreePosition, opt->MicroStepsStepperMotor, opt->ReductionRatioStepperMotor);
    opt->Freq = TurretParamCalculate::Freq(opt->RotationSpeed, opt->MicroStepsStepperMotor, opt->ReductionRatioStepperMotor);
    opt->Period = TurretParamCalculate::PeriodSignal(opt->RotationSpeed, opt->MicroStepsStepperMotor, opt->ReductionRatioStepperMotor);
}

void ParametersPrc::defaultAimOption(AimOption_DType* opt)
{
    opt->height = AIM_OPT_DEFAULT_HEIGHT;
    opt->width = AIM_OPT_DEFAULT_WIDTH;
    opt->X = AIM_OPT_DEFAULT_X;
    opt->Y = AIM_OPT_DEFAULT_Y;
    opt->Color = AIM_OPT_DEFAULT_COLOR; 
    opt->PixelLine = AIM_OPT_DEFAULT_PIXEL_LINE;
}

void ParametersPrc::defaultAimTextOption(AimOption_DType* opt)
{
    opt->AimText[0].Active = AIM_OPT_DEFAULT_TEXT1_ACTIVE;
    opt->AimText[0].LenLine = AIM_OPT_DEFAULT_TEXT1_LEN_LINE;
    opt->AimText[0].Offset = AIM_OPT_DEFAULT_TEXT1_OFFSET;
    std::strcpy(opt->AimText[0].Text, AIM_OPT_DEFAULT_TEXT1_TEXT);

    opt->AimText[1].Active = AIM_OPT_DEFAULT_TEXT2_ACTIVE;
    opt->AimText[1].LenLine = AIM_OPT_DEFAULT_TEXT2_LEN_LINE;
    opt->AimText[1].Offset = AIM_OPT_DEFAULT_TEXT2_OFFSET;
    std::strcpy(opt->AimText[1].Text, AIM_OPT_DEFAULT_TEXT2_TEXT);

    opt->AimText[2].Active = AIM_OPT_DEFAULT_TEXT3_ACTIVE;
    opt->AimText[2].LenLine = AIM_OPT_DEFAULT_TEXT3_LEN_LINE;
    opt->AimText[2].Offset = AIM_OPT_DEFAULT_TEXT3_OFFSET;
    std::strcpy(opt->AimText[2].Text, AIM_OPT_DEFAULT_TEXT3_TEXT);
}

void ParametersPrc::defaultDamageOption(DamageOption_DType* opt)
{
    opt->DamageDelay_min = DAMAGE_OPT_DEFAULT_DAMAGE_DELAY_MIN;
    opt->DamageDelay_sec = DAMAGE_OPT_DEFAULT_DAMAGE_DELAY_SEC;
    opt->DelaySensor_ms = DAMAGE_OPT_DEFAULT_DELAY_SENSOR_MS;
    opt->HP = DAMAGE_OPT_DEFAULT_HP;
    opt->MovementOff = DAMAGE_OPT_DEFAULT_MOVEMENT_OFF;
    opt->VideoOff = DAMAGE_OPT_DEFAULT_VIDEO_OFF;
    opt->WeaponOff = DAMAGE_OPT_DEFAULT_WEAPON_OFF;
}