/* Contiguous unnamed state-owner run for resource_385. */

#include "types.h"

/* Complete paired scene-cell presentation update. */



extern void Func_02001ede(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
extern void Func_02001ee8(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);
void Func_02000e30(void)
{
    s32 first_value = 13;
    s32 second_value = 25;

    Func_02001ede(41, 43, 1, 1, first_value, second_value);
    Func_02001ee8(40, 42, 12, 22, 3, 3);
}

#include "types.h"

/* Complete alternate paired scene-cell presentation update. */
/*
 * Both calls are named at their decoded bl sites, 0x02001f0e and 0x02001f18:
 * the semantic targets are the main-image pair Func_080091c0/Func_08009180,
 * but a direct bl cannot encode that displacement, and only the RAM-resident
 * sites reproduce the reference bytes.  The reference also materializes both
 * stacked constants before either store, which named locals preserve.
 */
extern void Func_02001f0e(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
extern void Func_02001f18(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);

void Func_02000e60(void)
{
    s32 first_value = 13;
    s32 second_value = 25;

    Func_02001f0e(37, 43, 1, 1, first_value, second_value);
    Func_02001f18(36, 42, 12, 22, 3, 3);
}

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

