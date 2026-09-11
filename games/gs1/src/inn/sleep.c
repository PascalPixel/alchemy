#include "types.h"
#include "scene.h"
#include "abi/inn/sleep.h"

struct FieldEffectState {
    u8 padding0[0x1C0];
    s32 effect;
    u8 padding1C4[4];
    s32 delay;
};

struct FieldObject {
    u8 padding0[0x34];
    u16 saved_x;
    u16 saved_y;
    s16 x;
    u16 y;
};

extern struct FieldEffectState *gWork;

struct FieldObject *Runtime_GetObject(s32);

void WaitFrames(s32);

void Audio_PlayCue(s32);
void AudioCommand_WaitForStateByteClear(void);

void Inn_PlaySleep(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct FieldObject *object;
    struct FieldEffectState *state;

    count = Sys_Check(objects);
    Sys_Do(-room_price);

    for (index = 0; index < count; index++) {
        object = Runtime_GetObject(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
            Sys_Do2(objects[index]);
        }
    }

    state = gWork;
    state->effect = 0x209;
    state->delay = 60;
    WaitFrames(20);
    Sys_Run();
    Sys_Run2();
    Audio_PlayCue(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Sys_Run3();
    Sys_Run2();
    WaitFrames(30);
    gWork->delay = 16;
}
