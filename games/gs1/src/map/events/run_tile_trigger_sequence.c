#include "object_lookup.h"
#include "types.h"

struct Controller_08099738 {
    u8 pad_00[5];
    u8 field_05;
};

struct State_08099738 {
    u8 pad_00[0x25];
    u8 field_25;
    u8 field_26;
    u8 pad_27;
    struct Controller_08099738 *controller;
};

struct Object_08099738 {
    u8 pad_00[0x50];
    struct State_08099738 *state;
    u8 pad_54[0x18];
    u32 field_6c;
};

struct Global_08099738 {
    u8 pad_000[0x1F4];
    u32 object_id;
    u8 pad_1f8[0x54];
    u16 field_24c;
};

extern struct Global_08099738 Data_02000240;

void Audio_PlayCue(s32);
s32 ScheduleCallback(void (*callback)(void));
void Object_SetMode(struct Object_08099738 *, s32);
void WaitFrames(s32);
void CheckObjectMapTile(void);

void MapEvent_RunTileTriggerSequence(void)
{
    struct Object_08099738 *object;
    struct State_08099738 *state;
    struct Controller_08099738 *controller;
    u32 i;

    object = ObjectTable_Get(Data_02000240.object_id);
    state = object->state;
    controller = state->controller;

    Audio_PlayCue(154);
    ScheduleCallback(CheckObjectMapTile);
    Object_SetMode(object, 0);
    object->field_6c = 0;

    for (i = 0; i < 5; ++i) {
        controller->field_05 = 7;
        state->field_25 = 1;
        state->field_26 = 2;
        WaitFrames(2);
        state->field_25 = 1;
        state->field_26 = 0;
        WaitFrames(2);
    }

    for (i = 0; i < 5; ++i) {
        controller->field_05 = 7;
        state->field_25 = 1;
        state->field_26 = 0;
        WaitFrames(2);
        controller->field_05 = 0;
        state->field_25 = 1;
        WaitFrames(2);
    }

    state->field_26 = 1;
    Data_02000240.field_24c = 0;
}
