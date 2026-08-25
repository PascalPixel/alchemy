#include "types.h"

struct SceneState {
    u8 reserved_00[14];
    s16 presentation_phase;
};

struct Actor {
    u8 reserved_00[35];
    u8 presentation_flags;
};

struct SceneState *Func_020016fa(s32);
struct Actor *Func_02001708(s32);
struct Actor *Func_02001718(s32);

void Func_02000324(void)
{
    struct SceneState *scene = Func_020016fa(0);
    u8 *flags;
    u8 value;

    if (scene->presentation_phase > 31) {
        flags = &Func_02001708(20)->presentation_flags;
        value = *flags | 2;
    } else {
        flags = &Func_02001718(20)->presentation_flags;
        value = *flags;
        value &= ~2;
    }
    *flags = value;
}
