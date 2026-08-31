#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunActorThreeBranchSequence:
 * 38 calls, actor staging, a two-armed runtime probe with branch-specific
 * scene-counter updates, and the complete closing presentation. */

#define FieldScene_RunActorThreeBranchSequence Func_02002684

void *Func_0200813c();
void Func_0200816e();
void Func_02008178();
void Func_02008196();
void *Func_020081b4();
void Func_020081ba();
void Func_020081c6();
void *Func_020081d6();
void Func_020081e0();
s32 Func_020081f6();
void Func_02008206();
void Func_0200821a();
void Func_0200821e();
void Func_02008228();
void Func_02008232();
void Func_02008238();
void Func_02008266();
void Func_0200826a();
void Func_02008270();
void Func_0200827e();
void Func_02008280();
void Func_0200828c();
void Func_020082a6();
void Func_020082aa();
void Func_020082ae();
void Func_020082b2();
void Func_020082c6();
void Func_020082d2();
void Func_0200830c();
void Func_0200830e();
void Func_02008318();
void Func_02008326();
void Func_02008346();
void Func_02008360();
void Func_02008376();
void Func_0200837a();

extern u8 *Data_03001ebc;

void Func_02002684(void)
{
    void *p1;
    void *p10;
    void *p14;

    p1 = Func_0200813c();
    Func_0200816e(3, 52428, 26214);
    Func_02008178(0, 52428, 26214);
    Func_0200821e(5495);
    Func_02008238(3, 0, 20);
    Func_020081c6(3, 840, 648);
    Func_0200826a(3, 256, 60);
    Func_02008266(3, 32768, 20);
    Func_02008206(3, 16);
    p10 = Func_020081b4(3);
    *(s32 *)(p10 + 24) = 0;
    Func_02008196(20);
    Func_02008280(3, 0, 20);
    Func_02008228(3, 1);
    p14 = Func_020081d6(3);
    *(s32 *)(p14 + 24) = 65536;
    Func_020081ba(20);
    Func_020082ae(3, 16384, 20);
    Func_020082a6(3, 0);
    if (Func_020081f6(0, 0) == 0) {
        Func_020081e0(20);
        Func_02008270(3, 3);
        Func_020082d2(3, 0, 20);
        (*(u16 *)(Data_03001ebc + 472))++;
    } else {
        (*(u16 *)(Data_03001ebc + 472))++;
        Func_0200821a(20);
        Func_020082aa(3, 4);
        Func_0200830c(3, 0, 20);
    }
    Func_02008232(20);
    Func_02008326(3, 49152, 20);
    Func_02008346(52428);
    Func_02008360(55050240, -1, 41418752, 1);
    Func_020082c6(3, 840, 632);
    Func_0200837a();
    Func_02008270(20);
    Func_02008318(3, 2);
    Func_0200827e(10);
    Func_0200830e(3, 4);
    Func_0200828c(20);
    Func_02008376(3, 0, 20);
    Func_0200827e(2160);
    Func_020082b2();
}
