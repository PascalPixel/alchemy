#include "HAIDIA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "CALL.H"

extern const struct SceneEvent gHaidiaDouEvents1[];
extern const struct SceneEvent gHaidiaDouEvents2[];
extern const struct SceneEvent gHaidiaDouEvents3[];
extern const struct SceneEvent gHaidiaDouEventsOther[];

void WaitFrames();
void HaidiaDou_ApplyEntryState();

void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void WaitFrames(s32 frames);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void *OverlayObject_CreateConfiguredB(s32 x, s32 y, s32 z, s32 kind);
void DialogueLayout_ConfigureRowsByFlag301(void);
s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);

/* What each of the sanctum's three areas answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouEvents1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouEvents2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouEvents3;
    }
    return gHaidiaDouEventsOther;
}

s32 HaidiaDou_RunSceneScript(void)
{
    s32 *request = &gEventWork->start_transition;

    *request = 0x204;
    if (gGameState.scene == (s32)&SceneId_HaidiaDou1) {
        *request = 0x100;
        WaitFrames(1);
        Engine_ActorSetSpritePriority(11, 3);
        Engine_ActorSetSpritePriority(12, 3);
        Engine_GameFlagClear(0x12f);
    }
    HaidiaDou_ApplyEntryState();
    return 0;
}

void SceneAudio_PlayCue123AndDispatchWork364(void)
{
    s32 val = gEventWork->touched_trigger;

    Audio_PlayCue(123);
    Engine_EventRequestExit(val);
}

void DialogueLayout_ConfigureRowsByFlag301(void)
{
    Map_CopyCellAttributeRect(0, 34, 13, 3, 23, 34);

    if (GameFlag_IsSet(0x301) != 0) {
        SceneActor_PlaceAtTile(11, 35, 35);
        Map_CopyCellAttributeRect(24, 34, 1, 3, 23, 34);
    } else {
        SceneActor_PlaceAtTile(11, 23, 35);
        Map_CopyCellAttributeRect(24, 34, 1, 3, 35, 34);
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
    Engine_EventBegin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Object_SetPosition(p, x, p->f0c, y);
    }
    Object_SetMode(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Object_SetPosition(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetMode(q, 4);
    } else {
        Object_SetMode(q, 3);
    }
    Object_CommitPosition(p);
    Engine_EventEnd();
}

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
        Engine_ActorSetSpritePriority(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* Advance actor eleven through the two presentation states used at scene end. */


/* Vale Cave entry: by the room and entrance, set the sprite priorities, spawn the stage objects and restore the opened cells and footprints the story flags record. */
void HaidiaDou_ApplyEntryState(void)
{
    s32 flag;

    if (gGameState.scene == (s32)&SceneId_HaidiaDou2) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
        case 3:
        case 4:
            Engine_ActorSetSpritePriority(15, 3);
            Engine_ActorSetSpritePriority(13, 3);
            OverlayObject_CreateConfiguredB(0x780000, 0, 0xe80000, 223);
            break;
        case 5:
        case 6:
        case 7:
            if (Engine_GameFlagIsSet(0x70) != 0) {
                break;
            }
            if (Engine_GameFlagIsSet(0x302) == 0) {
                break;
            }
            Engine_GameFlagSet(0x200);
            if (gGameState.entrance == 5) {
                Engine_GameFlagSet(0x201);
            }
            WaitFrames(1);
            if (Engine_GameFlagIsSet(0x109) != 0) {
                break;
            }
            Call3(Engine_ActorSetPosition, 8, 0x3180000, 0x1180000);
            Object_GetById(8)->update = SceneActor_FaceActorZero;
            break;
        case 8:
        case 9:
        case 10:
            OverlayObject_CreateConfiguredB(0x2820000, 0, 0x2280000, 20);
            Call6(Map_CopyCellAttributeRect, 23, 34, 13, 3, 0, 34);
            DialogueLayout_ConfigureRowsByFlag301();
            if (Engine_GameFlagIsSet(0x200) != 0) {
                Call6(Map_CopyCellAttributeRect, 23, 41, 1, 1, 23, 39);
            }
            if (Engine_GameFlagIsSet(0x201) != 0) {
                Call6(Map_CopyCellAttributeRect, 31, 39, 2, 1, 27, 41);
            }
            break;
        }
    } else if (gGameState.scene == (s32)&SceneId_HaidiaDou3) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
        case 3:
            if (Engine_GameFlagIsSet(0x202) != 0) {
                StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
                StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
            } else {
                FieldScene_RedrawActorFootprint(9);
            }
            if (Engine_GameFlagIsSet(0x203) != 0) {
                StagedActor_FillGridAttributeRectangle(2, 16, 16, 1, 4, 0);
                StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
            } else {
                FieldScene_RedrawActorFootprint(10);
            }
            flag = Engine_GameFlagIsSet(0x205);
            if (flag != 0) {
                StagedActor_FillGridAttributeRectangle(0, 13, 19, 4, 2, 0);
            } else if (Engine_GameFlagIsSet(0x204) != 0) {
                StagedActor_FillGridAttributeRectangle(0, 13, 15, 4, 2, flag);
                Map_CopyCellAttributeRect(14, 17, 2, 1, 14, 16);
                Map_CopyCellAttributeRect(14, 13, 1, 1, 14, 15);
            } else {
                FieldScene_RedrawActorFootprint(11);
                Engine_ActorSetSpritePriority(11, 3);
            }
            break;
        }
    }
}

void ActorPresentation_AdvanceActorElevenStates(void)
{
    Object_SetModeById(11, 1);
    Object_SetModeById(11, 2);
}
