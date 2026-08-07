#include "types.h"

/*
 * resource_39e owner at 0x02002ad0, 5000 bytes: the overlay's LARGEST
 * row and the last one it had left -- 525 calls in the familiar
 * Func_0808a0xx/a1xx staging vocabulary, the same ensemble-beat family
 * as the drafted 0x02001dbc (1704B) and 0x02001494 (2236B), just very
 * much longer. Opens Func_0808a018(), dialogue 0x1969, and the
 * four-slot (0/1/2/3) party staging block; runs long positioning,
 * facing and pose passes over slots 0-3, 8, 9, 10 and 17; calls the
 * one in-image callee Func_020041ec once at 0x02003a54; and closes
 * with Func_0808a020() and Func_080770c8(0x895) -- setting the story
 * flag 0x895 that the drafted 0x02002484 TESTS (its pool word 0x24fc
 * is 0x895), alongside 0x89a set by 0x02001494. The overlay's flag
 * chain therefore closes as: 1494 sets 0x89a + (0x898|0x899); 1d50
 * sets 0x898 on its non-skip arm only; 12e0 tests 0x898 to choose
 * 0x1160 vs 0x102c; 1dbc clears 0x898 and sets 0x89b; THIS row sets
 * 0x895; 2484 tests 0x89a/0x895; and 0x02000afc's three-way exit line
 * reports 0x895 vs 0x89b vs neither.
 *
 * Despite the size this row is TRANSCRIPTION, not analysis: the
 * prologue is a bare `push {lr}` -- no callee-saved registers and no
 * sp frame at all -- so nothing is live across a call and every
 * argument is rebuilt at its own site. All 28 distinct call targets
 * are vocabulary already declared elsewhere in this overlay; the row
 * introduces no new callee.
 *
 * Structure, complete: eight branches in 5000 bytes and no loops.
 *   - 0x02002b90 / 0x02002ba4 / 0x02002bb8: three consecutive null
 *     guards, `record = Func_0808a080(0); if (record != 0)
 *     Func_0808a0f0(N, *(s32*)(record+8), *(s32*)(record+16));` for
 *     N = 1, 2, 3. Func_0808a080(0) is re-fetched for each of the
 *     three -- it is NOT cached across them.
 *   - 0x02002ec4, 0x020034ca, 0x020038d4: forward `b.n` over inline
 *     literal pools (the inline-pool idiom of resource_39c/3bb); data,
 *     not control flow.
 *   - 0x02003cf6 `bne.n 0x02003d28` with its join branch at 0x02003d26
 *     `b.n 0x02003d56`: the row's ONE skip-beat gate, on
 *     Func_0808a070(0, 0). VERIFIED by reading both arms in this reconstruction
 *     (the carried-over description in the parked note was flagged
 *     UNVERIFIED and is now confirmed, with one point sharpened):
 *     BOTH arms bump the workspace u16 at Data_03001ebc + 472, and
 *     neither arm touches a story flag. The arms differ only in the
 *     slot they pose -- 1 when Func_0808a070 returns 0, 3 otherwise --
 *     and in where the bump sits (last in the ==0 arm, first in the
 *     other). This is the SYMMETRIC shape, matching the resource_3c9
 *     copies, and specifically NOT the shape of 0x02001d50 in this
 *     same overlay, where only one arm bumps and the other sets flag
 *     0x898. Branch sense stays untransferable between rows.
 *
 * Two bit operations on the record at 0x02003a1c and 0x02003a40 clear
 * and then set bit 0 of the byte at record+90, each around its own
 * fresh Func_0808a080(0). Role not identified.
 *
 * Complete owner: `push {lr}` at 0x02002ad0 through `pop {r0} / bx r0`
 * at 0x02003e3c-0x02003e3e, plus the three inline pools above and six
 * trailing pool words ending 0x02003e57; the next owner (`push {lr}`)
 * begins at 0x02003e58 (drafted, resource_39e_c_02003e58.c).
 * 0x02003e58 - 0x02002ad0 = 0x1388 = 5000 bytes exactly, computed and
 * not hand-translated.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/lib/overlay_call_targets.ts resource_39e 2ad0 3e58,
 * the +2 rule). Drafting it takes resource_39e's residue to zero.
 *
 * Method note, because the size made it necessary: the argument text
 * below was produced by an independent mechanical read of the bounded
 * listing, then checked against a hand transcription of the first 104
 * calls -- 104 for 104, exact, including every shift form. The
 * extractor throws rather than skips on any register it cannot
 * resolve, and its output count is asserted against the tool's own
 * sites=525. All eight branches and all four real stores were
 * enumerated separately by grep, so the non-call structure is covered
 * by construction rather than by eye. Note the three inline pools must
 * be stepped OVER: disassembled as instructions their words write
 * r0-r3 (0x0101 at 0x2002ed4 reads as `lsls r1, r0, #4`), which is a
 * live trap for any register-tracking reader of a row like this.
 *
 * Uncertainty: callee and argument roles are by shape; slot ids and
 * constants are transcribed, not interpreted. The a1e8 second
 * arguments (0x100, 0x101, 0x102, 0x103, 0x105) and the a090 pairs
 * (0x6666/0x3333 and 0xcccc/0x6666) are taken from the literal pools
 * verbatim. Data_03001ebc is the standard workspace pointer cited
 * across this overlay. Called 1x within this overlay.
 */

extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern void Func_0808a078(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 slot);
extern void Func_0808a0f0(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 slot, s32 arg1);
extern void Func_0808a110(s32 slot, s32 arg1);
extern void Func_0808a130(s32 slot, s32 arg1);
extern void Func_0808a138(s32 slot, s32 arg1);
extern void Func_0808a148(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a150(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a178(s32 slot, s32 arg1);
extern void Func_0808a188(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a190(s32 slot, s32 arg1);
extern void Func_0808a1b8(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 slot, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 slot, s32 arg1);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_020041ec(void);

extern u8 *Data_03001ebc;

void Func_02002ad0(void)
{
    u8 *record;
    u8 *workspace;

    Func_0808a018();
    Func_0808a090(0, 0x6666, 0x3333);
    Func_0808a0c8(0, 236 << 1, 134 << 2);
    Func_0808a360();
    Func_0808a370();
    Func_0808a0e8(0);
    Func_0808a1b8(9, 0, 20);
    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_0808a170(0x1969);
    Func_0808a188(9, 0, 20);
    Func_0808a1b8(0, 128 << 8, 20);
    Func_0808a110(0, 3);
    Func_0808a010(30);
    Func_0808a1b8(8, 128 << 7, 30);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1b8(0, 192 << 8, 20);
    Func_0808a138(0, 1);
    Func_0808a010(20);
    Func_0808a1b8(9, 208 << 8, 0);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    Func_0808a090(0, 128 << 8, 128 << 7);
    Func_0808a090(1, 128 << 8, 128 << 7);
    Func_0808a090(2, 128 << 8, 128 << 7);
    Func_0808a090(3, 128 << 8, 128 << 7);
    Func_0808a0c8(0, 232 << 1, 252 << 1);
    Func_0808a0c8(2, 240 << 1, 252 << 1);
    Func_0808a0c8(1, 248 << 1, 248 << 1);
    Func_0808a0c8(3, 224 << 1, 248 << 1);
    Func_0808a0e8(0);
    Func_0808a0e8(2);
    Func_0808a0e8(3);
    Func_0808a1b8(3, 224 << 8, 0);
    Func_0808a0e8(1);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a010(20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a1b8(1, 160 << 7, 20);
    Func_0808a190(1, 0);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1b8(1, 160 << 8, 20);
    Func_0808a1e8(3, 128 << 1, 60);
    Func_0808a188(3, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(2, 128 << 1, 60);
    Func_0808a188(2, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a1b8(8, 192 << 6, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(3, 128 << 6, 0);
    Func_0808a130(2, 2);
    Func_0808a1f0(2, 129 << 1);
    Func_0808a010(60);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a150(0, 2, 50);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 30);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a188(3, 0, 20);
    Func_0808a1b8(8, 160 << 7, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a138(3, 1);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1b8(0, 224 << 8, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a010(30);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 20);
    Func_0808a188(1, 0, 20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a148(8, 2, 0);
    Func_0808a010(10);
    Func_0808a188(8, 0, 20);
    Func_0808a148(0, 2, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a148(3, 2, 0);
    Func_0808a1e8(2, 129 << 1, 60);
    Func_0808a188(2, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a130(1, 2);
    Func_0808a010(20);
    Func_0808a188(1, 0, 20);
    Func_0808a1e8(8, 128 << 1, 60);
    Func_0808a148(8, 1, 0);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(2, 128 << 1, 60);
    Func_0808a188(2, 0, 20);
    Func_0808a148(8, 1, 0);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a130(2, 2);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a138(3, 2);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);
    Func_0808a1b8(8, 160 << 7, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(2, 2);
    Func_0808a138(3, 2);
    Func_0808a010(20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(1, 0x103, 0);
    Func_0808a130(1, 2);
    Func_0808a010(60);
    Func_0808a188(1, 0, 20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a188(1, 0, 20);
    Func_0808a1e8(8, 129 << 1, 0);
    Func_0808a130(8, 1);
    Func_0808a010(60);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a130(2, 1);
    Func_0808a138(3, 1);
    Func_0808a010(20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a1b8(1, 160 << 7, 20);
    Func_0808a190(1, 0);
    Func_0808a010(20);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a1b8(1, 160 << 8, 20);
    Func_0808a1b8(8, 192 << 8, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a1b8(8, 128 << 7, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(2, 2);
    Func_0808a138(3, 2);
    Func_0808a010(20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a188(2, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a1f0(0, 129 << 1);
    Func_0808a1f0(1, 129 << 1);
    Func_0808a1f0(2, 129 << 1);
    Func_0808a1f0(3, 129 << 1);
    Func_0808a010(60);
    Func_0808a188(3, 0, 20);
    Func_0808a1b8(8, 160 << 7, 20);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a138(1, 1);
    Func_0808a010(20);
    Func_0808a188(1, 0, 20);
    Func_0808a148(0, 1, 0);
    Func_0808a148(2, 1, 0);
    Func_0808a148(3, 1, 0);
    Func_0808a010(20);
    Func_0808a100(0, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(60);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 208 << 8, 0);
    Func_0808a010(20);
    Func_0808a190(8, 0);
    Func_0808a1e8(0, 129 << 1, 0);
    Func_0808a1e8(1, 129 << 1, 0);
    Func_0808a1e8(2, 129 << 1, 0);
    Func_0808a1e8(3, 129 << 1, 0);
    Func_0808a010(60);
    Func_0808a1b8(1, 192 << 7, 20);
    Func_0808a190(1, 0);
    Func_0808a010(20);
    Func_0808a1b8(3, 128 << 6, 20);
    Func_0808a188(3, 0, 20);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 30);
    Func_0808a1b8(8, 0, 20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a188(2, 0, 20);
    Func_0808a1b8(8, 192 << 8, 20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a138(8, 1);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 6, 0);
    Func_0808a010(60);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);
    Func_0808a010(30);
    Func_0808a1b8(8, 128 << 7, 20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a0f0(10, 236 << 17, 152 << 18);
    Func_0808a188(10, 0, 20);
    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 0);
    Func_0808a1e8(8, 128 << 1, 0);
    Func_0808a1e8(9, 128 << 1, 0);
    Func_0808a010(60);
    Func_0808a1b8(0, 128 << 7, 0);
    Func_0808a1b8(1, 128 << 7, 0);
    Func_0808a1b8(2, 128 << 7, 0);
    Func_0808a1b8(3, 128 << 7, 0);
    Func_0808a1b8(9, 0, 0);
    Func_0808a010(30);
    Func_0808a090(10, 0xcccc, 0x6666);
    Func_0808a0d0(10, 236 << 1, 134 << 2);
    Func_0808a1e8(8, 0x101, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a1b8(2, 192 << 8, 40);
    Func_0808a0d0(2, 244 << 1, 128 << 2);
    Func_0808a010(10);
    Func_0808a1b8(2, 192 << 7, 20);
    Func_0808a1b8(0, 0, 30);
    Func_0808a0d0(0, 228 << 1, 128 << 2);
    Func_0808a010(10);
    Func_0808a1b8(0, 128 << 6, 20);
    Func_0808a138(10, 2);
    Func_0808a010(20);
    Func_0808a188(10, 0, 20);
    Func_0808a1e8(8, 0x101, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a130(10, 2);
    Func_0808a1f0(10, 129 << 1);
    Func_0808a010(60);
    Func_0808a188(10, 0, 20);
    Func_0808a1e8(8, 0x101, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(10, 128 << 1, 60);
    Func_0808a188(10, 0, 20);
    Func_0808a130(8, 1);
    Func_0808a1e8(8, 128 << 1, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a1b8(0, 224 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);
    Func_0808a010(40);
    Func_0808a110(10, 3);
    Func_0808a010(20);
    Func_0808a188(10, 0, 20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(8, 0x101, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a130(17, 1);
    Func_0808a010(20);
    Func_0808a188(10, 0, 20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(8, 128 << 1, 30);
    Func_0808a188(8, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a010(2);
    Func_0808a100(2, 3);
    Func_0808a010(1);
    Func_0808a100(3, 3);
    Func_0808a010(5);
    Func_0808a110(1, 3);
    Func_0808a148(8, 0, 0);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a090(10, 0xcccc, 0x6666);
    Func_0808a0d0(10, 236 << 1, 252 << 1);
    Func_0808a010(20);
    Func_0808a130(10, 1);
    Func_0808a010(20);
    Func_0808a188(10, 0, 20);
    Func_0808a148(8, 10, 0);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a1b8(0, 128 << 6, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 128 << 6, 0);
    Func_0808a0e8(8);
    Func_0808a010(30);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a0c8(10, 236 << 1, 142 << 2);
    Func_0808a0d0(8, 236 << 1, 134 << 2);
    Func_0808a138(1, 2);
    Func_0808a0e8(10);
    Func_0808a188(1, 0, 20);
    Func_0808a130(8, 2);
    Func_0808a1e8(8, 129 << 1, 60);
    Func_0808a1b8(10, 208 << 8, 0);
    Func_0808a1b8(8, 208 << 8, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(2, 2);
    Func_0808a138(3, 2);
    Func_0808a0d0(8, 236 << 1, 128 << 2);
    Func_0808a1b8(8, 0, 0);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(9, 208 << 8, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    record = Func_0808a080(0);
    *(u8 *)(record + 90) &= ~1;
    Func_0808a0d0(0, 224 << 1, 128 << 2);
    Func_0808a010(1);
    record = Func_0808a080(0);
    Func_0808a010(20);
    *(u8 *)(record + 90) |= 1;
    Func_020041ec();
    Func_0808a010(60);
    Func_0808a078(2, 144);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a1b8(0, 128 << 6, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 128 << 6, 0);
    Func_0808a1b8(9, 192 << 6, 0);
    Func_0808a010(20);
    Func_0808a0d0(8, 236 << 1, 138 << 2);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a1b8(8, 208 << 8, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a090(8, 128 << 8, 128 << 7);
    Func_0808a0d0(8, 240 << 1, 135 << 2);
    Func_0808a188(8, 0, 20);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a090(9, 0xcccc, 0x6666);
    Func_0808a0c8(8, 236 << 1, 152 << 2);
    Func_0808a0c8(9, 236 << 1, 136 << 2);
    Func_0808a0d0(10, 236 << 1, 152 << 2);
    Func_0808a0e8(9);
    Func_0808a0c8(9, 236 << 1, 152 << 2);
    Func_0808a0f0(10, 0, 0);
    Func_0808a0e8(8);
    Func_0808a0f0(8, 0, 0);
    Func_0808a0e8(9);
    Func_0808a0f0(9, 0, 0);
    Func_0808a0d0(2, 244 << 1, 130 << 2);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a138(3, 1);
    Func_0808a010(20);
    Func_0808a1b8(3, 128 << 6, 20);
    Func_0808a188(3, 0, 20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(2, 160 << 8, 20);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a1e8(3, 0x105, 60);
    Func_0808a138(1, 1);
    Func_0808a1b8(1, 192 << 7, 20);
    Func_0808a190(1, 0);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a138(3, 2);
    Func_0808a010(20);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a188(3, 0, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a110(3, 4);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);
    Func_0808a1e8(0, 129 << 1, 0);
    Func_0808a1e8(1, 129 << 1, 60);
    Func_0808a178(3, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a110(1, 3);
        Func_0808a010(20);
        Func_0808a188(1, 0, 20);
        *(u16 *)(workspace + 472) += 1;
    } else {
        workspace = Data_03001ebc;
        Func_0808a010(20);
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Func_0808a110(3, 3);
        Func_0808a010(20);
        Func_0808a188(3, 0, 20);
    }

    Func_0808a138(1, 1);
    Func_0808a188(1, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a0c8(1, 224 << 1, 128 << 2);
    Func_0808a0d0(3, 224 << 1, 128 << 2);
    Func_0808a0f0(3, 0, 0);
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a010(20);
    Func_0808a148(0, 2, 20);
    Func_0808a010(30);
    Func_0808a148(2, 0, 20);
    Func_0808a188(2, 0, 20);
    Func_0808a1e8(0, 129 << 1, 60);
    Func_0808a130(2, 2);
    Func_0808a1f0(2, 129 << 1);
    Func_0808a010(60);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a0d0(2, 224 << 1, 128 << 2);
    Func_0808a0f0(2, 0, 0);
    Func_0808a020();
    Func_080770c8(0x895);
}
