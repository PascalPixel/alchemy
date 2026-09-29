#include "TASK.H"

/* The game state's cells, read here as halfwords. */
extern s16 gCell[];

void SceneState_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    GameFlag_Set(0x331);
    p = (u8 *)Engine_ActorGet(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        Map_CopyCellAttributes(46, 17, 1, 1, p5, p6);
    }
}

void FieldScene_SetFlag332AndDrawTiles(void)
{
    u8 *slot;

    GameFlag_Set(0x332);
    slot = (u8 *)Engine_ActorGet(21) + 85;
    *slot = 0;
    {
        s32 v5 = 50;
        s32 v6 = 17;

        Map_CopyCellAttributes(46, 17, 1, 1, v5, v6);
    }
}

void FieldScene_SetFlag333AndDrawTiles(void)
{
    GameFlag_Set(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        Map_CopyCellAttributes(32, 37, 1, 4, width, height);
    }
}

void SceneState_SendWord250With6(void)
{
    s16 *tbl = gCell;

    BattleFx_RunRisingObjectSequence(*(s32 *)(tbl + 250), 6, 0);
}

void FieldScene_Forward4358(void)
{
    Leader_CheckAhead();
}

void SceneActor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2)
{

    MovedObject *p;
    MovedObject *q;
    s32 x;
    s32 y;

    p = (MovedObject *)Engine_ActorGet(gGameState.selected_actor);
    q = (MovedObject *)Engine_ActorGet(a0);
    Event_Begin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Object_SetPosition(p, x, p->f0c, y);
    }
    Object_SetAnimation(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Object_SetPosition(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetAnimation(q, 4);
    } else {
        Object_SetAnimation(q, 3);
    }
    Audio_PlayCue(226);
    Object_CommitPosition(p);
    Audio_PlayCue(288);
    Object_SetAnimation(q, 2);
    Event_End();
}
