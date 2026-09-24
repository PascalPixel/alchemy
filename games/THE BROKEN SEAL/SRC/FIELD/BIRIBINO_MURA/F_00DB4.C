#include "TYPES.H"
#include "FIELD_EVENT.H"

struct FieldActor *SceneActor_FindAtTileXZ(s32 x, s32 z);
s32 Main_080091d8(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void ActorPresentation_RepaintTenCellsAndActorEightCell(void);
void Scene_UpdatePuzzleActors(void);
void FieldScene_DrawTilesByActor8Row(void);

extern s16 Data_02000240_t[][1];
extern u8 Value_0000001e[];
extern u8 Value_00000020[];
extern u8 Value_00000023[];

/* One cell step per facing sixteenth: x in the high half, z in the low. */
extern s32 Data_02009d3c[];

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Pushes the block the leader faces one cell ahead when nothing is in the
 * way, walking the leader along with it. */
/* Pushes the block the leader faces one cell ahead when nothing is in the way, walking the leader along with it. */
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
        (leader->x.part.pixel + (Data_02009d3c[dir] >> 16)) >> 4,
        (leader->z.part.pixel + (s16)Data_02009d3c[dir]) >> 4);
    if (block != NULL) {
        zero = 0;
        block->unknown_22 = 2;
        p = pos;
        step = Data_02009d3c[dir];
        p[0].fixed = block->x.fixed + (step & -0x10000);
        p[1].fixed = block->y.fixed;
        p[2].fixed = block->z.fixed + (step << 16);
        if (Value2((s32 (*)())Main_080091d8, (s32)block, (s32)p) <= 0) {
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
            if (Data_02000240_t[224][0] == (s32)Value_00000023)
                ActorPresentation_RepaintTenCellsAndActorEightCell();
            else if (Data_02000240_t[224][0] == (s32)Value_0000001e)
                Scene_UpdatePuzzleActors();
            else if (Data_02000240_t[224][0] == (s32)Value_00000020)
                FieldScene_DrawTilesByActor8Row();
        }
    }
}
