typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_39e owner at 0x02000518, 320 bytes: a scripted beat that poses
 * actor slot 9, spawns three objects around it at three different
 * headings, waits, awards something through Func_080091c0 and sets story
 * flag 0x892.
 *
 * Complete owner: `push {r5, r6, lr}` plus the r8/r9/sl spill at
 * 0x02000518-0x02000522 through `add sp, #56 / pop {r3, r5, r6} / ... /
 * pop {r0} / bx r0` at 0x0200063a-0x02000648, then the three-word
 * literal pool 0x0200064c-0x02000657. The next owner's prologue is
 * exactly at 0x02000658. **320 bytes**, measured to the epilogue.
 *
 * Published population (sweep B): no `bl` anywhere in the image targets
 * it. All 23 `bl` sites resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 518 658`; the tool
 * reports sites=23 and 23 bl lines were transcribed, so the site
 * arithmetic agrees.
 *
 * THE THREE SPAWNS ARE THE STRUCTURE. Func_0200013c -- the byte-identical
 * multi-copy spawn+configure routine already drafted in this overlay
 * (resource_39e_c_0200013c.c) and in resource_39c/resource_3c9 -- is
 * called three times with the SAME first three arguments and only the
 * fourth changing: 0x8000, 0, 0xffff8000. Those are +90, 0 and -90
 * degrees in the engine's 16-bit angle units, so the three objects are
 * placed a quarter-turn apart. Every other argument is shared:
 * a1/a2/a3 are record[8], record[12] and record[16] + 0x40000 read from
 * the slot-9 record fetched once at the top; a5/a6 are 0; flags is
 * 0x10000; extra is a two-word stack block at sp+16 whose second word is
 * 7. Because flags lacks bit 0x100000, Func_0200013c never dereferences
 * `extra` -- it is passed and ignored on this path.
 *
 * The 56-byte stack frame exists only to hold those four stack arguments
 * plus that two-word block; there is no local array.
 *
 * The 0x40000 added to record[16] and the 0x10000 written to record[72]
 * are carried in r9 and sl across the whole body -- the same two
 * constants are reused as a spawn argument and as a record field, which
 * is why they are held in callee-saved registers rather than rebuilt.
 * Both are built by shift chains (128 << 11 and 128 << 9), never pooled.
 *
 * SHARED IDIOMS, cited not re-derived: the Func_0808a018/Func_0808a020
 * scripted-scene bracket (resource_39e_c_02001494.c); dialogue-by-id
 * Func_0808a170 (here id 0x17b4, a pool word); wait-n-frames
 * Func_0808a010(30); story-flag set Func_080770c8(0x892).
 *
 * Uncertainty: Func_080091c0's six arguments (10, 24, 1, 1, 10, 22) are
 * transcribed, not interpreted. The meaning of the constant 7 in the
 * stack block is unknown -- Func_0200013c does not read it on this
 * path, so some later consumer must.
 */

extern void Func_0200013c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_080091c0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 slot);
extern void Func_0808a090(s32 slot, s32 a1, s32 a2);
extern void Func_0808a0b8(s32 slot, s32 a1, s32 a2);
extern void Func_0808a0d0(s32 slot, s32 a1, s32 a2);
extern void Func_0808a0e8(s32 slot);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a188(s32 slot, s32 a1, s32 a2);
extern void Func_0808a1b8(s32 slot, s32 a1, s32 a2);
extern void Func_0808a5e8(void);
extern void Func_080f9010(s32 arg0);

void Func_02000518(void)
{
    u8 *record = Func_0808a080(9);
    u8 *other;
    s32 spawnOffset = 128 << 11;   /* 0x40000, added to record[16] */
    s32 spawnFlags = 128 << 9;     /* 0x10000, also written to record[72] */
    s32 block[2];

    Func_0808a018();
    Func_0808a170(0x17b4);
    Func_0808a188(9, 0, 20);
    Func_0808a0d0(0, 168, 196 << 1);
    Func_0808a1b8(0, 192 << 8, 20);
    Func_080f9010(132);

    other = Func_0808a080(9);
    *(s32 *)(other + 40) = 160 << 13;

    other = Func_0808a080(9);
    *(s32 *)(other + 72) = spawnOffset;

    Func_0808a090(9, 192 << 10, 192 << 9);
    Func_0808a0b8(9, 152, 196 << 1);
    Func_0808a0e8(9);

    other = Func_0808a080(9);
    *(s32 *)(other + 72) = spawnFlags;

    Func_0808a1b8(9, 0, 0);
    Func_080f9010(132);

    block[1] = 7;

    Func_0200013c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + spawnOffset,
                  128 << 8, 0, 0, spawnFlags, (u8 *)block);

    Func_0200013c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + spawnOffset,
                  0, 0, 0, spawnFlags, (u8 *)block);

    Func_0200013c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + spawnOffset,
                  (s32)0xffff8000, 0, 0, spawnFlags, (u8 *)block);

    Func_0808a010(30);
    Func_0808a5e8();
    Func_080091c0(10, 24, 1, 1, 10, 22);
    Func_080770c8(0x892);
    Func_0808a020();
}
