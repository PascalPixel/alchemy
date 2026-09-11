#include "types.h"
#include "scene.h"
#include "abi/battle/motion/run_value_sequence.h"

struct ObjectSlot_080babdc {
    void *object;
};

struct Runtime_080babdc {
    u8 padding_00[65];
    u8 mode;
};

extern struct Runtime_080babdc *gBattleWork;

void *Runtime_GetObject(s32 id);
struct ObjectSlot_080babdc *GetBattleObjectSlot(s32 id);
void Object_SetMode(void *object, s32 mode);

void BattleMotion_SetRecordChildValues(void *object, s32 value);
void WaitFrames(s32 frames);

void BattleMotion_RunValueSequence(s32 id)
{
    u16 selection[2];
    u16 *sel;
    s32 target;
    s32 remaining;

    Runtime_GetObject(id);
    Object_SetMode(GetBattleObjectSlot(id)->object, 5);

    sel = selection;
    remaining = 1;
    do {
        target = 0xff;
        sel[1] = target;
        sel[0] = id;
        Battle_Do(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, 7);
        WaitFrames(2);

        sel[0] = id;
        Battle_Do(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, Battle_Check(id));
        WaitFrames(2);
        remaining--;
    } while (remaining >= 0);

    Battle_Do2(gBattleWork->mode);
}
