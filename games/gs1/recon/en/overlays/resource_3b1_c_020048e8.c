#include "types.h"

#define FieldScene_RunSceneStep Func_020048e8

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/* Loader-relocated overlay calls: each Func_ symbol names the pre-relocation
 * call word the image holds.
 *
 * Three of those pre-relocation words repeat in this owner while reaching
 * different runtime helpers (0x0200af5a, 0x0200b0e8 and 0x0200b20c each cover
 * two distinct destinations), so one Func_ spelling cannot name both sites.
 * Those six sites are declared by their runtime address instead, which the
 * overlay symbol resolver binds directly. Registering this owner as a
 * translation unit with explicit absolute_symbols would let them go back to
 * suffixed Func_ spellings without changing a byte. */
void Func_020082ba();
void Func_0200924a();
void Func_020092e8();
void Func_02009382();
void Func_02009390();
void Func_0200939e();
void Func_020093ac();
void Func_020093b8();
void Func_020093c2();
void Func_020094de();
void Func_02009596();
void Func_020095ca();
void Func_020095d6();
void Func_0200962a();
void Func_0200963c();
void Func_0200964a();
void Func_0200965a();
void Func_02009666();
void Func_0200967e();
void Func_02009694();
void Func_020096aa();
void Func_020096ba();
void Func_020096c8();
void Func_020096d8();
void Func_020096e6();
void Func_020096f6();
void Func_02009706();
void Func_02009712();
void Func_0200978a();
void Func_02009794();
void Func_020097a0();
void Func_020097c6();
void Func_0200aedc();
void Func_0200af14();
void Func_0200af1c();
void Func_0200af24();
u8 *Func_0200af28();
void Func_0200af2c();
u8 *Func_0200af38();
void Func_0200af44();
void Func_0200af4e();
void Func_0200af50();
void Value_0200e5d0();
u8 *Value_0200e4e8();
void Func_0200af6c();
void Func_0200af6e();
void Func_0200af76();
void Func_0200af7c();
void Func_0200af80();
void Func_0200af8a();
void Func_0200afb2();
void Func_0200afc2();
void Func_0200afe4();
void Func_0200afe8();
void Func_0200aff2();
void Func_0200affc();
void Func_0200b006();
u8 *Func_0200b018();
u8 *Func_0200b024();
u8 *Func_0200b038();
void Func_0200b040();
u8 *Func_0200b050();
u8 *Func_0200b060();
u8 *Func_0200b074();
void Func_0200b07a();
u8 *Func_0200b086();
u8 *Func_0200b090();
void Func_0200b09a();
void Func_0200b0b2();
void Func_0200b0d0();
void Func_0200b0d6();
void Func_0200b0dc();
void Value_0200e640();
void Value_0200e550();
void Func_0200b0ec();
void Func_0200b0f8();
void Func_0200b100();
void Func_0200b10e();
void Func_0200b11c();
void Func_0200b12a();
void Func_0200b138();
void Func_0200b146();
u8 *Func_0200b14e();
void Func_0200b154();
void Func_0200b162();
void Func_0200b1c0();
void Func_0200b1da();
void Func_0200b1e4();
void Func_0200b1ee();
void Func_0200b1f8();
void Func_0200b200();
void Value_0200e5d0_b();
void Value_0200e4c0();
void Func_0200b214();
void Func_0200b218();
void Func_0200b21e();
void Func_0200b224();
void Func_0200b234();
void Func_0200b23e();
void Func_0200b258();
void Func_0200b2a0();
void Func_0200b2aa();
void Func_0200b2b8();
void Func_0200b2ca();
void Func_0200b2f4();
void Func_0200b334();
void Func_0200b342();
void Func_0200b350();
void Func_0200b368();
void Func_0200b370();
void Func_0200b372();
void Func_0200b376();
void Func_0200b37a();
void Func_0200b37c();
void Func_0200b386();
void Func_0200b3de();
void Func_0200b3e4();
void Func_0200b3e6();
void Func_0200b3ea();
void Func_0200b402();
void Func_0200b41a();
void Func_0200b42c();
void Func_0200b50e();
u8 *Func_0200b52c();

/* The scene work record pointer; +0x1c0 holds the scene request word. */
extern u8 Data_03001ebc[];

