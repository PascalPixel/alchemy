/* Draft: Suhara gate, FR.
 * 2026-10-01: Four callbacks omit the European passage-cell repairs.
 * Their complete compiled owners are collectively 140 bytes short; the
 * remaining 24-byte scene difference belongs to the placement scripts.
 * Ordinary approved TBS flags, no compiler output changes.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE_IDS.H"
#include "../../../../games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GATE.H"
extern u8 MsgSuharaMeaning[];
extern u8 MsgFieldVenusLighthouseWasAttackedBy[];

s32 SuharaGate_EnterScene(void)
{
    s32 scene;

    if (Engine_GameFlagIsSet(0x89f)) {
        gGameState.saved_scene = (s32)&SceneId_RunpaSuhara;
        gGameState.saved_entrance = 10;
    }
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_SuharaGate1) {
        if (Engine_GameFlagIsSet(0x897))
            Engine_ActorSetPosition(10, 0, 0);
        if (gGameState.entrance == 3) {
            if (Engine_GameFlagIsSet(0x8fb)) {
                gGameState.retreat_scene = scene;
                gGameState.retreat_entrance = 1;
            }
            if (Engine_GameFlagIsSet(0x8fc)) {
                gGameState.retreat_scene = scene;
                gGameState.retreat_entrance = 5;
            }
            Engine_GameFlagClear(0x12f);
        }
        if (gGameState.entrance == 1) {
            Engine_GameFlagSet(0x8fb);
            if (!Engine_GameFlagIsSet(0x96f))
                Map_CopyCellAttributes(6, 0, 2, 1, 8, 27);
        }
        if (gGameState.entrance == 5)
            Engine_GameFlagSet(0x8fc);
    } else if (scene == (s32)&SceneId_SuharaGate2) {
        Engine_ActorSetAnimation(8, 4);
        Engine_ActorSetAnimation(9, 4);
        Engine_ActorSetAnimation(10, 3);
        Engine_ActorSetAnimation(11, 4);
        Engine_ActorSetAnimation(12, 3);
        Object_GetById(15)->scale_y = 0x19999;
        Map_CopyCellAttributes(108, 38, 1, 1, 102, 56);
    }
    return 0;
}

void Scene_RunPrimarySequence(void)
{
    Engine_EventBegin();
    Actor_SetSpeed(8, 65536, 32768);
    Actor_SetSpeed(9, 65536, 32768);
    Actor_WalkTo(8, 136, 384);
    Actor_WalkToAndWait(9, 152, 384);
    Actor_FaceDirection(8, 16384, 0);
    Actor_FaceDirection(9, 16384, 0);
    Engine_ActorSetAnimation(8, 1);
    Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
    Map_CopyCellAttributes(9, 26, 2, 1, 7, 26);
    Engine_EventEnd();
}

void Scene_RunScene3c3SequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;

    Engine_EventBegin();
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Actor_WalkToAndWait(0, 120, 0x1b6);
    Actor_FaceDirection(0, 0xc000, 0);
    record = Actor_Get(0);
    if (record != 0) {
        Actor_SetPosition(11, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_TaskWait(1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_WalkToAndWait(11, 108, 0x1af);
    Actor_FaceDirection(11, 0xd000, 10);
    Actor_ShowEmote(11, 0x100, 20);
    Actor_FaceDirection(11, 0xd000, 20);
    Actor_FaceDirection(11, 0, 40);
    Actor_FaceDirection(11, 0xd000, 40);
    Actor_FaceDirection(11, 0, 20);
    Engine_ActorStartRepeatedMotion(11, 2);
    Engine_EventSetMessage((s32)MsgSuharaMeaning);
    Event_ShowMessageAndWait(11, 0, 40);
    Actor_ShowEmote(8, 0x100, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_WalkToAndWait(11, 132, 0x1a4);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_WalkToAndWait(11, 138, 0x1a0);
    Actor_FaceDirection(11, 0xb000, 10);
    Engine_ActorStartRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_ShowEmote(9, 0x100, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_WalkToAndWait(11, 144, 0x1a4);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAttachedEffect(9, 0x102);
    Engine_ActorStartRepeatedMotion(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(11, 0x5000, 20);
    Event_AskYesNo(11, 0);
    if (GameFlag_IsSet(0x9b0) != 0) {
        Actor_FaceDirection(11, 0xd000, 40);
        Actor_SetAttachedEffect(11, 0x102);
        Engine_EventWait(40);
        Event_ShowMessageAndWait(11, 0, 10);
    } else {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(11, 0, 40);
    Actor_ShowEmote(11, 0x100, 40);
    Actor_FaceDirection(11, 0xb000, 10);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_WalkToAndWait(11, 138, 0x1a0);
    Actor_FaceDirection(11, 0xb000, 20);
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(11, 0x102);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(11, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(0, 0xe000, 10);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(9, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(11, 0, 20);
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(11, 0, 10);
    Engine_ActorSetAnimation(11, 2);
    record = Actor_Get(0);
    if (record != 0) {
        Actor_SetDestination(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(11);
    v5 = 7;
    Actor_SetPosition(11, 0, 0);
    Map_CopyCellAttributes(6, 27, 1, 1, v5, 27);
    Map_CopyCellAttributes(9, 26, 2, 1, v5, 26);
    GameFlag_Set(0x89f);
    Engine_EventEnd();
}

void Scene_RunActorTenRepeatedMotion(void)
{
    extern u8 *gWork;

    unsigned int beat;

    Engine_EventBegin();

    Engine_EventSetMessage((s32)MsgFieldVenusLighthouseWasAttackedBy);
    Event_ShowMessageAndWait(10, 0, 10);

    beat = 0;
    do {
        Actor_SetChildValue(10, 0);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
        Engine_TaskWait(4);

        Actor_SetChildValue(10, 15);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
        beat++;
        Engine_TaskWait(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Actor_SetChildValue(10, 0);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
        Engine_TaskWait(2);

        Actor_SetChildValue(10, 15);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
        beat++;
        Engine_TaskWait(2);
    } while (beat <= 11);

    Actor_SetPosition(10, 0, 0);

    GameFlag_Set(0x897);

    Engine_EventEnd();
}
