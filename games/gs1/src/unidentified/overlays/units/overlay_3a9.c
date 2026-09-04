#include "types.h"

#include "resource_3a9.h"

typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;

extern s16 Data_02000240[];
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 Data_020084d0[];
extern u8 Data_020086c8[];
extern u8 Data_020084a0[];
extern Placement Data_02008ef8[];
extern u8 *Data_03001ebc;

void Func_02000558(void);
void Func_020005a6(s32);
s32 Func_020005b6(s32, s32);
void Func_0200055e(s32);
void Func_0200057a(void);
void Func_02000580(void);
void Func_020005ce(s32);
s32 Func_020005e6(s32, s32);
void Func_0200059a(void);
u8 *Func_020005ba(s32);
void Func_02000638(s32, s32);
void Func_020005be(void);
s32 Func_020005ac(s32);
void Func_02000616(s32);
void Func_02000626(s32, s32);
void Func_02000626_b(s32);
void Func_0200063e(s32, s32);
void Func_020005de(s32);
void Func_020005fa(void);
u8 *Func_0200062a(int);
void Func_0200069e(int);
void Func_0200062c(void);
void Func_0200068a(int, int);
void Func_02000646(void);
void Func_02000658();
void Func_02000710();
u8 *Func_020006e6();
void Func_020006f0();
void Func_02000718();
void Func_02000714();
void Func_0200075c();
void Func_02000768();
void Func_02000774();
void Func_02000692(u32, u32, u32);
void Func_02000730();
void Func_0200065e(void);
void Func_020007b4();
void Func_020007ba();
void Func_020007c0();
void Func_020007c6();
void Func_020007cc();
void Func_020007d2();
void Func_020007d8();
void Func_020007de();
void Func_020007e4();
void Func_02000800();
void Func_02000806();
void Func_0200080e();
void Func_02000762(s32, s32, s32, s32, s32, s32);
s32 Func_02000772();
s32 Func_020007be();

/* Contiguous unnamed leaf-owner run for resource_3a9. */

/*
 * resource_3a9 owner at 0x02000074, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000078 holding 0x2008728.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000074 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008728 is image offset
 * 0x728 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* 0x02000626 serves two imports in sibling arms: the two-argument gesture in
 * the first and the one-argument message in the second. */

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
 * in the header of `games/gs1/semantic/overlays/resource_3a9_c_0200007c.c`.
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr}` at 0x02000240; single
 * epilogue `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020002fa, so the popped
 * word is the return address and the owner is **void**.  Code runs
 * 0x02000240..0x020002ff; the literal pool is the two words
 * 0x02000300..0x02000307 and 0x02000308 is the next (tracked byte-exact)
 * prologue.  200 bytes, matching the inventory row.
 *
 * All 13 call sites resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_3a9 0240` (an overlay `bl` stores target offset - 2).  Per-target:
 *   Func_0808a018 1   Scene_GetRecord 2   Audio_PlayCue 1   Func_08009178 1
 *   Func_0808a090 1   Func_0808a100 1   Func_0808a0d8 1   Func_0808a010 1
 *   Func_0808a248 1   Func_0808a368 1   Func_0808a370 1   Func_0808a020 1
 * The two Scene_GetRecord sites are the loop's per-slot fetch and the later
 * player fetch — distinct sites, not merged.
 *
 * The clearing loop runs slots 8..65 inclusive (`movs r5,#8` /
 * `cmp r5,#65 / bls`) and skips a slot whose record comes back null; the +85
 * byte it zeroes is the same field the tracked
 * `games/gs1/assets/code/resource_36f_c_02000054.c` clears on a scene record, so the
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
 * `games/gs1/semantic/overlays/resource_3ce_c_02000cf4.c`'s family uses.
 *
 * `Func_0808a090(0, 0x00008000, 0x00004000)` is the documented fixed-point
 * scale setter: 0x8000 and 0x4000 against 0x10000 as one are 0.5 and 0.25.
 * `Func_0808a0d8(0, 3, -8)` builds its -8 as `movs r2,#8 / negs r2`, which is
 * a negation and not the AND-mask variant of that idiom.
 *
 * `+ 364` (built as `movs r2,#182 / lsls r2,r2,#1`, the documented
 * displacement-as-shifted-constant habit) is the s16 sub-state slot of the
 * workspace the pointer cell 0x03001ebc addresses — one dereference, as the
 * tracked `games/gs1/assets/code/resource_3a9_c_02000308.c` spells it.  It is read twice,
 * once for the switch and once for Func_0808a248, and both reads are kept.
 *
 * Uncertainty: 158 is a Audio_PlayCue cue id from its argument position.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */

