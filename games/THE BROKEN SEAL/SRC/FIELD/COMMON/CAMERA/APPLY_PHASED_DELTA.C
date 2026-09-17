#include "TYPES.H"
#include "SCENE.H"

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

extern u32 gIw;

void Camera_ApplyPhasedDelta(void)
{
    u8 *state = (u8 *)gIw;
    struct SceneCameraState *camera = *(struct SceneCameraState **)((u8 *)&gIw - 108);
    u32 *phase = (u32 *)(state + 0x77B0);

    if (*phase == 1) {
        camera->field36 += *(s32 *)(state + 0x77AC);
        *phase = 0;
    } else {
        camera->field36 += *(s32 *)(state + 0x77AC) / 2;
        if (*phase == 2)
            *phase = 0;
        else
            *phase = 2;
    }
}
