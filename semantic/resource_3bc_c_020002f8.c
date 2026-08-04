#include "types.h"

/*
 * Resource 3bc, owner at 0x020002f8.  268 strict bytes: 248 bytes of code
 * (0x02f8-0x03ef) and a five-word literal pool at 0x03f0-0x0403.
 *
 * 16 call sites resolved with tools/overlay_call_targets.ts, all import
 * veneers.  The inventory row says calls=15, which is the documented
 * lower-bound behaviour of that field; the per-target multiset is the proof:
 *   080770c0 x1, 0808a080 x4, 0808a0b8 x1, 0808a0e8 x2, 08009150 x3,
 *   080091e0 x2, 080091c0 x1, 080000c0 x1, 080770c8 x1  =  16.
 * The C below contains exactly those 16 call expressions.
 *
 * Entry gate: `Func_080770c0(0x362)` must be zero.  Its zero result stays in
 * r7 and is then reused as the *value* stored into each object's +0x55 byte —
 * so those three stores are stores of 0, not of a live flag.  Written as 0
 * here, with this note, because the identity is only provable from the branch.
 *
 * `movs r3,#10 / ldrsh r1,[r0,r3]` and `movs r3,#18 / ldrsh r2,[r0,r3]` read
 * the integer halves of the 16.16 X at +8 and Z at +0x10.  r3 is left holding
 * 18 across the following `bl`; that is the documented offset-register trap,
 * NOT a fourth argument — Func_0808a0b8 takes three here.
 *
 * The 0x6666 / 0xCCCC pair at +0x34 / +0x30 followed by
 * Func_08009150(obj, obj->x, K, obj->z) is the shape the byte-exact sibling
 * assets/code/resource_3bc_c_02000274.c contains twice.  K is built by shifts:
 * 0x80 << 14 = 0x200000 and 0x80 << 11 = 0x40000.  The scene id is
 * `*(s32 *)&Data_02000240[250]`, as in assets/code/resource_3bc_c_02000658.c.
 *
 * Epilogue `add sp,#8 / pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` — r0 is the popped return address, so
 * the owner is void.
 *
 * Uncertainty: r9 caches `&subject->field55` across the middle of the body and
 * is only used for the final store of 3; nothing here says the byte is not
 * also touched by the calls in between.
 */

typedef struct Object {
    u8 filler00[8];
    s32 x;              /* 16.16; s16 at +0x0a is its integer part */
    s32 field0C;
    s32 z;              /* 16.16; s16 at +0x12 is its integer part */
    s32 field14;
    u8 filler18[0x18];
    s32 field30;
    s32 field34;
    u8 filler38[0x1D];
    u8 field55;
} Object;

extern s16 Data_02000240[];

s32 Func_080770c0();
Object *Func_0808a080();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_08009150();
void Func_080091e0();
void Func_080091c0();
void Func_080000c0();
void Func_080770c8();

void Func_020002f8(void)
{
    s16 *table;
    s32 sceneId;
    Object *object;
    Object *subject;

    table = Data_02000240;
    sceneId = *(s32 *)&table[250];

    if (Func_080770c0(0x362) != 0) {
        return;
    }

    object = Func_0808a080(10);
    if (object != 0) {
        Func_0808a0b8(sceneId,
                      *(s16 *)((u8 *)object + 0x0A),
                      *(s16 *)((u8 *)object + 0x12));
    }
    Func_0808a0e8(sceneId);

    object = Func_0808a080(11);
    object->field55 = 0;
    object->field34 = 0x6666;
    object->field30 = 0xCCCC;
    Func_08009150(object, object->x, 0x200000, object->z);

    object = Func_0808a080(10);
    object->field55 = 0;
    object->field34 = 0x6666;
    object->field30 = 0xCCCC;
    Func_08009150(object, object->x, 0x40000, object->z);

    subject = Func_0808a080(sceneId);
    subject->field55 = 0;
    subject->field30 = 0xCCCC;
    subject->field34 = 0x6666;
    Func_08009150(subject, subject->x, 0x40000, subject->z);

    Func_080091e0(subject, 1);
    Func_0808a0e8(sceneId);
    Func_080091c0(0, 24, 1, 1, 9, 12);
    Func_080000c0(2);
    Func_080091e0(subject, 1);

    subject->field55 = 3;
    subject->field14 = subject->field0C;
    Func_080770c8(0x367);
}
