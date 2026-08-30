#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSecondaryActorSequence:
 * 60 calls, actor-coordinate handoffs, and conditional event progress. */

#define FieldScene_RunSecondaryActorSequence Func_02002cb0

void Func_02008c70();
void Func_02008cf2();
void Func_02008cfc();
s32 Func_02008d20();
void Func_02008d32();
void Func_02008d38();
void Func_02008d42();
void Func_02008d52();
void Func_02008d7c();
void Func_02008d86();
s32 Func_02008d92();
void Func_02008db8();
void Func_02008dc8();
void Func_02008dcc();
void Func_02008dd4();
void Func_02008dd6();
void Func_02008de0();
void Func_02008de2();
void Func_02008de4();
void Func_02008e00();
void Func_02008e0a();
void Func_02008e10();
void Func_02008e1c();
void Func_02008e24();
void Func_02008e2e();
void Func_02008e3a();
void Func_02008e5a();
void Func_02008e62();
void Func_02008e70();
void Func_02008e78();
void Func_02008e7a();
void Func_02008e7e();
void Func_02008e88();
void Func_02008e92();
s32 Func_02008ea0();
void Func_02008eaa();
void Func_02008eac();
void Func_02008eae();
void Func_02008ebe();
void Func_02008ec0();
void Func_02008ece();
void Func_02008ed8();
void Func_02008eda();
void Func_02008edc();
void Func_02008ee2();
void Func_02008eec();
void Func_02008ef6();
void Func_02008efa();
void Func_02008f08();
void Func_02008f14();
void Func_02008f1a();
void Func_02008f2c();
void Func_02008f32();

void Func_02002cb0(void)
{
    u8 *actor;
    u8 *scene_state;

    Func_02008c70();
    Func_02008d38(0, 0);
    Func_02008d32(8, 0, 20);
    Func_02008d52(0x1c45);
    Func_02008d32(8, 2);
    Func_02008d7c(8, 0, 20);
    Func_02008dc8(65536, 8192);
    Func_02008de0(26083328, -1, 0x02460000, 1);
    Func_02008cf2(0, 0xcccc, 0x6666);
    Func_02008cfc(1, 0xcccc, 0x6666);
    Func_02008d42(0, 420, 608);
    Func_02008dd6(0, 40960, 0);
    Func_02008de2(8, 12288, 0);
    actor = (u8 *)Func_02008d20(0);
    if (actor != 0) {
        Func_02008d86(1, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_02008d7c(1, 402, 608);
    Func_02008e10(1, 53248, 20);
    Func_02008e00(0x1001, 0);
    Func_02008e24(8, 20480, 20);
    Func_02008dcc(8, 3);
    Func_02008e1c(0x4008, 0);
    Func_02008dd4(0, 3);
    Func_02008de4(1, 3);
    Func_02008d52(20);
    Func_02008e0a(8, 2);
    Func_02008e3a(0x4008, 0);
    if (Func_02008d92(0, 0) == 1) {
        scene_state = *(u8 **)0x03001ebc;
        (*(u16 *)(scene_state + 472))++;
        Func_02008e2e(8, 1);
    }
    Func_02008e78(0x4008, 0, 40);
    Func_02008eaa(8, 0x105, 60);
    Func_02008e70(0x1c4b);
    Func_02008e92(0x4008, 0, 20);
    Func_02008e62(1, 1);
    Func_02008db8(40);
    Func_02008eaa(0x1001, 0, 40);
    Func_02008e7a(8, 1);
    Func_02008ece(8, 53248, 20);
    Func_02008ebe(0x4008, 0);
    Func_02008e7e(1, 3);
    Func_02008ed8(0x1001, 0, 120);
    Func_02008ee2(0x4008, 0, 20);
    Func_02008f14(1, 0x105, 40);
    Func_02008ef6(0x1001, 0, 40);
    Func_02008eae(8, 4);
    Func_02008f08(0x4008, 0, 20);
    Func_02008ec0(1, 3);
    Func_02008e2e(40);
    Func_02008f32(8, 20480, 20);
    Func_02008f2c(0x4008, 0, 10);
    Func_02008edc(0, 3);
    Func_02008eec(1, 3);
    Func_02008e5a(20);
    Func_02008efa(8, 3);
    Func_02008efa(1, 2);
    actor = (u8 *)Func_02008ea0(0);
    if (actor != 0) {
        Func_02008eda(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_02008f08(1);
    Func_02008f1a(1, 0, 0);
    Func_02008e88(0x303);
    Func_02008eac();
}
