#include "REGION.H"

/* The entrance setups the exported entry dispatches to, and pushing a statue
 * one tile ahead of the leader. */

/* Moving a pushed statue: the collision probe, the move target and the
 * commit, through the overlay's import veneers. */
s32 Object_CheckMovementCollision();
void Object_SetPosition();
void Object_CommitPosition();

void RunGuardedSceneSetup(void)
{
    if (GameFlag_IsSet(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        Map_CopyCellAttributes(31, 0, 1, 1, width, height);
        Actor_SetAnimation(8, 0);
    }
}

void SceneState_SetRuntimeWord448To516(void)
{
    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; the two are not one running offset. */
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);

    Actor_SetAnimation(8, 1);
    Actor_SetAnimation(10, 2);

    if (GameFlag_IsSet(0x882) != 0) {
        Actor_SetPosition(9, 0, 0);
    } else {
        Actor_SetSpriteFlags(Actor_Get(9), 0);
    }
}

void FieldScene_RunScene398SequenceC(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    record = Actor_Get(18);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(19);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(20);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(21);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(22);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(23);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(24);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(25);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(26);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(18, 5);
    Actor_SetAnimation(19, 5);
    Actor_SetAnimation(20, 5);
    Actor_SetAnimation(21, 5);
    Actor_SetAnimation(22, 5);
    Actor_SetAnimation(23, 3);
    Actor_SetAnimation(24, 3);
    Actor_SetAnimation(25, 3);
    Actor_SetAnimation(26, 3);
    Actor_SetAnimation(9, 2);
    Actor_SetAnimation(10, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetAnimation(12, 2);
    Actor_SetAnimation(13, 2);
    Actor_SetAnimation(14, 2);
    Resource398_ImportBankNoOp(18);
    Resource398_ImportBankNoOp(19);
    Resource398_ImportBankNoOp(20);
    Resource398_ImportBankNoOp(21);
    Resource398_ImportBankNoOp(22);
    Resource398_ImportBankNoOp(23);
    Resource398_ImportBankNoOp(24);
    Resource398_ImportBankNoOp(25);
    Resource398_ImportBankNoOp(26);
    Resource398_ImportBankNoOp(9);
    Resource398_ImportBankNoOp(10);
    Resource398_ImportBankNoOp(11);
    Resource398_ImportBankNoOp(12);
    Resource398_ImportBankNoOp(13);
    Resource398_ImportBankNoOp(14);
    if (GameFlag_IsSet(0x883) != 0) {
        Actor_SetPosition(8, 0, 0);
        Actor_SetAnimation(15, 5);
        Actor_Get(15)->motion_flags = 0;
        Actor_Get(15)->y.fixed = -0x40000;
        Actor_Get(15)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(15, 2);
        Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
    } else {
        Actor_SetAnimation(8, 2);
        record = Actor_Get(8);
        Actor_SetSpriteFlags((s32)record, 0);
        Actor_SetAnimation(15, 1);
    }
    Actor_SetAnimation(16, 1);
    if (GameFlag_IsSet(0x302) != 0) {
        v5 = 36;
        Actor_SetAnimation(17, 1);
        Map_CopyCellAttributes(0, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(0, 2, 1, 1, v5, 24);
    } else {
        v5 = 36;
        Actor_SetAnimation(17, 5);
        Map_CopyCellAttributes(1, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(1, 2, 1, 1, v5, 24);
    }
    if (GameFlag_IsSet(0x303) != 0) {
        Actor_SetPosition(11, 0x23a0000, 0x1780000);
    }
    if (GameFlag_IsSet(0x304) != 0) {
        Actor_SetPosition(12, 0x23a0000, 0x1780000);
    }
}

s32 *SceneActor_FindSlotAtTile(s32 x, s32 z)
{
    s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{
    u8 *player;
    u8 *target;
    u8 *blocker;
    s32 heading;
    s32 tx;
    s32 tz;
    s32 pos[3];

    player = Actor_Get(ACTOR_PARTY_LEADER);
    heading = *(u16 *)(player + 6) >> 12;

    tx = (*(s16 *)(player + 10)
        + (StagedActor_DirectionSteps[heading] >> 16)) >> 4;
    tz = (*(s16 *)(player + 18)
        + ((StagedActor_DirectionSteps[heading] << 16) >> 16)) >> 4;
    target = (u8 *)SceneActor_FindSlotAtTile(tx, tz);
    if (target == 0) return;

    tx = (*(s16 *)(target + 10)
        + (StagedActor_DirectionSteps[heading] >> 16)) >> 4;
    tz = (*(s16 *)(target + 18)
        + ((StagedActor_DirectionSteps[heading] << 16) >> 16)) >> 4;
    blocker = (u8 *)SceneActor_FindSlotAtTile(tx, tz);
    if (blocker != 0) return;

    target[0x22] = 2;

    pos[0] = *(s32 *)(target + 8)
        + (StagedActor_DirectionSteps[heading] & (s32)0xffff0000);
    pos[1] = *(s32 *)(target + 12);
    pos[2] = *(s32 *)(target + 16) + (StagedActor_DirectionSteps[heading] << 16);

    if (Object_CheckMovementCollision(target, pos) > 0) return;

    Object_SetAnimation(player, 8);
    Task_Wait(15);
    Audio_PlayCue(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Object_SetPosition(target, pos[0], pos[1], pos[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Object_SetPosition(player, pos[0], pos[1], pos[2]);

    Object_CommitPosition(target);

    *(s32 *)(target + 8) = pos[0];
    *(s32 *)(target + 16) = pos[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Object_SetAnimation(player, 1);
    FieldScene_RunScene398SequenceB();
}

void Resource398_ImportBankNoOp(void)
{
}
