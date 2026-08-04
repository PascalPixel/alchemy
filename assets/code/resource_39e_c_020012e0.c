typedef int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_39e owner at 0x020012e0, 84 bytes: a DISPATCHER. It picks one
 * of two approach beats by the actor's facing angle, then one of two
 * follow-ups by story flag 0x898.
 *
 * Complete owner: `push {lr}` at 0x020012e0 through `pop {r0} / bx r0` at
 * 0x0200132a-0x0200132c, then the single pool word at 0x02001330
 * (0x00000898). The next owner's prologue is exactly at 0x02001334.
 * **84 bytes**, measured to the epilogue.
 *
 * Published population (sweep B), and this is the row an earlier
 * story-chain note already predicted: "0x12e0 tests 0x898". It does, and
 * it is a `bl` caller of FOUR already-drafted owners at once, so each of
 * those four drafts is independently corroborated by this one.
 * All 9 `bl` sites resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 12e0 1334`; sites=9
 * and nine bl lines were transcribed.
 *
 * THE ANGLE TEST IS A BAND, NOT A THRESHOLD, and the operands are
 * UNSIGNED. Slot 0's u16 at record[6] is read twice, from two separate
 * Func_0808a080(0) fetches, and compared against 0x4000 with `bls` and
 * against 0xc000 with `bcs`. Func_02000bf4 runs only when the value is
 * strictly inside (0x4000, 0xc000) -- the half-turn band. Everything else
 * goes to Func_02000cd4. Written here as the two guards in source order
 * so the short-circuit and the double fetch both survive.
 *
 * Note the record is fetched TWICE for what is logically one field read.
 * Do not collapse it: the two fetches are two calls in the assembly, and
 * Func_0808a080 is not known to be pure.
 *
 * The flag tail is the ordinary sense -- flag 0x898 SET selects
 * Func_02001160, clear selects Func_0200102c(0). 0x898 is set by this
 * overlay's 0x02001494 and cleared by its 0x02001dbc, so the two arms are
 * before-and-after versions of the same story point.
 *
 * SHARED IDIOM: the Func_0808a018/Func_0808a020 scripted-scene bracket,
 * cited from resource_39e_c_02001494.c. Note it brackets the WHOLE
 * dispatch including the callees, so the four dispatched owners run
 * inside an already-open scene.
 *
 * Uncertainty: record[6]'s units are inferred to be the engine's 16-bit
 * angle from the 0x4000/0xc000 quarter-turn constants and from the
 * four-way quadrant selector in the neighbouring 0x02001334; that is a
 * strong reading, not a proof.
 */

extern s32 Func_020056d0(s32 flagId);
extern void Func_020056c0(void);
extern void Func_0200570c(void);
extern u8 *Func_020056ee(s32 slot);
extern u8 *Func_020056fe(s32 slot);
extern void Func_02001efc(void);
extern void Func_02001fe2(void);
extern void Func_0200247c(void);
extern void Func_02002350(s32 arg0);

void Func_020012e0(void)
{
    Func_020056c0();

    if (*(u16 *)(Func_020056ee(0) + 6) > (128 << 7)
        && *(u16 *)(Func_020056fe(0) + 6) < (192 << 8)) {
        Func_02001efc();
    } else {
        Func_02001fe2();
    }

    if (Func_020056d0(0x898) != 0) {
        Func_0200247c();
    } else {
        Func_02002350(0);
    }

    Func_0200570c();
}
