#pragma once
#ifndef MAIN_H
#define MAIN_H


#include "resource.h"
#include "wx/wxprec.h"
#include "windows.h"

#define VIDEO_OPT_DEFAULT_WIDTH 1280
#define VIDEO_OPT_DEFAULT_HEIGHT 720
#define VIDEO_OPT_DEFAULT_PORT 20100
#define VIDEO_OPT_DEFAULT_BITRATE_K 3000
#define VIDEO_OPT_DEFAULT_FRAMERATE 25
#define VIDEO_OPT_DEFAULT_H265 0

#define MOTOR_OPT_DEFAULT_MAX_DEGREE_POSITION 100
#define MOTOR_OPT_DEFAULT_MIN_DEGREE_POSITION 100
#define MOTOR_OPT_DEFAULT_MICRO_STEPS 1280
#define MOTOR_OPT_DEFAULT_REDUCTION_RATIO 20
#define MOTOR_OPT_DEFAULT_ROTATION_SPEED 5.0

#define MOUSE_OPT_DEFAULT_SENS 1.0

#define AIM_OPT_DEFAULT_WIDTH 200
#define AIM_OPT_DEFAULT_HEIGHT 170
#define AIM_OPT_DEFAULT_X VIDEO_OPT_DEFAULT_WIDTH / 2
#define AIM_OPT_DEFAULT_Y VIDEO_OPT_DEFAULT_HEIGHT / 2
#define AIM_OPT_DEFAULT_COLOR 0x80000000
#define AIM_OPT_DEFAULT_PIXEL_LINE 3
#define AIM_OPT_DEFAULT_TEXT1_ACTIVE 1
#define AIM_OPT_DEFAULT_TEXT1_OFFSET 40
#define AIM_OPT_DEFAULT_TEXT1_LEN_LINE 120
#define AIM_OPT_DEFAULT_TEXT1_TEXT "30м"
#define AIM_OPT_DEFAULT_TEXT2_ACTIVE 1
#define AIM_OPT_DEFAULT_TEXT2_OFFSET 80
#define AIM_OPT_DEFAULT_TEXT2_LEN_LINE 90
#define AIM_OPT_DEFAULT_TEXT2_TEXT "60м"
#define AIM_OPT_DEFAULT_TEXT3_ACTIVE 1
#define AIM_OPT_DEFAULT_TEXT3_OFFSET 120
#define AIM_OPT_DEFAULT_TEXT3_LEN_LINE 60
#define AIM_OPT_DEFAULT_TEXT3_TEXT "90м"

#define DAMAGE_OPT_DEFAULT_DELAY_SENSOR_MS 100
#define DAMAGE_OPT_DEFAULT_DAMAGE_DELAY_MIN 1
#define DAMAGE_OPT_DEFAULT_DAMAGE_DELAY_SEC 30
#define DAMAGE_OPT_DEFAULT_HP 20
#define DAMAGE_OPT_DEFAULT_VIDEO_OFF false
#define DAMAGE_OPT_DEFAULT_WEAPON_OFF true
#define DAMAGE_OPT_DEFAULT_MOVEMENT_OFF false

enum ID_COMMANDS
{
    ID_BUTTON_CONNECT_MAINFRAME = wxID_HIGHEST,
    ID_BUTTON_DISCONNECT_MAINFRAME,
    ID_BUTTON_MOUSE_MAINFRAME,
    ID_BUTTON_KEYBOARD_MAINFRAME,
    ID_BUTTON_RESET_HP_MAINFRAME,
    ID_MENU_FILE_SAVE_COFIG,
    ID_MENU_FILE_LOAD_COFIG,
    ID_MENU_FILE_SAVE_TURRET,
    ID_MENU_FILE_LOAD_TURRET,
    ID_MENU_OPTION_VIDEO,
    ID_MENU_OPTION_DAMAGE,
    ID_MENU_OPTION_CONTROLE,
    ID_MENU_OPTION_AIM,
    ID_PANEL_VIDEO,

    ID_COMBOBOX_VIDEO_RESOLUTION_OPTION,
    ID_SPIN_BITRATE_OPTION,
    ID_SPIN_FRAMERATE_OPTION,
    ID_COMBOBOX_CODEC_OPTION,
    ID_SPIN_PORT_OPTION,

