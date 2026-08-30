#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunThreeActorEncounter:
 * 68 calls, one conditional sequence, one wait loop, and all actor state.
 * Recovered from the complete decoded owner and checked against the original
 * assembly. */

#define FieldScene_RunThreeActorEncounter Func_02003c88

void Func_02007768(); void Func_02007770(); void Func_02007786();
void Func_020077b2(); void Func_020077ca(); void Func_020077d0();
void Func_020077e8(); void Func_02007822(); void Func_020078ba();
void Func_020078c8(); void Func_02007f78(); void Func_02007fb6();
s32 Func_02007fc2(); s32 Func_02007fd6(); s32 Func_02007fea();
void Func_02008000(); void Func_02008012(); void Func_02008020();
void Func_0200802a(); void Func_02008030(); void Func_02008044();
void Func_02008058(); void Func_0200806e(); void Func_02008078();
void Func_02008084(); void Func_0200808c(); void Func_020080ac();
void Func_020080b4(); s32 Func_020080b8(); s32 Func_02008104();
void Func_02008106(); void Func_02008122(); void Func_02008126();
void Func_02008132(); void Func_02008138(); void Func_0200813c();
void Func_0200814c(); void Func_02008154(); void Func_02008158();
void Func_02008160(); void *Func_02008164(); void Func_0200816e();
void Func_02008184(); void *Func_02008186(); void Func_02008188();
void *Func_02008196(); void Func_020081a0(); void Func_020081a6();
void Func_020081ac(); void *Func_020081b6(); void Func_020081be();
void Func_020081ca(); void Func_020081ce(); void Func_020081d8();
void Func_020081dc(); void Func_020081fa(); void Func_020081fc();
void Func_02008206(); void Func_0200820c(); void Func_02008212();
void Func_0200822c(); void Func_02008236(); void Func_02008248();
void Func_02008292();

void Func_02003c88(void)
{
    void *actor;
    void *p50;
    void *p53;
    void *p54;
    void *p57;

    Func_02007f78();
    Func_02007fb6(0, 65536, 32768);
    Func_02008000(0, 180, 654);
    Func_02008084(0, 32768, 0);
    actor = (void *)Func_02007fc2(0);
    if (actor != 0) {
        Func_02008030(1, *(u32 *)((u8 *)actor + 8), *(u32 *)((u8 *)actor + 16));
    }
    actor = (void *)Func_02007fd6(0);
    if (actor != 0) {
        Func_02008044(2, *(u32 *)((u8 *)actor + 8), *(u32 *)((u8 *)actor + 16));
    }
    actor = (void *)Func_02007fea(0);
    if (actor != 0) {
        Func_02008058(3, *(u32 *)((u8 *)actor + 8), *(u32 *)((u8 *)actor + 16));
    }
    Func_02008012(1, 78643, 39321);
    Func_02008020(2, 65536, 32768);
    Func_0200802a(3, 78643, 39321);
    Func_0200806e(1, 194, 640);
    Func_02008078(2, 198, 654);
    Func_0200808c(3, 194, 672);
    Func_020080ac(1, 1);
    Func_020080b4(2, 1);
    Func_0200802a(10);
    Func_02008126(1, 32768, 0);
    Func_02008132(2, 32768, 0);
    Func_02007768(3, 32768);
    Func_02007770(22, 0);
    Func_02008122(8021);
    Func_02007768(22);
    Func_02007786(21, 53248);
    Func_02008154(21, 0, 40);
    Func_02008188(22, 256, 20);
    Func_02008138(22, 1);
    Func_02008160(22, 0);
    if (Func_020080b8(0, 0) == 1) {
        Func_0200813c(2, 4);
        Func_020077b2(2);
        Func_020077d0(3, 40960);
        Func_0200814c(3, 3);
        Func_020077ca(3);
        Func_020077e8(1, 24576);
        Func_02008184(1, 1);
        Func_020081ac(1, 0);
        while (Func_02008104(0, 0) == 1) {
            Func_020081a0(2, 1);
            Func_020081be(8019);
            Func_020081ce(2, 0);
        }
    }
    Func_02008106(20);
    Func_020081a6(22, 3);
    Func_020081dc(8027);
    Func_02007822(22);
    Func_02008160(22, 65536, 32768);
    Func_0200816e(21, 65536, 32768);
    p50 = Func_02008164(22);
    *(u8 *)((u8 *)p50 + 90) &= 0xfe;
    Func_020081ca(22, 162, 634);
    Func_02008158(1);
    p53 = Func_02008186(22);
    *(u8 *)((u8 *)p53 + 90) |= 1;
    p54 = Func_02008196(21);
    *(u8 *)((u8 *)p54 + 90) &= 0xfe;
    Func_020081fa(21, 162, 676);
    Func_02008188(1);
    p57 = Func_020081b6(21);
    *(u8 *)((u8 *)p57 + 90) |= 1;
    Func_02008292(22, 12288, 0);
    Func_020078c8(21, 53248);
    Func_020078ba(22);
    Func_0200822c(1, 180, 654);
    Func_02008236(2, 180, 654);
    Func_02008248(3, 180, 654);
    Func_02008206(1);
    Func_0200820c(2);
    Func_02008212(3);
    Func_020081d8();
    Func_020081fc();
}
