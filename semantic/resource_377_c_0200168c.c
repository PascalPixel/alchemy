#include "types.h"

/*
 * resource_377 owner at 0x0200168c, 284 bytes.  Complete owner: the
 * high-register prologue
 *   push {r5,r6,r7,lr} ; mov r7,fp ; mov r6,sl ; mov r5,r9 ; push {r5,r6,r7} ;
 *   mov r7,r8 ; push {r7} ; sub sp,#8
 * and the matching interworking return
 *   add sp,#8 ; pop {r3,r5,r6,r7} ; mov r8,r3 ; ... ; pop {r5,r6,r7} ;
 *   pop {r0} ; bx r0
 * r0 holds the popped return address, so the owner returns nothing.  The 8 bytes
 * of frame are a two-entry array of created-object pointers, addressed through
 * sl.
 *
 * It creates the pair of objects a scene needs from one descriptor, initialises
 * each object's sprite record, and finally installs a per-frame callback on each.
 *
 * LINK BASE WITNESS.  The two callbacks installed at object +108 are the pool
 * words 0x02009639 and 0x020095e9.  Both are ODD, so Thumb entries; under this
 * family's 0x02008000 link base they are 0x02001638 + 1 and 0x020015e8 + 1, i.e.
 * exactly this overlay's own Func_02001638 and Func_020015e8, whose semantic
 * sources are already in the tree.  That confirms the base independently for
 * resource_377 and identifies the callbacks rather than leaving them as data.
 *
 * POOL MAP, taken from a control-flow walk of the owner (not from the
 * disassembler's referenced-words list).  Three pools, two of them INSIDE the
 * body and hopped by a bare unconditional 'b.n' with no conditional structure
 * around it, with register state carried straight across:
 *   0x020016e0..0x020016e7  0x00000000 and 0x03001f30    (hopped by 0x020016de)
 *   0x0200174e..0x02001757  alignment, 0xfffffc00, 0x03001b10
 *                                                        (hopped by 0x0200174c)
 *   0x020017a0..0x020017a7  0x02009639 and 0x020095e9    (after the epilogue)
 * 0x03001f30 is an IWRAM pointer cell whose target is held in fp for the whole
 * owner; 0x03001b10 is an IWRAM table of 4-byte entries indexed by the sprite
 * record's +28 byte.
 *
 * Registers carried across those pools and across the loop: r7 is the 0..1 loop
 * counter, r6 the incoming descriptor, sl the frame array, r9 the constant 63,
 * r8 the constant 0 (loaded from the first pool word), fp the 0x03001f30 target.
 *
 * All three calls resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 168c --json' (3 sites,
 * 3 distinct veneer targets):
 *   0x16b4 -> 0x185c -> Func_080090c8(26, d->arg0, d->arg1, d->arg2)
 *   0x16ec -> 0x184c -> Func_08009020(sprite, 0)
 *   0x16fa -> 0x183c -> Func_080001b8(sprite->index)
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 *
 * Uncertainties, all faithful to the assembly rather than tidied:
 * - the +5 byte is masked with ~0x20 and then with 0x3f, and 0x3f already
 *   contains 0x20, so the first mask is redundant in the original code;
 * - the post-loop block indexes slots[0] and slots[1] unconditionally, although
 *   the loop stores a null there when Func_080090c8 fails.  The original does
 *   not re-test, and this reconstruction does not add a test;
 * - 'lsls #17 ; lsrs #22' extracts bits 5..14 of the table halfword into the low
 *   10 bits of the sprite record's +8 field; it is written as the same shift
 *   pair rather than as a mask, because the intermediate is 32-bit.
 */

struct Sprite_0200168c {
    u8 filler00[5];
    u8 flags05;                 /* 0x05 */
    u8 filler06;
    u8 flags07;                 /* 0x07 */
    u16 attr08;                 /* 0x08 */
    u8 mode09;                  /* 0x09 */
    u8 filler0a[18];
    u8 index1c;                 /* 0x1c */
    u8 flags1d;                 /* 0x1d */
    u8 filler1e[8];
    u8 flags26;                 /* 0x26 */
    u8 filler27;
    u8 *link28;                 /* 0x28 */
};

struct Object_0200168c {
    u8 filler00[20];
    s32 field14;                /* 0x14 */
    u8 filler18[11];
    u8 field23;                 /* 0x23 */
    u8 filler24[44];
    struct Sprite_0200168c *sprite;  /* 0x50 */
    u8 filler54;
    u8 field55;                 /* 0x55 */
    u8 filler56[14];
    u16 field64;                /* 0x64 */
    u8 filler66[2];
    void *owner;                /* 0x68 */
    void (*callback)();         /* 0x6c */
};

struct Descriptor_0200168c {
    u8 filler00[8];
    s32 arg0;                   /* 0x08 */
    s32 arg1;                   /* 0x0c */
    s32 arg2;                   /* 0x10 */
    s32 arg3;                   /* 0x14 */
};

struct Object_0200168c *Func_080090c8();
void Func_08009020();
void Func_080001b8();

/* The two installed per-frame callbacks, identified from their pool words. */
void Func_02001638();
void Func_020015e8();

void Func_0200168c(struct Descriptor_0200168c *descriptor)
{
    s32 permuted_11;
    struct Object_0200168c *slots[2];
    u8 *base = *(u8 **)0x03001F30;
    u8 *table = (u8 *)0x03001B10;
    u32 i;

    for (i = 0; i <= 1; i++) {
        struct Object_0200168c *object;
        struct Sprite_0200168c *sprite;
        u16 packed;

        object = Func_080090c8(26, descriptor->arg0, descriptor->arg1,
                               descriptor->arg2);
        if (object == 0) continue;
        slots[i] = object;

        object->field14 = descriptor->arg3;
        sprite = object->sprite;
        object->field55 = 0;
        object->field64 = 0;
        object->owner = descriptor;
        if (sprite == 0) continue;

        Func_08009020(sprite, 0);
        sprite->flags26 = 0;
        Func_080001b8(sprite->index1c);

        sprite->index1c = (u8)*(u16 *)(base + 70);
        sprite->flags1d |= 1;

        sprite->attr08 = (u16)((sprite->attr08 & 0xFC00) |
                               (((u32)packed << 17) >> 22));
        packed = *(u16 *)(table + sprite->index1c * 4 + 2);

        sprite->flags05 = (u8)(((sprite->flags05 & ~0x20) & 0x3F) | 0x40);
        sprite->flags07 = (u8)((sprite->flags07 & 0x3F) | 0x80);
        sprite->link28[22] = 0;
    }

    slots[0]->callback = Func_02001638;
    slots[0]->sprite->mode09 = (u8)((slots[0]->sprite->mode09 & ~0x0C) | 8);

    slots[1]->callback = Func_020015e8;
    slots[1]->sprite->mode09  = permuted_11;
    permuted_11 = (u8)((slots[1]->sprite->mode09 & ~0x0C) | 8);
    slots[1]->field23 = 2;
}
