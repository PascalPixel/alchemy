#include "IRIGUCHI.H"
#include "TYPES.H"

s32 battle_owner_69(void);
s32 FieldEffect_UpdateGridPlacement(void);
void SceneActor_PushObjectAheadIfLevel(void);

extern const struct SceneEvent gBabiIriguchiEvents3[];
extern const struct SceneEvent gBabiIriguchiEvents2[];
extern const struct SceneEvent gBabiIriguchiEvents1[];
extern const struct SceneEvent gBabiIriguchiEventsOther[];

void SceneState_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)gEffectWork, s8 *, 0x34) = 1;
}

void ActorPresentation_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Actor_Get(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Iriguchi_CopyCellAttributes(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

void SceneActor_PushObjectAheadIfLevel(void)
{
    struct LevelCheckRecord *p = Actor_Get(ACTOR_PARTY_LEADER);
    struct LevelCheckRecord *q = ((s32 (*)())BabiIriguchi_FindActorAhead)(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (diff >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    StagedActor_AdvancePair();
}

/* With the party leader facing north or south and either the byte at 498 of
 * the game state set or no actor ahead, runs the grid placement for that
 * facing; unless that placement reports zero, pushes the object ahead when
 * the byte is clear. */
void SceneActor_RunSlotZeroFacingCheck(void)
{
    struct Obj *p = (struct Obj *)Object_GetById(0);
    s32 x = (s32)BabiIriguchi_FindActorAhead();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;

    if (gGameState.movement_mode == 1 || x == 0) {
        if (m == 0xc000) {
            r = battle_owner_69();
        }
        if (m == 0x4000) {
            r = FieldEffect_UpdateGridPlacement();
        }
    }
    if (r != 0) {
        if (gGameState.movement_mode != 1) {
            SceneActor_PushObjectAheadIfLevel();
        }
    }
}

/* What each of the entrance's scenes answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiEvents3;
    }
    if (scene == (s32)&SceneId_BabiIriguchi2) {
        return gBabiIriguchiEvents2;
    }
    if (scene == (s32)&SceneId_BabiIriguchi1) {
        return gBabiIriguchiEvents1;
    }
    return gBabiIriguchiEventsOther;
}

void SceneState_ConfigureRegion82_7AndApply768(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Iriguchi_CopyCellAttributes(82, 7, 1, 2, a, b);
    Iriguchi_TaskWait(1);
    GameFlag_Set(768);
}

void SceneState_ApplyRectsAtActors8And9(void)
{
    s32 *p = Actor_Get(8);

    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Iriguchi_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Iriguchi_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Iriguchi_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Actor_Get(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Iriguchi_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
}
