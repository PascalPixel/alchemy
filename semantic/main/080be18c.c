#include "types.h"

/*
 * Rebuild the target list of a selection block from the caster's row window.
 *
 * The base register is saved and restored by the prologue but never written,
 * and is read exactly once, at 0x080be19e.  That is the frame-balanced
 * live-in already recorded for 0x080e7338 and resource_3cd:04b0 - a close
 * without its open - so it converts as an uninitialised local rather than a
 * skip.  Everything the owner touches hangs off it at a negative offset:
 * base-4 is the selection block, base-8 the single-target value, base-12 the
 * candidate list, base-16 the caster state and base-20 the flag block.  The
 * str at 0x080be1a0 parks the same value at [sp,#24], which no later
 * instruction reads, so it is a dead store and is not written.
 *
 * Kinds 0 and 4 are assembled as two identical arms at 0x080be1dc and
 * 0x080be1f4 - both store 0 to +16, 1 to +1 and +30, and the base-8 value to
 * +2 - so they are reconstructed as one case block.
 *
 * The window runs from centre-radius+1 to centre+radius-1 inclusive, where
 * the centre is the low nibble of the caster's halfword at +10.  Bit 7 of
 * that same halfword picks the far list at +102 over the near list at +88;
 * each list is scanned to its 255 terminator first so the window can be
 * clipped to it.  254 marks an empty cell.  When the caster's action is 5,
 * 56 or 57 the per-target liveness check at +56 is skipped.
 *
 * The epilogue at 0x080be35c ends pop {r1}; bx r1 with N != 0, so r0 is the
 * result, but only the empty-list path at 0x080be358 sets it, to -1; the
 * kind-0/kind-4 arms and the non-empty path reach the epilogue with r0
 * holding whatever the last computation left there.  That is written as an
 * uninitialised result local, assigned only on the path that sets it.
 *
 * Six direct callers: 0x080be77c, 0x080be7e6, 0x080be90e, 0x080be992,
 * 0x080bec96 and 0x080bed62.
 */

struct Action_080be18c {
    u8 kind;
    u8 unknown_01[2];
    u8 action;
};

struct Selection_080be18c {
    u8 unknown_00;
    u8 count;
    u8 ids[14];
    u8 offsets[14];
    u8 flags[14];
};

struct State_080be18c {
    s16 identifier;
    u8 unknown_02[8];
    u16 window;
    s16 radius;
};

struct List_080be18c {
    u8 unknown_00[88];
    s16 near[7];
    s16 far[15];
};

struct Target_080be18c {
    u8 unknown_00[56];
    s16 alive;
};

struct Action_080be18c *Func_08077080(s32 actor);
struct Target_080be18c *Func_08077008(s32 id);
void Func_08015120(s32 identifier, s32 mode);
void Func_080151c8(s32 message);

s32 Func_080be18c(s32 actor)
{
    u8 *base;
    struct Action_080be18c *action;
    struct Selection_080be18c *selection;
    struct State_080be18c *state;
    struct List_080be18c *list;
    s32 result;
    s32 unchecked;
    s32 near_count;
    s32 far_count;
    s32 centre;
    s32 radius;
    s32 first;
    s32 last;
    s32 index;
    s32 count;

    action = Func_08077080(actor);
    unchecked = 0;
    if (action->action == 5 || action->action == 56 || action->action == 57)
        unchecked = 1;

    if (action->kind == 0 || action->kind == 4) {
        selection = *(struct Selection_080be18c **)(base - 4);
        selection->offsets[0] = 0;
        selection->count = 1;
        selection->flags[0] = 1;
        selection->ids[0] = (u8)*(u32 *)(base - 8);
        return result;
    }

    count = 0;

    near_count = 0;
    list = *(struct List_080be18c **)(base - 12);
    while (list->near[near_count] != 255)
        near_count++;

    far_count = 0;
    if (list->far[0] != 255) {
        while (list->far[far_count + 1] != 255)
            far_count++;
        far_count++;
    }

    state = *(struct State_080be18c **)(base - 16);
    centre = state->window & 15;
    radius = state->radius;
    first = centre - radius + 1;
    last = centre + radius - 1;

    for (index = first; index <= last; index++) {
        s32 id;

        if (index < 0)
            continue;

        state = *(struct State_080be18c **)(base - 16);
        if ((state->window & 128) != 0) {
            if (index >= far_count)
                continue;
            list = *(struct List_080be18c **)(base - 12);
            id = list->far[index];
        } else {
            if (index >= near_count)
                continue;
            list = *(struct List_080be18c **)(base - 12);
            id = list->near[index];
        }

        if (id == 254)
            continue;
        if (unchecked == 0 && Func_08077008(id)->alive == 0)
            continue;

        selection = *(struct Selection_080be18c **)(base - 4);
        selection->flags[count] = 1;
        selection->offsets[count] = (u8)(index - centre);
        selection->ids[count] = (u8)id;
        count++;
    }

    selection = *(struct Selection_080be18c **)(base - 4);
    selection->count = (u8)count;
    if (count > 0)
        return result;

    state = *(struct State_080be18c **)(base - 16);
    Func_08015120(state->identifier, 1);
    Func_080151c8(0x816);
    {
        s8 *flag = (s8 *)(*(u8 **)(base - 20) + 0x12b);

        if (*flag == 0)
            *flag = 1;
    }
    result = -1;
    return result;
}
