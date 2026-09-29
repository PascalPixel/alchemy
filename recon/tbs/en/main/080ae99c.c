/* main:080ae99c, complete 84-byte owner through 080ae9f0, pools included.
 * Current (2026-09-29): the baseline's u32 resource again, with the build's
 * names (UiIcon_CreateStatChangeArrow, gMenuWork): 84/84 bytes, alchemy
 * permute score 100 (7 register-only, 1 moved), against 640 for H3 below.
 * The reference's shape needs variant to be a global pseudo: local-alloc
 * ties a block-local variant to its incoming r3 (copy suggestion first),
 * so the menu-cell address takes r2 and evicts y to r4. The reference
 * evicts variant to r5 and keeps the address and data in r3. A ternary,
 * a switch, an offset local, a pointer bump, per-arm loads and a (u16)
 * argument cast do not make variant live past block 0. Ten minutes of
 * permutation (42,708 candidates): none below 100.
 * Own-ROM caller UiText_DrawStatComparison passes (window, x, y, variant).
 * RenderOutput_CreateFar takes (resource, flags, window, x, y), as its exact
 * implementation and the exact shop drawing family establish.
 * Baseline: 84/84 bytes, 10 differing halfwords, 9 aligned edits. The compiler
 * already merges the resource load; its first divergence saves y in r4 while
 * the ROM saves variant in r5 and retains y in r2 until its outgoing store.
 * H1: select a typed resource pointer, then load it once. Use a RenderInput
 * pointer for the window; preserve the output's byte/halfword store view.
 * Prediction: one shared resource load and the ROM argument lifetimes/frame.
 * Gate: complete exact bytes, compare-all, test, coverage and verify.
 * Budget: this model plus at most two evidence-backed structural follow-ups;
 * preserve the full diff/result here before moving, never register sweeps.
 * H1 result: 84/84 bytes, 18 differing halfwords, 17 aligned edits. Pointer
 * selection hoists the first address and removes the ROM's branch-to-join;
 * y still moves to r4. Rejected: the full argument/frame invariant did not
 * improve, and the previously correct selection topology regressed.
 * H2: reuse variant as the selected byte offset, rather than introducing a
 * pointer live across the arms. H1 local-allocation diagnostics bind variant
 * to r3 within block zero, forcing the global-cell address into r2 and y
 * into a saved register. A selector reused across the join should instead
 * expose the ROM's longer selector lifetime while retaining the shared add.
 * H2 result: 80/84 bytes, 37 differing halfwords, 13 aligned edits. It does
 * recover the named-cell base in r3, y in r2 and the exact outgoing-store
 * position. The selector coalesces with its offset in r1, however, and the
 * indexed halfword load replaces the ROM's distinct add/load; r6 disappears.
 * This proves the input selector's lifetime causes the earlier y evacuation,
 * but offset reuse is not the original source boundary.
 * H3: retain the resource ID in a u16 local, matching its two storage fields,
 * rather than carrying a selected pointer/offset or a widened u32 ID. This
 * tests a halfword value boundary before the signed-int renderer argument;
 * prediction: distinct selector and loaded-ID lifetimes with one joined load.
 * This is the final model; stop on any nonexact result.
 * H3 result: 88/84 bytes, 37 differing halfwords, 15 aligned edits. The u16
 * local becomes ldrsh plus explicit zero-extension shifts rather than the
 * ROM's ldrh, and y still evacuates r2 at entry. Branch directions agree;
 * resource conversion, argument carriers and pool locations do not.
 * STOP: one pointer model plus two structural follow-ups exhausted. No
 * adoption/registration or DONE gain. H2's recovered y/base/store invariant
 * is evidence for a future source-boundary audit, not permission to sweep
 * declarations or repeat these pointer/offset/halfword-local axes.
 */
#include "TYPES.H"
#include "RENDER_INPUT.H"

struct ArrowResources {
    u8 unknown_000[0x392];
    u16 resource[2];
};

extern struct ArrowResources *gMenuWork;

struct MarkerObject {
    u8 pad00[4];
    u8 state;
    u8 active;
    u8 pad06[6];
    u16 timer;
};

struct MarkerObject *RenderOutput_CreateFar(s32 resource, s32 flags,
    struct RenderInput *window, s32 x, s32 y);

s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct MarkerObject *object;
    u32 resource;
    struct ArrowResources *data = gMenuWork;

    if (variant == 0)
        resource = data->resource[0];
    else
        resource = data->resource[1];
    object = RenderOutput_CreateFar(resource, 0x40000000, window, x, y);
    if (object == 0)
        return -1;
    object->state = 0;
    object->timer = 0;
    object->active = 1;
    return 1;
}
