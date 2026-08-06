#include "types.h"

/* Scene-four presentation script, including the actor-relative move cue. */
extern s16 Data_02000240[];



























extern void Func_020034de();
extern void Func_02005628();
extern s32 Func_020036f4();
extern void Func_020056d6();
extern void Func_0200572a();
extern void Func_02005744();
extern void Func_02005750();
extern void Func_0200565e();
extern void Func_02005752();
extern void Func_0200576c();
extern void Func_0200577a();
extern void Func_0200573a();
extern void Func_02004310();
extern void Func_02005702();
extern void Func_0200577e();
extern void Func_020057ea();
extern void Func_020057b6();
extern void Func_020057d0();
extern void Func_0200571e();
extern void Func_0200575e();
extern u8 * Func_0200571c();
extern void Func_02005654();
extern void Func_0200576a();
extern void Func_0200583e();
extern void Func_02005818();
extern void Func_020057e0();
extern void Func_0200576e();
extern void Func_02004902();
extern void Func_02005816();
extern void Func_02005862();
extern void Func_0200582a();
extern void Func_020045d4();
extern void Func_02005868();
extern void Func_02003918();
extern void Func_0200583c();
extern void Func_02005854();
extern void Func_02003992();
extern void Func_020057c2();
void Func_020016d4(s32 scene)
{
    s32 status;

    if (Data_02000240[225] == 2) {
        return;
        Func_020034de(scene);
    }

    Func_02005628();
    status = Func_020036f4(scene, 4);

    if (status == 0) {
        u8 *actor;

        Func_020056d6(0x20aa);
        Func_0200572a(0x30000, 0x6000);
        Func_02005744(0x2180000, -1, 0xf00000, 1);
        Func_02005750();
        Func_0200565e(45);
        Func_02005752(0x10000, 0x2000);
        Func_0200576c(0x2180000, -1, 0xc00000, 1);
        Func_0200577a();
        Func_0200573a(scene, 0);
        Func_02004310(0, 0x278, 0x108);
        Func_020056d6(0, 0x10000, 0x8000);
        Func_02005702(0, 0x268, 0x108);
        Func_0200577e(0, 0x6000, 20);
        Func_020057ea();
        Func_020057b6(0x4000, 0x800);
        Func_020057d0(0x2180000, -1, 0xa00000, 1);
        Func_0200571e(0, 0xc000, 0x4000);
        Func_0200575e(0, 10);
        actor = Func_0200571c(0);
            *(s32 *)(actor + 8),
            *(s32 *)(actor + 12) + 0x400000,
        Func_02005654(actor,
            *(s32 *)(actor + 16));
        Func_0200576a(0);
        Func_0200583e();
        Func_02005818(-1, -1, -1, 0);
        Func_020057e0(scene, 0);
        Func_0200576e(0, 0x18000, 0xc000);
        Func_02004902(0, 0x1e8, 248);
        Func_02005816(0, 0x4000, 20);
        Func_020057e0(0, 6, 0);
        Func_02005862(0x2180000, -1, 0xa00000, 1);
        Func_0200582a(scene, 0);
        Func_020045d4(0);
        Func_02005868(0, 0);
        Func_02003918(scene, 4);
    } else if (status == 1) {
        Func_0200583c(0x20a9);
        Func_02005854(scene, 0);
    }

    Func_02003992(status, scene, 4);
    Func_020057c2();
}
