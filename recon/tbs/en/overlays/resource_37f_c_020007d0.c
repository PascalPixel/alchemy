/* NONMATCHING: 292 bytes, candidate 284, 140 differing halfwords (2026-09-24).
 * Hand-written push-block step. Unit symbols beyond the resolved veneers:
 * gSoruPushSteps = 0x0200a65c (data). Remaining: register allocation only; the
 * body is instruction for instruction the reference's. The reference keeps the
 * step table address in sl and the velocity zero in r9 (three saved high
 * registers); here the table takes r6 and the zero r10, which saves one push.
 * A table pointer variable makes gcc fold base and index into one pointer. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Each facing's push step: the x step in the high half, the z step in the low. */
extern s32 gSoruPushSteps[];

struct FieldActor *SceneActor_FindSlotByTilePosition(s32 x, s32 z);
s32 Main_080091d8(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void Scene_UpdateOuterActor9Flags(void);
void Scene_UpdateOuterActor10Flags(void);
void Scene_UpdateFormationActor9Flags(void);
void Scene_UpdateFormationActor10Flags(void);
void Scene_UpdateFormationActor11Flags(void);
void Scene_UpdateFormationActor12Flags(void);
void Scene_UpdateFormationActor13Flags(void);
void Scene_UpdateFormationActor14Flags(void);

void Local_020007d0(void)
{
    struct FieldActor *leader;
    struct FieldActor *block;
    s32 dir;
    s32 step;
    s32 zero;
    union FieldCoordinate pos[3];

    leader = Engine_ActorGet(0);
    dir = leader->facing >> 12;
    step = gSoruPushSteps[dir];
    block = SceneActor_FindSlotByTilePosition((leader->x.part.pixel + (step >> 16)) >> 4,
        (leader->z.part.pixel + (s16)step) >> 4);
    if (block == NULL)
        return;
    zero = 0;
    block->unknown_22 = 2;
    step = gSoruPushSteps[dir];
    pos[0].fixed = block->x.fixed + (step & 0xffff0000);
    pos[1].fixed = block->y.fixed;
    pos[2].fixed = block->z.fixed + (step << 16);
    if (Main_080091d8(block, pos) > 0)
        return;
    Engine_ObjectSetAnimation(leader, 8);
    Engine_TaskWait(15);
    Engine_AudioPlayCue(185);
    block->speed = 0x3333;
    block->acceleration = 0x3333;
    Engine_ObjectSetPosition(block, pos[0].fixed, pos[1].fixed, pos[2].fixed);
    leader->speed = 0x3333;
    leader->acceleration = 0x3333;
    Engine_ObjectSetPosition(leader, pos[0].fixed, pos[1].fixed, pos[2].fixed);
    Engine_ObjectCommitPosition(block);
    block->x.fixed = pos[0].fixed;
    block->z.fixed = pos[2].fixed;
    block->velocity_x = zero;
    block->velocity_z = zero;
    Engine_ObjectSetAnimation(leader, 1);
    switch (gGameState.entrance) {
    case 11:
    case 12:
    case 13:
        Scene_UpdateOuterActor9Flags();
        Scene_UpdateOuterActor10Flags();
        break;
    case 14:
    case 15:
    case 16:
        Scene_UpdateFormationActor9Flags();
        Scene_UpdateFormationActor10Flags();
        Scene_UpdateFormationActor11Flags();
        Scene_UpdateFormationActor12Flags();
        Scene_UpdateFormationActor13Flags();
        Scene_UpdateFormationActor14Flags();
        break;
    }
}
