#include "types.h"

/* Alternate scene-camera setup used by the later field presentation. */
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

struct SceneCameraAuxiliary {
    s32 field00;
    u8 filler04[12];
    s32 field10;
    s32 field14;
};

struct SceneCameraRuntime {
    struct SceneCameraState *state;
    u8 filler04[124];
    struct SceneCameraAuxiliary *secondary;
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

extern struct SceneCameraRuntime Data_03001e80;
extern struct SceneCameraObject Data_03001ce0;

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_08005258(u32, u32, u32);
void Func_080c0a24(u32, u32, s32, s32, u32);

/* Keep the address symbol for the build map while exposing its role to C. */
#define ConfigureSceneCamera Func_080b8fd4
void Func_080b8fd4(s32 arg0) {
    struct SceneCameraState *state = Data_03001e80.state;
    struct SceneCameraAuxiliary *secondary = Data_03001e80.secondary;
    struct SceneCameraTransfer local;
    u32 result;

    state->field10 = 160 << 11;
    state->field0c = 0;
    state->field14 = 0;
    secondary->field00 = 128 << 7;
    state->field36 = 128 << 7;
    state->field34 = 244 << 8;
    state->field1c = 0;
    state->field20 = 0x02ee0000;
    state->field18 = 0;

    Func_080049ac();
    Func_08004cb4(&state->field0c);
    Func_08004c1c(state->field36);
    Func_08004bd4(state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    ((void (*)(struct SceneCameraTransfer *, struct SceneCameraState *))0x03000250)(&local, state);

    result = ((u32 (*)(u32, u32))0x0300013c)(0x03c90000, 192 << 8);
    Func_08005258(0, result, 0x07920000);

    Data_03001ce0.anchor = arg0 + 120;
    secondary->field10 = 1;
    Func_080c0a24(240 << 15, (0x76 - arg0) << 16, 0, 128 << 4, 128 << 10);
    secondary->field14 = 1;
    secondary->field10 = 0;
}
