#include "scene.h"
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

SecondaryStageObject *Map_unk2_4();

void Colosso_ConfigureSecondaryObjects(void)
{
    s16 *table;
    SecondaryStageObject *object;

    table = gCell;

    Map_unk3_4(*(s32 *)&table[250], 1);

    object = Map_Run(11);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Map_unk4_4(object, object->x, 0x40000, object->z);

    object = Map_unk2_4(10);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Map_unk5_4(object, object->x, 0x200000, object->z);

    Map_unk6_4(10);
    {
        s32 stack_first = 9;
        s32 stack_second = 12;
        Map_unk7_4(0, 25, 1, 1, stack_first, stack_second);
    }
    Map_unk8_4(2);
    Map_unk9_4(0x367);
}