/* In-image placement table: four 8-byte entries {u32, u16, u16}. */

/*
 * resource_3a9 owner at 0x0200033c, 172 bytes: the per-sub-state slot cleanup
 * — decide which set of scene slots this sub-state leaves behind and clear
 * them.
 *
 * Unlike this overlay's other rows, 0x0200033c is NOT named by any in-image
 * pool word and no script-table record carries 0x0200833d, so nothing inside
 * the overlay references it and `overlay_call_targets.ts` reports no prologue
 * targets anywhere here.  It is nonetheless an ordinary, self-contained,
 * frame-balanced owner — one prologue, one interworking return, no live state
 * crossing either end — so it converts normally, exactly as the analogous
 * unreferenced owner in `games/gs1/semantic/overlays/resource_3ce_c_02000cf4.c` did.
 * Link-base evidence for the overlay is in the header of
 * `games/gs1/semantic/overlays/resource_3a9_c_0200007c.c`.
 *
 * Complete owner.  Prologue `push {lr}` at 0x0200033c with an 8-byte frame
 * (`sub sp,#8`) for the two stacked arguments of the six-argument call; single
 * epilogue `add sp,#8 / pop {r0} / bx r0` at 0x020003d8, so the popped word is
 * the return address and the owner is **void**.  Code runs
 * 0x0200033c..0x020003dd, the 2-byte zero at 0x020003de is the alignment word,
 * the literal pool is 0x020003e0..0x020003e7, and 0x020003e8 is the first
 * import veneer.  172 bytes, matching the inventory row.
 *
 * All 15 call sites resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_3a9 033c` (an overlay `bl` stores target offset - 2).  Per-target:
 *   Func_0808a088 11   GameFlag_IsSet 2   Func_08009180 1   Func_0808a158 1
 *
 * THE ELEVEN Func_0808a088 SITES ARE ELEVEN SITES, NOT A LOOP.  Nine of them
 * are a straight run of `movs r0,#k / bl` pairs with no counter, no back edge
 * and no compare, and the ids they pass are 10, 11, 12, 13, 14, 17, 18, 19, 15
 * — non-contiguous and ending out of order, which is the tell.  The other two
 * are on the unrelated final arm with ids 16 and 17.  Folding either run into a
 * loop would deflate the multiset by nine; that is the documented script-table
 * shape and it is written out here the same way.  The odd trailing 15 is
 * preserved rather than sorted, on the same grounds as the documented
 * non-sequential refresh orders.
 *
 * The sub-state dispatch is a three-way compare chain with a SHARED taken arm:
 * `> 15` falls to a `== 17` test, otherwise `>= 9` takes the same arm, and only
 * a bare `== 3` gets the six-argument call.  Everything else — including 16,
 * which sits inside the middle of the accepted range — goes to the last arm.
 * That hole is real and is why the test is not written as `9..17`.  The same
 * hole appears in Func_0200007c's window over the same halfword, which is a
 * free cross-check that both were read correctly.
 *
 * `Data_02000240 + 450` is the s16 sub-state slot of the shared work area,
 * `Data_02000240[225]`, as the tracked `games/gs1/assets/code/resource_3a9_c_02000308.c`
 * and the tracked resource_36f sources spell it.
 *
 * Uncertainties: 0x911 is read as an event-flag id from its argument position
 * on GameFlag_IsSet; the six-argument Func_08009180(30, 14, 30, 16, 4, 2) is the
 * established six-argument scripted-actor ABI, with the last two arguments
 * passed on the stack, and its argument meanings are not established here.
 */

/* RAM: the shared work area. */

s32 Func_02000030(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000064) {
        return (s32)Data_020084d0;
    }
    if (v == (s32)&Value_00000065) {
        return (s32)Data_020086c8;
    }
    return (s32)Data_020084a0;
}

