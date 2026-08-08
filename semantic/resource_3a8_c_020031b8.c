#include "types.h"

/*
 * resource_3a8 owner at 0x020031b8, 236 bytes: try to shove the scene object
 * standing on the tile the hero faces one tile further in that direction.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl saves and a
 * 12-byte stack frame at 0x020031b8, through the single epilogue at
 * 0x02003288 (`add sp,#12 / pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / pop {r5,r6,r7} / pop {r0} / bx r0`), followed by its three-word
 * literal pool at 0x02003298-0x020032a3.  The pool is data, not code, and is
 * excluded.  Nothing is live past the return.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  r0 is written with 0
 * before any read, and r1-r3 are never read before being written, so it takes
 * no arguments.
 *
 * Call accounting: 12 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a8 31b8` (10 distinct targets:
 * 8 import veneers and 2 intra-overlay prologues).  The disassembler's own
 * annotations are wrong in the usual overlay way - an overlay `bl` stores
 * `target_offset - 2` - and were not used.  In body order:
 *   0x020031c6 -> 0x02003c1c veneer -> Func_0808a080
 *   0x020031f0 -> 0x02003184 prologue (byte-exact sibling in assets/code)
 *   0x02003224 -> 0x02003bac veneer -> Func_080091d8
 *   0x02003230 -> 0x02003b5c veneer -> Func_08009080
 *   0x02003238 -> 0x02003b14 veneer -> Func_080000c0
 *   0x0200323e -> 0x02003d7c veneer -> Func_080f9010
 *   0x0200324e -> 0x02003b84 veneer -> Func_08009150
 *   0x02003260 -> 0x02003b84 veneer -> Func_08009150
 *   0x02003266 -> 0x02003b8c veneer -> Func_08009158
 *   0x0200326a -> 0x02003d6c veneer -> Func_0808a5e8
 *   0x02003280 -> 0x02003b5c veneer -> Func_08009080
 *   0x02003284 -> 0x020032a4 prologue (this overlay's next owner)
 *
 * LINK BASE 0x02008000, confirmed again here and with an unusually strong
 * witness.  The pool word 0x0200bd84 is used as the base of a table indexed by
 * `(entity->facing >> 12) * 4`, so it must address 16 words of in-image data;
 * under the base that is file offset 0x3d84, and the 64 bytes there really are
 * a 16-entry direction table whose halfword pairs are only 0x0000, 0x0010 and
 * 0xfff0 - a unit step of +-0x10 in each axis for each of the 16 facings, and
 * nothing that could be read as code.  The other two pool words (0xffff0000
 * and 0x00003333) are plain constants.
 *
 * Geometry.  Positions are 16.16 fixed point; the halfword at entity+0x0a is
 * the integer part of x and the one at entity+0x12 the integer part of z, and
 * one tile is 0x10 of those integer units.  The table's HIGH half is the x
 * step and its LOW half the z step - it is used twice, once as
 * `asr #16` / sign-extended halves added to the hero's integer coordinates and
 * once as `& 0xffff0000` / `<< 16` added to the object's 16.16 coordinates,
 * and the two uses agree on that assignment.  The `>> 4` then converts to tile
 * indices, which is exactly the scale Func_02003184 compares against
 * (`p->x >> 20`, `p->z >> 20`).
 *
 * UNCERTAINTIES.
 *  - Func_080091d8 is the terrain probe used elsewhere in the semantic tree
 *    with the same (entity, position) shape; here only `result > 0` aborts, so
 *    negative and zero results are both treated as "the tile is free".  That
 *    asymmetry is what the `bgt` encodes and is preserved literally.
 *  - The destination y (sp[1]) is computed and handed to both Func_08009150
 *    calls but, unlike x and z, is never written back into the object.  That
 *    is what the code does; whether it is deliberate is not established.
 *  - Both Func_08009150 calls are handed the SAME destination - the object's
 *    new tile - the first with r0 = the object and the second with r0 = r8,
 *    which is the selector-0 entity fetched at entry.  r8 is not reloaded
 *    anywhere between, so the reading is not in doubt, but why the mover ends
 *    up on the moved object's tile is not established; it is most consistent
 *    with Func_08009150 setting an interpolation target that the pusher walks
 *    into as the object vacates.
 *  - 0x00003333 is stored to both +0x30 and +0x34 of the object and of the
 *    hero before the move; the byte-exact sibling for 0x02003a3c shows +0x30
 *    as a plain s32 field.  Its meaning is not established here.
 */

struct SceneEntity {
    u8 unknown_00[6];
    u16 facing;                     /* 0x06, high nibble selects 1 of 16 */
    s32 x;                          /* 0x08, 16.16 */
    s32 y;                          /* 0x0c, 16.16 */
    s32 z;                          /* 0x10, 16.16 */
    u8 unknown_14[0xe];
    u8 field22;                     /* 0x22 */
    u8 unknown_23;
    s32 field24;                    /* 0x24 */
    u8 unknown_28[4];
    s32 field2c;                    /* 0x2c */
    s32 field30;                    /* 0x30 */
    s32 field34;                    /* 0x34 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneEntity *Func_02006de4();
struct SceneEntity *Func_02006376();
s32 Func_02006dd2();
void Func_02006d8e();
void Func_02006d4e();
void Func_02006fbc();
void Func_02006dd4();
void Func_02006de6();
void Func_02006df4();
void Func_02006fd8();
void Func_02006dde();
void Func_0200652a();
                                        /* scene entity by selector */
                                        /* set presentation mode */
                                        /* move an entity to (x, y, z) */
                                        /* commit an entity's placement */
                                        /* terrain probe at a position */
                                        /* wait n frames */
                                        /* play a sound cue */
                                        /* refresh the scene after a move */

/* Intra-overlay callees. */
                                        /* object occupying tile (x, z), or 0 */

/* In-image direction table at file offset 0x3d84: 16 packed halfword steps,
 * high half = x, low half = z. */
extern u32 Data_0200bd84[];

void Func_020031b8(void)
{
    struct SceneEntity *hero;
    struct SceneEntity *object;
    u32 step;
    s32 facing;
    s32 destination[3];

    hero = Func_02006de4(0);

    facing = hero->facing >> 12;
    step = Data_0200bd84[facing];

    object = Func_02006376(
        (((s16)(hero->x >> 16) + ((s32)step >> 16)) >> 4),
        (((s16)(hero->z >> 16) + (s16)step) >> 4));
    if (object == 0) {
        return;
    }

    object->field22 = 2;

    /* Reloaded from the table rather than kept in a register, but it is the
     * same word: the index register r5 and the base in sl are both untouched
     * across the call. */
    step = Data_0200bd84[facing];

    destination[1] = object->y;
    destination[0] = object->x + (s32)(step & 0xffff0000);
    destination[2] = object->z + (s32)(step << 16);

    if (Func_02006dd2(object, destination) > 0) {
        return;                     /* the target tile is blocked */
    }

    Func_02006d8e(hero, 8);
    Func_02006d4e(15);
    Func_02006fbc(0xb9);

    Func_02006dd4(object, destination[0], destination[1], destination[2]);
    object->field30 = 0x3333;
    object->field34 = 0x3333;

    hero->field30 = 0x3333;
    hero->field34 = 0x3333;
    Func_02006de6(hero, destination[0], destination[1], destination[2]);

    Func_02006df4(object);
    Func_02006fd8();

    object->field24 = 0;
    object->field2c = 0;
    object->x = destination[0];
    object->z = destination[2];

    Func_02006dde(hero, 1);
    Func_0200652a();
}
