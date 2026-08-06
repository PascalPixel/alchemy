#include "types.h"

/* STILL-OPEN: adopt --where differing_bytes=116/308. alchemist.ts exhausted
 * (tiers: class, depend-count, model-divergence, original-order, priority;
 * 11 licensed compiles, no improving move found). Genuinely unfixed within
 * budget -- large multi-tier residual, not a simple symbol issue. */

/*
 * Scene-one presentation script.  The shared scene state supplies the early
 * teardown path; otherwise a status probe selects the one-time setup, retry,
 * and common closing paths.  Calls are intentionally written in reachable
 * machine order so the complete 29-site inventory stays reviewable.
 */
extern s16 Data_02000240[];






















extern void Func_020030fa();
extern void Func_02005244();
extern s32 Func_02003310();
extern void Func_020052f0();
extern void Func_02005344();
extern void Func_0200535e();
extern void Func_0200536a();
extern void Func_0200532a();
extern void Func_02003efe();
extern void Func_020052c8();
extern void Func_0200445a();
extern void Func_02004466();
extern void Func_02004470();
extern void Func_02005370();
extern void Func_02004482();
extern void Func_0200448e();
extern void Func_020052dc();
extern u8 * Func_02005302();
extern void Func_02005358();
extern void Func_020053d2();
extern void Func_02005300();
extern void Func_020053b8();
extern void Func_02004162();
extern void Func_020053f6();
extern void Func_020034a6();
extern void Func_020053ca();
extern void Func_020053e2();
extern void Func_02003520();
extern void Func_02005350();
void Func_020012f0(s32 scene)
{
    s32 status;

    if (Data_02000240[225] == 2) {
        Func_020030fa(scene);
        return;
    }

    Func_02005244();
    status = Func_02003310(scene, 1);

    if (status == 0) {
        u8 *actor;

        Func_020052f0(0x209e);
        Func_02005344(0x30000, 0x6000);
        Func_0200535e(0x4c80000, -1, 0xb80000, 1);
        Func_0200536a();
        Func_0200532a(scene, 0);
        Func_02003efe(0, 0x4f8, 168);
        Func_020052c8(0, 0x18000, 0xc000);
        Func_0200445a(0, 0x508, 184);
        Func_02004466(0, 0x508, 216);
        Func_02004470(0, 0x4c8, 216);
        Func_02005370(scene, 0);
        Func_02004482(0, 0x4c8, 248);
        Func_0200448e(0, 0x4a8, 248);
        Func_020052dc(3);
        actor = Func_02005302(0);
        *(s32 *)(actor + 40) = 0x40000;
        Func_02005358(0, 28);
        Func_020053d2(0, 258);
        Func_02005300(30);
        Func_020053b8(scene, 0);
        Func_02004162(0);
        Func_020053f6(0, 0);
        Func_020034a6(scene, 1);
    } else if (status == 1) {
        Func_020053ca(0x209d);
        Func_020053e2(scene, 0);
    }

    Func_02003520(status, scene, 1);
    Func_02005350();
}
