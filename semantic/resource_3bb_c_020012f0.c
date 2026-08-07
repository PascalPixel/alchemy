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

extern void Func_02001df4();
extern void Func_0808a018();
extern s32 Func_02001ffc();
extern void Func_0808a170();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void Func_0808a180();
extern void Func_02002ba8();
extern void Func_0808a090();
extern void Func_020030e8();
extern void Func_0808a010();
extern u8 *Func_0808a080();
extern void Func_0808a100();
extern void Func_0808a1f0();
extern void Func_02002d84();
extern void Func_0808a200();
extern void Func_020020b8();
extern void Func_02002114();
extern void Func_0808a020();

void Func_020012f0(s32 scene)
{
    s32 status;

    if (Data_02000240[225] == 2) {
        Func_02001df4(scene);
        return;
    }

    Func_0808a018();
    status = Func_02001ffc(scene, 1);

    if (status == 0) {
        u8 *actor;

        Func_0808a170(0x209e);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x4c80000, -1, 0xb80000, 1);
        Func_0808a218();
        Func_0808a180(scene, 0);
        Func_02002ba8(0, 0x4f8, 168);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_020030e8(0, 0x508, 184);
        Func_020030e8(0, 0x508, 216);
        Func_020030e8(0, 0x4c8, 216);
        Func_0808a180(scene, 0);
        Func_020030e8(0, 0x4c8, 248);
        Func_020030e8(0, 0x4a8, 248);
        Func_0808a010(3);
        actor = Func_0808a080(0);
        *(s32 *)(actor + 40) = 0x40000;
        Func_0808a100(0, 28);
        Func_0808a1f0(0, 258);
        Func_0808a010(30);
        Func_0808a180(scene, 0);
        Func_02002d84(0);
        Func_0808a200(0, 0);
        Func_020020b8(scene, 1);
    } else if (status == 1) {
        Func_0808a170(0x209d);
        Func_0808a180(scene, 0);
    }

    Func_02002114(status, scene, 1);
    Func_0808a020();
}
