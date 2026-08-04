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

void Func_0808a018();
void Func_0808a170();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a180();
void Func_0808a368();
void Func_0808a370();
void Func_0808a360();
void Func_0808a020();
void Func_02002640();

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

    Func_0808a018();
    Func_0808a170(0x2073 + index * 3);
    Func_0808a178(index, 0);

    if (Func_0808a070(sceneId, 0) == 0) {
        Func_0808a170(0x2073 + index * 3 + 1);
        Func_0808a180(index, 0);
        *(s32 *)(workspace + 448) = 0x200;
        *(s32 *)(workspace + 456) = 15;
        Func_0808a368();
        Func_0808a370();
        Func_02002640(index);
        Func_0808a360();
        Func_0808a370();
    } else {
        Func_0808a170(0x2073 + index * 3 + 2);
        Func_0808a180(index, 0);
    }

    Func_0808a020();
}
