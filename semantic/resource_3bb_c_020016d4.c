#include "types.h"

/* Scene-four presentation script, including the actor-relative move cue. */
extern s16 Data_02000240[];

extern void Func_02001df4();
extern void Func_0808a018();
extern s32 Func_02001ffc();
extern void Func_0808a170();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void Func_0808a010();
extern void Func_0808a180();
extern void Func_02002ba8();
extern void Func_0808a090();
extern void Func_0808a0d0();
extern void Func_0808a1b8();
extern void Func_0808a2b0();
extern void Func_0808a100();
extern u8 *Func_0808a080();
extern void Func_08009150();
extern void Func_0808a0e8();
extern void Func_020030e8();
extern void Func_0808a140();
extern void Func_02002d84();
extern void Func_0808a200();
extern void Func_020020b8();
extern void Func_02002114();
extern void Func_0808a020();

void Func_020016d4(s32 scene)
{
    s32 status;

    if (Data_02000240[225] == 2) {
        Func_02001df4(scene);
        return;
    }

    Func_0808a018();
    status = Func_02001ffc(scene, 4);

    if (status == 0) {
        u8 *actor;

        Func_0808a170(0x20aa);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x2180000, -1, 0xf00000, 1);
        Func_0808a218();
        Func_0808a010(45);
        Func_0808a208(0x10000, 0x2000);
        Func_0808a210(0x2180000, -1, 0xc00000, 1);
        Func_0808a218();
        Func_0808a180(scene, 0);
        Func_02002ba8(0, 0x278, 0x108);
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0d0(0, 0x268, 0x108);
        Func_0808a1b8(0, 0x6000, 20);
        Func_0808a2b0();
        Func_0808a208(0x4000, 0x800);
        Func_0808a210(0x2180000, -1, 0xa00000, 1);
        Func_0808a090(0, 0xc000, 0x4000);
        Func_0808a100(0, 10);
        actor = Func_0808a080(0);
        Func_08009150(actor,
            *(s32 *)(actor + 8),
            *(s32 *)(actor + 12) + 0x400000,
            *(s32 *)(actor + 16));
        Func_0808a0e8(0);
        Func_0808a2b0();
        Func_0808a210(-1, -1, -1, 0);
        Func_0808a180(scene, 0);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_020030e8(0, 0x1e8, 248);
        Func_0808a1b8(0, 0x4000, 20);
        Func_0808a140(0, 6, 0);
        Func_0808a210(0x2180000, -1, 0xa00000, 1);
        Func_0808a180(scene, 0);
        Func_02002d84(0);
        Func_0808a200(0, 0);
        Func_020020b8(scene, 4);
    } else if (status == 1) {
        Func_0808a170(0x20a9);
        Func_0808a180(scene, 0);
    }

    Func_02002114(status, scene, 4);
    Func_0808a020();
}
