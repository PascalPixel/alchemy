/* NONMATCHING: 568 bytes, candidate 556, 216 differing halfwords,
 * 56 aligned edits (2026-09-27). H2 is byte-identical to the baseline;
 * retain its named position and corrected layout-pointer interface.
 * Hand-written: push the block in front of the leader along
 * its run. The instructions agree up to allocation: the reference keeps a
 * pointer to pos in r4 (caller-saved around each call, hence sub sp 20 and 12
 * bytes more), the leader in r7, the direction in r8, the counter in sl and
 * the slide target in fp and r9; here the leader and direction take r7/r8 and
 * the pointer folds into pos. A goto loop keeps the reference's loop shape.
 * Whole own-ROM extent [02000c2c,02000e64), including pool, audited.
 * Runtime frame is 20: cursor save at sp+0, moved at sp+4, position at
 * sp+8/+12/+16. r6 owns the position; r4 is copied only at loop entry,
 * survives polar offset and the four-cell test, and dies on loop exit.
 * Polar offset is void and mutates x/z of one three-word position; the
 * search callee walks 12-byte block records, matching PLACE_OBJECTS.C.
 * H1 transfers ARUTIN_YAMA/ROLL_STEP.C's FieldCoordinate[3] plus explicit
 * loop cursor p: 556 bytes / 222 halfwords / 92 edits. Complete diff:
 * cursor still folds into r6, frame remains 16, and union aliasing adds
 * load reuse/order differences. Rejected witness preserved in 196f654e2.
 * H2 transfers TRY_JUMP_FORWARD.C's named three-word position record,
 * separates its function lifetime from a loop-local projection cursor,
 * and types layout as the 12-byte record pointer the search consumes.
 * Candidate is binary-identical to baseline: this source boundary does
 * not recover the reference's separate r4 cursor or its save slot.
 * Full loop, action tail and pool compared. Aside from the cursor/frame
 * and resulting displacement changes, the tail retains the speed-call
 * r0-versus-literal-load order difference. No exact bytes unlocked.
 * Stop both tested axes. A future model needs evidence explaining why
 * the loop projection cursor remains distinct from the position base;
 * another array/record spelling or cursor declaration is not new evidence.
 * Stable not-yet-C unit and missing owner registration now reproduce the
 * baseline; this is inventory completeness, not credit. No exact neighbour
 * or frozen resource_396 palette source was edited. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Unit symbols: Data_02009164, Data_02009168 and Data_0200916c (data). */

struct BlockPosition {
    s32 x;
    s32 y;
    s32 z;
};

void Main_08000128(s32 distance, s32 angle, struct BlockPosition *pos);
void *Engine_AllocateBlock(s32 slot, s32 size);
void Main_080090e0(void *list, struct FieldActor *object);
void Engine_ObjectCommitPosition(struct FieldActor *object);

/* A block record: its cell and its object. */
struct Block_394 {
    s16 unknown_0;
    s16 x;
    s16 z;
    s16 upright;
    struct FieldActor *object;
};

struct Block_394 *SceneData_FindTileRunAt(struct Block_394 *layout, s32 x, s32 z);
s32 State_CheckFourCellRun(s32 x, s32 z, s32 upright);

extern u8 Data_02009164[];
extern s8 Data_02009168[];
extern s8 Data_0200916c[];

void Scene_PushBlockAlongRun(struct Block_394 *layout)
{
    struct FieldActor *leader;
    struct FieldActor *object;
    struct Block_394 *block;
    struct BlockPosition pos;
    s32 moved;
    s32 dir;
    s32 i;
    s32 x;
    s32 z;
    s32 frame;

    moved = 0;
    leader = Engine_ActorGet(0);
    dir = (leader->facing + 0x2000) & 0xc000;
    pos.x = (leader->x.fixed & 0xfff00000) + 0x80000;
    pos.y = leader->y.fixed;
    pos.z = (leader->z.fixed & 0xfff00000) + 0x80000;
    Main_08000128(0x100000, dir, &pos);
    block = SceneData_FindTileRunAt(layout, pos.x / 0x100000, pos.z / 0x100000);
    if (block == NULL)
        return;
    i = 0;
next:
    {
        struct BlockPosition *p = &pos;

        p->x = block->x << 20;
        p->z = block->z << 20;
        Main_08000128(0x100000, dir, p);
        if (State_CheckFourCellRun(pos.x / 0x100000, pos.z / 0x100000, block->upright) != 0)
            goto done;
        moved = 1;
        if (block->upright == 0) {
            x = pos.x + 0x200000;
            z = pos.z + 0x80000;
        } else {
            x = pos.x + 0x80000;
            z = pos.z + 0x200000;
        }
        block->x = pos.x / 0x100000;
        block->z = pos.z / 0x100000;
    }
    if (++i <= 10)
        goto next;
done:
    if (!moved)
        return;
    pos.x = (leader->x.fixed & 0xfff00000) + 0x80000;
    pos.y = leader->y.fixed;
    pos.z = (leader->z.fixed & 0xfff00000) + 0x80000;
    Main_08000128(0x80000, dir, &pos);
    object = block->object;
    frame = dir / 0x4000;
    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 8);
    Engine_EventWait(6);
    object->speed = 0x8000;
    object->acceleration = 0x3333;
    Engine_AudioPlayCue(239);
    Engine_ObjectSetAnimation(object, Data_02009164[frame]);
    Engine_ObjectSetPosition(object, x, 0, z);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    Main_080090e0(*(void **)((u8 *)Engine_AllocateBlock(27, 0xccc) + 0x1e0), object);
    Engine_ActorSetSpeed(0, 0x4ccc, 0x3333);
    Engine_ActorSetDestinationOffset(0, Data_02009168[frame], Data_0200916c[frame]);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Engine_ObjectCommitPosition(object);
    Engine_ObjectSetAnimation(object, 1);
    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(213);
    Engine_EventWait(15);
    Engine_EventEnd();
}
