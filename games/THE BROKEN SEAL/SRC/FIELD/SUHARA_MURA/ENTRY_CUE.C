#include "TYPES.H"
#include "FIELD_EVENT.H"

extern s16 SuharaMura_CellAnimationOrigins[];

/*
 * Scene script for overlay resource_3c1: the layout step, the indexed effect
 * setups, the event table choice and the entry state.
 */

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_WalkBy(actor, axis, offset);
}

static __inline__ void SetOffset_020004ba(s32 actor, s32 axis, s32 offset)
{
    Actor_WalkBy(actor, axis, offset);
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
    u8 *work = *(u8 **)&gEventWork;
    s32 no = *(s16 *)(work + 364);
    u16 x = SuharaMura_CellAnimationOrigins[no * 2];
    u16 y = SuharaMura_CellAnimationOrigins[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(0x020085e8, x, y);
    SetOffset(0, 0, -16);
    *(s32 *)(*(u8 **)&gEventWork + 456) = 16;
    Event_RequestExit(no);
}

void SceneEffect_ConfigureIndexedEffect85fe(void)
{
    u8 *work = *(u8 **)&gEventWork;
    s32 no = *(s16 *)(work + 364);
    u16 x = SuharaMura_CellAnimationOrigins[no * 2];
    u16 y = SuharaMura_CellAnimationOrigins[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(0x020085fe, x, y);
    SetOffset_020004ba(0, 0, -16);
    *(s32 *)(*(u8 **)&gEventWork + 456) = 16;
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
    if (gGameState.entrance == 90) {
        GameFlag_Set(0x96f);
    }

    work = *(u8 **)&gEventWork;
    *(s32 *)(work + 448) = 256;
    *(s32 *)(work + 456) = 24;

    /* The dressing sequence and cue are unlocked by the shared event flag. */
    if (GameFlag_IsSet(0x201) != 0) {
        FieldScene_RunLayoutStepThenSet201();
        Actor_SetAnimation(16, 4);
    }
    return 0;
}
