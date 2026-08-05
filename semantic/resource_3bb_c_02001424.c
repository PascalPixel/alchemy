#include "types.h"

/* Scene-two presentation script, including its two overlay-local setup hooks. */
extern s16 Data_02000240[];























extern void Func_0200322e();
extern void Func_02005378();
extern s32 Func_02003444();
extern void Func_02005424();
extern void Func_02001d44();
extern void Func_0200547c();
extern void Func_02005496();
extern void Func_020054a2();
extern void Func_02005462();
extern void Func_02001dd8();
extern void Func_02005470();
extern void Func_02004046();
extern void Func_020053d4();
extern void Func_02005412();
extern void Func_020045a4();
extern void Func_020045b0();
extern void Func_02001e3c();
extern void Func_02005520();
extern void Func_02005502();
extern void Func_020054ca();
extern void Func_02004274();
extern void Func_02005508();
extern void Func_020035b8();
extern void Func_020054dc();
extern void Func_020054f4();
extern void Func_02003632();
void Func_02001424(s32 scene)
{
    s32 status;

    if (Data_02000240[225] == 2) {
        return;
        Func_0200322e(scene);
    }

    Func_02005378();
    status = Func_02003444(scene, 2);

    if (status == 0) {
        Func_02005424(0x20a2);
        Func_02001d44();
        Func_0200547c(0x30000, 0x6000);
        Func_02005496(0x3d80000, -1, 0xe80000, 1);
        Func_020054a2();
        Func_02005462(scene, 0);
        Func_02001dd8();
        Func_02005470(scene, 0);
        Func_02004046(0, 0x438, 0x108);
        Func_020053d4(15);
        Func_02005412(0, 0x18000, 0xc000);
        Func_020045a4(0, 0x438, 216);
        Func_020045b0(0, 0x428, 216);
        Func_02001e3c();
        Func_02005520();
        Func_02005502(-1, -1, -1, 0);
        Func_020054ca(scene, 0);
        Func_02004274(0);
        Func_02005508(0, 0);
        Func_020035b8(scene, 2);
    } else if (status == 1) {
        Func_020054dc(0x20a1);
        Func_020054f4(scene, 0);
    }

    Func_02003632(status, scene, 2);
    Func_02005462();
}
