#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunFirstActorInteraction:
 * 29 calls, a progress gate, result-dependent presentation, and common
 * actor cleanup across the complete callable owner. */

#define FieldScene_RunFirstActorInteraction Func_020012f0

void *Func_020030fa();
s32 Func_02003310();
void Func_020034a6();
void Func_02003520();
void Func_02003efe();
void Func_02004162();
void Func_0200445a();
void Func_02004466();
void Func_02004470();
void Func_02004482();
void Func_0200448e();
void Func_02005244();
void Func_020052c8();
void Func_020052dc();
void Func_020052f0();
void Func_02005300();
void *Func_02005302();
void Func_0200532a();
void Func_02005344();
void Func_02005350();
void Func_02005358();
void Func_0200535e();
void Func_0200536a();
void Func_02005370();
void Func_020053b8();
void Func_020053ca();
void Func_020053d2();
void Func_020053e2();
void Func_020053f6();

extern s16 Data_02000240[];

void Func_020012f0(int actor)
{
    void *p18;
    int result;

    if (Data_02000240[225] == 2) {
        Func_020030fa();
        return;
    }
    Func_02005244();
    result = Func_02003310(actor, 1);
    if (result == 0) {
    Func_020052f0(0x9e20);
    Func_02005344(196608, 24576);
    Func_0200535e(80216064, -1, 12058624, 1);
    Func_0200536a();
    Func_0200532a(actor, 0);
    Func_02003efe(0, 1272, 168);
    Func_020052c8(0, 98304, 49152);
    Func_0200445a(0, 1288, 184);
    Func_02004466(0, 1288, 216);
    Func_02004470(0, 1288, 216);
    Func_02005370(actor, 0);
    Func_02004482(0, 1288, 248);
    Func_0200448e(0, 1192, 248);
    Func_020052dc(3);
    p18 = Func_02005302(0);
    *(s32 *)(p18 + 40) = 262144;
    Func_02005358(0, 28);
    Func_020053d2(0, 258);
    Func_02005300(30);
    Func_020053b8();
    Func_02004162(0);
    Func_020053f6(0, 0);
    Func_020034a6(actor, 1);
    } else if (result == 1) {
        Func_020053ca(0x9d20);
        Func_020053e2(actor, 0);
    }
    Func_02003520(result, actor, 1);
    Func_02005350();
}
