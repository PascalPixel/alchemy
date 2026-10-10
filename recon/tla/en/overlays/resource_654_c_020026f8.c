/* alchemy permute resource_654_c_020026f8: weighted 20, differ 2 (4 register); draft weighted 80, differ 2 (4 register, 1 reordered).
   Job 0, iteration 310; rewrites: 1x introduce or reuse a temporary (minimized from 1: 1x introduce or reuse a temporary).
   Source hypothesis: review type and behavior changes, and tag compiler-steering devices under S2 before adoption. */
/* Pin-free Kabe_Jump caller trial: 64 native instructions, 64 built,
   63 common, differ 2. The actor constant precedes the height/frame setup.
   An input barrier differs 13; an output barrier, u16 temporaries and a
   reordered inline forwarding layer still differ 2. A volatile output
   barrier fixes the jump setup but reorders the preceding camera mode,
   also leaving differ 2. The linked helper keeps its bindings. */
#include "../../../../games/tla/src/field/overlay_654/kabe.h"

static inline void Kabe_JumpTrial(s32 actor, s32 height, s32 frames)
{
    s32 h = height;
    s32 f = frames;

    Func_02003aa8(actor, h, f);
}

void Func_020026f8(void)
{
    /* FAKEMATCH: the separate height temporary improves the weighted residual from 80 to 20, but still leaves differ 2. */
    s32 height;
    u8 off;

    if (GameFlag_Test(0x877)) {
        Func_020039d8();
        Func_02003b08(0);
        off = 0;
        Func_02003ad0()->motion_flags = off;
        Kabe_CamMove(320 << 16, -1, 300 << 16, 1);
        height = 0x100;
        Kabe_JumpTrial(14, height, 40);
        Kabe_ObjSpeed(14, 0x13333, 0x9999);
        Kabe_ObjWalk(14, 345, 296);
        Kabe_ObjWalk(14, 392, 296);
        Object_LinkObjectAndSetCallback(14, gPartyState.current_owner);
        Object_GetById(14)->state = off;
        WaitFrames(10);
        GameFlag_SetBit(0x87c);
        Func_020039e0();
    }
}
