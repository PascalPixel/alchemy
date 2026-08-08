#include "types.h"

/*
 * Resource 37a cutscene beat at 0x020009f4 (524 bytes, 0x020009f4-0x02000bff).
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x020009f4 and the matching
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x02000bca.  r0
 * holds the popped return address, so nothing is returned: the owner is
 * `void`.  No stack frame beyond the pushed registers.
 *
 * Pool map, derived by walking the owner's control flow from the prologue: the
 * body is straight-line apart from four forward branches (0x02000a26,
 * 0x02000afe/0x02000b06 and 0x02000b6e/0x02000b7c), all of which stay inside
 * the code, so every halfword from 0x020009f4 to 0x02000bce is reached and the
 * 48 bytes at 0x02000bd0-0x02000bff are the literal pool:
 *   0x00000ff6 0x00026666 0x00004ccc 0x023f0000 0x00001010 0x00004010
 *   0x00000ffa 0x00000ffb 0x00000ffc 0x00000105 0x00000896 0x0000080a
 * Note that overlay_show.ts's "pool words referenced" listing additionally
 * names 0x02000f0c: the pool word 0x00004ccc itself decodes as
 * `ldr r4, [pc, #816]`.  That entry is an artefact of decoding a constant as an
 * instruction and is not part of this owner.
 *
 * All 49 call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are not the callees.
 * sites=49, distinct_targets=20 (44 veneer, 5 overlay-local prologue), against
 * the inventory's calls=47 (that field counts distinct targets and is a known
 * low lower bound).  The five prologue sites (0x02000a6e, 0x02000a98,
 * 0x02000aca, 0x02000b20, 0x02000b88) all reach the overlay-local two-argument
 * helper Func_020025fc; the remaining 44 reach the veneer band at
 * 0x020029bc-0x02002aa4, resolving to the main-image imports declared below.
 *
 * Func_0808a080 is the scene-record accessor (the sense established in
 * HANDOVER: 0x0808a080 is the accessor, not the four-argument action).  It is
 * called with 0 and its result guarded against null before the words at +8 and
 * +0x10 are read; those are the 16.16 coordinate words of the record, passed
 * straight into Func_0808a0f0.
 *
 * Func_0808a070 is a two-argument predicate whose result selects a cue at two
 * points: 0x0ffa / 0x0ffb after the first test, and (base + 1) / (base + 2)
 * after the second, where the base 0x0ffc is the cue emitted just above.  The
 * zero arm of the second test additionally sets story flag 0x0896 through
 * Func_080770c8 (the flag setter resolved in
 * semantic/overlays/resource_3cb_c_020010e8.c); the owner closes by setting
 * flag 0x080a.  Both tests are `cmp r0, #0 / bne`, so the zero arm is the
 * fall-through.
 *
 * r5 holds 0x4010 across the second half and r6 holds the cue base 0x0ffc; both
 * are reloaded from the pool rather than preserved across calls, and r5 is
 * reloaded with the same value at 0x02000ab8 and 0x02000b0e.
 *
 * Shift-built constants are spelled with their assembled values and the source
 * shift noted, because the assembly builds them from 8-bit immediates.
 *
 * Uncertainties: 0x00026666/0x00004ccc and 0x00040000/0x00008000 are the two
 * Func_0808a208 argument pairs and read as 16.16 rates, but their units are not
 * established; Func_0808a218 is reached with no argument register set by this
 * owner.  Arities are taken from the registers written before each site.
 */

/* Old-style declarations: arities vary per call site across this overlay. */
void Func_0200344e();
void Func_020033fc();
void Func_02003420();
void Func_02003440();
s32 *Func_0200340e();
void Func_0200344c();
void Func_020034ae();
void Func_0200343c();
void Func_02003460();
void Func_020034d2();
void Func_0200349c();
void Func_0200306c();
void Func_0200350c();
void Func_02003524();
void Func_02003530();
void Func_0200345e();
void Func_02003096();
void Func_02003548();
void Func_02003554();
void Func_02003482();
void Func_02003538();
void Func_020030c8();
void Func_0200354a();
void Func_0200351a();
void Func_0200355e();
void Func_0200354e();
s32 Func_020034de();
void Func_02003558();
void Func_02003560();
void Func_0200358e();
void Func_0200311e();
void Func_0200357e();
void Func_020035a8();
void Func_020035ba();
void Func_02003572();
void Func_020035c6();
void Func_020035be();
s32 Func_0200354e_b();
void Func_020035c8();
void Func_02003536();
void Func_020035d6();
void Func_02003186();
void Func_0200361e();
void Func_020035aa();
void Func_020035b6();
void Func_0200362a();
void Func_02003656();
void Func_02003584();

                     

                     

                     

                     

                     

                     

/* Used for their return values. */

void Func_020009f4(void)
{
    s32 *record;
    s32 actor;
    s32 cue_base;

    Func_0200344e(0xff6);
    Func_020033fc(0, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_02003420(0, 488, 176);                 /* 244 << 1 */
    Func_02003440(0, 0);

    record = Func_0200340e(0);
    if (record != 0)
        Func_0200344c(16, record[2], record[4]);    /* +0x08, +0x10 */

    Func_020034ae(0, 0, 1);
    Func_0200343c(16, 0x10000, 0x8000);         /* 128 << 9, 128 << 8 */
    Func_02003460(16, 472, 168);                /* 236 << 1 */
    Func_020034d2(16, 0, 60);
    Func_0200349c(16, 4, 40);
    Func_0200306c(16, 6);
    Func_0200350c(0x26666, 0x4ccc);
    Func_02003524(0x023f0000, -1, 0xb50000, 1); /* 181 << 16 */
    Func_02003530();
    Func_0200345e(120);
    Func_02003096(0x1010, 80);
    Func_02003548(0x1ec0000, -1, 0xa80000, 1);  /* 246 << 17, 168 << 16 */
    Func_02003554();
    Func_02003482(20);

    Func_02003538(16, 0x3000, 20);              /* 192 << 6 */
    Func_020030c8(actor, 6);
    actor = 0x4010;
    Func_0200354a(16, 0, 60);
    Func_0200351a(16, 2);
    Func_0200355e(16, 0x3000, 10);              /* 192 << 6 */
    Func_0200354e(actor, 0);

    if (Func_020034de(0, 0) == 0)
        Func_02003558(0xffa);
    else
        Func_02003560(0xffb);

    actor = 0x4010;
    Func_0200358e(0, 0xa000, 10);               /* 160 << 8 */
    Func_0200311e(actor, 10);

    cue_base = 0xffc;
    Func_0200357e(cue_base);
    Func_020035a8(16, 0, 40);
    Func_020035ba(16, 0x105, 40);
    Func_02003572(16, 4);
    Func_020035c6(16, 0x3000, 10);              /* 192 << 6 */
    Func_0200357e(16, 4);
    Func_020035be(actor, 0);

    if (Func_0200354e_b(0, 0) == 0) {
        Func_020035c8(cue_base + 1);
        Func_02003536(0x896);
    } else {
        Func_020035d6(cue_base + 2);
    }

    Func_02003186(actor, 4);
    Func_0200361e(16, 1);
    Func_020035aa(16, 486, 131);                /* 243 << 1 */
    Func_020035b6(16, 576, 120);                /* 144 << 2 */
    Func_0200362a(16, 0xc000, 2);               /* 192 << 8 */
    Func_02003656(0x40000, 0x8000);             /* 128 << 11, 128 << 8 */
    Func_02003584(0x80a);
}
