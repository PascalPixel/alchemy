/* Push the faced block and leader together, then refresh the current puzzle.
 * The coordinate lookup and position lifetime follow the exact Biribino
 * push-block family; Soru retains its own entrance-dependent flag updates. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Each facing's push step: the x step in the high half, the z step in the low. */
extern s32 gSoruPushSteps[];

struct FieldActor *SceneActor_FindSlotByTilePosition(s32 x, s32 z);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void Scene_UpdateOuterActor9Flags(void);
void Scene_UpdateOuterActor10Flags(void);
void Scene_UpdateFormationActor9Flags(void);
void Scene_UpdateFormationActor10Flags(void);
void Scene_UpdateFormationActor11Flags(void);
void Scene_UpdateFormationActor12Flags(void);
void Scene_UpdateFormationActor13Flags(void);
void Scene_UpdateFormationActor14Flags(void);

void SoruIriguchi_PushFacedBlock(void)
{
    struct FieldActor *leader;
    struct FieldActor *block;
    s32 zero;
    s32 step;
    u32 dir;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = Engine_ActorGet(0);
    dir = leader->facing >> 12;
    block = SceneActor_FindSlotByTilePosition(
        (leader->x.part.pixel + (gSoruPushSteps[dir] >> 16)) >> 4,
        (leader->z.part.pixel + (s16)gSoruPushSteps[dir]) >> 4);
    if (block != NULL) {
        zero = 0;
        block->unknown_22 = 2;
        p = pos;
        step = gSoruPushSteps[dir];
        p[0].fixed = block->x.fixed + (step & -0x10000);
        p[1].fixed = block->y.fixed;
        p[2].fixed = block->z.fixed + (step << 16);
        if (((s32 (*)())Object_CheckMovementCollision)((s32)block, (s32)p) <= 0) {
            Engine_ObjectSetAnimation(leader, 8);
            Engine_TaskWait(15);
            Engine_AudioPlayCue(185);
            block->speed = 0x3333;
            block->acceleration = 0x3333;
            Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
            leader->speed = 0x3333;
            leader->acceleration = 0x3333;
            Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
            Engine_ObjectCommitPosition(block);
            block->x.fixed = p[0].fixed;
            block->z.fixed = p[2].fixed;
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
    }
}