s32 Func_02000070(void)
{
    return 0;
}

u8 *Func_02000074(void)
{
    return (u8 *)0x02008728;
}

void Func_02000144(void)
{
    Func_02000558();
    Func_020005a6(0x1ADD);
    Func_020005b6(0xC, 0);
    Func_0200055e(0x910);
    Func_0200057a();
}

void Func_0200016c(void)
{
    Func_02000580();
    Func_020005ce(0x1AE3);
    Func_020005e6(16, 0);
    Func_0200059a();
}

void Func_0200018c(void)
{
    u8 *player = Func_020005ba(0);

    /* Band guard: facing in 0x6001..0x9fff. The reference falls through to the
     * short arm and branches away to the scene, so the test is spelled as the
     * short arm's condition. */
    if ((u16) (*(u16 *) (player + 6) - 0x6001) <= 0x3FFE) {
        Func_02000638(7, 8);
    } else {
        Func_020005be();

        if (Func_020005ac(0x911) != 0) {
            Func_02000616(0x1AFB);
            Func_02000626(8, 0);
        } else {
            Func_02000626_b(0x1AD7);
            Func_0200063e(8, 0);
            Func_020005de(0x910);           /* 145 << 4 */
        }

        Func_020005fa();
    }
}

void Func_020001fc(void)
{
    void Func_0200067a(int);

    u8 *player = Func_0200062a(0);

    /* Band guard: facing in 0xa001..0xdfff. The reference falls through to the
     * short arm and branches away to the scene, so the test is spelled as the
     * short arm's condition. */
    if ((u16) (*(u16 *) (player + 6) + 0x5FFF) <= 0x3FFE) {
        Func_0200069e(8);
    } else {
        Func_0200062c();
        Func_0200067a(0x1A8F);
        Func_0200068a(8, 0);
        Func_02000646();
    }
}

void Func_02000240(void)
{
    u8 *Func_0200067a();

    u8 *work = *(u8 **)0x03001ebc;
    u32 slot;
    s32 index;
    u8 *player;

    Func_02000658();

    for (slot = 8; slot <= 65; slot++) {
        u8 *record = Func_0200067a(slot);

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

    Func_02000730(158);

    {
        u32 offset = index << 3;
        u32 value = (u32)Data_02008ef8;
        u32 coordinateAddress = offset + 4;
        u16 x = *(volatile u16 *)(value + coordinateAddress);
        u16 y;

        coordinateAddress += value;
        y = *(volatile u16 *)(coordinateAddress + 2);
        value = *(volatile u32 *)(value + offset);
        Func_02000692(value, x, y);
    }

    Func_020006f0(0, 0x00008000, 0x00004000);

    player = Func_020006e6(0);
    player[85] = 0;

    Func_02000714(0, 2);
    Func_02000718(0, 3, -8);
    Func_020006e6(10);

    Func_0200075c(*(s16 *)(work + 364));
    Func_02000768();
    Func_02000774();
    Func_02000710();
}

s32 Func_02000308(void)
{
    *(s32 *)(Data_03001ebc + 448) = 0x209;
    if (Data_02000240[224] == (s32) (u32) &Value_00000064) {
        Func_0200065e();
    }
    return 0;
}

void Func_0200033c(void)
{
    s16 sub = Data_02000240[225];

    switch (sub) {
    case 3:
    {
        s32 fifth = 4;
        s32 sixth = 2;
        Func_02000762(30, 14, 30, 16, fifth, sixth);
        return;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
        break;
    default:
        goto other;
    }

    /* sub is 9..15 or 17. */
    if (Func_02000772(0x911) != 0) {
        /* Nine distinct sites; the trailing 15 is out of order in the
         * reference and is kept that way. */
        Func_020007b4(10);
        Func_020007ba(11);
        Func_020007c0(12);
        Func_020007c6(13);
        Func_020007cc(14);
        Func_020007d2(17);
        Func_020007d8(18);
        Func_020007de(19);
        Func_020007e4(15);
    } else {
        Func_0200080e(13, 2);
    }
    return;

other:
    if (Func_020007be(0x911) != 0) {
        Func_02000800(16);
        Func_02000806(17);
    }
}
