/* alchemy permute resource_654_c_020025fc: weighted 0, differ 0; draft weighted 30, differ 2 (6 register).
   Job 1, iteration 21; rewrites: 2x introduce or reuse a temporary, 1x wrap statements in a block (minimized from 3: 2x introduce or reuse a temporary, 1x wrap statements in a block).
   Source hypothesis: review type and behavior changes, and tag compiler-steering devices under S2 before adoption. */
/* This candidate is instruction-identical in tla-en; it is not linked.
   The shared helper and every caller still need twelve-edition validation.
   Reusing only the old flag result for the angle closes the difference;
   the search's later zero-coordinate reuse and trailing empty block proved
   unnecessary on direct comparison and were removed.
   Earlier plain trial: 98 native instructions, 98 built,
   97 common, differ 2. The angle's initial immediate moves three rows late.
   A one-pass angle assignment differs 6; an x-derived cancelled byte differs
   13, adding a zero add and changing allocation earlier in the caller.
   The linked helper keeps its binding until every caller stays identical. */
#include "../../../../games/tla/src/field/overlay_654/kabe.h"

static inline void Kabe_PlaceTurnTrial(s32 actor, s32 x, s32 z, s32 angle)
{
    s32 g = angle;

    Func_02003a38(actor, x, z, g);
}

void Func_020025fc(void)
{
    s32 done = Kabe_FlagChk(0x876);

    if (done == 0) {
        Func_020039d8();
        Func_02003b08(0);
        Func_02003aa8(gPartyState.current_owner, 0x100, 0);
        Func_02003ad0()->motion_flags = done;
        Kabe_CamMove(328 << 16, 48 << 16, 552 << 16, 1);
        Func_02003ac8();
        /* FAKEMATCH: reusing the old flag result for the angle retains the native early constant construction; the plain unbound angle differs 2. */
        done = 0x4000;
        Kabe_PlaceTurnTrial(14, 424 << 16, 520 << 16, done);
        WaitFrames(1);
        Object_LinkObjectAndSetCallback(gPartyState.current_owner, 14);
        ObjectMotion_SetActionVariant(14, 1);
        Kabe_ObjSpeed(14, 0x19999, 0xcccc);
        Kabe_ObjWalk(14, 380, 552);
        Kabe_ObjWalk(14, 264, 552);
        Kabe_ObjWalk(14, 264, 540);
        ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
        WaitFrames(1);
        Kabe_ObjPlace(14, 0, 0);
        ObjectMotion_SetActionVariant(14, 2);
        Object_GetById(14)->priority_flags |= 1;
        WaitFrames(1);
        Kabe_FlagSet(0x876);
        Func_020039e0();
    }
}
