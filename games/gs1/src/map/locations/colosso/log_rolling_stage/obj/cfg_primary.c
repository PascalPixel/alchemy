#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/obj/cfg_primary.h"
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

PrimaryStageObject *Map_Run(s32);
PrimaryStageObject *Map_Run2(s32);
PrimaryStageObject *Map_Run3(s32);

void Colosso_ConfigurePrimaryObjects(void)
{
    PrimaryStageObject *object;

    object = Map_Run(9);
    object->scale_x = 0x10000;
    object->scale_z = 0x10000;

    object = Map_Run2(11);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Map_SetMode(object, object->x, 0x200000, object->y);

    object = Map_Run3(10);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Map_SetMode2(object, object->x, 0x40000, object->y);

    Map_Do(0x362);
    Map_SetRect(15, 12, 1, 1, 13, 12);
    Map_SetRect2(14, 12, 1, 1, 9, 12);
}
