typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a9 owner at 0x02000240, 200 bytes: the scene arrival routine —
 * clear every scene slot's residue, look the current sub-state up in a small
 * in-image table, and place the player where that entry says.
 *
 * Role known in advance from the in-image scene-script tables: FOUR three-word
 * records name 0x02008241 as their callback — selectors 0x10 and 0x13 in one
 * table and 0x0c and 0x0d in another — and 0x02008241 - 0x8000 - 1 = 0x0240.
 * So one function serves four script selectors, which is why it re-reads the
 * sub-state itself rather than taking it as an argument.  Link-base evidence is
 * in the header of `semantic/overlays/resource_3a9_c_0200007c.c`.
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr}` at 0x02000240; single
 * epilogue `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020002fa, so the popped
 * word is the return address and the owner is **void**.  Code runs
 * 0x02000240..0x020002ff; the literal pool is the two words
 * 0x02000300..0x02000307 and 0x02000308 is the next (banked byte-exact)
 * prologue.  200 bytes, matching the inventory row.
 *
 * All 13 call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3a9 0240` (an overlay `bl` stores target offset - 2).  Per-target:
 *   Func_0808a018 1   Func_0808a080 2   Func_080f9010 1   Func_08009178 1
 *   Func_0808a090 1   Func_0808a100 1   Func_0808a0d8 1   Func_0808a010 1
 *   Func_0808a248 1   Func_0808a368 1   Func_0808a370 1   Func_0808a020 1
 * The two Func_0808a080 sites are the loop's per-slot fetch and the later
 * player fetch — distinct sites, not merged.
 *
 * The clearing loop runs slots 8..65 inclusive (`movs r5,#8` /
 * `cmp r5,#65 / bls`) and skips a slot whose record comes back null; the +85
 * byte it zeroes is the same field the banked
 * `assets/code/resource_36f_c_02000054.c` clears on a scene record, so the
 * offset is evidence rather than inference.  The later single clear of the
 * player's own +85 byte does NOT test for null, and that asymmetry is in the
 * reference.
 *
 * The sub-state switch is a compare chain, not a jump table: 12, 13, 16 and 19
 * map to placement indices 0, 1, 2 and 3, and EVERY other value returns
 * immediately without touching anything.  Index times 8 addresses an 8-byte
 * entry `{u32, u16, u16}` in the in-image table at 0x02008ef8 (even pool word,
 * so data at file offset 0x0ef8), and the three fields go straight into
 * Func_08009178 — the same three-argument shape
 * `semantic/overlays/resource_3ce_c_02000cf4.c`'s family uses.
 *
 * `Func_0808a090(0, 0x00008000, 0x00004000)` is the documented fixed-point
 * scale setter: 0x8000 and 0x4000 against 0x10000 as one are 0.5 and 0.25.
 * `Func_0808a0d8(0, 3, -8)` builds its -8 as `movs r2,#8 / negs r2`, which is
 * a negation and not the AND-mask variant of that idiom.
 *
 * `+ 364` (built as `movs r2,#182 / lsls r2,r2,#1`, the documented
 * displacement-as-shifted-constant habit) is the s16 sub-state slot of the
 * workspace the pointer cell 0x03001ebc addresses — one dereference, as the
 * banked `assets/code/resource_3a9_c_02000308.c` spells it.  It is read twice,
 * once for the switch and once for Func_0808a248, and both reads are kept.
 *
 * Uncertainty: 158 is a Func_080f9010 cue id from its argument position.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();            /* scene entity by selector, or null */
void Func_0808a018();           /* open a scene bracket */
void Func_0808a020();           /* close a scene bracket */
void Func_0808a010();           /* wait this many frames */
void Func_0808a090();           /* set a 16.16 x/y scale */
void Func_0808a0d8();           /* nudge an entity by a delta */
void Func_0808a100();
void Func_0808a248();
void Func_0808a368();           /* scene bookkeeping, no arguments */
void Func_0808a370();           /* scene bookkeeping, no arguments */
void Func_08009178();           /* place the player (destination, x, y) */
void Func_080f9010();           /* play a sound cue */

/* In-image placement table: four 8-byte entries {u32, u16, u16}. */
extern u8 Data_02008ef8[];

void Func_02000240(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 slot;
    s32 index;
    u8 *entry;
    u8 *player;

    Func_0808a018();

    for (slot = 8; slot <= 65; slot++) {
        u8 *record = Func_0808a080(slot);

        if (record != 0) {
            record[85] = 0;
        }
    }

    switch (*(s16 *)(work + 364)) {         /* 182 << 1 */
    case 12: index = 0; break;
    case 13: index = 1; break;
    case 16: index = 2; break;
    case 19: index = 3; break;
    default: return;
    }

    Func_080f9010(158);

    entry = Data_02008ef8 + index * 8;
    Func_08009178(*(u32 *)entry, *(u16 *)(entry + 4), *(u16 *)(entry + 6));

    Func_0808a090(0, 0x00008000, 0x00004000);   /* 0.5, 0.25 in 16.16 */

    player = Func_0808a080(0);
    player[85] = 0;

    Func_0808a100(0, 2);
    Func_0808a0d8(0, 3, -8);
    Func_0808a010(10);

    Func_0808a248(*(s16 *)(work + 364));
    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
}
