#include "types.h"
#include "scene.h"

struct SceneCameraState {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct SceneCameraRuntime {
    struct SceneCameraState *state;
    u8 filler04[124];
    void *secondary;
};

struct SceneCameraTransfer {
    s32 first;
    s32 second;
    s32 third;
};

struct SceneCameraObject {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0c;
    s32 anchor;
};

extern struct SceneCameraRuntime gIw;
extern struct SceneCameraObject gIw2;

void Camera_ResetSceneDefaults(void)
{
    struct SceneCameraState *state = gIw.state;
    struct SceneCameraTransfer local;
    u32 result;
    u32 param1;

    state->field34 = 152 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field36 = 0;
    state->field1c = 0;
    gIw2.field0c = 0;
    gIw2.anchor = 0;
    state->field18 = 0;

    Sys_Run();
    Sys_Do(&state->field0c);
    Sys_unk2_2(state->field36);
    Sys_unk3_2(state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    ((void (*)(struct SceneCameraTransfer *, struct SceneCameraState *))0x03000250)(&local, state);

    param1 = 250;
    param1 = param1 << 16;
    result = ((u32 (*)(u32, u32))0x0300013c)(param1, 192 << 8);

    param1 = 250;
    param1 = param1 << 16;
    Sys_Place(param1, result, 0x7fff0000);
}
