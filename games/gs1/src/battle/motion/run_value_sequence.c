#include "types.h"

struct ObjectSlot_080babdc {
    void *object;
};

struct Runtime_080babdc {
    u8 padding_00[65];
    u8 mode;
};

extern struct Runtime_080babdc *Data_03001e74;

void *Runtime_GetObject(s32 id);
struct ObjectSlot_080babdc *GetBattleObjectSlot(s32 id);
void Object_SetMode(void *object, s32 mode);
void Func_080152b8(u16 *selection);
void BattleMotion_SetRecordChildValues(void *object, s32 value);
void WaitFrames(s32 frames);
s32 Func_080b6cd0(s32 id);
void Func_08015130(s32 mode);

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
        Func_080152b8(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, 7);
        WaitFrames(2);

        sel[0] = id;
        Func_080152b8(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, Func_080b6cd0(id));
        WaitFrames(2);
        remaining--;
    } while (remaining >= 0);

    Func_08015130(Data_03001e74->mode);
}
