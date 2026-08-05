#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 owner at 0x02002440 (292 bytes, 4 call sites).
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr}` plus two high-register
 * saves (`fp, sl, r9` then `r8`) and `sub sp, #8` at 0x02002440; interworking
 * return at 0x0200254a restoring sp, r8-fp and `pop {r0} ; bx r0`.  r0 holds
 * the popped return address, so the owner is `void`.
 *
 * Three literal pools sit inside the span and are branched over, not executed:
 * 0x0200249a-0x020024a3 (alignment zeros, 0x00000000, 0x03001f30),
 * 0x0200250a-0x02002513 (alignment, 0xfffffc00, 0x03001b10) and
 * 0x0200255c-0x02002563 (0x0200a3ed, 0x0200a39d).  They are reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Link base 0x02008000 (as on the other confirmed overlays): the last pool's
 * words are 0x0200a3ed = file offset 0x23ec + Thumb bit and 0x0200a39d =
 * 0x239c + Thumb bit, i.e. the byte-exact per-frame steppers
 * `assets/code/resource_374_c_020023ec.c` and `..._0200239c.c`.  This owner
 * installs exactly those two as the two records' +0x6c update hooks, which is
 * both the link-base witness and the identity of the two constants.
 *
 * The struct layout is taken from those two proven sources, not rederived:
 * `f64` is the step counter they increment and `f68` the source record they
 * read `f08`/`f10` from — and this owner is the writer of both fields.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all four sites
 * are import veneers (Func_080f9010, Func_080090c8, Func_08009020,
 * Func_080001b8).  The disassembler's `bl` annotations are not used.
 *
 * Uncertainties:
 *  - the pool word at 0x0200249c is 0x00000000 and is loaded into r8 purely to
 *    supply the byte 0 stored at sprite+0x26 and at *(sprite+0x28)+22.  It is
 *    modelled as the constant 0;
 *  - after the loop both slots of the two-entry stack array are dereferenced
 *    without a null test, although the loop stores NULL there when
 *    Func_080090c8 fails.  That is what the assembly does and it is preserved;
 *  - `*(u8 *)(sprite + 28) = *(u16 *)(workspace + 70)` is a halfword load
 *    narrowed by a byte store; the truncation is in the original.
 */

struct Source {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    s32 f14;
};

struct Attach {
    u8 pad00[22];
    u8 f16;
};

/*
 * f08 is read and written as a halfword at +8 while the byte at +9 is also
 * accessed on its own, so that byte is reached through explicit pointer
 * arithmetic rather than a field.
 */
struct Sprite {
    u8 pad00[5];
    u8 f05;
    u8 pad06[1];
    u8 f07;
    u16 f08;
    u8 pad0a[0x12];
    u8 f1c;
    u8 f1d;
    u8 pad1e[0xa];
    struct Attach *f28;
};

struct Obj {
    u8 pad00[0x14];
    s32 f14;
    u8 pad18[0x38];
    struct Sprite *f50;
    u8 pad54[1];
    u8 f55;
    u8 pad56[0xe];
    u16 f64;
    u8 pad66[2];
    struct Source *f68;
    void (*f6c)(struct Obj *);
};

/* Table of 4-byte entries in IWRAM; only the halfword at +2 is read. */
extern u16 Data_03001b10[];

/* Workspace pointer global. */
extern u8 *Data_03001f30;

/* Per-frame steppers installed by this owner (byte-exact siblings). */
void Func_020023ec(struct Obj *);
void Func_0200239c(struct Obj *);

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: their interfaces are not established here. */
void Func_080f9010();
struct Obj *Func_080090c8();
void Func_08009020();
void Func_080001b8();

void Func_02002440(struct Source *source)
{
    struct Obj *made[2];
    u8 *workspace;
    struct Obj *obj;
    struct Sprite *sprite;
    u32 i;
    s32 shape;
    u16 packed;

    workspace = Data_03001f30;

    Func_080f9010(0x83);

    for (i = 0; i <= 1; i++) {
        obj = Func_080090c8(26, source->f08, source->f0c, source->f10);
        made[i] = obj;
        if (obj == NULL) continue;

        obj->f14 = source->f14;
        sprite = obj->f50;
        obj->f55 = 0;
        obj->f68 = source;
        obj->f64 = 0;
        if (sprite == NULL) continue;

        Func_08009020(sprite, 0);
        Func_080001b8(sprite->f1c);
        *((u8 *)sprite + 0x26) = 0;

        /* Halfword source, byte destination: the truncation is original. */
        sprite->f1d |= 1;
        sprite->f1c = (u8)*(u16 *)(workspace + 70);

        /* Bits 5-14 of the table halfword replace the low 10 bits of f08. */
        packed = Data_03001b10[sprite->f1c * 2 + 1];
        sprite->f08 = (u16)((sprite->f08 & 0xfc00) | ((packed >> 5) & 0x3ff));

        sprite->f28->f16 = 0;
        sprite->f05 = (u8)(((sprite->f05 & ~0x21) & 0x3f) | 0x40);
        sprite->f07 = (u8)((sprite->f07 & 0x3f) | 0x80);
    }

    *((u8 *)made[0]->f50 + 9) = (u8)((*((u8 *)made[0]->f50 + 9) & ~13) | 8);
    made[0]->f6c = Func_020023ec;

    *((u8 *)made[1]->f50 + 9) = (u8)((*((u8 *)made[1]->f50 + 9) & ~13) | 8);
    made[1]->f6c = Func_0200239c;
    *((u8 *)made[1] + 0x23) = 2;
}
