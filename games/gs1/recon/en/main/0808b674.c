#include "types.h"

/*
 * games/gs1/asm/0808b674.s: "複数領域に分割された呼出し可能関数の先頭部。
 * 後続するリテラルプールを同一ファイルに保持する。" (head portion of a
 * callable function split across multiple regions; the following literal
 * pool is kept in the same file.)
 *
 * The retained assembly pushes {r5, r6, r7, lr} plus the shuffled-in
 * {r8, r10} pair and never pops them: the function falls through its own
 * body and ends in a plain `b sub_0808b7c4` tail branch into a *different,
 * later* owner's emitted region, which is expected to perform the matching
 * pop/return. No ordinary single C function can express "push callee-saved
 * registers, then tail-branch into the middle of another translation
 * unit's code without an intervening call/return/longjmp" through this
 * project's approved GCC 2.96 route.
 *
 * candidate-show confirms this structurally: every body tried (empty stub,
 * this best-effort reconstruction, arithmetic filler of various sizes)
 * scores an identical differing_halfwords=162 against the 324-byte
 * reference and reports topology=uncovered:reference-multiple-owner-entries
 * / class=compiler-unemittable, regardless of candidate content or size.
 * See games/gs1/recon/en/dossiers.json#main:0808b674 for the recorded classification
 * proof and the full list of refuted shapes.
 *
 * The block below is a best-effort semantic reading of the retained
 * assembly (an object-table slot initializer that zeroes a header, copies
 * two 24-byte template blocks, clears the battle-slot table, and seeds a
 * few fields from a lookup table before falling into the continuation at
 * sub_0808b7c4). It is NOT adopted -- it exists only to record the closest
 * available reading of the algorithm for a future session, and its
 * function signature/body will never be compiler-matched while the tail
 * branch remains outside this file's owner boundary.
 */

extern u8 *Data_03001ebc;

void ObjectTable_ClearBattleSlots(void);

struct Fields_0808b674 {
    u8 header[20];
    void *objects[1];
};

void Func_0808b674(u32 arg0) {
    struct Fields_0808b674 *state = (struct Fields_0808b674 *)Data_03001ebc;
    s32 *header = (s32 *)state;
    s32 i;

    for (i = 3; i >= 0; i--) {
        header[i] = 0;
    }

    ObjectTable_ClearBattleSlots();
}
