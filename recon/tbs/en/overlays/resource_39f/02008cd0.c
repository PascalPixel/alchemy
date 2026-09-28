/* Draft of resource_39f 0x02008cd0 (SceneActor_TryRunSlotZeroMoveStep), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but its cue call reaches the veneer COMMON/OBJECT/STAGED_ACTOR.C names Audio_PlayCue, which FIELD_EVENT.H's inline of that name hides from this source; linking it would give the veneer a second name.
 * The listing keeps these rows. */
#include "MORI.H"

s32 Func_02003a40(s32 *p, s16 *q);

/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */
s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg)
{
    s32 *p = Actor_Get(ACTOR_PARTY_LEADER);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Func_02003a40(p, arg);

    if (r == 0) {
        s32 m;

        Event_Begin();
        Object_SetAnimation(p, 6);
        Task_Wait(6);
        Audio_PlayCue(152);
        Object_SetAnimation(p, 7);
        p[12] = 0x30000;
        p[13] = 0x20000;
        p[10] = 0x40000;
        m = 0x7e;
        m &= *f;
        *f = m;
        Actor_SetSpriteFlags(p, 0);
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, arg[1], arg[5]);
        Object_SetAnimation(p, 6);
        Actor_SetSpriteFlags(p, 1);
        *f = saved;
        Event_End();
        return 1;
    }
    return 0;
}
