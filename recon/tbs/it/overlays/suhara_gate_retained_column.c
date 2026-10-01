/* Draft: Suhara gate farewell IT.
 * 2026-10-01: With the European map repairs, retaining the destination
 * column automatic swaps r5/r6 in six instructions (12 differing bytes)
 * inside the otherwise complete 892-byte callback.
 * Ordinary approved TBS flags; no output changes.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE_IDS.H"
#include "../../../../games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GATE.H"
extern u8 MsgSuharaMeaning[];

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
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Map_CopyCellAttributes(11, 26, 1, 1, v5, 26);
    Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
    Map_CopyCellAttributes(9, 26, 2, 1, v5, 26);
#endif
    GameFlag_Set(0x89f);
    Engine_EventEnd();
}
