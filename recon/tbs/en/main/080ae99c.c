/* main:080ae99c, complete 84-byte owner through 080ae9f0, pools included.
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
 */
#include "TYPES.H"
#include "RENDER_INPUT.H"

struct ArrowResources {
    u8 unknown_000[0x392];
    u16 resource[2];
};

extern struct ArrowResources *Data_03001f2c;

struct MarkerObject {
    u8 pad00[4];
    u8 state;
    u8 active;
    u8 pad06[6];
    u16 timer;
};

struct MarkerObject *RenderOutput_CreateFar(s32 resource, s32 flags,
    struct RenderInput *window, s32 x, s32 y);

s32 Func_080ae99c(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct MarkerObject *object;
    u16 *resource;
    struct ArrowResources *data = Data_03001f2c;

    if (variant == 0)
        resource = &data->resource[0];
    else
        resource = &data->resource[1];
    object = RenderOutput_CreateFar(*resource, 0x40000000, window, x, y);
    if (object == 0)
        return -1;
    object->state = 0;
    object->timer = 0;
    object->active = 1;
    return 1;
}
