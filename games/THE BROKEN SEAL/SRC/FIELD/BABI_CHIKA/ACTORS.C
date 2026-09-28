/* Moving and positioning actor zero. */
#include "BABI.H"

/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */
s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Actor_SetSpriteFlags(no, 0);
    return 0;
}

/*
 * Tries to move actor 0 onto the caller's target.  It builds a three-word
 * 12.20 probe from the record's own position -- the horizontal words snapped
 * to their whole-unit grid and lifted by half a unit -- asks the collision
 * service about it, and refuses when either the probe or the target is
 * rejected.  Returns 1 when refused and 0 when the move ran.  The 248-byte
 * owner includes its one pool word.
 */
s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Vector_AddPolarOffset(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    /* Both guards branch to one shared exit placed after the body.  Writing
     * `return 1` twice would put an inline copy near the top instead. */
    if (Object_CheckMovementCollision(actor, probe) == 1) {
        goto refuse;
    }
    if (Object_CheckMovementCollision(actor, target) != 0) {
        goto refuse;
    }

    Event_Begin();
    Object_SetAnimation(actor, 6);
    Task_Wait(6);
    Audio_PlayCue(152);
    Object_SetAnimation(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Object_SetAnimation(actor, 6);
    Actor_SetSpriteFlags(actor, 1);
    Task_Wait(6);

    actor->flags = saved;
    Event_End();
    return 0;

refuse:
    return 1;
}

void SceneActor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *p = Actor_Get(ACTOR_PARTY_LEADER);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    SceneActor_MoveActorZeroToTarget(pos);
}

void SceneActor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    SceneActor_MoveActorZeroToTarget(pos);
}
