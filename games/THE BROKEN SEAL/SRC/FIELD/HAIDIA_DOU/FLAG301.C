#include "HAIDIA.H"

void SceneAudio_PlayCue123AndDispatchWork364(void)
{
    s32 val = gEventWork->touched_trigger;

    Audio_PlayCue(123);
    Event_RequestExit(val);
}

/*
 * resource_3a6 owner at 0x02001770, complete 104-byte span through its
 * one-word pool. It installs the common window rectangle, then selects one of
 * two row layouts from story flag 0x301.
 */
void DialogueLayout_ConfigureRowsByFlag301(void)
{
    Map_CopyCellRect(0, 34, 13, 3, 23, 34);

    if (GameFlag_IsSet(0x301) != 0) {
        SceneActor_PlaceAtTile(11, 35, 35);
        Map_CopyCellRect(24, 34, 1, 3, 23, 34);
    } else {
        SceneActor_PlaceAtTile(11, 23, 35);
        Map_CopyCellRect(24, 34, 1, 3, 35, 34);
    }
}

void SceneActor_PositionPair(s32 a0, s32 a1, s32 a2)
{

    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Actor_Get(gGameState.selected_actor);
    q = Actor_Get(a0);
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
    Object_CommitPosition(p);
    Event_End();
}

/*
 * The 54-byte owner at 0x020018b4 includes its two pool words: 0x301 and
 * 0x121 are identifiers passed as arguments, never dereferenced as
 * addresses.  0x301 is this scene's event flag.  The two shift calls carry
 * a displacement and its opposite, not two unrelated magnitudes.
 */
void FieldScene_RunShiftAndSetFlag301(void)
{

    Audio_PlayCue(241);
    SceneActor_PositionPair(11, 112, 0);
    SceneActor_PositionPair(11, 80, 0);
    GameFlag_Set(0x301);
    WaitFrames(2);
    DialogueLayout_ConfigureRowsByFlag301();
    Audio_PlayCue(0x121);
}

void FieldScene_RunActor11Transition301(void)
{

    Audio_PlayCue(241);
    SceneActor_PositionPair(11, -112, 0);
    SceneActor_PositionPair(11, -80, 0);
    GameFlag_Clear(0x301);
    WaitFrames(2);
    DialogueLayout_ConfigureRowsByFlag301();
    Audio_PlayCue(0x121);
}

void SceneActor_PlaceAtTile(s32 id, s32 x, s32 y)
{
    struct Rec_3a6 *rec = Actor_Get(id);

    if (rec != 0) {
        Actor_SetSpritePriority(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* Advance actor eleven through the two presentation states used at scene end. */
