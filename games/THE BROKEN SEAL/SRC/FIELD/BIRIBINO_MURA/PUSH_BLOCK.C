#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

struct FieldActor *SceneActor_FindAtTileXZ(s32 x, s32 z);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void ActorPresentation_RepaintTenCellsAndActorEightCell(void);
void Scene_UpdatePuzzleActors(void);
void FieldScene_DrawTilesByActor8Row(void);

/* One cell step per facing sixteenth: x in the high half, z in the low. */
extern s32 BiribinoMura_FacingCellSteps[];

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Pushes the block the leader faces one cell ahead when nothing is in the
 * way, walking the leader along with it. */
void BiribinoMura_PushFacedBlock(void)
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
    block = SceneActor_FindAtTileXZ(
        (leader->x.part.pixel + (BiribinoMura_FacingCellSteps[dir] >> 16)) >> 4,
        (leader->z.part.pixel + (s16)BiribinoMura_FacingCellSteps[dir]) >> 4);
    if (block != NULL) {
        zero = 0;
        block->unknown_22 = 2;
        p = pos;
        step = BiribinoMura_FacingCellSteps[dir];
        p[0].fixed = block->x.fixed + (step & -0x10000);
        p[1].fixed = block->y.fixed;
        p[2].fixed = block->z.fixed + (step << 16);
        if (Value2((s32 (*)())Object_CheckMovementCollision, (s32)block, (s32)p) <= 0) {
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
            if (gGameState.scene == (s32)&SceneId_BiribinoMura3)
                ActorPresentation_RepaintTenCellsAndActorEightCell();
            else if (gGameState.scene == (s32)&SceneId_BiribinoMura1)
                Scene_UpdatePuzzleActors();
            else if (gGameState.scene == (s32)&SceneId_BiribinoMura2)
                FieldScene_DrawTilesByActor8Row();
        }
    }
}
