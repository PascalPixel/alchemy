#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 Value_000025b8;
extern u8 Value_000025dc;
extern s16 Data_020085d0[];
extern s16 Data_02000240[];

void Func_0200035e();

/*
 * Scene script for overlay resource_3c1: data table getters, two actor cue
 * branches, the indexed effect setups, and the entry state.
 */

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_WalkBy(actor, axis, offset);
}

static __inline__ void SetOffset_020004ba(s32 actor, s32 axis, s32 offset)
{
    Actor_WalkBy(actor, axis, offset);
}

/*
 * Returns the in-image table address 0x020082f0, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020082f0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x020083c8, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020083c8;
}

s32 SceneData_SelectActorTableByFlag96f(void)
{
    if (GameFlag_IsSet(0x96f) != 0) {
        return 0x020084e0;
    }
    return 0x020083f0;
}

void FieldScene_RunActorCue25b8Branch(s32 obj)
{
    s32 cue = (s32)&Value_000025b8;
    Event_SetMessage(cue);
    Event_OpenMessage(obj, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(obj, 0);
}

void FieldScene_RunActorCue25dcBranch(s32 obj)
{
    s32 cue = (s32)&Value_000025dc;
    Event_SetMessage(cue);
    Event_OpenMessage(obj, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(obj, 0);
}

void FieldScene_RunLayoutStepThenSet201(void)
{
    s32 width = 4;
    s32 height = 9;

    Map_CopyCellAttributes(25, 9, 1, 1, width, height);
    GameFlag_Set(0x201);
}

void SceneEffect_ConfigureIndexedEffect85e8(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 no = *(s16 *)(work + 364);
    u16 x = Data_020085d0[no * 2];
    u16 y = Data_020085d0[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(0x020085e8, x, y);
    SetOffset(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Event_RequestExit(no);
}

void SceneEffect_ConfigureIndexedEffect85fe(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 no = *(s16 *)(work + 364);
    u16 x = Data_020085d0[no * 2];
    u16 y = Data_020085d0[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(0x020085fe, x, y);
    SetOffset_020004ba(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Event_RequestExit(no);
}

s32 SceneData_SelectTable8614ByFlag96f(void)
{
    if (GameFlag_IsSet(0x96F) != 0) {
        return 0x02008758;
    }
    return 0x02008614;
}

s32 SceneState_InitEntryWorkspaceAndFlag96f(void)
{
    u8 *work;

    /* Record arrival on map 90, then publish the initial scene phase/timer. */
    if (Data_02000240[225] == 90) {
        GameFlag_Set(0x96f);
    }

    work = *(u8 **)0x03001ebc;
    *(s32 *)(work + 448) = 256;
    *(s32 *)(work + 456) = 24;

    /* The dressing sequence and cue are unlocked by the shared event flag. */
    if (GameFlag_IsSet(0x201) != 0) {
        Func_0200035e();
        Actor_SetAnimation(16, 4);
    }
    return 0;
}
