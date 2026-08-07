#include "types.h"

/*
 * Resource 3bc, owner at 0x020004a4.  216 strict bytes: 208 bytes of code
 * (0x04a4-0x0573) and a two-word literal pool at 0x0574-0x057b.
 *
 * All 11 call sites resolved with tools/lib/overlay_call_targets.ts (every one an
 * import veneer), matching the row's calls=11 exactly:
 *   0808a080 x4, 080091e0 x1, 08009150 x3, 080770c8 x1, 080091c0 x2.
 * Note the four Func_0808a080 sites are four *separate* fetches in the
 * assembly, including two with the same argument 12; they are written out
 * separately rather than cached, so the multiset stays 11.
 *
 * `asrs r3,#20` on the 16.16 X and Z words at +8 and +0x10 is the tile-grid
 * idiom (>>16 to integer units, then >>4 for the 16-pixel grid), the same
 * reading the byte-exact sibling assets/code/resource_3bc_c_02000658.c uses.
 * The whole body is gated on the object 12 standing on tile (9, 12).
 *
 * The 0x6666 / 0xCCCC pair written to +0x34 / +0x30 and the following
 * Func_08009150(obj, obj->x, K, obj->z) move are the same shape the byte-exact
 * assets/code/resource_3bc_c_02000274.c contains twice.  The K constants are
 * built by shifts: 0x80 << 11 = 0x40000 and 0x80 << 14 = 0x200000.
 *
 * Trailing Func_080091c0 pair: the last two arguments travel on the stack
 * (`str r3,[sp,#0]` / `str r7,[sp,#4]`), and the second call reuses the tile
 * column held in r9 and the tile row held in r7 — hence the high-register
 * saves in the prologue.  Func_080770c8's argument is 218 << 2 = 0x368.
 *
 * Epilogue `add sp,#8 / pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` — r0 is the popped return address, so
 * the owner is void.
 *
 * Uncertainty: object 10's +0x23 byte is deliberately NOT written, unlike
 * objects 12 and 11.  That asymmetry is in the assembly, not a transcription
 * slip.
 */

typedef struct Object {
    u8 filler00[8];
    s32 x;              /* 16.16 */
    u8 filler0C[4];
    s32 z;              /* 16.16 */
    u8 filler14[0x0F];
    u8 field23;
    u8 filler24[0x0C];
    s32 field30;
    s32 field34;
    u8 filler38[0x1D];
    u8 field55;
} Object;

Object *Func_02004ecc();
Object *Func_02004ee4();
void Func_02004e2c();
void Func_02004e0e();
Object *Func_02004f1c();
void Func_02004e2e();
Object *Func_02004f3c();
void Func_02004e46();
void Func_02004ef6();
void Func_02004e90();
void Func_02004ea2();

                     

void Func_020004a4(void)
{
    Object *object;
    s32 column;
    s32 row;

    object = Func_02004ecc(12);
    column = object->x >> 20;
    if (column != 9) {
        return;
    }
    row = object->z >> 20;
    if (row != 12) {
        return;
    }

    object = Func_02004ee4(12);
    Func_02004e2c(object, 0);
    object->field23 = 2;
    object->field55 = 0;
    object->field30 = 0xCCCC;
    Func_02004e0e(object, object->x, 0x40000, object->z);
    object->field34 = 0x6666;

    object->field34 = 0x6666;
    object = Func_02004f1c(11);
    object->field23 = 2;
    object->field30 = 0xCCCC;
    Func_02004e2e(object, object->x, 0x200000, object->z);

    object->field34 = 0x6666;
    object = Func_02004f3c(10);
    object->field30 = 0xCCCC;
    Func_02004e46(object, object->x, 0x40000, object->z);

    Func_02004ef6(0x368);
    Func_02004e90(15, 12, 1, 1, 13, row);
    Func_02004ea2(1, 25, 1, 1, column, row);
}
