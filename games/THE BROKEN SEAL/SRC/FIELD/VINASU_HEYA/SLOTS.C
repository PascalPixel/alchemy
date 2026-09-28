#include "ENTRY_SETUP.H"

void FieldScene_PlaceAndPinSlots8To10(void)
{

    u32 i;
    Struct_18f8 *rec;
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Actor_Get(8);
    Event_Begin();
    x = 12;
    y = 44;
    Map_CopyCellAttributes(19, 44, 4, 1, x, y);
    x = 11;
    y = 51;
    Map_CopyCellAttributes(17, 51, 2, 2, x, y);
    i = 0;
    do {
        rec = Actor_Get(i + 8);
        a = rec->unk8 >> 20;
        b = rec->unk10 >> 20;
        Map_CopyCellAttributes(12, 50, 1, 1, a, b);
        i++;
    } while (i <= 2);
    SceneActor_SwapPositionsByDepth(10, 9);
    Event_End();
}

void SceneState_ApplyRectAt19_44AndRunThree(void)
{
    s32 x;
    s32 y;

    Event_Begin();
    x = 12;
    y = 44;
    Map_CopyCellAttributes(19, 44, 4, 1, x, y);
    RunStagedActorTransition();
    FieldScene_PlaceAndPinSlots8To10();
    Event_End();
}

void SceneActor_ApplyKind45AtActorsElevenAndTwelve(void)
{
    u32 i;
    Struct_199c *p;

    i = 0;
    do {
        p = Actor_Get(i + 11);
        i++;
        SetMapCellCollision(0, p->unk8, p->unk10, 45);
    } while (i <= 1);
}

void SceneActor_ApplyPositionsOfActors11And12(void)
{
    u32 i;
    Struct_19c0 *p;

    i = 0;
    do {
        p = Actor_Get(i + 11);
        if (p->unkC > -0x100000) {
            SetMapCellCollision(0, p->unk8, p->unk10, 255);
        }
        i++;
    } while (i <= 1);
}

void FieldScene_RunGuardedThreeStepSetup(void)
{

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        SceneActor_ApplyKind45AtActorsElevenAndTwelve();
        RunStagedActorTransition();
        SceneActor_ApplyPositionsOfActors11And12();
    }
    Event_End();
}

void SceneState_MarkActorAndApplyRectAtTile(Struct_1a14 *obj)
{
    s32 x;
    s32 z;

    obj->unk23 |= 2;
    obj->unk55 = 0;
    x = obj->unk8 >> 20;
    z = obj->unk10 >> 20;
    Map_CopyCellAttributes(9, 24, 1, 1, x, z);
}

void OverlayObject_ResetObjectWhenFlatbs2Set(Struct_1a50 *o)
{
    Struct_Sub *q;
    s32 v;
    s32 z;
    s32 t;
    s32 m;

    q = o->unk50;
    v = q->unk9;
    if ((v & 12) == 12) {
        m = -13;
        m &= v;
        m |= 4;
        {
            u8 *pq = &q->unk9;
            *pq = m;
        }
        z = 0;
        o->unk44 = z;
        t = OverlayObject_SpawnWithMode14(o->unk8, 0, 0x2000000, 223);
        OverlayObject_WaitUntilIdle(o);
        o->unk8 = z;
        o->unk10 = z;
        Engine_ObjectDispatchRelease(t);
    } else {
        SceneActor_ApplyPositionsOfActors11And12();
    }
}

void SceneActor_UpdateSlots11And12ByTile(void)
{
    Struct_1a9c *o;

    Event_Begin();
    o = Actor_Get(11);
    if (o->unk8 >> 20 == 8) {
        OverlayObject_WaitUntilIdle();
        SceneState_MarkActorAndApplyRectAtTile(o);
    } else {
        OverlayObject_ResetObjectWhenFlatbs2Set(o);
    }
    o = Actor_Get(12);
    if (o->unk8 >> 20 == 7) {
        OverlayObject_WaitUntilIdle();
        SceneState_MarkActorAndApplyRectAtTile(o);
    } else {
        OverlayObject_ResetObjectWhenFlatbs2Set(o);
    }
    Event_End();
}
