#include "types.h"

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

extern struct FieldEffectState *Data_03001ebc;

s32 Func_08077158(s16 *);
void Func_08077230(s32);
struct FieldObject *Runtime_GetObject(s32);
void Func_08077128(s32);
void WaitFrames(s32);
void Func_0808a368(void);
void Func_0808a370(void);
void Audio_PlayCue(s32);
void Func_080b04c4(void);
void Func_0808a360(void);

void Inn_PlaySleepSequence(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct FieldObject *object;
    struct FieldEffectState *state;

    count = Func_08077158(objects);
    Func_08077230(-room_price);

    for (index = 0; index < count; index++) {
        object = Runtime_GetObject(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
            Func_08077128(objects[index]);
        }
    }

    state = Data_03001ebc;
    state->effect = 0x209;
    state->delay = 60;
    WaitFrames(20);
    Func_0808a368();
    Func_0808a370();
    Audio_PlayCue(86);
    Func_080b04c4();
    WaitFrames(10);
    Func_0808a360();
    Func_0808a370();
    WaitFrames(30);
    Data_03001ebc->delay = 16;
}
