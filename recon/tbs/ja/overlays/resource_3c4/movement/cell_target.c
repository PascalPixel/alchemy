/* NONMATCHING: Japanese cell-centered Babi tunnel movement, 2026-10-01.
 * The complete movement owner compiles with approved TBS flags but rounds
 * the target to cell centers, adding eight bytes. The Japanese game takes
 * the signed pixel halves directly; the other five editions center cells.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/BABI.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct CellTarget {
    s32 x;
    s32 height;
    s32 z;
};

s32 SceneActor_MoveActorZeroToTarget(const struct CellTarget *target)
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

    Engine_EventBegin();
    Object_SetMode(actor, 6);
    WaitFrames(6);
    Audio_PlayCue(152);
    Object_SetMode(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Engine_ActorSetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    WaitFrames(6);

    actor->flags = saved;
    Engine_EventEnd();
    return 0;

refuse:
    return 1;
}
