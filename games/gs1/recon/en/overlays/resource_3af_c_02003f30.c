#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunEncounterClosingSequence:
 * 72 calls, one early-exit branch, event progress, and terminal workspace state. */

#define FieldScene_RunEncounterClosingSequence Func_02003f30

void Func_02004b8a(); void Func_02007982(); void Func_0200799a();
void Func_02007a2e(); void Func_02007a6e(); void Func_02007a9e();
void Func_02007aac(); void Func_02007ac4(); void Func_02007adc();
void Func_02007afa(); void Func_02007b14(); void Func_02007b32();
void Func_02007b38(); void Func_02007b88(); void Func_02007b96();
void Func_02007ba6(); void Func_020081a0(); void Func_02008224();
void Func_0200823a(); void Func_02008272(); void Func_020082a6();
s32 Func_020082a8(); void Func_020082be(); void Func_02008306();
void Func_0200830e(); void Func_02008334(); void Func_02008344();
void Func_0200834c(); void Func_02008352(); void Func_0200835c();
void Func_0200835e(); void Func_02008370(); void Func_0200837e();
void Func_02008384(); void Func_0200839a(); void Func_020083b4();
void Func_020083bc(); void Func_020083c0(); void Func_020083ca();
void Func_020083e2(); void Func_020083ee(); void Func_020083fa();
void Func_02008404(); void Func_02008440(); void Func_02008446();
void Func_02008450(); void Func_02008458(); void Func_0200845e();
void Func_02008462(); void Func_02008466(); void Func_0200846c();
void Func_0200846e(); void Func_02008470(); void Func_0200847e();
void Func_02008480(); void Func_02008490(); void Func_0200849c();
void Func_0200849e(); void Func_020084aa(); void Func_020084ac();
void Func_020084ae(); void Func_020084cc(); void Func_020084d4();
void Func_020084dc(); void Func_02008506(); void Func_0200852a();
void Func_02008532(); void Func_020085f6(); void Func_0200860c();
void Func_02008618();

void Func_02003f30(void)
{
    u8 *workspace;

    Func_02008224();
    Func_0200823a(0x0200d1d8);
    Func_020081a0(1);
    Func_02008384();
    Func_02008272(0, 65536, 32768);
    Func_020082be(0, 148, 656);
    Func_0200835c(22, 256, 0);
    Func_0200830e(22, 1);
    Func_02007982(22, 20480);
    Func_02008334(0x1f69);
    Func_02008344(0x2016, 0);
    Func_02008370(0, 57344, 0);
    if (Func_020082a8(0, 0) == 1) {
        Func_0200799a(0x2016);
        Func_020082a6();
        return;
    }
    (*(u16 *)(*(u32 *)0x03001ebc + 472))++;
    Func_0200839a(0x2016, 0);
    Func_02004b8a();
    Func_0200834c(26, 14155776, 38535168);
    Func_02008306(26, 78643, 39321);
    Func_02008352(26, 216, 596);
    Func_0200835e(26, 188, 616);
    Func_020083e2(0, 57344, 0);
    Func_020083ee(21, 53248, 0);
    Func_020083fa(22, 53248, 0);
    Func_02007a2e(26, 20480);
    Func_020083bc(26, 2, 0);
    Func_020083b4(26, 4);
    Func_02008404(26, 0);
    Func_020083c0(20, 11796480);
    Func_0200837e(20, 65536, 32768);
    Func_020083ca(20, 180, 664);
    Func_02008450(20, 53248, 0);
    Func_02007a6e(0x2014);
    Func_02008462(0, 8192, 0);
    Func_0200846e(22, 12288, 0);
    Func_02008490(26, 257, 60);
    Func_02008440(20, 1);
    Func_02007a9e();
    Func_02008446(21, 2);
    Func_02007aac(21);
    Func_0200849e(20, 20480, 20);
    Func_02008446(20, 3);
    Func_02007ac4(0x6014);
    Func_02008466(26, 2, 20);
    Func_0200845e(26, 4);
    Func_02007adc(26);
    Func_02008458(20, 182, 640);
    Func_020084dc(20, 53248, 0);
    Func_02007afa(0x8014);
    Func_02008506(26, 256, 20);
    Func_020084ae(26, 2);
    Func_02007b14(26);
    Func_020084ac(20, 3);
    Func_02007b38(22, 0);
    Func_020084d4(22, 1);
    Func_02007b32(22);
    Func_0200846c(22, 104857, 52428);
    Func_0200847e(22, 33605912);
    Func_02008480(21, 104857, 52428);
    Func_020084cc(21, 168, 632);
    Func_0200849c(21, 33605912);
    Func_02008462(80);
    Func_020084aa(26, 33605912);
    Func_02008470(40);
    Func_02007b96(20, 32768);
    Func_02007b88(0x2014);
    Func_02007ba6(0, 57344);
    Func_0200852a(0, 3);
    Func_02008532(20, 3);
    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 513;
    Func_0200860c();
    Func_02008618();
    Func_020085f6(17);
}
