#ifndef ALCHEMY_UNIT_FIELD_TOREBI_IZUMI_IZUMI_H
#define ALCHEMY_UNIT_FIELD_TOREBI_IZUMI_IZUMI_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetSceneTableA(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSceneTableB(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void TorebiIzumi_AskForLuckyMedal(s32 object);
void TorebiIzumi_AskIfFirstTime(s32 object);
void TorebiIzumi_WalkLeaderToSpring(void);
void SceneDialogue_RunMessage0e34(void);
void SceneDialogue_RunMessage0e35(void);
void FieldScene_RunIndexedStep0(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 TorebiIzumi_OpenScene(void);
void TorebiIzumi_RevealPrize(s32 item);
void TorebiIzumi_RiseAndFadeIn(void);
void TorebiIzumi_OfferLuckyWheels(s32 spoken);
void TorebiIzumi_PayToPlay(void);
s32 TorebiIzumi_RunSpringGame(void);
s32 SceneDialogue_PickTopicVariantId(s32 topic);
void TorebiIzumi_PlaceActor(s32 id, s32 *pos, s32 dir, s32 palette, s32 value);
void TorebiIzumi_SetActorActive(s32 arg0, s32 arg1);
void FieldScene_RunSecondaryScript(void);
void SceneState_InitFourActorRecordsAndInstallTask(void);
s32 TorebiIzumi_RunSpringRide(s32 side);
extern const struct SceneEntrance TorebiIzumi_SceneTableA[];
extern const u32 TorebiIzumi_SceneTableB[];
extern const struct ScenePlacement gTorebiIzumiPlacements2[];
extern const struct ScenePlacement gTorebiIzumiPlacementsOther[];
extern const struct SceneEvent gTorebiIzumiEventsOther[];
extern const struct SceneEvent gTorebiIzumiEvents2[];
extern const u16 TorebiIzumi_AlphaSteps[];
extern const u32 Data_02001fd0[];
extern const u32 Data_0200200c[];
extern const u32 TorebiIzumi_TopicIds[];
extern const u8 TorebiIzumi_SliderFrames[];
extern const u8 TorebiIzumi_CircleFrames[];
extern const u16 TorebiIzumi_ActorTileX[];
extern const u16 TorebiIzumi_ActorTileZ[];
extern const u16 TorebiIzumi_ActorHeadings[];

#endif
