#include "types.h"

/* Complete actor-zero placement and scene-delay setup helper through its pool. */
struct SceneWork_02000a80 {
    u8 reserved000[456];
    s32 scene_delay;
};

extern struct SceneWork_02000a80 *Data_03001ebc;




extern void Func_02001ba0(s32 actor, s32 width, s32 height);
extern void Func_02001bc2(s32 actor, s32 x, s32 y);
extern void Func_02001c56(s32 value);
void Func_02000a80(s32 x, s32 y, s32 value)
{
    Func_02001ba0(0, 0x8000, 0x4000);
    Func_02001bc2(0, x, y);
    Data_03001ebc->scene_delay = 16;
    Func_02001c56(value);
}
