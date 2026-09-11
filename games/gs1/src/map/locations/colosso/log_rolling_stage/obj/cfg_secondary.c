#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/obj/cfg_secondary.h"
#include "colosso_log_rolling_stage.h"

typedef struct SecondaryStageObject {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x1c];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} SecondaryStageObject;

extern s16 gCell[];

SecondaryStageObject *Map_Run();

SecondaryStageObject *Map_Run2();

void Colosso_ConfigureSecondaryObjects(void)
{
    s16 *table;
    SecondaryStageObject *object;

    table = gCell;

    Map_Run3(*(s32 *)&table[250], 1);

    object = Map_Run(11);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Map_Run4(object, object->x, 0x40000, object->z);

    object = Map_Run2(10);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Map_Run5(object, object->x, 0x200000, object->z);

    Map_Run6(10);
    {
        s32 stack_first = 9;
        s32 stack_second = 12;
        Map_Run7(0, 25, 1, 1, stack_first, stack_second);
    }
    Map_Run8(2);
    Map_Run9(0x367);
}