extern const s32 Data_0200e840[];
extern const s32 Data_0200f2a0[];
extern const s32 Data_0200f300[];
extern const s32 Data_0200f360[];
extern const s32 Data_0200f3c0[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call lets the compiler precompute a
 * shifted constant into a pseudo and share it with the rest of the block. */

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The halfword field at +6 of a scene record is written from an int-sized
 * value; storing a plain constant through the cast would make the compiler
 * fetch a halfword literal from the pool instead. */

static __inline__ void SetPose(u8 *rec, s32 pose)
{
    *(u16 *)(rec + 6) = pose;
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    u32 slot;

    switch (step) {
    case 0:
        Func_0200af44(0, 0, 0);
        Call3(Func_0200af50, 1, 0x8000, 0);
        Value_0200e5d0(2, 0, 0);
        Call3(Func_0200af8a, 3, 0x8000, opt);
        break;
    case 1:
        Func_0200af6c(0, arg, 0);
        Func_0200af76(1, arg, 0);
        Func_0200af80(2, arg, 0);
        Call3(Func_0200af8a, 3, arg, opt);
        break;
    case 2:
        Func_0200af14(0, 3);
        Func_0200af1c(1, 3);
        Func_0200af24(2, 3);
        Func_0200af2c(3, 3);
        if (arg != 0) {
            Func_0200af4e(3);
        }
        if (opt == 0) {
            break;
        }
        Func_0200aedc(opt);
        break;
    case 3:
        Call2(Func_0200afe8, 0, 0x102);
        Call2(Func_0200aff2, 1, 0x102);
        Call2(Func_0200affc, 2, 0x102);
        Call2(Func_0200b006, 3, 0x102);
        Func_0200aedc(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Func_0200af7c(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            u8 *rec;

            rec = Func_0200af28(arg);
            SetPose(rec, 0x5000);
        }
        Func_0200afb2(arg, 5);
        Func_0200afc2(arg, opt);
        break;
    case 6:
        {
            u8 *rec;

            rec = Func_0200af38(arg);
            SetPose(rec, 0x5000);
            *(s32 *)(rec + 24) = -0x10000;
        }
        Func_0200afb2(arg, 5);
        Func_0200afc2(arg, opt);
        break;
    case 7:
        {
            u8 *rec;

            rec = Value_0200e4e8(arg);
            SetPose(rec, 0x5000);
        }
        Func_0200924a(arg);
        if (opt == 0) {
            Func_0200afe4(arg, 0);
        }
        break;
    case 8:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
        Func_0200b0d0();
        if (arg != 0) {
            Value_0200e640();
        }
        Func_0200af6e(0);
        break;
    case 9:
        Func_0200b0ec();
        Func_0200b0f8();
        if (arg == 0) {
            break;
        }
        Func_0200b0dc(arg);
        break;
    case 10:
        Func_020093b8(24, 1, 0);
        Func_020093c2(25, 0, 0);
        Func_020082ba(0);
        Func_02009382(0, 0x1b0, 168, 0x4000);
        Func_02009390(1, 0x1c0, 168, 0x4000);
        Func_0200939e(2, 0x1a8, 152, 0x4000);
        Func_020093ac(3, 0x1ca, 152, 0x4000);
        break;
    case 11:
        if (arg != 0) {
            u8 *rec;

            Func_0200b07a(13, 1);
            rec = Func_0200b018(13);
            SetPose(rec, 0x3000);
            rec = Func_0200b024(13);
            *(s32 *)(rec + 24) = 0x10000;
        }
        {
            u8 *rec;

            Func_0200b09a(14, 1);
            rec = Func_0200b038(14);
            SetPose(rec, 0x5000);
            Func_0200b0b2(15, 1);
            rec = Func_0200b050(15);
            SetPose(rec, 0x3000);
            rec = Func_0200b060(15);
            *(s32 *)(rec + 24) = 0x10000;
            Func_0200b0d6(16, 1);
            rec = Func_0200b074(16);
            SetPose(rec, 0x5000);
            Value_0200e550(17, 1);
            rec = Func_0200b086(17);
            SetPose(rec, 0x3000);
            rec = Func_0200b090(17);
            *(s32 *)(rec + 24) = 0x10000;
        }
        Call3(Func_0200b100, 28, 0x19a0000, 0xae0000);
        Call3(Func_0200b10e, 29, 0x1d60000, 0xae0000);
        Call3(Func_0200b11c, 30, 0x19a0000, 0xce0000);
        Call3(Func_0200b12a, 31, 0x1d60000, 0xce0000);
        Call3(Func_0200b138, 32, 0x19a0000, 0x11e0000);
        Call3(Func_0200b146, 33, 0x1d60000, 0x11e0000);
        Call3(Func_0200b154, 34, 0x19a0000, 0x13c0000);
        Call3(Func_0200b162, 35, 0x1d60000, 0x13c0000);
        Func_0200b040(1);
        if (arg != 0) {
            Call3(Func_0200b200, 13, 0xb000, 0);
        }
        Call3(Value_0200e5d0_b, 14, 0xd000, 0);
        Call3(Func_0200b218, 15, 0xb000, 0);
        Call3(Func_0200b224, 16, 0xd000, 0);
        Func_020094de(17, 0xb000);
        break;
    case 12:
        {
            u8 *rec;

            rec = Func_0200b14e(arg);
            Func_0200b1c0(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            *(s32 *)(rec + 24) = 0x10000;
        }
        break;
    case 13:
        Func_0200b1da(9, 0, 0);
        Func_0200b1e4(12, 0, 0);
        Func_0200b1ee(11, 0, 0);
        Func_0200b1f8(13, 0, 0);
        Func_0200b21e(10, 0, 0);
        break;
    case 14:
        Func_0200b214(14, 0, 0);
        Func_0200b21e(13, 0, 0);
        break;
    case 15:
        Func_020095ca(24, 1, 0);
        Func_0200b234(9, 0, 0);
        Func_0200b23e(10, 0, 0);
        Func_02009596(8, 0x1bc, 0x266, 0xd000);
        Func_0200b258(0, 0, 0);
        if (arg != 0) {
            Func_020092e8();
        }
        Func_020095d6(0x1c00000, 0x200000, 0x2700000, 0x1000001);
        if (opt == 0) {
            break;
        }
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
        Func_0200b372();
        Func_0200b386();
        Value_0200e4c0(20);
        break;
    case 16:
        Func_0200b2a0(8, 0, 0);
        Func_0200b2aa(9, 0, 0);
        Call3(Func_0200b2b8, 27, 0x1b60000, 0x980000);
        break;
    case 17:
        slot = 0;
        do {
            Func_0200b2ca(slot + 28, 0, 0);
            slot++;
        } while (slot <= 7);
        break;
    case 18:
        Func_0200962a(12, 152, 0x214, 0xb000);
        Func_0200963c(8, 134, 0x1ea, 0x3000);
        Func_0200964a(9, 166, 0x1ea, 0x5000);
        Func_0200965a(10, 182, 0x1f8, 0x5000);
        Func_02009666(11, 118, 0x1f8, 0x3000);
        FieldScene_RunSceneStep(14, 0, 0);
        break;
    case 19:
        Func_0200967e(8, 0x1a0, 0x148, 0);
        Func_02009694(9, 0x1c0, 0x160, 0xd000);
        Func_020096aa(10, 0x1c6, 248, 0x3000);
        Func_020096ba(arg, 0x198, 0x122, 0);
        Func_020096c8(opt, 0x198, 0x156, 0);
        Func_020096d8(13, 0x1a4, 0x164, 0xd000);
        Func_020096e6(14, 0x198, 0x130, 0);
        Func_020096f6(15, 0x1a2, 0x17a, 0xd000);
        Func_02009706(16, 0x1b8, 0x106, 0x3000);
        Func_02009712(17, 0x1c0, 0x17a, 0xd000);
        break;
    case 20:
        for (slot = arg; slot <= opt; slot++) {
            Func_0200b342(slot);
        }
        break;
    case 21:
        Func_0200978a(20, 0x92c, 0x93d);
        Func_02009794(20, 0x917, 0x91f);
        Func_020097a0(20, 0x990, 0x998);
        Func_0200b370(0x300);
        Func_0200b376(0x301);
        Func_0200b37c(0x302);
        break;
    case 22:
        Func_0200b2f4(1);
        Func_020097c6(23, 0, 0);
        Func_0200b3e6(12, Data_0200e840);
        break;
    case 23:
        Func_0200b3de(1);
        Func_0200b3e4(2);
        Func_0200b3ea(3);
        break;
    case 24:
        Call4(Func_0200b50e, -1, -1, -1, 0);
        Func_0200b334(1);
        if (arg != 0) {
            *(u8 *)(Func_0200b52c() + 0x55) = 0;
        }
        break;
    case 25:
        Func_0200b402(Data_0200f2a0);
        Func_0200b350(1);
        if (arg == 1) {
            Func_0200b41a(Data_0200f300);
            Func_0200b368(1);
        } else if (arg == 2) {
            Func_0200b41a(Data_0200f360);
            Func_0200b368(1);
        } else if (arg == 3) {
            Func_0200b42c(Data_0200f3c0);
            Func_0200b37a(1);
        }
        break;
    }
}
