#include "TASK.H"

/* The game state's cells, read here as halfwords. */
extern s16 gCell[];

/* The placed actor whose cell holds a position.  Callers also pass the
 * record asking, which the search does not need. */
s32 *SceneData_FindSlotAtPosition(s32 *pos, void *self)
{
    s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((pos[0] >> 20) == (p[2] >> 20)
            && (pos[1] >> 20) == (p[3] >> 20)
            && (pos[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/*
 * The push interaction: probe the tile one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. Only the
 * occupant's position is committed. The 360-byte owner includes its four-word
 * literal pool. Record fields are asserted only where written: facing at +6,
 * position at +8/+12/+16, state at +34, occupancy flag bit 0 at +89, speeds
 * at +48/+52, and the words cleared at +36/+44.
 */
void StagedActor_PushActorAhead(void)
{

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)gCell + (idx << 1));
    subject = (SceneRecord *)Engine_ActorGet(handle);

    dir = subject->facing >> 12;

    step = KorosseoKabe_DirectionSteps[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = (SceneRecord *)SceneData_FindSlotAtPosition((s32 *)&pos, subject);
    if (target == 0) {
        return;
    }

    step = KorosseoKabe_DirectionSteps[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = (SceneRecord *)SceneData_FindSlotAtPosition((s32 *)&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    pos.x = target->x;
    pos.y = target->y + 0x100000;
    pos.z = target->z;

    blocker = (SceneRecord *)SceneData_FindSlotAtPosition((s32 *)&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = KorosseoKabe_DirectionSteps[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    /* Terrain probe: signed, so only a positive code refuses the move. */
    if (Object_CheckMovementCollision(target, &pos) > 0) {
        return;
    }

    Object_SetAnimation(subject, 8);
    Task_Wait(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Object_SetPosition(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Object_SetPosition(subject, pos.x, pos.y, pos.z);

    Audio_PlayCue(0xee);
    Object_CommitPosition(target);
    Audio_PlayCue(0x120);

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Object_SetAnimation(subject, 1);
}

/*
 * Probe the two cells ahead of the active subject and return what occupies the
 * nearer one, else the further one, else zero. The 160-byte owner includes its
 * alignment bytes and two-word literal pool. Facing is the biased quadrant of
 * the halfword at +6, with no sign extension; each probe rounds x and z down to
 * whole units and re-centres them by half a unit, carrying y unrounded. Only
 * the record fields at +6, +8, +12 and +16 are asserted.
 */
s32 *SceneActor_FindOccupantAheadOfSubject(void)
{

    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = (u8 *)Engine_ActorGet(gGameState.selected_actor);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, facing, position);          /* 128 << 13 */

    occupant = SceneData_FindSlotAtPosition(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, facing, position);      /* 128 << 14 */

        occupant = SceneData_FindSlotAtPosition(position, record);
    }

    return occupant;
}