    ID_SPIN_MIN_DEGREE_POSITION_M1,
    ID_SPIN_MAX_DEGREE_POSITION_M1,
    ID_SPIN_ROTATION_SPEED_M1,
    ID_SPIN_MICRO_STEPS_M1,
    ID_SPIN_REDUCTION_RATIO_M1,
    ID_SPIN_MIN_DEGREE_POSITION_M2,
    ID_SPIN_MAX_DEGREE_POSITION_M2,
    ID_SPIN_ROTATION_SPEED_M2,
    ID_SPIN_MICRO_STEPS_M2,
    ID_SPIN_REDUCTION_RATIO_M2,
    ID_BUTTON_RESET_POSITION,
    ID_CHECKBOX_NO_LIMIT,
    ID_SLIDER_MOUSE,

    ID_BUTTON_AIM_FONT,
    ID_SPIN_AIM_X,
    ID_SPIN_AIM_Y,
    ID_SPIN_AIM_WIDTH,
    ID_SPIN_AIM_HEIGHT,
    ID_SPIN_AIM_PIXEL_LINE,
    ID_BUTTON_AIM_COLOR,
    ID_BUTTON_AIM_TEXT1_ACTIVE,
    ID_BUTTON_AIM_TEXT1_OFFSET,
    ID_BUTTON_AIM_TEXT1_LEN,
    ID_BUTTON_AIM_TEXT1_TEXT,
    ID_BUTTON_AIM_TEXT2_ACTIVE,
    ID_BUTTON_AIM_TEXT2_OFFSET,
    ID_BUTTON_AIM_TEXT2_LEN,
    ID_BUTTON_AIM_TEXT2_TEXT,
    ID_BUTTON_AIM_TEXT3_ACTIVE,
    ID_BUTTON_AIM_TEXT3_OFFSET,
    ID_BUTTON_AIM_TEXT3_LEN,
    ID_BUTTON_AIM_TEXT3_TEXT,

    ID_CHECKBOX_DAMAGE_MOVEMENT_OFF,
    ID_CHECKBOX_DAMAGE_WEAPON_OFF,
    ID_CHECKBOX_DAMAGE_VIDEO_OFF,
    ID_SPIN_DAMAGE_DELAY_SENSOR_MS,
    ID_SPIN_DAMAGE_DELAY_DAMAGE_MIN,
    ID_SPIN_DAMAGE_DELAY_DAMAGE_SEC,
    ID_SPIN_DAMAGE_DELAY_HP,
};

typedef enum {
    CODEC_H264,
    CODEC_H265,
}CodecH26X_DType;

typedef struct
{
    int width;	// 
    int height;  //
    int port; //
    int bitrate; //2500
    int framerate; //
    CodecH26X_DType H26X;
}VideoOption_DType;

typedef struct
{
    int MaxStepsPosition;           //Ограничение вращения в шагах
    int MinStepsPosition;
    int MaxDegreePosition;	        //Ограничение вращения в градусах
    int MinDegreePosition;
    float RotationSpeed;	        //Скорость вращения град/сек
    int MicroStepsStepperMotor;	        //Шагов шагового двигателя на 1 оборот
    float ReductionRatioStepperMotor;	//Коэффициегт редукции редуктора на двигателе
    float Freq;	                    //Частота управляющего сигнала гц
    float Period;	                //Период сек
}MotorOption_DType;

typedef struct
{
    float Sensitivity;
}MouseOptoin_DType;

typedef struct
{
    bool Active;
    int Offset;
    int LenLine;
    char Text[40];
}AimText_DType;

typedef struct
{
    int X;
    int Y;
    int width;
    int height;
    int PixelLine;
    char FontInfo[255];
    AimText_DType AimText[3];
    unsigned int Color;
}AimOption_DType;

typedef struct
{
    int DelaySensor_ms;

    int DamageDelay_min;
    int DamageDelay_sec;
    int HP;

    bool MovementOff;
    bool WeaponOff;
    bool VideoOff;
}DamageOption_DType;

typedef struct
{
    MotorOption_DType M1;
    MotorOption_DType M2;
    VideoOption_DType Video;
    MouseOptoin_DType Mouse;
    AimOption_DType Aim;
    DamageOption_DType Damage;
    int NoLimit;
}UserParameters_DType;



// Define a new application
class TurretControlApp : public wxApp
{
    HWND hWnd;
public:
    bool OnInit() override;
};

extern const char* gun_xpm[];

#endif