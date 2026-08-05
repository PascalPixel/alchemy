#include "types.h"

/*
 * Resource 3bc, owner at 0x02000ba4.  184 strict bytes: 170 bytes of code
 * (0x0ba4-0x0c4d), a 2-byte alignment `nop`, and a three-word literal pool at
 * 0x0c50-0x0c5b.
 *
 * Takes one argument in r0 (kept in r6 across the whole body): a small index
 * that selects a group of three consecutive cue ids, 0x2073 + 3*index,
 * +1 and +2.  The base 0x2073 is a plain pooled constant, not an address —
 * under the 0x02008000 link base it would be far below the image.
 *
 * All 14 call sites resolved with tools/overlay_call_targets.ts, matching the
 * row's calls=14 exactly:
 *   0808a018 x1, 0808a170 x3, 0808a178 x1, 0808a070 x1, 0808a180 x2,
 *   0808a368 x1, 0808a370 x2, 0808a360 x1, 0808a020 x1, and one internal
 *   prologue call to Func_02002640 (verified: `push {r5, lr}` at 0x2640).
 * Each of the repeated targets sits on a distinct arm — there is no shared
 * call site here, so the per-arm C calls do not inflate the multiset.
 *
 * sl holds the scene workspace (*0x03001ebc); r8 holds the scene id
 * `*(s32 *)&Data_02000240[250]`, the same fetch the byte-exact sibling
 * assets/code/resource_3bc_c_02000658.c uses.  The gate reads the *signed
 * halfword* at &Data_02000240[225] (byte offset 450) and runs the body only
 * when it equals 2.
 *
 * Workspace stores: `movs r2,#224 / lsls #1` = +448 and `movs r2,#228 /
 * lsls #1` = +456; the values are 0x80 << 2 = 0x200 and 15.
 *
 * Epilogue `pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop {r5,r6,r7} /
 * pop {r0} / bx r0` — r0 is the popped return address, so the owner is void.
 *
 * Uncertainty: Func_0808a070's second argument is a literal 0 at this site
 * only; its arity elsewhere in the overlay is not pinned by this owner.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;   /* pointer cell holding the scene workspace */

/* Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including the repeated Func_0808a170,
 * Func_0808a180 and Func_0808a370 calls) -- declared/named as the literal
 * per-site targets, not the shared ultimate-destination symbol. */
void Func_020055ce();          /* Func_0808a018 veneer */
void Func_0200568a();          /* Func_0808a170 veneer #1 */
void Func_0200569a();          /* Func_0808a178 veneer */
s32 Func_020055fa();           /* Func_0808a070 veneer */
void Func_020056a6();          /* Func_0808a170 veneer #2 */
void Func_020056be();          /* Func_0808a180 veneer #1 */
void Func_02005770();          /* Func_0808a368 veneer */
void Func_0200577c();          /* Func_0808a370 veneer #1 */
void Func_02003262();          /* Func_02002640 veneer */
void Func_02005776();          /* Func_0808a360 veneer */
void Func_0200578a();          /* Func_0808a370 veneer #2 */
void Func_020056e4();          /* Func_0808a170 veneer #3 */
void Func_020056fc();          /* Func_0808a180 veneer #2 */
void Func_02005648();          /* Func_0808a020 veneer */

void Func_02000ba4(s32 index)
{
    u8 *workspace;
    s16 *table;
    s32 sceneId;

    workspace = Data_03001ebc;
    table = Data_02000240;
    sceneId = *(s32 *)&table[250];

    if (table[225] != 2) {
        return;
    }

    Func_020055ce();
    Func_0200568a(0x2073 + index * 3);
    Func_0200569a(index, 0);

    if (Func_020055fa(sceneId, 0) == 0) {
        Func_020056a6(0x2073 + index * 3 + 1);
        Func_020056be(index, 0);
        *(s32 *)(workspace + 448) = 0x200;
        *(s32 *)(workspace + 456) = 15;
        Func_02005770();
        Func_0200577c();
        Func_02003262(index);
        Func_02005776();
        Func_0200578a();
    } else {
        Func_020056e4(0x2073 + index * 3 + 2);
        Func_020056fc(index, 0);
    }

    Func_02005648();
}
