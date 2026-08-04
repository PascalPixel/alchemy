#include "types.h"

/*
 * Complete actor-18 dialogue/restoration scene.  If cue 231 remains available
 * and its movement scene has not set flag 0x858, the shared scene marker at
 * +370 is enabled before the dialogue scene closes.
 */
struct SceneWork_02000e90 {
    u8 reserved000[370];
    u16 actor18_marker;
};

extern struct SceneWork_02000e90 *Data_03001ebc;
extern void Func_0808a018(void);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a150(s32 actor, s32 mode, s32 value);
extern void Func_0808a010(s32 frames);
extern void Func_0808a180(s32 actor, s32 mode);
extern s32 Func_08077040(s32 cue);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a020(void);

void Func_02000e90(void)
{
    Func_0808a018();
    Func_0808a170(0x1342);
    Func_0808a100(18, 0);
    Func_0808a150(18, 0, 0);
    Func_0808a010(2);
    Func_0808a180(18, 0);
    Func_0808a100(18, 1);

    if (Func_08077040(231) != -1 && Func_080770c0(0x858) == 0) {
        Data_03001ebc->actor18_marker = 1;
    }

    Func_0808a020();
}
