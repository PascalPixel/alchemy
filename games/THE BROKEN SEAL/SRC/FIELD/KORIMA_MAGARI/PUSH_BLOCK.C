#include "TYPES.H"
#include "FIELD_EVENT.H"

/* A block on the ice: the four cells it covers from (x, z), across or down,
   and the object that draws it. */
struct TileRun {
    s16 id;
    s16 x;
    s16 z;
    s16 vertical;
    struct FieldActor *object;
};

/* By the leader's facing, in quarter turns: the push animation and the
   leader's step after the block. */
extern const u8 KorimaMagari_PushAnimations[];
extern const s8 KorimaMagari_PushStepX[];
extern const s8 KorimaMagari_PushStepZ[];

struct TileRun *SceneData_FindTileRunAt(struct TileRun *runs, s32 x, s32 z);
s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *point);
u8 *Engine_AllocateBlock(s32 block, s32 size);
void ObjectDispatch_InitFromTable4WithArgument(s32 table, struct FieldActor *object);
void Object_SetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
void Object_CommitPosition(struct FieldActor *object);

/* Push the block in front of the leader: slide it cell by cell, up to eleven
   cells, while the four cells beyond it are open, then play the push with
   the block gliding to where it stopped. The actor is first the leader,
   then the block. */
void Scene_PushBlockAlongRun(struct TileRun *runs)
{
    struct FieldActor *actor;
    struct TileRun *run;
    s32 point[3];
    s32 facing;
    s32 moved;
    s32 i;
    s32 x;
    s32 z;
    s32 quarter;

    moved = 0;
    actor = Engine_ActorGet(0);
    facing = (actor->facing + 0x2000) & 0xc000;
    point[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
    point[1] = actor->y.fixed;
    point[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, facing, point);
    run = SceneData_FindTileRunAt(runs, point[0] / 0x100000, point[2] / 0x100000);
    if (run == 0)
        return;
    for (i = 0; i <= 10; i++) {
        point[0] = run->x << 20;
        point[2] = run->z << 20;
        Vector_AddPolarOffset(0x100000, facing, point);
        if (State_CheckFourCellRun(point[0] / 0x100000, point[2] / 0x100000, run->vertical) != 0)
            break;
        moved = 1;
        if (run->vertical == 0) {
            x = point[0] + 0x200000;
            z = point[2] + 0x80000;
        } else {
            x = point[0] + 0x80000;
            z = point[2] + 0x200000;
        }
        run->x = point[0] / 0x100000;
        run->z = point[2] / 0x100000;
    }
    if (moved == 0)
        return;
    point[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
    point[1] = actor->y.fixed;
    point[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x80000, facing, point);
    actor = run->object;
    quarter = facing / 0x4000;
    Event_Begin();
    Actor_SetAnimation(0, 8);
    Event_Wait(6);
    actor->speed = 0x8000;
    actor->acceleration = 0x3333;
    Audio_PlayCue(239);
    Object_SetAnimation(actor, KorimaMagari_PushAnimations[quarter]);
    Object_SetPosition(actor, x, 0, z);
    Event_Wait(6);
    Actor_SetAnimation(0, 2);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(Engine_AllocateBlock(27, 0xccc) + 0x1e0), actor);
    Actor_SetSpeed(0, 0x4ccc, 0x3333);
    Actor_SetDestinationOffset(0, KorimaMagari_PushStepX[quarter], KorimaMagari_PushStepZ[quarter]);
    Event_Wait(24);
    Actor_SetAnimation(0, 1);
    Object_CommitPosition(actor);
    Object_SetAnimation(actor, 1);
    Audio_PlayCue(0x120);
    Audio_PlayCue(213);
    Event_Wait(15);
    Event_End();
}
