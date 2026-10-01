/* NONMATCHING: localized Venus leap with a direct map call, 2026-10-01.
 * The complete leap owner compiles with approved TBS flags at the right size
 * but differs in nine instruction bytes: the direct Engine map call changes
 * destination-register setup and zero/state scheduling. Production uses the
 * existing tagged Map_CopyCellAttributes adapter and matches all six scenes.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY_SETUP.H"
#include "IWRAM_CALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "text/MSG_IDS.H"
TEXT_MESSAGE_ENUM(MsgVinasuThereWordsCarvedIntoRelief);

extern u8 *gActorEffectWork;

extern const struct SceneEntrance gVinasuHeyaEntrances1[];
extern const struct SceneEntrance gVinasuHeyaEntrances3[];
extern const struct SceneEntrance gVinasuHeyaEntrances4[];
extern const struct SceneEntrance gVinasuHeyaEntrances5[];
extern const struct SceneEntrance gVinasuHeyaEntrances6[];
extern const struct SceneEntrance gVinasuHeyaEntrancesOther[];

extern u8 gVinasuHeyaPlacements1[];
extern u8 gVinasuHeyaPlacements2[];
extern u8 gVinasuHeyaPlacements3[];
extern u8 gVinasuHeyaPlacements4[];
extern u8 gVinasuHeyaPlacements5[];
extern u8 gVinasuHeyaPlacements6[];
extern u8 gVinasuHeyaPlacementsOther[];
void FieldScene_PrepareActors(u8 *placements);

extern u8 gVinasuLeaderApproachScript[];
s32 SceneEffect_SpawnRandomEveryEightFramesB();
extern u8 MsgFieldDoorTightlyLocked[];
extern u8 MsgFieldVenusLighthouseWasAttackedBy[];
extern u8 MsgVinasuHmmmWeCantPushBlock[];
extern u8 MsgVinasuIveWaitedLongSeeIts[];
extern u8 MsgVinasuStatueSpeaksRobinSoulYe[];

extern u8 MsgVinasuThoughtIdExploreAfterDoor[];

void OverlayObject_WaitUntilIdle();
void Engine_ActorSetSpriteFlags();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_MapCopyCellAttributes();
void Engine_ActorMoveToAndWait();
void Engine_ActorSetAnimation();
void Engine_EventEnd();
void Engine_ActorRunRepeatedMotion();
void Object_SetActionById();
void Engine_CameraMoveTo();
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetChildValue();
void Engine_EventRequestExit();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();

void Scene_RunActorLeapSequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 zero;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    u8 *state;
#endif

#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
    zero = 0;
#endif
    rec7 = (s32)Object_GetById(0);
    Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    state = (u8 *)(rec7 + 85);
    zero = 0;
    *state = zero;
#endif
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    record = (s32)Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        s32 shown = 0x4000;

        *(u16 *)(rec7 + 6) = shown;
    }
    Call3(Engine_ActorSetSpeed, 0, 0x30000, 0x18000);
    Engine_ActorMoveToAndWait(0, *(s16 *)(rec7 + 10), 0x228);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(0, 22);
    Engine_EventWait(30);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(20);
    {
        s32 shown = 0xc000;

        *(u16 *)(rec7 + 6) = shown;
    }
    Engine_ActorSetAnimation(0, 5);
    Object_SetActionById(0, 24);
    Engine_EventWait(40);
    *(s32 *)(rec7 + 72) = 0x9999;
    {
        s32 z = *(s32 *)(rec7 + 16) + 0x480000;

        *(s32 *)(rec7 + 68) = zero;
        OverlayObject_SpawnWithMode14(*(s32 *)(rec7 + 8), 0, z, 223);
    }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Engine_MapCopyCellAttributes(33, 35, 7, 1, 33, 34);
    *state = 3;
#else
    Engine_MapCopyCellAttributes(34, 35, 5, 1, 34, 34);
#endif
    OverlayObject_WaitUntilIdle(0);
    Engine_ActorSetChildValue(0, 15);
    Engine_EventRequestExit(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}
