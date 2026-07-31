typedef signed int s32;
typedef signed short s16;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * Resource 3bc, owner at 0x02000204.  72 strict bytes: 58 bytes of code
 * (0x0204-0x023d), a 2-byte alignment `nop`, and a three-word literal pool at
 * 0x0240-0x024b.
 *
 * Single call site at 0x0200021c -> veneer 0x02004a18 -> Func_0808a080, the
 * scene-record accessor.  The byte-exact sibling
 * assets/code/resource_3bc_c_02000658.c spells the same import (under its
 * printed name Func_0200507e) as `Object *f(s32 id)` fed from
 * `*(s32 *)&Data_02000240[250]`, i.e. the u32 at byte offset 500 of the
 * cross-overlay Data_02000240 table.  That file also proves the record
 * layout used here: the 16.16 X word at +8 and the 16.16 Z word at +0x10.
 *
 * `movs r1,#18 / ldrsh r3,[r0,r1]` reads the *upper* halfword of the Z word,
 * i.e. Z's 16.16 integer part.  `subs r3,#183 / cmp r3,#3 / bhi` is an
 * unsigned range test, so the guard is 183 <= (z >> 16) <= 186.  r1 is left
 * holding 18 afterwards but there is no following call, so no phantom
 * argument arises here.
 *
 * The pool word 0xffff3334 is the signed constant -0xCCCC applied to both
 * records' X words.  0xCCCC is the same 16.16 fraction that the byte-exact
 * resource_3bc_c_02000274.c writes as a scale field, so this is a fixed-point
 * nudge of 0.8 of a unit in -X.
 *
 * Epilogue `pop {r5} / pop {r0} / bx r0` — void.
 *
 * Uncertainty: the object fetched from `workspace + 480` is typed as the same
 * record only because the +8 field is used identically; nothing here pins the
 * rest of its layout.
 */

typedef struct Object {
    u8 filler00[8];
    s32 x;      /* 16.16 */
    u8 filler0C[4];
    s32 z;      /* 16.16; the s16 at +0x12 is its integer part */
} Object;

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;   /* pointer cell holding the scene workspace */

Object *Func_0808a080();

void Func_02000204(void)
{
    u8 *workspace;
    s16 *table;
    Object *actor;
    Object *subject;
    s32 z;

    workspace = Data_03001ebc;
    table = Data_02000240;
    subject = *(Object **)(workspace + 480);
    actor = Func_0808a080(*(s32 *)&table[250]);

    z = *(s16 *)((u8 *)actor + 0x12);
    if ((s32)((u32)(z - 183)) <= 3) {
        subject->x += -0xCCCC;
        actor->x += -0xCCCC;
    }
}
