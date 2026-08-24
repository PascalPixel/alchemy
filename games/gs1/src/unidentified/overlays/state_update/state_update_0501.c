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










extern void Func_02001f7c(void);
extern void Func_02002002(s32 dialogue);
extern void Func_02001fda(s32 actor, s32 mode);
extern void Func_02002004(s32 actor, s32 mode, s32 value);
extern void Func_02001f92(s32 frames);
extern void Func_02002032(s32 actor, s32 mode);
extern void Func_02001ffa(s32 actor, s32 mode);
extern s32 Func_02001f80(s32 cue);
extern s32 Func_02001f96(s32 flag);
extern void Func_02001fdc(void);
void Func_02000e90(void)
{
    Func_02001f7c();
    Func_02002002(0x1342);
    Func_02001fda(18, 0);
    Func_02002004(18, 0, 0);
    Func_02001f92(2);
    Func_02002032(18, 0);
    Func_02001ffa(18, 1);

    if (Func_02001f80(231) != -1 && Func_02001f96(0x858) == 0) {
        Data_03001ebc->actor18_marker = 1;
    }

    Func_02001fdc();
}
