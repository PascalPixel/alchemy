#include "effect_runtime.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(s32, s32);
s32 Func_0808ec14();
s32 Audio_PlayCue(s32);

void Func_0808ece0(void) {
    s32 slot;
    s32 object;

    slot = Func_0808ec14();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 4);
        }
        Audio_PlayCue(0x7C);
        WaitFrames(0xC);
    }
}
