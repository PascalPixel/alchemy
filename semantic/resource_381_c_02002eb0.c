#include "types.h"

/*
 * Resource 381, paired-effect spawner at 0x02002eb0
 * (320 bytes, 6 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7,fp / mov r6,sl / mov r5,r9 / push {r5,r6,r7} / mov r7,r8 /
 * push {r7}` and `sub sp, #8` at 0x02002eb0, with the matching unwind at
 * 0x02002fd6 ending `pop {r0} / bx r0`.  The popped branch register IS r0, so
 * it holds the return address and the owner is `void`.
 *
 * Pool map by control-flow walk — this owner has THREE pools, two of them
 * interior, so the span cannot be read as "code then pool":
 *   0x02002f0c-0x02002f13 (0x00000000 and 0x03001f30), hopped by the
 *     `b.n 0x02002f14` at 0x02002f0a — the documented lone-forward-branch pool
 *     hop, with no conditional structure around it;
 *   0x02002f7c-0x02002f83 (0xfffffc00 and 0x03001b10), hopped by the
 *     `b.n 0x02002f84` at 0x02002f78;
 *   0x02002fe8-0x02002fef (0x0200ae5d and 0x0200ae0d), past the final `bx`.
 * The 0x00000000 word is the reason r8 is loaded from a pool at all rather
 * than built with `movs`; it is a zero, not an address.
 *
 * The two trailing pool words are ODD, so under the proven 0x02008000 link
 * base they are Thumb entries, not data: 0x0200ae5d -> Func_02002e5c and
 * 0x0200ae0d -> Func_02002e0c, BOTH of which are already byte-exact in
 * `assets/code/`.  This owner therefore spawns two effect entities and hooks
 * each one's per-frame handler at record + 108; reading those two tracked
 * sources confirms the record layout used here (the 16.16 triple at +8/+12/+16
 * and the sub-record pointer at +0x50) without inference.
 *
 * `Data_03001f30` is a pointer CELL: `ldr r3,[pc] / ldr r3,[r3]` is one
 * dereference to the workspace, whose u16 at +70 is the palette/graphic id
 * stamped into each spawned sub-record.  `Data_03001b10` is a table of 4-byte
 * entries whose second halfword carries a 10-bit field at bits 5..14, which is
 * what `lsls #17 / lsrs #22` extracts before it is merged into the
 * sub-record's u16 at +8 under the mask 0xfc00.
 *
 * r9 holds the constant 63 for the whole body and is the low-six-bits mask
 * applied to the sub-record's flag bytes at +5 and +7; -33 is a
 * `movs r1,#33 / negs` pair, i.e. `~0x20`.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 6 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_0808a080 x2, and one each of Func_080001b8, Func_08009020,
 *   Func_080090c8, Func_080f9010.
 * The two Func_0808a080(15) sites are distinct calls in the assembly and are
 * NOT folded into one local; the spawn loop runs exactly twice and contributes
 * one site per import, not two.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080001b8();   /* release a graphic slot by id */
void Func_08009020();   /* reset a sub-record */
u8 *Func_080090c8();    /* spawn an entity at a 16.16 position */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_080f9010();   /* play a cue by id */

void Func_02002e5c(void);   /* this overlay, byte-exact in assets/code */
void Func_02002e0c(void);   /* this overlay, byte-exact in assets/code */

extern u8 *Data_03001f30;   /* pointer cell: the scene workspace */
extern u8 Data_03001b10[];  /* 4-byte graphic descriptors */

void Func_02002eb0(u8 *descriptor)
{
    u8 *workspace = Data_03001f30;
    u8 *made[2];
    u8 *entity;
    u8 *sub;
    u8 *peer;
    int i;
    unsigned int id;

    Func_080f9010(292);

    for (i = 0; i <= 1; i++) {
        entity = Func_080090c8(26,
                               *(s32 *)(descriptor + 8),
                               *(s32 *)(descriptor + 12),
                               *(s32 *)(descriptor + 16));
        made[i] = entity;
        if (entity == 0) {
            continue;
        }

        *(s32 *)(entity + 20) = *(s32 *)(descriptor + 20);
        sub = *(u8 **)(entity + 0x50);
        entity[85] = 0;
        *(u16 *)(entity + 100) = 0;
        *(u8 **)(entity + 104) = descriptor;
        if (sub == 0) {
            continue;
        }

        Func_08009020(sub, 0);
        sub[38] = 0;

        Func_080001b8(sub[28]);
        sub[28] = (u8)*(u16 *)(workspace + 70);
        sub[29] |= 1;

        id = *(u16 *)(Data_03001b10 + sub[28] * 4 + 2);
        *(u16 *)(sub + 8) = (u16)((*(u16 *)(sub + 8) & 0xfc00)
                                  | ((id << 17) >> 22));

        sub[5] = (u8)(((sub[5] & ~0x20) & 63) | 0x40);
        sub[7] = (u8)((sub[7] & 63) | 0x80);

        *(u8 *)(*(u8 **)(sub + 40) + 22) = 0;
    }

    /* Hook the two handlers and copy the reference entity's low flag bits. */
    *(void **)(made[0] + 108) = (void *)Func_02002e5c;

    entity = Func_0808a080(15);
    sub = *(u8 **)(made[0] + 0x50);
    sub[9] = (u8)(((*(u8 **)(entity + 0x50))[9] & 12) | (sub[9] & ~13));

    entity = Func_0808a080(15);
    peer = made[1];
    sub = *(u8 **)(peer + 0x50);
    id = ((*(u8 **)(entity + 0x50))[9] & 12) | (sub[9] & ~13);
    *(void **)(peer + 108) = (void *)Func_02002e0c;
    sub[9] = (u8)id;
    peer[35] = 2;
}
