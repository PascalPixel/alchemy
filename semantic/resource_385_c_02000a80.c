#include "types.h"

/* Complete actor-zero placement and scene-delay setup helper through its pool. */
struct SceneWork_02000a80 {
    u8 reserved000[456];
    s32 scene_delay;
};

extern struct SceneWork_02000a80 *Data_03001ebc;
extern void Func_0808a090(s32 actor, s32 width, s32 height);
extern void Func_0808a0c8(s32 actor, s32 x, s32 y);
extern void Func_0808a248(s32 value);

void Func_02000a80(s32 x, s32 y, s32 value)
{
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a0c8(0, x, y);
    Data_03001ebc->scene_delay = 16;
    Func_0808a248(value);
}
