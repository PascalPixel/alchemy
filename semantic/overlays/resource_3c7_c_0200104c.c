typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Resource 3c7 scene-entry fixup at 0x0200104c.
 *
 * Complete owner: the four-part prologue at 0x0200104c-0x02001058
 * (`push {r5, r6, r7, lr}`, then fp/sl/r9 through r7/r6/r5 and `push`, then r8
 * through r7 and `push`) and the matching four-part epilogue at
 * 0x020011ae-0x020011bc.  The final `pop {r1} / bx r1` puts the return address
 * in r1, not r0, so r0 survives and IS the result (HANDOVER §0): the owner
 * returns the `movs r0, #0` at 0x020011ac, i.e. always 0.
 *
 * r0's entry value is never read — every path writes r0 before using it — so
 * the owner takes no arguments.
 *
 * The bytes from 0x020011be to 0x020011db are the alignment halfword and this
 * owner's single literal pool, past the epilogue.  A control-flow walk from
 * the prologue reaches nothing else that is not an instruction; there is no
 * interior pool in this span.
 *
 * Link base 0x02008000, witnessed here: the pool word 0x02009314 is even and
 * so is in-image data at file offset 0x1314, and it is passed as the second
 * argument of Func_0808a098 — the same word and the same position as in
 * Func_02000308, which is an independent cross-check between two owners.
 *
 * `Data_03001ebc` is the overlay workspace POINTER, not the workspace: the
 * byte-exact sibling `assets/code/resource_3c7_c_0200048c.c` already reads it
 * as `u8 *state = Data_03001ebc`, so that layout is proven rather than
 * inferred here.  This owner stores the word 0x209 at workspace + 448.
 *
 * `Data_02000240` is the cross-overlay scene-state block; the signed halfwords
 * at indices 224-227 (byte offsets 448-454) are the progress ids this owner
 * tests and updates.
 *
 * Call accounting, per target, against `overlay_call_targets.ts`
 * (13 sites, 7 distinct), each checked at its own site:
 *   Func_0808a080  6    Func_0808a5d8  2    Func_080770c0  1
 *   Func_080770c8  1    Func_0808a098  1    Func_0808a100  1
 *   Func_02000508  1
 * Func_02000508 is the only in-overlay callee; the tool classifies it as a
 * real prologue, and it is the large owner at file offset 0x0508.
 *
 * Uncertainties, recorded rather than guessed:
 *  - the actor-record field offsets 35, 80 and 89 and the sub-record byte at
 *    +9 are transcribed from the loads and stores; their names are unknown.
 *    The +9 update is `(byte & ~0x0c) | 0x08`, which reads as a two-bit mode
 *    field being set to 2.
 *  - 0x9a7 and 0x300 are story-flag ids; only the truth value of
 *    Func_080770c0(0x300) is used, and Func_080770c8(0x9a7)'s result is
 *    discarded.
 *  - the scene ids 90, 0xb3, 0xb4, 12, 99 and 21 are literal.
 */

/* Imports.  Old-style declarations: arity is left open, and the two used in
 * conditions or for their results declare a return type. */
s32 Func_080770c0();
void Func_080770c8();
u8 *Func_0808a080();
void Func_0808a098();
void Func_0808a100();
void Func_0808a5d8();
void Func_02000508(void);

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

/* In-image data at file offset 0x1314 under the 0x02008000 link base.  The
 * overlay image is writable EWRAM, so this is not const. */
extern u8 Data_02009314[];

/* The three-store update applied to an actor record, spelled once because the
 * assembly performs it at four sites with only the record id differing.  The
 * sites at 0x020010e2 (id 19) and 0x0200113c (id 13) emit the +89 store before
 * the +35 store and the others emit it after; the two stores are to different
 * addresses, so the orderings are equivalent and one helper covers all four. */
static void ResetActor_0200104c(u8 *actor)
{
    u8 *sub;

    actor[35] = 0;
    actor[89] |= 4;
    sub = *(u8 **)(actor + 80);
    sub[9] = (u8)((sub[9] & ~0x0c) | 0x08);
}

s32 Func_0200104c(void)
{
    s16 progress;

    if (Data_02000240[225] == 90) {
        Func_080770c8(0x9a7);
    }

    *(u32 *)(Data_03001ebc + 448) = 0x209;

    progress = Data_02000240[224];
    if (progress == 0xb3) {
        ResetActor_0200104c(Func_0808a080(20));
        ResetActor_0200104c(Func_0808a080(18));
        ResetActor_0200104c(Func_0808a080(19));
        Func_0808a100(15, 6);

        if (Data_02000240[225] == 12) {
            Data_02000240[226] = progress;
            Data_02000240[227] = 12;
        }
    }

    if (Data_02000240[224] != 0xb4) {
        return 0;
    }

    ResetActor_0200104c(Func_0808a080(13));

    if (Func_080770c0(0x300) != 0) {
        Func_0808a098(14, Data_02009314);
    }

    if (Data_02000240[225] != 99) {
        return 0;
    }

    Func_02000508();
    Func_0808a5d8(Func_0808a080(12), 6);
    Func_0808a5d8(Func_0808a080(11), 6);
    Data_02000240[225] = 21;
    return 0;
}
