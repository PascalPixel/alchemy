/* Mogall Forest: when the leader can step onto the next slot, play the hop
 * and move the leader there. The forest links the staged-actor code, so its
 * scenes reach the engine by the names that code gives the imports. */
#define FIELD_STAGED_ACTOR_IMPORTS
#include "MORI.H"

/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */
s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg)
{
    s32 *p = Actor_Get(ACTOR_PARTY_LEADER);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Object_CheckMovementCollision(p, arg);

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
