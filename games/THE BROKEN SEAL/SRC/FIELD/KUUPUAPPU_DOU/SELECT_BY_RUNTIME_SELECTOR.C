#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "TYPES.H"

u8 *Object_GetById(s32);

#include "TYPES.H"

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetMoveTarget(s32 *, s32, s32, s32);

#include "TYPES.H"

void KuupuappuDou_PushBlockAhead();

s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);

#include "TYPES.H"

void Scheduler_AddOrUpdateCallback();

#include "TYPES.H"

void KuupuappuDou_SpawnPuffs();
s32 IwramUnsignedRemainder();

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "TYPES.H"

#include "TYPES.H"

#include "TYPES.H"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern void KuupuappuDou_RaiseActorPriorities(void);

#include "TYPES.H"

/* Deliberate no-op callback. */

#include "TYPES.H"

#include "TYPES.H"

enum SelectByRuntimeSelectorMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

extern u8 *Object_GetById(s32);

s32 IsActor9AtTile15x54(void)
{
    s32 *actor = Actor_Get(9);
    s32 z = actor[4];
    s32 x;
    s32 z_tile;
    s32 x_tile;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = actor[2];
    z_tile = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    x_tile = x >> 20;
    if (x_tile == 15 && z_tile == 54) {
        return 1;
    }
    return 0;
}

void FieldScene_RunFlag9a9GuardedScene(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9a9) == 0) {
        KuupuappuDou_PushBlockAhead();
        if (IsActor9AtTile15x54()!= 0) {
            GameFlag_Set(0x9a9);
            Audio_PlayCue(80);
            SceneState_ApplyThreeRects();
        }
    }
}

void SceneState_ApplyThreeRects(void)
{
    s32 strip = 16;

    {
        s32 fifth = 80;
        s32 sixth = 50;

        Map_CopyCells(87, 50, 2, 4, fifth, sixth);
    }
    Map_CopyCells(23, 52, 1, 2, strip, 52);
    Map_CopyCellAttributes(16, 52, 1, 1, strip, 53);
}

void FieldScene_RunScene3a7SequenceA(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9a9) == 0) {
        if (IsActor9AtTile15x54()!= 0) {
            GameFlag_Set(0x9a9);
            Audio_PlayCue(80);
            SceneState_ApplyThreeRects();
        }
    }
}

void FieldScene_NoOp(void) {}

void SceneState_ApplyThreeRectsRows9And10(void)
{
    s32 strip = 17;

    {
        s32 p5 = 80;
        s32 p6 = 9;

        Map_CopyCells(90, 9, 2, 3, p5, p6);
    }
    Map_CopyCells(27, 10, 1, 2, strip, 10);
    Map_CopyCellAttributes(17, 10, 1, 1, strip, 11);
}

s32 SceneActor_IsActor10AtTile16x12(void)
{
    s32 *p = Actor_Get(10);
    s32 z = p[4];
    s32 x;
    s32 cz;
    s32 cx;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = p[2];
    cz = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    cx = x >> 20;
    if (cx == 16 && cz == 12) {
        return 1;
    }
    return 0;
}

void FieldScene_RunGuardedStep9AAAfterSetup(void)
{
    u32 i;
    s32 record;

    KuupuappuDou_PushBlockAhead();
    if (GameFlag_IsSet(0x9aa) == 0) {
        if (SceneActor_IsActor10AtTile16x12()!= 0) {
            if (GameFlag_IsSet(0x207) == 0) {
                Audio_PlayCue(80);
                SceneState_ApplyThreeRectsRows9And10();
                GameFlag_Set(0x9aa);
            }
        }
    }
}

void Resource3a7_NoOpCallback(void)
{
}

void FieldScene_RunGuardedStep9AA(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9aa) == 0) {
        if (SceneActor_IsActor10AtTile16x12()!= 0) {
            if (GameFlag_IsSet(0x207) == 0) {
                Audio_PlayCue(80);
                SceneState_ApplyThreeRectsRows9And10();
                GameFlag_Set(0x9aa);
            }
        }
    }
}

void SceneState_ApplyRectAndMarkActor16(void)
{
    u8 *rec = Actor_Get(16);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 32;

    Map_CopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        /* The rec is reloaded with the same selector before this store. */
        Object_GetById(16)[85] = 0;
        rec[35] = 1;
    }

    GameFlag_Set(0x200);
}

void SceneState_ConfigureRegion26_30AndMarkActor17(void)
{
    u8 *rec = Actor_Get(17);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 p5 = 23;
    s32 p6 = 34;

    Map_CopyCellAttributes(26, 30, 1, 1, p5, p6);

    if (rec != 0) {
        /* The record is reloaded with the same selector before this store. */
        Object_GetById(17)[85] = 0;
        rec[35] = 1;
    }

    GameFlag_Set(0x201);
}

void SceneState_ConfigureRegion26_30AndClearActor18Mode(void)
{
    u8 *record = Actor_Get(18);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 24;
    s32 b = 34;

    Map_CopyCellAttributes(26, 30, 1, 1, a, b);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        Object_GetById(18)[85] = 0;
        record[35] = 1;
    }

    GameFlag_Set(0x202);
}

void SceneState_ApplyRectAndSetupActor19(void)
{
    u8 *p = Actor_Get(19);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a5 = 26;
    s32 a6 = 32;

    Map_CopyCellAttributes(26, 30, 1, 1, a5, a6);

    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        /* The record is reloaded with the same selector before this store. */
        Object_GetById(19)[85] = 0;
        p[35] = 1;
    }

    GameFlag_Set(0x203);
}

void SceneActor_SetupSlotTwenty(void)
{
    u8 *rec = Actor_Get(20);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 34;

    Map_CopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        Actor_SetSpriteFlags(rec, 0);
        /* The rec is reloaded with the same selector before this store. */
        Object_GetById(20)[85] = 0;
        rec[35] = 1;
    }

    GameFlag_Set(0x204);
}

void SceneActor_MarkSlot21AndSetFlag205(void)
{
    u8 *record = Actor_Get(21);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 28;
    s32 sixth = 33;

    Map_CopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        Actor_SetSpriteFlags(record, 0);
        /* The record is reloaded with the same selector before this store. */
        Object_GetById(21)[85] = 0;
        record[35] = 1;
    }

    GameFlag_Set(0x205);
}

void SceneState_DispatchByActorZeroDepth(void)
{
    struct Actor *p = Actor_Get(ACTOR_PARTY_LEADER);

    if (p->f0c >= 0x100000) {
        KuupuappuDou_RaiseActorPriorities();
    } else {
        SceneState_SetEntries16To21Byte35();
    }
}
void SceneState_SetEntries16To21Byte35(void);
