#include "colosso_log_rolling_stage.h"

typedef struct PrimaryStageObject {
    u8 pad00[8];
    s32 x;
    u8 pad0C[4];
    s32 y;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_z;
    u8 pad20[0x10];
    s32 move_rate_x;
    s32 move_rate_z;
} PrimaryStageObject;

void Func_02004bae(PrimaryStageObject *, s32, s32, s32);
void Func_02004bc4(PrimaryStageObject *, s32, s32, s32);
void Func_02004c0e(s32, s32, s32, s32, s32, s32);
void Func_02004c20(s32, s32, s32, s32, s32, s32);
void Func_02004c72(s32);
PrimaryStageObject *Func_02004c94(s32);
PrimaryStageObject *Func_02004ca2(s32);
PrimaryStageObject *Func_02004cbc(s32);

void ColossoLogRollingStage_ConfigurePrimaryObjects(void)
{
    PrimaryStageObject *object;

    object = Func_02004c94(9);
    object->scale_x = 0x10000;
    object->scale_z = 0x10000;

    object = Func_02004ca2(11);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Func_02004bae(object, object->x, 0x200000, object->y);

    object = Func_02004cbc(10);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Func_02004bc4(object, object->x, 0x40000, object->y);

    Func_02004c72(0x362);
    Func_02004c0e(15, 12, 1, 1, 13, 12);
    Func_02004c20(14, 12, 1, 1, 9, 12);
}
