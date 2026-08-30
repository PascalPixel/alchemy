#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSecondActorInteraction:
 * 27 calls, a progress gate, result-dependent presentation, and common
 * actor cleanup across the complete callable owner. */

#define FieldScene_RunSecondActorInteraction Func_02001424

void Func_02001d44();
void Func_02001dd8();
void Func_02001e3c();
void Func_0200322e();
s32 Func_02003444();
void Func_020035b8();
void Func_02003632();
void Func_02004046();
void Func_02004274();
void Func_020045a4();
void Func_020045b0();
void Func_02005378();
void Func_020053d4();
void Func_02005412();
void Func_02005424();
void Func_02005462();
void Func_02005470();
void Func_0200547c();
void Func_02005496();
void Func_020054a2();
void Func_020054ca();
void Func_020054dc();
void Func_020054f4();
void Func_02005502();
void Func_02005508();
void Func_02005520();

extern s16 Data_02000240[];

void Func_02001424(int actor)
{
    int result;

    if (Data_02000240[225] == 2) {
        Func_0200322e();
        return;
    }
    Func_02005378();
    result = Func_02003444(actor, 2);
    if (result == 0) {
    Func_02005424(0xa220);
    Func_02001d44();
    Func_0200547c(196608, 24576);
    Func_02005496(64487424, -1, 15204352, 1);
    Func_020054a2();
    Func_02005462(actor, 0);
    Func_02001dd8();
    Func_02005470(actor, 0);
    Func_02004046(0, 1080, 264);
    Func_020053d4(15);
    Func_02005412(0, 98304, 49152);
    Func_020045a4(0, 1080, 216);
    Func_020045b0(0, 1064, 216);
    Func_02001e3c();
    Func_02005520();
    Func_02005502(-1, -1, -1, 0);
    Func_020054ca();
    Func_02004274(0);
    Func_02005508(0, 0);
    Func_020035b8(actor, 2);
    } else if (result == 1) {
        Func_020054dc(0xa120);
        Func_020054f4(actor, 0);
    }
    Func_02003632(result, actor, 2);
    Func_02005462();
}
