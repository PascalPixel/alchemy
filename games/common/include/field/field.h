#ifndef FIELD_H
#define FIELD_H

/* FIELD */

#include "types.h"
#include "edition.h"
#include "object_runtime.h"
#if !GAME_TLA
#include "fieldrun.h"
#include "field_scene.h"
#include "menu_list.h"
#endif
#if !GAME_TLA
#include "farcall.h"
#endif

struct DispatchObject;
struct EffectPosition;

extern const s32 Camera_FixedViewMatrix[];
extern s32		 Iwram_Data1;
extern s32		 Iwram_Data2;
extern u8		 Map_TileDissolveOrder[];
extern const u32 ObjectDispatch_DefaultScript[];
extern u8		 Render_DecodeFrameCodeSize[];
extern u8		 Runtime_ByteRemapTable[];
s32				 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32				 Event_ValidatePackedId(u32 packed_id);
void			 Map_RenderAnimatedTileFrame(u8 *object, u32 position);
void			 Object_AttachWorkTargetToObject(s32 id, s32 flag);
void			 Object_UpdateAllMotion(void);
void			 Object_UpdateAllThumb(void);
#ifndef OWN_ObjectDispatch_SetSingleChildFlags
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *object, u32 value);
#endif
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
s32	 ObjectTable_FindLastActiveId(void);
#if GAME_TLA
void			 BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);
s32				 Equipment_HasValueFar(s32 group, s32 entry);
s32	 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32	 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
#endif
s32	 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);

#if !GAME_TLA
struct AbilityListEntry;
struct BattleEffectArgument;
struct BtlUnit;
struct BurstParticle;
struct DispatchObject;
struct EffectObject_02000c5c;
struct EffectPosition;
struct EffectRecord;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct FieldPosition;
struct MakyuriLights;
struct MapActor;
struct MapCell;
struct MenuSprite;
struct ModeRecord;
struct MotionObject;
struct ObjectRuntime;
struct PillarEffect;
struct S_02000474;
struct SceneEntrance;
struct SceneEntry;
struct SceneEvent;
struct SceneObject_02000b98;
struct ScenePlacement;
struct SceneRegion;
struct SceneRegionEntry;
struct ScriptTable;
struct SparkleFrame;
struct Sub;
struct Swirl;
struct TileRun2;
struct V;
union FieldObject;

struct Actor_02000640
{
	u8	reserved00[6];
	u16 facing;
	u8	reserved08[92];
	u16 state_flags;
};
struct Actor_02000754
{
	u8 reserved00[91];
	u8 accepted;
};
struct Actor_020007d4
{
	u8 reserved00[91];
	u8 accepted;
};
struct EffectWork
{
	u8					 pad[80];
	struct EffectRecord *record;
};
struct Flags35
{
	u8 pad[35];
	u8 flags;
};
struct Flags9
{
	u8 pad[9];
	u8 low : 2;
	u8 mode : 2;
};
struct HeightTrackedObject
{
	u8	pad00[12];
	s32 height;
};
struct Obj_02002608
{
	u8			pad00[0x18];
	s32			f18;
	u8			pad1c[7];
	u8			f23;
	u8			pad24[12];
	s32			f30;
	s32			f34;
	u8			pad38[24];
	struct Sub *f50;
	u8			pad54[1];
	u8			f55;
};
struct OverlayActorPosition
{
	u8	pad00[8];
	s32 depth_fixed;
};
struct OverlayActorState
{
	u8 pad00[35];
	u8 flags;
};
struct OverlayEffectMotion
{
	u8	pad00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	pad14[28];
	s32 horizontal_rate;
	s32 vertical_rate;
	s32 shadow_x;
	s32 shadow_y;
	s32 shadow_z;
	u8	pad44[32];
	s16 mode;
};
struct Particle_02000c4c
{
	u8	 unknown_00[8];
	s32	 x;
	s32	 y;
	u8	 unknown_10[0x20];
	s32	 angle;
	u8	 unknown_34[4];
	s32	 base_x;
	s32	 base_y;
	u8	 unknown_40[0x10];
	u16 *sprite;
};
struct PathRecorder
{
	s16 mode;
	s16 mirror;
	s16 actor;
	u16 pos;
	s16 still;
};
struct Presentation
{
	u8 reserved_00[9];
	u8 flags;
};
struct Presentation_02000c1c
{
	u8 unk_00[9];
	u8 flags;
};
struct S_020009dc
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	pad14[15];
	u8	f23;
};
struct SceneActor_020004b4
{
	u8					 reserved_00[35];
	u8					 state_23;
	u8					 reserved_24[44];
	struct Presentation *presentation;
};
struct SceneActor_02000c1c
{
	u8							  unk_00[35];
	u8							  state_23;
	u8							  unk_24[44];
	struct Presentation_02000c1c *presentation;
};
struct SceneActor_0200113c
{
	u8					 reserved_00[35];
	u8					 state_23;
	u8					 reserved_24[44];
	struct Presentation *presentation;
};
struct SceneWork_020006b4
{
	u8	reserved000[472];
	u16 branch_counter;
};
struct SceneWork_02000e90
{
	u8	reserved000[370];
	u16 actor18_marker;
};
struct ScrollLayer
{
	u8	unknown_00[8];
	s32 x;
};
struct Slot
{
	u16 f00;
	u16 f02;
	u16 f04;
	u16 f06;
};
struct SwitchCell
{
	u32 x;
	u32 z;
};
struct SwitchEffect
{
	s32 active;
	u8	unknown_04[0x5f];
	u8	finished;
};
struct T
{
	u8	pad00[30];
	u16 f1e;
};
struct TileRun
{
	s16 id;
	s16 x;
	s16 y;
	s16 vertical;
	s16 unused08;
	s16 unused0a;
};
struct V6
{
	s32 a;
	s32 b;
	s32 c;
	s32 d;
	s32 e;
	s32 f;
};
struct WorldMapVramBlock
{
	u16 base;
	u16 offset;
};
extern s32 ArutamiraDou_PulseScales[];
#ifndef OWN_ArutinYama_ActorScript
extern u8 ArutinYama_ActorScript[];
#endif
extern s16		 ArutinYama_PaletteHold;
extern s8		 ArutinYama_PaletteScript[];
extern s16		 ArutinYama_PaletteStep;
extern s32		 ArutinYama_RollRadii[];
extern const s32 BabiFune_ActionScriptA[];
extern const s32 BabiFune_ActionScriptB[];
extern const s32 BabiFune_ActionScriptC[];
extern const s32 BabiFune_ActionScriptD[];
extern s32		 BabiFune_Count;
#ifndef OWN_BabiFune_CountTicks
extern s32 BabiFune_CountTicks;
#endif
#ifndef OWN_BabiFune_DriftActive
extern s32 BabiFune_DriftActive;
#endif
extern const s32 BabiFune_DriftScript[];
extern s16		 BabiFune_FadeSlot;
extern s16		 BabiFune_FadeStep;
extern const u16 BabiFune_PaletteFrames[];
extern u16		 BabiFune_PaletteStep;
extern u8		 BabiFune_SceneTableA[];
extern u8		 BabiFune_SceneTableB[];
extern u8		 BabiFune_SceneTableC[];
extern u8		 BabiFune_SceneTableD[];
#ifndef OWN_BabiFune_ShimmerActive
extern s32 BabiFune_ShimmerActive;
#endif
#ifndef OWN_BabiFune_ShimmerPhase
extern s32 BabiFune_ShimmerPhase;
#endif
extern s32 BabiFune_StoredRecord1;
extern s32 BabiFune_StoredRecord2;
extern s32 BabiFune_StoredSlot0;
extern s32 BabiFune_StoredSlot3;
#ifndef OWN_BabiFune_SwellActive
extern s32 BabiFune_SwellActive;
#endif
#ifndef OWN_BabiFune_SwellLayerSevenY
extern s32 BabiFune_SwellLayerSevenY;
#endif
#ifndef OWN_BabiFune_SwellLayerSixY
extern s32 BabiFune_SwellLayerSixY;
#endif
extern s32				  BabiFune_SwellPhase;
extern const s16		  BabiFune_WaveSine[256];
extern const u8			  BattleFx_BurstParticleScriptA[];
extern const u8			  BattleFx_BurstParticleScriptB[];
extern u8				  BeamSequence_CellHeights[];
extern u8				  BeamSequence_CellReaches[];
extern u16				  BeamSequence_CellSheetOffsets[];
extern u8				  BeamSequence_CellWidths[];
extern u8				  BiribinoHeya_PrimaryTable[];
extern u8				  BiribinoHeya_SecondaryTable[];
extern u8				  BiribinoHeya_TertiaryTable[];
extern s32				  BiribinoMura_FacingCellSteps[];
extern u8				  BiribinoNiwa_GuardScript[];
extern s32				  Data_02001fd0[];
extern u16				  Data_0200200c[];
extern const s32		  Data_02003c54[];
extern const u8			  Data_020060ac[];
extern const u8			  Data_0200622c[];
extern u8				  Data_02008fc8[];
extern u8				  Data_02009028[];
extern u8				  Data_02009038[];
extern u8				  Data_02009098[];
extern s32				  Data_0200b350[];
extern u32				  WorldMap_GraphicsResources[][6];
extern const u8			  ObjectMotion_WaitScript[];
extern const u8			  FieldFx_SparkleTiles[];
extern u8				  EntryLayout_FourthTable[];
extern u8				  EntryLayout_PrimaryTable[];
extern u8				  EntryLayout_Records[];
extern u8				  EntryLayout_SecondaryTable[];
extern const u8			  Field_TileMessageOffsets[];
extern const u8			  FieldFx_MoteTiles[];
extern s32				  FourObjectMotion_ResourceIds[];
extern u8				  FuneHeya_ActionScriptA[];
extern u8				  FuneHeya_ActionScriptB[];
extern u8				  FuneHeya_ActionScriptC[];
extern u8				  FuneHeya_ActionScriptD[];
extern const s32		  FuneHeya_ActionScriptE[];
extern const s32		  FuneHeya_ActionScriptF[];
extern struct FieldActor *FuneHeya_AnchorObject;
extern const s32		  FuneHeya_CellStepsA[];
extern const s32		  FuneHeya_CellStepsB[];
extern const s32		  FuneHeya_CellStepsC[];
extern const s32		  FuneHeya_CellStepsD[];
extern s32				  FuneHeya_CueTimer;
extern const u8			  FuneHeya_EntryActionScript[];
extern u8				  FuneHeya_ProgressTableA[];
extern u8				  FuneHeya_ProgressTableB;
extern u8				  FuneHeya_ProgressTableC;
extern u8				  FuneHeya_SceneTable01[];
extern u8				  FuneHeya_SceneTable02[];
extern u8				  FuneHeya_SceneTable03;
extern u8				  FuneHeya_SceneTable04[];
extern u8				  FuneHeya_SceneTable05[];
extern u8				  FuneHeya_SceneTable06[];
extern u8				  FuneHeya_SceneTable07[];
extern u8				  FuneHeya_SceneTable08[];
extern u8				  FuneHeya_SceneTable09[];
extern u8				  FuneHeya_SceneTable10[];
extern u8				  FuneHeya_SceneTable11[];
extern u8				  FuneHeya_SceneTable12[];
extern u8				  FuneHeya_SceneTable13[];
extern u8				  FuneHeya_SceneTable14[];
extern const s32		  FuneHeya_Script01[];
extern u8				  FuneHeya_StepScriptA[];
extern u8				  FuneHeya_StepScriptB[];
extern u8				  FuneHeya_StepScriptC[];
extern u8				  FuneHeya_StepScriptD[];
extern u8				  FuneHeya_StepScriptE[];
extern u8				  FuneHeya_StepScriptF[];
extern u8				  FuneHeya_StepScriptG[];
extern u8				  FuneHeya_StepScriptH[];
#ifndef OWN_FuneHeya_TurnSteps
extern s32 FuneHeya_TurnSteps[];
#endif
extern u8						   FuneHobashira_EnsembleWalk10[];
extern u8						   FuneHobashira_EnsembleWalk11[];
extern u8						   FuneHobashira_EnsembleWalk12[];
extern u8						   FuneHobashira_EnsembleWalk13[];
extern u8						   FuneHobashira_EnsembleWalk14[];
extern u8						   FuneHobashira_EnsembleWalk15[];
extern u8						   FuneHobashira_EnsembleWalk9[];
extern u8						   FuneKanpan_ClosingCrewScript[];
extern u8						   FuneKanpan_CrewActionsA[];
extern u8						   FuneKanpan_CrewActionsB[];
extern u8						   FuneKanpan_CrewActionsC[];
extern u8						   FuneKanpan_CrewActionsD[];
extern u8						   FuneKanpan_CrewActionsE[];
extern u8						   FuneKanpan_CrewScript[];
extern u8						   FuneKanpan_CrewScriptB[];
extern u8						   FuneKanpan_CrewScriptC[];
extern u8						   FuneKanpan_CrewScriptD[];
extern u8						   FuneKanpan_CrewScriptE[];
extern u8						   FuneKanpan_DeckActionsA[];
extern u8						   FuneKanpan_DeckActionsB[];
extern u8						   FuneKanpan_DeckActionsC[];
extern u8						   FuneKanpan_DeckEventActions[];
extern s32						   FuneKanpan_FlagGroupEntries[];
extern s32						   FuneKanpan_LayerScroll[];
extern s32						   FuneKanpan_LayerSpeed[];
extern u8						   FuneKanpan_LeadActionsA[];
extern u8						   FuneKanpan_LeadActionsB[];
extern u8						   FuneKanpan_LeadActionsC[];
extern u8						   FuneKanpan_LeaveActions[];
extern u8						   FuneKanpan_PresentationActionsA[];
extern u8						   FuneKanpan_PresentationActionsB[];
extern u8						   FuneKanpan_RandomActorActions[];
extern const u8					   FuneKanpan_RosterActions[];
extern u8						   FuneKanpan_SailorActions[];
extern u8						   FuneKanpan_SceneTableA[];
extern u8						   FuneKanpan_SceneTableB[];
extern u8						   FuneKanpan_SceneTableC[];
extern s32						   FuneKanpan_SlotDepth[];
extern u32						   FuneKanpan_SlotMode[];
extern u16						   FuneKanpan_SlotPhase[];
extern u16						   FuneKanpan_SlotValue[];
extern s32						   FuneKanpan_WaveAngleX;
extern s32						   FuneKanpan_WaveAngleY;
extern s32						   Funka_ArcOrigins[][2];
extern const s32				   Funka_EmberScript[];
extern s32						   gActorEightPuffScript[];
extern const u8					   gAltmillerActionA[];
extern const u8					   gAltmillerActionB[];
extern const u8					   gAltmillerActionC[];
extern const u8					   gAltmillerActionD[];
extern const struct SceneEvent	   gArutamiraDouEvents2[];
extern const struct SceneEvent	   gArutamiraDouEvents3[];
extern const struct SceneEvent	   gArutamiraDouEvents4[];
extern const struct SceneEvent	   gArutamiraDouEvents5[];
extern const struct SceneEvent	   gArutamiraDouEvents6[];
extern const struct SceneEvent	   gArutamiraDouEventsOther[];
extern const struct ScenePlacement gArutamiraDouPlacements2[];
extern const struct ScenePlacement gArutamiraDouPlacements4[];
extern const struct ScenePlacement gArutamiraDouPlacements6[];
extern const struct ScenePlacement gArutamiraDouPlacementsOther[];
extern const struct SceneEntrance  gArutinMuraEntrances1[];
extern const struct SceneEntrance  gArutinMuraEntrances2[];
extern const struct SceneEntrance  gArutinMuraEntrancesOther[];
extern const struct SceneEvent	   gArutinMuraEvents1[];
extern const struct SceneEvent	   gArutinMuraEvents2[];
extern const struct SceneEvent	   gArutinMuraEventsOther[];
extern u8						   gArutinMuraPlacements1[];
extern u8						   gArutinMuraPlacements2[];
extern u8						   gArutinMuraPlacementsOther[];
extern const struct SceneEntrance  gArutinYamaEntrances1[];
extern const struct SceneEntrance  gArutinYamaEntrances10[];
extern const struct SceneEntrance  gArutinYamaEntrances11[];
extern const struct SceneEntrance  gArutinYamaEntrances2[];
extern const struct SceneEntrance  gArutinYamaEntrances3[];
extern const struct SceneEntrance  gArutinYamaEntrances4[];
extern const struct SceneEntrance  gArutinYamaEntrances5[];
extern const struct SceneEntrance  gArutinYamaEntrances6[];
extern const struct SceneEntrance  gArutinYamaEntrances7[];
extern const struct SceneEntrance  gArutinYamaEntrances8[];
extern const struct SceneEntrance  gArutinYamaEntrances9[];
extern const struct SceneEntrance  gArutinYamaEntrancesOther[];
extern const struct SceneEvent	   gArutinYamaEvents1[];
extern const struct SceneEvent	   gArutinYamaEvents10[];
extern const struct SceneEvent	   gArutinYamaEvents11[];
extern const struct SceneEvent	   gArutinYamaEvents2[];
extern const struct SceneEvent	   gArutinYamaEvents3[];
extern const struct SceneEvent	   gArutinYamaEvents4[];
extern const struct SceneEvent	   gArutinYamaEvents5[];
extern const struct SceneEvent	   gArutinYamaEvents6[];
extern const struct SceneEvent	   gArutinYamaEvents7[];
extern const struct SceneEvent	   gArutinYamaEvents8[];
extern const struct SceneEvent	   gArutinYamaEvents9[];
extern const struct SceneEvent	   gArutinYamaEventsOther[];
extern const struct ScenePlacement gArutinYamaPlacements1[];
extern const struct ScenePlacement gArutinYamaPlacements10[];
extern const struct ScenePlacement gArutinYamaPlacements11[];
extern const struct ScenePlacement gArutinYamaPlacements3[];
extern const struct ScenePlacement gArutinYamaPlacements5[];
extern const struct ScenePlacement gArutinYamaPlacements6[];
extern const struct ScenePlacement gArutinYamaPlacements7[];
extern const struct ScenePlacement gArutinYamaPlacements8[];
extern const struct ScenePlacement gArutinYamaPlacements9[];
extern const struct ScenePlacement gArutinYamaPlacementsOther[];
extern const struct SceneRegion	   gArutinYamaRegions10[];
extern const struct SceneRegion	   gArutinYamaRegions9[];
extern const struct SceneEntrance  gBabiChikaEntrances1[];
extern const struct SceneEntrance  gBabiChikaEntrances2[];
extern const struct SceneEntrance  gBabiChikaEntrancesOther[];
extern const struct SceneRegion	   gBabiChikaRegions2[];
extern const struct SceneEntrance  gBabiIriguchiEntrances1[];
extern const struct SceneEntrance  gBabiIriguchiEntrances2[];
extern const struct SceneEntrance  gBabiIriguchiEntrances3[];
extern const struct SceneEntrance  gBabiIriguchiEntrancesOther[];
extern const struct SceneEvent	   gBabiIriguchiEvents1[];
extern const struct SceneEvent	   gBabiIriguchiEvents2[];
extern const struct SceneEvent	   gBabiIriguchiEvents3[];
extern const struct SceneEvent	   gBabiIriguchiEventsOther[];
extern const struct ScenePlacement gBabiIriguchiPlacements1[];
extern const struct ScenePlacement gBabiIriguchiPlacements2[];
extern const struct ScenePlacement gBabiIriguchiPlacements3[];
extern const struct ScenePlacement gBabiIriguchiPlacementsOther[];
extern const struct SceneRegion	   gBabiIriguchiRegions3[];
extern const struct SceneEntrance  gBiribinoDouEntrances1[];
extern const struct SceneEntrance  gBiribinoDouEntrances2[];
extern const struct SceneEntrance  gBiribinoDouEntrances3[];
extern const struct SceneEntrance  gBiribinoDouEntrancesOther[];
extern const struct SceneEvent	   gBiribinoDouEvents1[];
extern const struct SceneEvent	   gBiribinoDouEvents2[];
extern const struct SceneEvent	   gBiribinoDouEvents3[];
extern const struct SceneEvent	   gBiribinoDouEventsOther[];
extern u8						   gBiribinoDouExits[];
extern const struct ScenePlacement gBiribinoDouPlacements1[];
extern const struct ScenePlacement gBiribinoDouPlacements2[];
extern const struct ScenePlacement gBiribinoDouPlacements3[];
extern const struct ScenePlacement gBiribinoDouPlacementsOther[];
extern u8						   gBiribinoHeyaPlacements[];
extern const struct SceneEvent	   gBiribinoKyudenEvents[];
extern const struct SceneEvent	   gBiribinoKyudenEventsOther[];
extern u8						   gBiribinoKyudenPlacements[];
extern const struct ScenePlacement gBiribinoKyudenPlacementsOther[];
extern const struct SceneEntrance  gBiribinoMuraEntrances1[];
extern const struct SceneEntrance  gBiribinoMuraEntrances2[];
extern const struct SceneEntrance  gBiribinoMuraEntrances3[];
extern const struct SceneEntrance  gBiribinoMuraEntrancesOther[];
extern const struct SceneEvent	   gBiribinoMuraEvents1[];
extern const struct SceneEvent	   gBiribinoMuraEvents2[];
extern const struct SceneEvent	   gBiribinoMuraEvents3[];
extern const struct SceneEvent	   gBiribinoMuraEventsOther[];
extern const struct ScenePlacement gBiribinoMuraPlacements1[];
extern const struct ScenePlacement gBiribinoMuraPlacements2[];
extern const struct ScenePlacement gBiribinoMuraPlacements3[];
extern const struct ScenePlacement gBiribinoMuraPlacementsOther[];
extern const struct SceneRegion	   gBiribinoMuraRegions2[];
extern const struct SceneEvent	   gBiribinoNiwaEvents[];
extern const struct SceneEvent	   gBiribinoNiwaEventsOther[];
extern u8						   gBiribinoNiwaPlacements[];
extern const struct ScenePlacement gBiribinoNiwaPlacementsOther[];
extern const u8					   gBlackOrbLeaderScript[];
extern const s32				   gFallingEffectScript[];
extern u8						   gFuneKanpanEvents[];
extern u8						   gFuneKanpanEventsFlag8a0[];
extern u8						   gFuneKanpanEventsFlag928[];
extern u8						   gFuneKanpanEventsFlag93e[];
extern u8						   gFuneKanpanPlacements[];
extern u8						   gFuneKanpanPlacementsFlag911[];
extern u8						   gFuneKanpanPlacementsFlag927[];
extern u8						   gFuneKanpanPlacementsFlag928[];
extern u8						   gFuneKanpanPlacementsFlag93e[];
extern const struct SceneEntrance  gGomaSuiroEntrances2[];
extern const struct SceneEntrance  gGomaSuiroEntrancesOther[];
extern const struct SceneEvent	   gGomaSuiroEvents2[];
extern const struct SceneEvent	   gGomaSuiroEventsOther[];
extern const struct ScenePlacement gGomaSuiroPlacements2[];
extern const struct ScenePlacement gGomaSuiroPlacementsOther[];
extern s32						   gHaidiaBabiActor8Departure[];
extern const u8					   gHaidiaBabiActorExitAction[];
extern s32						   gHaidiaBabiBagLiftScript[];
extern s32						   gHaidiaBabiBagShowScript[];
extern const u8					   gHaidiaBabiLeaderExitAction[];
extern const s32				   gHaidiaBabiRampActor10Action[];
extern const s32				   gHaidiaBabiRampActor10ActionB[];
extern const s32				   gHaidiaBabiRampActor8Action[];
extern const s32				   gHaidiaBabiRampActor8ActionB[];
extern const s32				   gHaidiaBabiRampFinalAction[];
extern const s32				   gHaidiaBabiRampLeaderAction[];
extern const s32				   gHaidiaBabiRampLeaderActionB[];
extern const u8					   gHaidiaBabiSharedAction[];
extern const struct SceneEntrance  gHaidiaDouEntrances1[];
extern const struct SceneEntrance  gHaidiaDouEntrances2[];
extern const struct SceneEntrance  gHaidiaDouEntrances3[];
extern const struct SceneEntrance  gHaidiaDouEntrancesOther[];
extern const struct SceneEvent	   gHaidiaDouEvents1[];
extern const struct SceneEvent	   gHaidiaDouEvents2[];
extern const struct SceneEvent	   gHaidiaDouEvents3[];
extern const struct SceneEvent	   gHaidiaDouEventsOther[];
extern const struct ScenePlacement gHaidiaDouPlacements1[];
extern const struct ScenePlacement gHaidiaDouPlacements2[];
extern const struct ScenePlacement gHaidiaDouPlacements3[];
extern const struct SceneEntrance  gImiruFuchinEntrances1[];
extern const struct SceneEntrance  gImiruFuchinEntrances2[];
extern const struct SceneEntrance  gImiruFuchinEntrances3[];
extern const struct SceneEntrance  gImiruFuchinEntrances4[];
extern const struct SceneEntrance  gImiruFuchinEntrances5[];
extern const struct SceneEntrance  gImiruFuchinEntrances6[];
extern const struct SceneEntrance  gImiruFuchinEntrances7[];
extern const struct SceneEntrance  gImiruFuchinEntrancesOther[];
extern const struct SceneEvent	   gImiruFuchinEvents1[];
extern const struct SceneEvent	   gImiruFuchinEvents2[];
extern const struct SceneEvent	   gImiruFuchinEvents3[];
extern const struct SceneEvent	   gImiruFuchinEvents4[];
extern const struct SceneEvent	   gImiruFuchinEvents5[];
extern const struct SceneEvent	   gImiruFuchinEvents6[];
extern const struct SceneEvent	   gImiruFuchinEvents7[];
extern const struct SceneEvent	   gImiruFuchinEventsOther[];
extern const struct ScenePlacement gImiruFuchinPlacements1[];
extern const struct ScenePlacement gImiruFuchinPlacements2[];
extern const struct ScenePlacement gImiruFuchinPlacements3[];
extern const struct ScenePlacement gImiruFuchinPlacements4[];
extern const struct ScenePlacement gImiruFuchinPlacements5[];
extern const struct ScenePlacement gImiruFuchinPlacements7[];
extern const struct ScenePlacement gImiruFuchinPlacementsOther[];
extern const struct SceneEntrance  gImiruMuraEntrances2[];
extern const struct SceneEntrance  gImiruMuraEntrancesOther[];
extern const struct SceneEvent	   gImiruMuraEvents2[];
extern const struct SceneEvent	   gImiruMuraEventsOther[];
extern const struct ScenePlacement gImiruMuraPlacements[];
extern u8						   gImiruMuraPlacements2[];
extern const struct ScenePlacement gImiruMuraPlacementsFlag881[];
extern const struct SceneEntrance  gJailEntrances[];
extern const u32				   gJailExits[];
extern const struct SceneEvent	   gJailPrisonerEvents[];
extern const struct ScenePlacement gJailPrisonerPlacements[];
extern const struct SceneEvent	   gJailThiefEvents[];
extern const struct ScenePlacement gJailThiefPlacements[];
extern const struct SceneEntrance  gKaragoruDouEntrances1[];
extern const struct SceneEntrance  gKaragoruDouEntrances2[];
extern const struct SceneEntrance  gKaragoruDouEntrances3[];
extern const struct SceneEntrance  gKaragoruDouEntrancesOther[];
extern const struct SceneEvent	   gKaragoruDouEvents1[];
extern const struct SceneEvent	   gKaragoruDouEvents1Flag96f[];
extern const struct SceneEvent	   gKaragoruDouEvents2[];
extern const struct SceneEvent	   gKaragoruDouEvents3[];
extern const struct SceneEvent	   gKaragoruDouEventsOther[];
extern const u32				   gKaragoruDouExits[];
extern const struct ScenePlacement gKaragoruDouPlacements1[];
extern const struct ScenePlacement gKaragoruDouPlacements1Flag96f[];
extern const struct ScenePlacement gKaragoruDouPlacements2[];
extern const struct ScenePlacement gKaragoruDouPlacements3[];
extern const struct ScenePlacement gKaragoruDouPlacementsOther[];
extern const struct SceneEntrance  gKareiHeyaEntrances1[];
extern const struct SceneEntrance  gKareiHeyaEntrances2[];
extern const struct SceneEntrance  gKareiHeyaEntrancesOther[];
extern const struct SceneEvent	   gKareiHeyaEvents1[];
extern const struct SceneEvent	   gKareiHeyaEvents1Entrance9[];
extern const struct SceneEvent	   gKareiHeyaEvents2[];
extern const struct SceneEvent	   gKareiHeyaEventsOther[];
extern const struct ScenePlacement gKareiHeyaPlacements1[];
extern const struct ScenePlacement gKareiHeyaPlacements1Entrance9[];
extern const struct ScenePlacement gKareiHeyaPlacements2[];
extern const struct ScenePlacement gKareiHeyaPlacementsOther[];
extern const struct SceneEntrance  gKareiKyudenEntrances[];
extern const struct SceneEntrance  gKareiKyudenEntrancesOther[];
extern const struct SceneEvent	   gKareiKyudenEvents[];
extern const struct SceneEvent	   gKareiKyudenEventsOther[];
extern u8						   gKareiKyudenPlacements[];
extern const struct ScenePlacement gKareiKyudenPlacementsOther[];
extern const struct SceneEntrance  gKareiTorebiEntrances1[];
extern const struct SceneEntrance  gKareiTorebiEntrances2[];
extern const struct SceneEntrance  gKareiTorebiEntrances3[];
extern const struct SceneEntrance  gKareiTorebiEntrancesOther[];
extern const struct SceneEvent	   gKareiTorebiEvents1[];
extern const struct SceneEvent	   gKareiTorebiEvents1Flag93e[];
extern const struct SceneEvent	   gKareiTorebiEvents2[];
extern const struct SceneEvent	   gKareiTorebiEvents2Flag93e[];
extern const struct SceneEvent	   gKareiTorebiEvents2Flag950[];
extern const struct SceneEvent	   gKareiTorebiEvents3[];
extern const struct SceneEvent	   gKareiTorebiEvents3Flag950[];
extern const struct SceneEvent	   gKareiTorebiEventsOther[];
extern const struct ScenePlacement gKareiTorebiPlacements1[];
extern const struct ScenePlacement gKareiTorebiPlacements1Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacements3[];
extern const struct ScenePlacement gKareiTorebiPlacements3Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacementsOther[];
extern const struct SceneEntrance  gKorashiamuIriguchiEntrances1[];
extern const struct SceneEntrance  gKorashiamuIriguchiEntrances3[];
extern const struct SceneEntrance  gKorashiamuIriguchiEntrancesOther[];
extern const struct SceneEvent	   gKorashiamuIriguchiEvents1[];
extern const struct SceneEvent	   gKorashiamuIriguchiEvents1Entrance12[];
extern const struct SceneEvent	   gKorashiamuIriguchiEvents2[];
extern const struct SceneEvent	   gKorashiamuIriguchiEvents3[];
extern const struct SceneEvent	   gKorashiamuIriguchiEventsOther[];
extern const u32				   gKorashiamuIriguchiExits[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance12[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance5[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance66[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance7[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance8[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3Flag950[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3Flag962[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacementsOther[];
extern u8						   gKorimaMagariEntrances[];
extern u8						   gKorimaMagariEvents[];
extern u8						   gKorimaMagariExits[];
extern s16						  *gKorimaMagariLayout;
extern u8						   gKorimaMagariPlacements[];
extern s16						  *gKorimaMagariRecords;
extern u16						  *gKorimaMagariReturned;
extern const struct SceneEntrance  gKorimaMuraEntrances[];
extern const struct SceneEntrance  gKorimaMuraEntrances2[];
extern const struct SceneEntrance  gKorimaMuraEntrances3[];
extern const struct SceneEvent	   gKorimaMuraEvents[];
extern const struct SceneEvent	   gKorimaMuraEvents3[];
extern const u32				   gKorimaMuraExits[];
extern const struct ScenePlacement gKorimaMuraPlacements[];
extern const struct ScenePlacement gKorimaMuraPlacements1[];
extern const struct ScenePlacement gKorimaMuraPlacements3[];
extern const struct SceneRegion	   gKorimaMuraRegions2[];
extern const struct SceneEntrance  gKorosseoKabeEntrances[];
extern const struct SceneEvent	   gKorosseoKabeEvents[];
extern const u32				   gKorosseoKabeExits[];
extern const struct ScenePlacement gKorosseoKabePlacements[];
extern const struct SceneEntrance  gKuupuappuDouEntrances1[];
extern const struct SceneEntrance  gKuupuappuDouEntrances2[];
extern const struct SceneEntrance  gKuupuappuDouEntrances3[];
extern const struct SceneEntrance  gKuupuappuDouEntrancesOther[];
extern const struct SceneEvent	   gKuupuappuDouEvents1[];
extern const struct SceneEvent	   gKuupuappuDouEvents2[];
extern const struct SceneEvent	   gKuupuappuDouEvents3[];
extern const struct SceneEvent	   gKuupuappuDouEventsOther[];
extern const u32				   gKuupuappuDouExits1[];
extern const u32				   gKuupuappuDouExits2[];
extern const u32				   gKuupuappuDouExits3[];
extern const u32				   gKuupuappuDouExitsOther[];
extern const struct ScenePlacement gKuupuappuDouPlacements1[];
extern const struct ScenePlacement gKuupuappuDouPlacements2[];
extern const struct ScenePlacement gKuupuappuDouPlacements3[];
extern const struct ScenePlacement gKuupuappuDouPlacementsOther[];
extern u8						   gKuupuappuDouRegions[];
extern const struct SceneEvent	   gKuupuappuHeyaEvents[];
extern const struct SceneEvent	   gKuupuappuHeyaEventsEntrances15To17[];
extern const struct SceneEvent	   gKuupuappuHeyaEventsFlag855[];
extern const struct SceneEntrance  gMakyuriHeyaEntrances2[];
extern const struct SceneEntrance  gMakyuriHeyaEntrances3[];
extern const struct SceneEntrance  gMakyuriHeyaEntrances4[];
extern const struct SceneEntrance  gMakyuriHeyaEntrancesOther[];
extern const struct SceneEvent	   gMakyuriHeyaEvents1[];
extern const struct SceneEvent	   gMakyuriHeyaEvents2[];
extern const struct SceneEvent	   gMakyuriHeyaEvents3[];
extern const struct SceneEvent	   gMakyuriHeyaEventsOther[];
extern const struct ScenePlacement gMakyuriHeyaPlacements1[];
extern const struct ScenePlacement gMakyuriHeyaPlacements2[];
extern const struct ScenePlacement gMakyuriHeyaPlacements3[];
extern const struct ScenePlacement gMakyuriHeyaPlacements4[];
extern const struct ScenePlacement gMakyuriHeyaPlacementsOther[];
extern const struct SceneEntrance  gMogoruMoriEntrances1[];
extern const struct SceneEntrance  gMogoruMoriEntrances2[];
extern const struct SceneEntrance  gMogoruMoriEntrances3[];
extern const struct SceneEntrance  gMogoruMoriEntrancesOther[];
extern const struct SceneEvent	   gMogoruMoriEvents1[];
extern const struct SceneEvent	   gMogoruMoriEvents2[];
extern const struct SceneEvent	   gMogoruMoriEvents3[];
extern const struct SceneEvent	   gMogoruMoriEventsOther[];
extern const struct ScenePlacement gMogoruMoriPlacements1[];
extern const struct ScenePlacement gMogoruMoriPlacements2[];
extern const struct ScenePlacement gMogoruMoriPlacements3[];
extern const struct ScenePlacement gMogoruMoriPlacementsOther[];
#ifndef OWN_gObjectSlots
extern struct ObjectRuntime *gObjectSlots;
#endif
extern s32						   GomaHashira_Actor13Frames;
extern u8						   GomaHashira_Actors[];
extern u8						   GomaHashira_Extras[];
extern u8						   GomaHashira_Messages[];
extern struct ScriptTable		   GomaHashira_PillarScripts;
extern u8						   GomaHashira_Scripts[];
extern u8						   GomaIke_SceneTable0[];
extern u8						   GomaIke_SceneTable1[];
extern u8						   GomaIke_SceneTable2[];
extern u8						   GomaIke_SceneTable3[];
extern u8						   GomaIriguchi_SceneTableA[];
extern u8						   GomaIriguchi_SceneTableB[];
extern u8						   GomaIriguchi_SceneTableC[];
extern u8						   GomaIriguchi_SceneTableD[];
extern const u32				   GomaSuiro_Exits[];
extern s32						   gPaletteCycleBlue;
extern s32						   gPaletteCycleGreen;
extern s32						   gPaletteCycleRed;
extern u8						   gPresentGuide5[];
extern u8						   gPresentGuide8[];
extern u8						   gPresentGuide9[];
extern const struct SceneEntrance  gRamakanSabakuEntrances1[];
extern const struct SceneEntrance  gRamakanSabakuEntrances2[];
extern const struct SceneEntrance  gRamakanSabakuEntrances3[];
extern const struct SceneEntrance  gRamakanSabakuEntrances4[];
extern const struct SceneEntrance  gRamakanSabakuEntrancesOther[];
extern const struct ScenePlacement gRamakanSabakuPlacements1[];
extern const struct ScenePlacement gRamakanSabakuPlacements2[];
extern const struct ScenePlacement gRamakanSabakuPlacements3[];
extern const struct ScenePlacement gRamakanSabakuPlacementsOther[];
extern const u8					   gRariberoPoseAction[];
extern const struct SceneEntrance  gRunpaJoEntrances1[];
extern const struct SceneEntrance  gRunpaJoEntrances2[];
extern const struct SceneEntrance  gRunpaJoEntrances3[];
extern const struct SceneEntrance  gRunpaJoEntrancesOther[];
extern const struct SceneEvent	   gRunpaJoEvents1[];
extern const struct SceneEvent	   gRunpaJoEvents2[];
extern const struct SceneEvent	   gRunpaJoEvents3[];
extern const struct SceneEvent	   gRunpaJoEventsOther[];
extern const u32				   gRunpaJoExits2[];
extern const u32				   gRunpaJoExits3And4[];
extern const u32				   gRunpaJoExitsOther[];
extern const struct ScenePlacement gRunpaJoPlacements1[];
extern const struct ScenePlacement gRunpaJoPlacements2[];
extern const struct ScenePlacement gRunpaJoPlacements3[];
extern const struct ScenePlacement gRunpaJoPlacements4[];
extern const struct ScenePlacement gRunpaJoPlacementsOther[];
extern const struct ScenePlacement gRunpaJoPlacementsRunpaDou[];
extern s32						   gRunpaJoSupportPairs[];
extern u32						   gScanlineAngle;
#ifndef OWN_gScanlinePitch
extern u32 gScanlinePitch;
#endif
extern const struct SceneEntrance  gSoruIriguchiEntrances1[];
extern const struct SceneEntrance  gSoruIriguchiEntrances2[];
extern const struct SceneEntrance  gSoruIriguchiEntrancesOther[];
extern const struct SceneEvent	   gSoruIriguchiEvents1[];
extern const struct SceneEvent	   gSoruIriguchiEvents1Entrances11To13[];
extern const struct SceneEvent	   gSoruIriguchiEvents1Entrances14To16[];
extern const struct SceneEvent	   gSoruIriguchiEvents2[];
extern const struct SceneEvent	   gSoruIriguchiEventsOther[];
extern const struct ScenePlacement gSoruIriguchiPlacements1[];
extern const struct ScenePlacement gSoruIriguchiPlacements1Entrances11To13[];
extern const struct ScenePlacement gSoruIriguchiPlacements1Entrances14To16[];
extern const struct ScenePlacement gSoruIriguchiPlacements2[];
extern const struct ScenePlacement gSoruIriguchiPlacementsOther[];
extern const struct SceneEntrance  gSoruNichigetsuEntrances[];
extern const struct SceneEvent	   gSoruNichigetsuEvents[];
extern const u32				   gSoruNichigetsuExits[];
extern const struct ScenePlacement gSoruNichigetsuPlacements[];
extern s32						   gSoruPushSteps[];
extern const struct SceneEntrance  gSuharaGateEntrances2[];
extern const struct SceneEntrance  gSuharaGateEntrances3[];
extern const struct SceneEntrance  gSuharaGateEntrancesOther[];
extern const struct SceneEvent	   gSuharaGateEvents2[];
extern const struct SceneEvent	   gSuharaGateEvents3[];
extern const struct SceneEvent	   gSuharaGateEventsOther[];
extern const struct ScenePlacement gSuharaGatePlacements1[];
extern const struct ScenePlacement gSuharaGatePlacements1Flagged[];
extern const struct ScenePlacement gSuharaGatePlacements2[];
extern const struct ScenePlacement gSuharaGatePlacementsOther[];
extern const struct SceneEntrance  gSuharaSabakuEntrances1[];
extern const struct SceneEntrance  gSuharaSabakuEntrances2[];
extern const struct SceneEntrance  gSuharaSabakuEntrances3[];
extern const struct SceneEntrance  gSuharaSabakuEntrancesOther[];
extern const struct SceneEvent	   gSuharaSabakuEvents3[];
extern const struct SceneEvent	   gSuharaSabakuEventsOther[];
extern const struct ScenePlacement gSuharaSabakuPlacements1[];
extern const struct ScenePlacement gSuharaSabakuPlacements2[];
extern const struct ScenePlacement gSuharaSabakuPlacements3[];
extern const struct ScenePlacement gSuharaSabakuPlacementsOther[];
extern const struct SceneEntrance  gTakaraAshibaEntrances1[];
extern const struct SceneEntrance  gTakaraAshibaEntrances2[];
extern const struct SceneEntrance  gTakaraAshibaEntrances3[];
extern const struct SceneEntrance  gTakaraAshibaEntrancesOther[];
extern const struct SceneEvent	   gTakaraAshibaEvents1[];
extern const struct SceneEvent	   gTakaraAshibaEvents2[];
extern const struct SceneEvent	   gTakaraAshibaEvents3[];
extern const struct SceneEvent	   gTakaraAshibaEventsOther[];
extern const struct ScenePlacement gTakaraAshibaPlacements1[];
extern const struct ScenePlacement gTakaraAshibaPlacements2[];
extern const struct ScenePlacement gTakaraAshibaPlacements3[];
extern const struct ScenePlacement gTakaraAshibaPlacementsOther[];
extern const struct SceneEntrance  gTakaraHashiraEntrances1[];
extern const struct SceneEntrance  gTakaraHashiraEntrances2[];
extern const struct SceneEntrance  gTakaraHashiraEntrances3[];
extern const struct SceneEntrance  gTakaraHashiraEntrances4[];
extern const struct SceneEntrance  gTakaraHashiraEntrances5[];
extern const struct SceneEntrance  gTakaraHashiraEntrancesOther[];
extern const struct SceneEvent	   gTakaraHashiraEvents1[];
extern const struct SceneEvent	   gTakaraHashiraEvents2[];
extern const struct SceneEvent	   gTakaraHashiraEvents3[];
extern const struct SceneEvent	   gTakaraHashiraEvents4[];
extern const struct SceneEvent	   gTakaraHashiraEvents5[];
extern const struct SceneEvent	   gTakaraHashiraEventsOther[];
extern const u32				   gTakaraHashiraExits[];
extern const struct ScenePlacement gTakaraHashiraPlacements1[];
extern const struct ScenePlacement gTakaraHashiraPlacements2[];
extern const struct ScenePlacement gTakaraHashiraPlacements3[];
extern const struct ScenePlacement gTakaraHashiraPlacements5[];
extern const struct ScenePlacement gTakaraHashiraPlacementsOther[];
extern const struct SceneEntrance  gTakaraShimaEntrances1[];
extern const struct SceneEntrance  gTakaraShimaEntrances2[];
extern const struct SceneEntrance  gTakaraShimaEntrances3[];
extern const struct SceneEntrance  gTakaraShimaEntrances4[];
extern const struct SceneEntrance  gTakaraShimaEntrances5[];
extern const struct SceneEntrance  gTakaraShimaEntrancesOther[];
extern const struct SceneEvent	   gTakaraShimaEvents1[];
extern const struct SceneEvent	   gTakaraShimaEvents2[];
extern const struct SceneEvent	   gTakaraShimaEvents3[];
extern const struct SceneEvent	   gTakaraShimaEvents4[];
extern const struct SceneEvent	   gTakaraShimaEvents5[];
extern const struct SceneEvent	   gTakaraShimaEventsOther[];
extern const u32				   gTakaraShimaExits[];
extern const struct ScenePlacement gTakaraShimaPlacements1[];
extern const struct ScenePlacement gTakaraShimaPlacements3[];
extern const struct ScenePlacement gTakaraShimaPlacements6To14[];
extern const struct ScenePlacement gTakaraShimaPlacementsOther[];
extern const struct SceneEvent	   gTorebiIzumiEvents2[];
extern const struct SceneEvent	   gTorebiIzumiEventsOther[];
extern const struct ScenePlacement gTorebiIzumiPlacements2[];
extern const struct ScenePlacement gTorebiIzumiPlacementsOther[];
extern const struct SceneEntrance  gTorebiKyudenEntrances2[];
extern const struct SceneEntrance  gTorebiKyudenEntrancesOther[];
extern const struct SceneEvent	   gTorebiKyudenEvents2[];
extern const struct SceneEvent	   gTorebiKyudenEvents2AfterColosso[];
extern const struct SceneEvent	   gTorebiKyudenEvents2Colosso[];
extern const struct SceneEvent	   gTorebiKyudenEventsAfterColosso[];
extern const struct SceneEvent	   gTorebiKyudenEventsColosso[];
extern const struct SceneEvent	   gTorebiKyudenEventsOther[];
extern const struct ScenePlacement gTorebiKyudenPlacements2[];
extern const struct ScenePlacement gTorebiKyudenPlacementsAfterColosso[];
extern const struct ScenePlacement gTorebiKyudenPlacementsColosso[];
extern const struct ScenePlacement gTorebiKyudenPlacementsOther[];
extern const struct SceneEntrance  gToretoHeyaEntrances[];
extern const struct SceneEvent	   gToretoHeyaEvents[];
extern const u32				   gToretoHeyaExits[];
extern const struct ScenePlacement gToretoHeyaPlacements[];
extern const struct SceneRegion	   gToretoHeyaRegions[];
extern const s32				   gTransitionSparkScript[];
extern s32						   gVinasuBlockHeights[];
extern s32						   gVinasuFirstGateOpenScript[];
extern s32						   gVinasuFirstGateScript[];
extern u16						   gVinasuGateCells[];
extern const struct SceneEntrance  gVinasuHeyaEntrances1[];
extern const struct SceneEntrance  gVinasuHeyaEntrances3[];
extern const struct SceneEntrance  gVinasuHeyaEntrances4[];
extern const struct SceneEntrance  gVinasuHeyaEntrances5[];
extern const struct SceneEntrance  gVinasuHeyaEntrances6[];
extern const struct SceneEntrance  gVinasuHeyaEntrancesOther[];
extern u8						   gVinasuHeyaEvents1[];
extern u8						   gVinasuHeyaEvents2[];
extern u8						   gVinasuHeyaEvents3[];
extern u8						   gVinasuHeyaEvents4[];
extern u8						   gVinasuHeyaEvents5[];
extern u8						   gVinasuHeyaEvents6[];
extern u8						   gVinasuHeyaPlacements1[];
extern u8						   gVinasuHeyaPlacements2[];
extern u8						   gVinasuHeyaPlacements3[];
extern u8						   gVinasuHeyaPlacements4[];
extern u8						   gVinasuHeyaPlacements5[];
extern u8						   gVinasuHeyaPlacements6[];
extern u8						   gVinasuHeyaPlacementsOther[];
extern u8						   gVinasuLeaderApproachScript[];
extern const s32				 **gVinasuPillarPushScripts[];
extern const u8					  *gVinasuPillarShapes[];
extern u8						   gVinasuPushCells[];
extern u8						   gVinasuPushScript[];
extern const s32				   gVinasuRefusedPushScript[];
extern s32						   gVinasuSecondGateScript[];
extern u16						   gVinasuSettleCells[];
extern u8						   gVinasuSettleScriptA[];
extern u8						   gVinasuSettleScriptB[];
extern const u8					   gVinasuShapesPushedDown[];
extern const u8					   gVinasuShapesPushedLeft[];
extern const u8					   gVinasuShapesPushedRight[];
extern const u8					   gVinasuShapesPushedUp[];
extern const s32				   gVinasuSprayScript[];
extern struct SwitchCell		   gVinasuSwitchCells[];
extern const s32				 **gVinasuSwitchPushScripts[];
extern const u8					  *gVinasuSwitchShapes[];
extern const u8					   gWorldMapPackedFrames[];
extern const u8					   gWorldMapPackedTiles[];
extern const u8					   gWorldMapPalettes[];
extern u8						   HaidiaArashi_ActorNineScriptA[];
extern u8						   HaidiaArashi_ActorNineScriptB[];
extern u8						   HaidiaArashi_ActorNineScriptC[];
extern u8						   HaidiaArashi_ActorNineScriptD[];
extern const u8					   HaidiaArashi_ActorNineteenRiseScript[];
extern u8						   HaidiaArashi_ActorTwentySixScriptA[];
extern u8						   HaidiaArashi_ActorTwentySixScriptB[];
extern u8						   HaidiaArashi_ActorTwentySixScriptC[];
extern u8						   HaidiaArashi_ActorTwentyTwoScriptA[];
extern u8						   HaidiaArashi_ActorTwentyTwoScriptB[];
extern const u8					   HaidiaArashi_ArriveScript[];
extern u8						   HaidiaArashi_CellSteps4[];
extern u8						   HaidiaArashi_CellSteps5[];
extern const u8					   HaidiaArashi_GatherPath0[];
extern const u8					   HaidiaArashi_GatherPath22[];
extern const u8					   HaidiaArashi_GatherPath26[];
extern const u8					   HaidiaArashi_GatherPath8[];
extern const u8					   HaidiaArashi_LookAroundScript[];
extern u8						   HaidiaArashi_StormRunActions[];
extern u8						   HaidiaFunka_ActionScript[];
extern u8						   HaidiaFunka_SceneTable0[];
extern u8						   HaidiaFunka_SceneTable1[];
extern u8						   HaidiaFunka_SceneTable2[];
extern u8						   HaidiaFunka_SceneTable3[];
extern u8						   HaidiaIe_Actor23Actions[];
extern u8						   HaidiaIe_Actor24Actions[];
extern u8						   HaidiaIe_Actor25Actions[];
extern u8						   HaidiaIe_LeaderActions[];
extern s32						   ImiruFuchin_TrackLeader;
extern s16						   ImiruMura_ExitCellPoints[][2];
extern const u16				  *ImiruMura_ExitCellSteps[];
extern const s32				   ImiruMura_SparkScript[];
extern s32						   ImiruMura_TurnScript[];
extern u8						   KareiHeya_Exits[];
extern u8						   KareiKyuden_PartyActions[];
extern const s32				   KareiMachi_ActionScript01[];
extern u8						   KareiMachi_DanceScriptA[];
extern u8						   KareiMachi_DanceScriptB[];
extern u8						   KareiMachi_DanceScriptC[];
extern u8						   KareiMachi_DanceScriptD[];
extern u8						   KareiMachi_DanceScriptE[];
extern u8						   KareiMachi_DanceScriptF[];
extern s32						   KareiMachi_DanceStep;
extern s32						   KareiMachi_Data01[];
extern const u8					   KareiMachi_Data02[];
extern const u8					   KareiMachi_Data03[];
extern u8						   KareiMachi_SceneTable01[];
extern u8						   KareiMachi_Script01[];
extern const s32				   KareiMachi_Script02[];
extern u8						   KareiMachi_StagedGroupScript[];
extern u8						   KareiTorebi_ActorSixteenScript[];
extern const u16				   KareiTorebi_LeaveCells1[];
extern const u16				   KareiTorebi_LeaveCells3[];
extern const struct SceneEntrance  KorimaHashi_Entrances[];
extern u8						   KorimaHashi_EntryAction[];
extern const struct SceneEvent	   KorimaHashi_Events[];
extern const u32				   KorimaHashi_Exits[];
extern u8						   KorimaHashi_FinishAction[];
extern u8						   KorimaHashi_FormationAction[];
extern u8						   KorimaHashi_GeraldAction[];
extern u8						   KorimaHashi_IvanAction[];
extern u8						   KorimaHashi_MiaAction[];
extern u8						   KorimaHashi_Object26Script[];
extern u8						   KorimaHashi_PartyAction[];
extern const struct ScenePlacement KorimaHashi_Placements[];
extern u8						   KorimaHashi_ResetAction[];
extern u8						   KorimaHashi_TriggerScript[];
extern u8						   KorimaHiroba_Actors[];
extern u8						   KorimaHiroba_Extras[];
extern u8						   KorimaHiroba_Messages[];
extern u8						   KorimaHiroba_Scripts[];
extern u16						   KorimaMagari_DefaultRecords[];
extern const u8					   KorimaMagari_PushAnimations[];
extern const s8					   KorimaMagari_PushStepX[];
extern const s8					   KorimaMagari_PushStepZ[];
extern u32						   KorimaMagari_ShakeChance;
extern u32						   KorimaMagari_ShakeScroll[];
extern u8						   KorimaMura_Actor8Path[];
extern u8						   KorimaMura_DebrisScript[];
extern u8						   KorimaMura_EntryActions[];
extern u8						   KorimaMura_EntryScript[];
extern u8						   KorimaMura_RegionScript[];
extern u8						   KorimaMura_SwitchCells[];
extern u8						   KorimaPalette_First[];
extern u8						   KorimaPalette_Second[];
extern s32						   Korosseo_CompetitorStartAngle;
extern s32						   Korosseo_CompetitorStartX;
extern s32						   Korosseo_CompetitorStartZ;
extern u16						   Korosseo_GaugePalette[];
extern s32						   Korosseo_MarkerOam[3];
extern s16						   Korosseo_ModeBlendDuration;
extern s16						   Korosseo_ModeBlendStart;
extern s16						   Korosseo_ModeBlendStep;
extern s16						   Korosseo_ModeBlendTarget;
extern s16						   Korosseo_ModeMoveStart;
extern s16						   Korosseo_ModeMoveStep;
extern s16						   Korosseo_ModeScaleDuration;
extern s16						   Korosseo_ModeScaleStart;
extern s16						   Korosseo_ModeScaleStep;
extern s16						   Korosseo_ModeScaleTarget;
extern u32						   Korosseo_ModeTaskSprites[];
extern u8						   Korosseo_PortraitPaletteOffsets[];
extern s16						   Korosseo_PortraitSlot;
extern const s32				   Korosseo_RivalFinishScript[];
extern u8						   KorosseoKabe_RollLogScript[];
extern const s32				   KorosseoKawa_ApproachScript[];
extern u32						   KorosseoKawa_DirectionSteps[];
extern u8						   KorosseoKawa_ModeRecordFour[];
extern u8						   KorosseoKawa_ModeRecordTwo[];
extern u16						   KorosseoKawa_RoundSpans[];
extern u8						   KorosseoKawa_SceneTableA[];
extern u8						   KorosseoKawa_SceneTableB[];
extern u8						   KorosseoKawa_SceneTableC[];
extern u8						   KorosseoKawa_SceneTableD[];
extern u8						   KorosseoKawa_ScriptA[];
extern u8						   KorosseoKawa_ScriptB[];
extern struct ModeRecord		   KorosseoKawa_SpanA;
extern struct ModeRecord		   KorosseoKawa_SpanB;
extern s8						   KorosseoMaruta_LogPattern[][6];
extern const s32				   KorosseoMaruta_PlaceScript[];
extern u8						   KorosseoMaruta_Record30Script[];
extern s32						   KuupuappuDou_DirectionSteps[];
extern const s32				   KuupuappuDou_PuffScript[];
extern s32						   KuupuappuDou_ScheduleFrames[];
extern s32						   KuupuappuDou_ScheduleIndex;
extern s32						   KuupuappuDou_ScheduleTimer;
extern s32						   KuupuappuDou_TickCounter;
extern s32						   KuupuappuDou_TickValue;
extern u8						   KuupuappuHeya_ActionTable[];
extern s32						   KuupuappuHeya_IdleActions[];
extern u8						   KuupuappuHeya_Messages[];
extern u8						   KuupuappuHeya_PairScriptA[];
extern u8						   KuupuappuHeya_PairScriptB[];
extern u8						   KuupuappuHeya_PairScriptC[];
extern u8						   KuupuappuHeya_PairScriptD[];
extern u8						   KuupuappuHeya_PairScriptE[];
extern u8						   KuupuappuHeya_PairScriptF[];
extern u8						   KuupuappuHeya_PairScriptG[];
extern u8						   KuupuappuHeya_PairScriptH[];
extern u8						   KuupuappuHeya_PairScriptI[];
extern u8						   KuupuappuHeya_PairScriptJ[];
extern u8						   KuupuappuHeya_PairScriptK[];
extern u8						   KuupuappuHeya_PairScriptL[];
extern u8						   KuupuappuHeya_PairScriptM[];
extern u8						   KuupuappuHeya_PairScriptN[];
extern u8						   KuupuappuHeya_PairScriptO[];
#ifndef OWN_KuupuappuHeya_PairScriptP
extern const u8 KuupuappuHeya_PairScriptP[];
#endif
extern u8 KuupuappuHeya_PairScriptQ[];
#ifndef OWN_KuupuappuHeya_PairScriptR
extern const u8 KuupuappuHeya_PairScriptR[];
#endif
extern u8		 KuupuappuHeya_Regions[];
extern const u8 *KuupuappuHeya_ResponseScripts[2][4];
extern u8		 KuupuappuHeya_SceneTableA[];
extern u8		 KuupuappuHeya_SceneTableB[];
extern u8		 KuupuappuHeya_Scripts[];
extern s32		 KuupuappuHeya_StepActions[];
extern u8		 KuupuappuHeya_Stops[];
extern u16		 KuupuappuHeya_StopTimer;
extern u8		 KuupuappuHeya_VaultScriptA[];
extern u8		 KuupuappuHeya_VaultScriptB[];
extern u8		 KuupuappuHeya_VaultScriptC[];
extern u8		 KuupuappuHeya_VaultScriptD[];
extern u8		 KuupuappuHeya_VaultScriptE[];
extern u8		 KuupuappuHeya_Villager11ExitScript[];
extern u8		 KuupuappuHeya_Villager12ExitScript[];
extern u8		 KuupuappuMura_Actors[];
extern u8		 KuupuappuMura_ActorsFlag855[];
extern const s32 KuupuappuMura_DriftScriptA[];
extern const s32 KuupuappuMura_DriftScriptB[];
extern u8		 KuupuappuMura_Extras[];
extern u8		 KuupuappuMura_ExtrasFlag855[];
extern u8		 KuupuappuMura_Messages[];
extern const u16 KuupuappuMura_Scene10Cells[];
extern const u16 KuupuappuMura_Scene12Cells[];
extern const u16 KuupuappuMura_Scene5Cells[];
extern const u16 KuupuappuMura_Scene6Cells[];
extern const u16 KuupuappuMura_Scene7Cells[];
extern const u16 KuupuappuMura_Scene8Cells[];
extern const u16 KuupuappuMura_Scene9Cells[];
extern u8		 KuupuappuMura_Scripts[];
extern const u8	 KuupuappuMuraSai_Actors[];
extern const u8	 KuupuappuMuraSai_Extras[];
extern const u8	 KuupuappuMuraSai_Messages[];
extern const u16 KuupuappuMuraSai_Scene10Cells[];
extern const u16 KuupuappuMuraSai_Scene12Cells[];
extern const u16 KuupuappuMuraSai_Scene5Cells[];
extern const u16 KuupuappuMuraSai_Scene6Cells[];
extern const u16 KuupuappuMuraSai_Scene7Cells[];
extern const u16 KuupuappuMuraSai_Scene8Cells[];
extern const u16 KuupuappuMuraSai_Scene9Cells[];
extern const u8	 KuupuappuMuraSai_Scripts[];
extern s16		 Makyuri_MoveAngles[];
#ifndef OWN_Makyuri_PillarScript
extern const s32 Makyuri_PillarScript[];
#endif
extern const s32			   Makyuri_RampScript[];
extern const u16			   MakyuriHeya_ColumnCells[];
extern const u16			   MakyuriHeya_FloorSwitchCloseCells[];
extern const u8				   MakyuriHeya_PushScriptA[];
extern const u8				   MakyuriHeya_PushScriptB[];
extern const u8				   MakyuriHeya_PushScriptC[];
extern const u8				   MakyuriHeya_PushScriptD[];
extern const s32			   MakyuriHeya_RiseScript[];
extern u8					   MakyuriHeya_SceneTable[];
extern const s32			   MakyuriHeya_SparkBurstScript[];
extern s32					   MakyuriHeya_SparkScript[];
extern u8					   MogoruMori_ActorScript[];
extern u8					   Object_UpdateAll[];
extern u8					   Object_UpdateAllCodeSize[];
extern u8					   ObjectGroup_BlinkChildValues[];
extern const u8				   ObjectMotion_ActionKind1Script[];
extern const u8				   ObjectMotion_ActionKind2Script[];
extern const u8				   ObjectMotion_ActionKind3Script[];
extern const u8				   ObjectMotion_ActionKind4Script[];
extern u8					   ObjectMotion_LaunchScript;
extern const u8				   ObjectMotion_MoveTowardTargetScript[];
extern const u8				   ObjectMotion_ResetActionScript[];
extern const u8				   ObjectMotion_TurnTowardLinkedScript[];
extern const u8				   ObjectMotion_VariantScripts[];
extern u8					   Placement_Actors9a7[];
extern u8					   Placement_Effects9a7[];
extern const struct SceneEvent RamakanSabaku_Events[];
extern const u32			   RamakanSabaku_Exits[];
extern const s32			   RamakanSabaku_SafePoints1[];
extern const s32			   RamakanSabaku_SafePoints2[];
extern const s32			   RamakanSabaku_SafePointsOther[];
#ifndef OWN_RamakanSabaku_SandCounter
extern u16 RamakanSabaku_SandCounter;
#endif
extern u8		 RamakanSabaku_SandEffectTiles[];
extern s16		 RamakanSabaku_SandFlash;
extern s16		 RamakanSabaku_SandLevel;
extern const u16 RamakanSabaku_SandPalette[16];
#ifndef OWN_RamakanSabaku_SandPhase
extern u16 RamakanSabaku_SandPhase;
#endif
extern u8 RamakanSabaku_SandTileBuffer[];
#ifndef OWN_RamakanSabaku_SandVramSlot
extern u16 RamakanSabaku_SandVramSlot;
#endif
extern struct MenuSprite RamakanSabaku_SandWork[];
extern s16				 RariberoMachi_DoorCells[][2];
extern u8				 RariberoMachi_DoorOpenSteps[];
extern u8				 RariberoMachi_GateOpenSteps[];
extern const u8			 RariberoMachi_LamentActions[];
#ifndef OWN_Render_DecodeFrame
extern u8 Render_DecodeFrame[];
#endif
extern const s32 SceneAction_EntryGroup[];
extern const s32 SceneAction_EntryPair[];
extern const u32 SceneAction_GroupMotion[];
extern const u32 SceneAction_GroupOffsetMotion[];
extern const s32 Script_MainScript[];
extern const s32 ScriptObject_CycleTable[];
extern u8		 ShianHeya_SceneTable0[];
extern u8		 ShianHeya_SceneTable1[];
extern u8		 ShianHeya_SceneTable2[];
extern u8		 ShianHeya_SceneTable2Entrance8[];
extern u8		 ShianHeya_SceneTable3[];
extern u8		 ShianHeya_SceneTable3Entrance8[];
extern const s32 ShianMura_Actor19Motion[];
extern const s32 ShianMura_EffectScript[];
extern const s32 ShindenHeya_CursorBob[16];
extern const s32 ShindenHeya_GeraldCircleScript[];
extern const s32 ShindenHeya_ItemIconEndScript[];
extern const s32 ShindenHeya_ItemIconGrowScript[];
extern const s32 ShindenHeya_LeaderCircleScript[];
extern const s32 ShindenHeya_OwnerEffectScript[];
extern u8		 ShindenHeya_PlacementA[];
extern u8		 ShindenHeya_PlacementB[];
extern u8		 ShindenHeya_PlacementC[];
extern u8		 ShindenHeya_PlacementD[];
extern u8		 ShindenHeya_PlacementE[];
extern const s32 ShindenHeya_PlacementSequenceB[];
extern u8		 ShindenHeya_SceneTableA[];
extern u8		 ShindenHeya_SceneTableB[];
extern u8		 ShindenHeya_SceneTableC[];
extern u8		 ShindenHeya_SceneTableD[];
extern u8		 ShindenHeya_SceneTableE[];
extern u8		 ShindenHeya_SceneTableF[];
extern u8		 ShindenHeya_SceneTableG[];
extern u8		 ShindenHeya_SceneTableH[];
extern u8		 ShindenHeya_SceneTableI[];
extern const s32 ShindenHeya_SparkEndScript[];
extern u8		 ShindenHeya_TableA;
extern u8		 ShindenHeya_TableB;
extern s8		 Soru_RingDirection[][3];
extern u8		 Soru_RingDrift[][3];
extern s32		 Soru_RingOffsetX[];
extern s32		 Soru_RingOffsetZ[];
extern u8		 Soru_RingSwing[][3];
extern u8		 SoruDou_SceneTable0[];
extern u8		 SoruDou_SceneTable1[];
extern u8		 SoruDou_SceneTable2[];
extern u8		 SoruDou_SceneTable3[];
extern const u32 SoruIriguchi_Exits[];
extern u8		 SoruMeiro_SceneTable0[];
extern u8		 SoruMeiro_SceneTable1[];
extern u8		 SoruMeiro_SceneTable2[];
extern u8		 SoruMeiro_SceneTable3[];
extern u8		 SoruRoka_SceneTable0[];
extern u8		 SoruRoka_SceneTable1[];
extern u8		 SoruRoka_SceneTable2[];
extern u8		 SoruRoka_SceneTable3[];
extern const s32 SoruStar_AngleScript[];
extern const s32 SoruStar_PresentItemScript[];
extern s32		 StagedActor_FootprintKinds[];
extern u8		 SuharaGate_SceneTable[];
extern u8		 SuharaMura_Actors[];
extern u8		 SuharaMura_ActorsFlag96f[];
extern s16		 SuharaMura_CellAnimationOrigins[];
extern const u16 SuharaMura_CellSteps0[];
extern const u16 SuharaMura_CellSteps1[];
extern u8		 SuharaMura_Extras[];
extern u8		 SuharaMura_ExtrasFlag96f[];
extern u8		 SuharaMura_Messages[];
extern u8		 SuharaMura_Scripts[];
extern u8		 Sukureta_Actor11Actions[];
extern u8		 Sukureta_Actor11RockslideActions[];
extern u8		 Sukureta_StrangerActions[];
extern const s32 TakaraAshiba_ActorFifteenMotionScript[];
extern const s32 TakaraAshiba_ActorFifteenWalkTargets[];
extern s32		 TakaraAshiba_IconTimer;
extern u8		 TakaraAshiba_SceneTable[];
extern s32		 TakaraAshiba_SlotColumns[];
extern s32		 TakaraHashira_ProbePosition[];
extern u8		 TakaraShima_EntranceCells[];
extern const u8	 TileMap_DrawRows[];
extern u8		 TileMap_DrawRowsCodeSize[];
extern const u16 TorebiIzumi_AlphaSteps[];
extern const u16 TorebiKyuden_CellSteps[];
extern u8		 ToretoEda_SceneTable0[];
extern u8		 ToretoEda_SceneTable1[];
extern u8		 ToretoEda_SceneTable2[];
extern u8		 ToretoEda_SceneTable3[];
extern s16		 ToretoHeya_MapPatches[];
extern const u8	 ToretoHeya_TableActions0[];
extern const u8	 ToretoHeya_TableActions1[];
extern const u8	 ToretoHeya_TableActions2[];
extern const u8	 ToretoHeya_TableActions3[];
extern s32		 ToretoHeya_TintIndex;
extern s32		 ToretoHeya_TintSteps[];
extern u8		 Transform_UpdateVerticesSize;
extern u8		 VinasuChojo_BeamCloseAction[];
extern u8		 VinasuChojo_BeamEndAction[];
#ifndef OWN_VinasuChojo_BeamOpenAction
extern const u8 VinasuChojo_BeamOpenAction[];
#endif
extern u8		 VinasuChojo_BeamRiseAction[];
extern const s32 VinasuChojo_BesideParticleScript[];
extern u8		 VinasuChojo_LeapAction[];
extern const s32 VinasuChojo_OrbitParticleScript[];
extern u8		 VinasuChojo_PairActionScript[];
extern u8		 VinasuChojo_PartyExitAction[];
extern u8		 VinasuChojo_PartyHeadingAction[];
extern const u8	 VinasuChojo_RiseActionScript[];
extern const s32 VinasuChojo_RiseParticleScript[];
#ifndef OWN_VinasuChojo_ScaleStepAction
extern const u8 VinasuChojo_ScaleStepAction[];
#endif
extern u8						   VinasuChojo_StepBackAction[];
extern s32						   VinasuChojo_TransitionStep;
extern s32						   VinasuChojo_TransitionTimer;
extern const u8					   WorldMap_FootprintActions[];
extern const s16				   WorldMap_KeyHeadings[16];
extern const s32				   WorldMap_KeyYaws[16];
extern u8						   WorldMap_TerrainBehaviorTable[];
extern u8						   YamaRama_ActorNineAction[];
extern const u16				   YamaRama_BoulderCells[];
extern const u16				   YamaRama_BoulderCellsBack[];
extern const struct SceneEntrance  YamaRama_Entrances[];
extern const struct SceneEvent	   YamaRama_Events[];
extern const u32				   YamaRama_Exits[];
extern const u8					   YamaRama_HsuAction[];
extern const u8					   YamaRama_LeaderAction[];
extern const struct ScenePlacement YamaRama_Placements[];
extern const struct SceneEntrance  YamaRama_TempleEntrances[];
extern const struct SceneEvent	   YamaRama_TempleEvents[];
extern const struct ScenePlacement YamaRama_TemplePlacements[];
void							   ActorPresentation_PlaceActorFourteenOnActorNine(void);
void							   ActorPresentation_PrepareActorFourteenWithCallback(void);
void							   ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void);
void							   ActorPresentation_RepaintTenCellsAndActorEightCell(void);
void							   ActorPresentation_RunActorModeOneThenZero(s32 actor);
void							   ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x);
void							   ArutamiraDou_ReleaseWallBurst(void);
void							   ArutamiraDou_SpinActorWheel(void);
void							   ArutinYama_AdvanceRollingObject(struct FieldActor *object);
void							   ArutinYama_ApplyEntryState(void);
void							   ArutinYama_PlaceFlaggedActors(void);
void							   ArutinYama_RunRollingObject(s32 id, s32 heading);
void							   ArutinYama_SettleAndMountLeader(struct FieldActor *object);
void							   ArutinYama_StepPaletteAnim(void);
void							   ArutinYama_TurnRollingObjectA(struct FieldActor *object);
void							   ArutinYama_TurnRollingObjectB(struct FieldActor *object);
void							   BabiChika_MarkActorCells(void);
void							   BabiChika_RestorePlatformCells(void);
void							   BabiChika_SettleSteps(s32 wait);
void							   BabiChika_UpdateTrackedActor(void);
void							   BabiFune_CyclePalette(void);
void							   BabiFune_UpdateDriftingObject(u8 *obj);
void							   BabiFune_UpdateWaves(void);
void							   BattleFx_ConfigureLinkedObject(s32 id, s32 flags);
void							   BattleFx_ResetCounters(void);
void							   BattleFx_RunVisibilityTransition(void);
s32								   BattleFx_SumCounters(void);
void BattleFx_UpdateParticleLinearMotion(struct BurstParticle *particle);
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect);
void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect);
void BiribinoMura_RestoreRaisedEntrance(void);
void BiribinoMura_RestoreVillageActors(void);
void BiribinoMura_UpdateCornerSpawn(void);
void BiribinoNiwa_ApplyEntryState(void);
s32	 CalculateFacingAngle(s32, s32);
void Camera_ApplyPhasedDelta(void);
#ifndef OWN_Camera_ApplyShake
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
#endif
void ConfigureAndPlaceActorFourteen(void);
void DialogueLayout_ConfigureGroupOne(void);
void DialogueLayout_ConfigureGroupThree(void);
void DialogueLayout_ConfigureGroupTwo(void);
void DialogueLayout_ConfigureRowsByFlag301(void);
void DialogueLayout_ConfigureTwoRegions(void);
u16	 Effect_AdjustColorChannels(u16 color, s32 adj);
void Effect_AdjustPaletteColors(s32 amount);
void Effect_SetBg3HofsSplit(void);
void Effect_UpdateBg3HofsByVcount(void);
#ifndef OWN_EffectPosition_ApplyAlternateStepAndYOffset
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
#endif
#ifndef OWN_EffectPosition_ApplyAnimationAndYOffset
void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position);
#endif
void EffectRuntime_ClearCurrentFlags(s32 id);
#ifndef OWN_EffectRuntime_GetCurrentObject
struct ObjectRuntime *EffectRuntime_GetCurrentObject(s32 id);
#endif
void EffectRuntime_SetMode2(s32 id);
void EffectRuntime_SetMode4AndPlayCue(s32 id);
void EffectRuntime_SetMode5AndPlayCue(s32 id);
void EffectRuntime_SetMode7AndLaunch(s32 id);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Engine_ActorFollow(s32 actor, s32 leader);
void Engine_EventClearInvalidPackedValues(void);
s32	 Engine_PartyRemoveItem(s32 item);
void Event_ClearInvalidPackedValues(void);
void Event_NoOpHook(void);
#ifndef OWN_Event_RequestDestination
void Event_RequestDestination(u16 first, u16 second);
#endif
void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot);
void FacingObject_TurnPairToFaceEachOther(
	struct FacingObject *raw_first, struct FacingObject *raw_second);
s32	 FarCall_BlankLoadTable(s32 entrance);
s32	 FarCall_BlankRunTable(s32 entrance);
s32	 Field_CheckConfiguredKeys(void);
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z);
#ifndef OWN_Field_RunTileAction
s32 Field_RunTileAction(s32 tile);
#endif
#ifndef OWN_Field_TryJumpForward
s32 Field_TryJumpForward(void);
#endif
void				  FieldEffect_UpdateSparkles(void);
void				  FieldMotes_Update(void);
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z);
#ifndef OWN_FieldScene_BuildSceneDescriptorAndInstallTask
void FieldScene_BuildSceneDescriptorAndInstallTask(
	s32 first, s32 second, s32 mode, s32 centre, s32 extra, s32 third, s32 fourth);
#endif
void FieldScene_CallWith560And44(void);
void FieldScene_ConfigureFixedPointValues(void);
void FieldScene_ConfigureFlaggedActors(void);
void FieldScene_ConfigureLeadActors(void);
void FieldScene_ConfigureThreeActors(void);
void FieldScene_DispatchBySelector(void);
void FieldScene_DispatchSceneByIndex(void);
void FieldScene_DrawTilesByActor8Row(void);
void FieldScene_PlaceAndPinSlots10And11(void);
void FieldScene_PlaceAndPinSlots8And9(void);
void FieldScene_PlaceAndPinSlots8To10(void);
#ifndef OWN_FieldScene_PrepareActors
void FieldScene_PrepareActors(struct SceneRegionEntry *entry);
#endif
void			   FieldScene_RequestAndWaitFrames(s32 selector, s32 frames);
void			   FieldScene_RunActorEightApproach(void);
void			   FieldScene_RunActorEventSequence(void);
void			   FieldScene_RunActorNinePresentationCycles(void);
void			   FieldScene_RunActorPresentationSequence(void);
void			   FieldScene_RunActors24And25Setup(void);
void			   FieldScene_RunActors24And25SetupWithValue929(void);
void			   FieldScene_RunActors24And25SetupWithValue92a(void);
void			   FieldScene_RunActorSequence(void);
void			   FieldScene_RunActorTransferSequence(void);
void			   FieldScene_RunActorTwentyDialogueSequence(void);
void			   FieldScene_RunActorUpdateSequence(void);
void			   FieldScene_RunBranchingActorSequence(void);
void			   FieldScene_RunBranchingFormationPresentation(void);
void			   FieldScene_RunCommandSequence(s32 a0);
void			   FieldScene_RunExtendedActorSequence(void);
void			   FieldScene_RunExtendedFormationPresentation(void);
void			   FieldScene_RunFlag845And847Branches(void);
void			   FieldScene_RunFlagBranchedLayoutSteps(void);
void			   FieldScene_RunFlagBranchedSetupCascade(void);
void			   FieldScene_RunFourActorPresentation(void);
void			   FieldScene_RunLargeStagingSequence(void);
void			   FieldScene_RunLateSequenceHead(void);
void			   FieldScene_RunLeaderDropSequence(void);
void			   FieldScene_RunMainCutsceneSequence(void);
void			   FieldScene_RunPartyRosterScene(void);
void			   FieldScene_RunPillarBurst(void);
void			   FieldScene_RunPillarSequence(void);
void			   FieldScene_RunRoofEnsembleSequence(void);
void			   FieldScene_RunScene373SequenceB(void);
void			   FieldScene_RunScene38bSequenceA(void);
void			   FieldScene_RunScene39aSequenceA(void);
void			   FieldScene_RunScene3a2SequenceA(void);
void			   FieldScene_RunScene3a3SequenceD(void);
void			   FieldScene_RunScene3a7SequenceD(void);
void			   FieldScene_RunScene3b2SequenceA(void);
void			   FieldScene_RunScene3b8SequenceB(void);
void			   FieldScene_RunScene3c6SequenceA(void);
void			   FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);
void			   FieldScene_RunScriptedSceneSequence(void);
void			   FieldScene_RunSevenActorEnsemble(void);
void			   FieldScene_RunShipDeckEventScript(void);
void			   FieldScene_RunSixSteps380To3A8(void);
void			   FieldScene_RunStep6(void);
void			   FieldScene_RunTwoArmSequence(s32 a);
void			   FieldScene_RunTwoCallSequence(void);
struct FieldActor *FindActorNearPosition(s32 x, s32 y);
s32				   FindNearestF2Actor(void);
void			   FourObjectMotion_ClearSlotsAndSchedule(void);
void			   FourObjectMotion_ClearSlotsAndScheduleAlt(void);
#ifndef OWN_FourObjectMotion_InitializeBottomRow
void FourObjectMotion_InitializeBottomRow(void);
#endif
s32				   FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative);
void			   FourObjectMotion_UpdateAllPositions(void);
void			   FourObjectMotion_UpdateBottomRow(void);
void			   FuneHeya_AdvanceWalkerAfterRise(u8 *o);
void			   FuneHeya_AnnounceDeparture(void);
void			   FuneHeya_AnnounceTurn(void);
void			   FuneHeya_ApplyFlaggedLayout(void);
void			   FuneHeya_AskAboutMonsters(void);
void			   FuneHeya_CallActorEighteenToRow(void);
void			   FuneHeya_CallActorSixteenToRow(void);
void			   FuneHeya_CastOff(void);
void			   FuneHeya_DismissReplacementRowers(void);
s32				   FuneHeya_FindFirstSetFlag(u32 group, s32 alternate);
struct FieldActor *FuneHeya_PlaceAnchorCharm(void);
void			   FuneHeya_SetupFirstRowingPair(void);
void			   FuneHeya_SetupReplacementPair(void);
void			   FuneHeya_SetupReplacementRower(void);
void			   FuneHeya_TurnActorToOpenSide(s32 a0);
void			   FuneHobashira_SetupHiddenLeaderLookout(void);
void			   FuneHobashira_SetupVisibleLeaderLookout(void);
void			   FuneHobashira_UpdateSway(void);
void			   FuneKanpan_ApplyEntryState(void);
void			   FuneKanpan_ArriveAtTolbi(void);
void			   FuneKanpan_ChooseSlotModes(void);
void			   FuneKanpan_PlaceDeckActors(s32 a0, s32 a1);
void			   FuneKanpan_PlaceLeaderOnDeck(void);
void			   FuneKanpan_PlaceRandomDeckActors(void);
void			   FuneKanpan_RockDeck(void);
void			   FuneKanpan_RunDeckCrewScene(void);
void			   FuneKanpan_RunJumpScene(void);
void			   FuneKanpan_SetupDeckCrewman(void);
void			   FuneKanpan_SetupHiddenLeaderDeparture(void);
void			   FuneKanpan_SetupRaisedCrewman(void);
void			   FuneKanpan_SetupVisibleLeaderDeparture(void);
#ifndef OWN_GameFlag_IsSet
s32 GameFlag_IsSet(s32 flag);
#endif
void GomaHashira_DriveActor13Idle(void);
void HaidiaArashi_RunRiverSearch(void);
void HaidiaArashi_UpdatePulsingGlow(void);
void HaidiaBabi_SpawnEffectPair(union FieldObject *object);
void ImiruFuchin_ApplyEntrySetup(void);
void ImiruFuchin_ApplyRoomLayout(void);
void ImiruFuchin_PlaceDragonsEye(void);
void InitializeEscapeSceneActors(void);
void InitializeOrbitingRenderEffect(s32 id);
void KareiMachi_SetupEntryActors(void);
void KareiMachi_StartWindowTransition(void);
void KorimaHashi_AlternateBridgePostHeights(void);
void KorimaMagari_CaptureShakeScroll(void);
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row);
void KorosseoKawa_EnsureMarkerSlot(void);
void KorosseoKawa_RunPostMatchScene(s32 a0);
void KorosseoKawa_WaitForCountdown(void);
void KorosseoKawa_WaitForStageReady(void);
void KuupuappuDou_EnableGroupAutomaticPriority(void);
void KuupuappuDou_RaiseActorPriorities(void);
void KuupuappuDou_RunRumble(void);
s32	 KuupuappuHeya_GetIwanVenusDjinnCount(void);
s32	 KuupuappuHeya_IsActorNearPoint(s32 x, s32 z, s32 id);
s32	 KuupuappuHeya_IsRecordNearPoint(s32 x, s32 y, s32 id);
void KuupuappuHeya_StartActorStops(void);
void KuupuappuHeya_UpdateActorStops(void);
void Makyuri_ClearPalette(void);
#ifndef OWN_Makyuri_SpawnLightObjects
void Makyuri_SpawnLightObjects(s32 region, struct MakyuriLights *st);
#endif
void MakyuriChojo_FlickerActorEight(void);
void MakyuriHeya_OpenStoneDoor(s32 side);
void MakyuriHeya_RideLift(void);
void MakyuriHeya_SpawnSparkPair(void);
s32	 MakyuriHeya_StartPillarPush(void);
s32	 Map_CheckObjectStep(struct ObjectRuntime *object, struct FieldPosition *position);
void Map_LoadAreaGraphics(void);
void Map_RenderPaletteMappedColumn(u32 value);
void Map_RenderPaletteMappedRow(u32 value);
void Map_ResumeAnimation(void);
void Map_SetCameraCenter(s32 x, s32 y);
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py);
void Map_ShowBg1FromBuffer(void);
void Map_ShowWorldMap(s32 picture);
void Map_UpdateCurrentTileBlock(void);
void Map_UpdateCurrentTileBlockUntilBlocked(void);
void Map_UpdateLayerScroll(void);
void Map_WaitWorkValuesBelow256(void);
s32	 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
void MapActor_UpdateContact(void);
void MapAnimation_ApplyAffineFrame(void);
void MapAnimation_PresentFrame(void);
void MapAnimation_Update(void);
#ifndef OWN_MapGroupTable_SelectEntry
void MapGroupTable_SelectEntry(s32 kind);
#endif
void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
s32	 Math_Atan2(s32 dy, s32 dx);
#ifndef OWN_Menu_FindShortcutEntries
void Menu_FindShortcutEntries(
	u32 *first_index, u32 *second_index, const struct AbilityListEntry *entries);
#endif
void  Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter);
void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth);
#ifndef OWN_Object_CollectResources
s32 Object_CollectResources(struct AbilityListEntry *output);
#endif
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id);
s32					  Object_GetScreenPosition(s32 object_id, s32 *position);
s32					  Object_IsTargetUnset(struct ObjectRuntime *object);
void				  Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1);
#ifndef OWN_Object_ReplaceResourceEntry
void *Object_ReplaceResourceEntry(void *src, void *alt);
#endif
#ifndef OWN_ObjectDispatch_InitFromTable4WithArgument
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument);
#endif
#ifndef OWN_ObjectDispatch_RegisterChildMetadata
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value);
#endif
void ObjectEffect_BeginContextEffect25(void);
void ObjectEffect_BeginContextEffect26(void);
void ObjectEffect_EndContextEffect(s32 arg0);
void ObjectEffect_PrepareContextEffect(s32 value);
s32	 ObjectEffect_RunPendingFlagEvent(void);
void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object);
#ifndef OWN_ObjectGroup_UpdateMembers
void ObjectGroup_UpdateMembers(
	s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value);
#endif
void ObjectSystem_Initialize(s32 mode);
void ObjectTable_ClearBattleSlots(void);
#ifndef OWN_ObjectTable_ResetForObject
void ObjectTable_ResetForObject(struct ScenePlacement *table);
#endif
void ObjectTable_Restore(void);
#ifndef OWN_ObjectVisual_CopyAttributes
void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id);
#endif
void  OverlayObject_ActivateSlotWithMode3(s32 a);
void  OverlayObject_AdvancePositionByDelta(union FieldObject *raw);
void  OverlayObject_ApplyRandomSlotOnOddFrames(union FieldObject *object);
void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void  OverlayObject_InitObject22(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void  OverlayObject_InitWithRandomFields(s32 a);
#ifndef OWN_OverlayObject_PrepareObject
void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth);
#endif
void OverlayObject_ResetMotionFields(s32 actor);
s32	 OverlayObject_RunObjectTwoWhenFlagged(void);
s32	 OverlayObject_SetFacingTowardObject10(void *self);
void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d);
void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);
#ifndef OWN_OverlayObject_WaitUntilSettledAndReset
void OverlayObject_WaitUntilSettledAndReset(u8 *obj);
#endif
void Party_SetReturnPoint(s32 reason);
void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset);
#ifndef OWN_RamakanSabaku_EmitSandEffect
s32 RamakanSabaku_EmitSandEffect(struct FieldActor *actor);
#endif
void				 RamakanSabaku_RaiseQuarterTriggers(void);
void				 RamakanSabaku_RunSandstorm(void);
void				 Resource3ba_NoOpCallback(void);
s32					 ResourceTable_CountFreeBlocks(void);
void				 RunActorsEightAndNineMapEvent(void);
void				 RunScene59Sequence(void);
void				 RunSceneSelectionChain(void);
s32					 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
#ifndef OWN_Scene_FadeColorFromWhite
s32 Scene_FadeColorFromWhite(void);
#endif
#ifndef OWN_Scene_PushBlockAlongRun
void Scene_PushBlockAlongRun(struct TileRun2 *runs);
#endif
void Scene_RepairTheHouse(void);
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep);
void Scene_RunActorExchange(void);
void Scene_RunEastParticleWaveSequence(void);
void Scene_RunExtendedPresentationSequence(void);
void Scene_RunFourActorProgressPresentation(void);
void Scene_RunFourActorStagingSequence(void);
void Scene_RunPairedParticleWaveSequence(void);
void Scene_RunParticleWaveSequence(void);
void Scene_RunScene371SequenceA(s32 palette);
void Scene_UpdateFormationActor10Flags(void);
void Scene_UpdateFormationActor11Flags(void);
void Scene_UpdateFormationActor12Flags(void);
void Scene_UpdateFormationActor13Flags(void);
void Scene_UpdateFormationActor14Flags(void);
void Scene_UpdateFormationActor9Flags(void);
void Scene_UpdateOuterActor10Flags(void);
void Scene_UpdateOuterActor9Flags(void);
void Scene_UpdatePuzzleActors(void);
void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);
void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay);
void SceneActor_ApplyPositionsOfActors11And12(void);
void SceneActor_ApplySlotsMatchingKind212(void);
void SceneActor_BranchOnSlotZeroAtTile38(void);
void SceneActor_CheckActors8To11NearSlotZero(void);
void SceneActor_ClearActorModeAndSetState5(s32 no);
void SceneActor_FaceActors24And25TowardActorZero(void);
void SceneActor_InitSlots10To15AndStartTask(void);
s32	 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);
void SceneActor_MarkSlot13AndSetFlag200(void);
void SceneActor_MarkSlot21AndSetFlag205(void);
void SceneActor_PassActorNinePositionWithId107(void);
void SceneActor_PlaceActors20To27(void);
void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 delay);
#ifndef OWN_SceneActor_PlaceFiveActorsInRow
void SceneActor_PlaceFiveActorsInRow(u8 *p);
#endif
void SceneActor_PlaceSlots1To3FromWork(void);
void SceneActor_PublishMarkerBySlotZeroHeight(void);
void SceneActor_PushObjectAheadIfLevel(void);
void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);
void SceneActor_RunActorCommandWithFlag91(s32 x);
void SceneActor_RunWhenActor9AtTile45x43(void);
s32	 SceneActor_SetFlagBitByRankAgainstActorZero(struct FieldActor *actor);
void SceneActor_SetFlagByteBySlotZeroPosition(void);
void SceneActor_SetMode55OnSevenRecords(void);
void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c);
void SceneActor_SetupActors11To14AndInstallTask(void);
void SceneActor_SetupSlotTwenty(void);
void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc);
#ifndef OWN_SceneActor_UpdateTracking
s32 SceneActor_UpdateTracking(
	struct MapActor *actor, struct MapActor *target, s32 distance_limit, s32 force_tracking);
#endif
void SceneActor_WaitHeightBelowLimit(struct FieldActor *obj, s32 limit);
#ifndef OWN_SceneData_FindTileRunAt
const struct TileRun *SceneData_FindTileRunAt(const struct TileRun *run, s32 x, s32 y);
#endif
s32	 SceneData_GetDifferenceOfPairSums(void);
void SceneData_SelectBlockAndResetCounters(u32 mode, u32 param);
s32	 SceneData_SelectTableEntryByFlagGroup(u32 sel);
s32	 SceneDialogue_PickTopicVariantId(s32 topic);
void SceneDialogue_PromptAndCountSkip(s32 x);
void SceneDialogue_RunActorFourteenFlagDialogue(void);
void SceneEffect_AdjustPaletteColors(s32 a);
void SceneEffect_AdvanceGatedRiseCounter(struct FieldActor *actor);
void SceneEffect_AdvancePositionByAxisMode(struct PillarEffect *effect);
void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot);
void SceneEffect_InitSlotsEightToNineteen(void);
void SceneEffect_LoadTablesAndStopDma0(void);
void SceneEffect_RestoreBlendRegisters(void);
void SceneEffect_RunActorBurst(s32 no);
void SceneEffect_SelectSlotValueAndPosition(s32 id, s32 slot, s32 flags);
void SceneEffect_SetupBlendByFlag201(void);
#ifndef OWN_SceneEffect_SpawnEffect284AtCell
struct EffectObject_02000c5c *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2);
#endif
#ifndef OWN_SceneEffect_SpawnRandomizedParticleEveryFourFrames
s32 SceneEffect_SpawnRandomizedParticleEveryFourFrames(struct SceneObject_02000b98 *obj);
#endif
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor);
void SceneEffect_UpdateLobeOrbitEffect26(void);
void ScenePlacement_ClipToView(s32 placements);
void SceneState_ApplyFiveRectsAtColumn78(void);
void SceneState_ApplyFlagGatedActorEightSetup(void);
void SceneState_ApplyRectAndMarkActor16(void);
void SceneState_ApplyRectAndSetupActor19(void);
void SceneState_ApplyRectsByFlag844(s32 flag);
void SceneState_ApplyRectsByFlatla384And962(void);
void SceneState_ApplyStepToSlots15To18(void);
void SceneState_ApplyThreeRects(void);
void SceneState_ApplyThreeRectsRows9And10(void);
void SceneState_ApplyTwoRects(void);
void SceneState_ApplyTwoRectsAtRow56(void);
void SceneState_CallWith432And32(void);
void SceneState_CheckFlags941And940(void);
void SceneState_CheckPositionWindow(void);
void SceneState_ClearSlotsBySubState(void);
void SceneState_ConfigureEntries8Through19(void);
void SceneState_ConfigureRegion26_30AndClearActor18Mode(void);
void SceneState_ConfigureRegion26_30AndMarkActor17(void);
void SceneState_CountDownEveryFortyTicks(void);
void SceneState_DispatchByActorZeroDepth(void);
s32	 SceneState_FindFirstSetFlagOfGroup(u32 sel);
void SceneState_InitActorSlots8To19(void);
void SceneState_InitControlWhenFlag109Clear(s32 resource);
void SceneState_InitFourActorRecordsAndInstallTask(void);
void SceneState_LinkRecordZeroWhenFlag200Clear(void);
void SceneState_RaiseTriggerOnPsynergyTarget(void);
void SceneState_ResetCounterAndStartTask(void);
void SceneState_RunFlagGatedSetupCascade(void);
void SceneState_RunGuardedActorStep(s32 x);
void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b);
void SceneState_Send210AndApplyRectAt40x84(void);
void SceneState_SetFlagByActorPosition(void);
void SceneState_SetMode66AndPassOpeningSequence(void);
void SceneState_TriggerColumnNineteen(void);
void SceneState_TriggerColumnTen(void);
void SceneTable_Prepare(void *);
void ShianJiin_AskIfCurious(void);
s32	 ShindenHeya_ChooseRestartOption(void);
void ShindenHeya_RaiseItemIcon(s32 item);
void ShindenHeya_ResetActorUpdate(void);
void ShindenHeya_UpdateRisingSpark(union FieldObject *object);
void SoruIriguchi_ApplyEntryState(void);
void SoruSekizo_RunSealOpenedSequence(void);
#ifndef OWN_StagedActor_CountdownUntilPositionUnset
s32 StagedActor_CountdownUntilPositionUnset(struct FieldActor *o);
#endif
void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz);
void StagedActor_StepDownUntilClamp(s32 index);
void State_ApplyRectByLayoutSelector(void);
s32	 State_CheckFourCellRun(s32 x, s32 z, s32 mode);
void State_StampRecordCells(s16 *records, s32 value);
void State_UpdateScrollRegistersWithPreset(void);
s32	 StoryActor_ApplyMapRotation(struct FieldActor *actor);
#ifndef OWN_StoryActor_ConfigureSpawnedObject
void StoryActor_ConfigureSpawnedObject(struct FieldActor *actor);
#endif
void StoryScene_StartTransition(void);
void TakaraAshiba_InitializeScene1(void);
void TakaraAshiba_InitializeScene2(void);
void TakaraAshiba_InitializeScene3(void);
void TakaraAshiba_OpenPlatformPassage(s32 a0);
void TakaraAshiba_RaiseTriggerOnStand(void);
void TakaraAshiba_RepaintActorCells(void);
void TakaraAshiba_RestoreActorEightTerrain(void);
void TakaraAshiba_RestoreActorTenLeftCell(void);
void TakaraAshiba_RestoreActorTenRightCell(void);
s32	 TakaraHashira_LowerActorToLedge(s32 id, s32 far);
void TakaraHashira_PushPillar(void);
#ifndef OWN_TakaraHashira_ReadMapCell
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst);
#endif
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src);
s32	 TakaraHashira_SyncPriorityIfAhead(struct FieldActor *front, struct FieldActor *actor);
s32	 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *front);
void TakaraShima_LeaveClearedRoom(void);
void TakaraShima_RestoreRoomActors(void);
void TorebiIzumi_RevealPrize(s32 item);
s32	 TorebiIzumi_RunSpringRide(s32 side);
void TorebiIzumi_SetActorActive(s32 arg0, s32 arg1);
void ToretoEda_StartBg3Split(void);
void ToretoHeya_ApplyFlaggedMapPatches(void);
void ToretoHeya_RunLandingDustScene(void);
void ToretoHeya_UpdateSwirlObject(struct Swirl *obj);
void ToretoPalette_CaptureBank(void);
void Transform_UpdateVertices(void);
s32	 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested);
s32	 UpdateActorProximity(u8 *actor);
s32	 UpdateFacingFromResolvedObject(struct FacingObject *object);
void VinasuChojo_StepSpriteRotation(struct FieldActor *actor);
void VinasuChojo_UpdateOrbitingEffect(union FieldObject *object);
void VinasuHeya_LowerFloatingBlocks(s32 wait);
s32	 VinasuHeya_UpdateRisingSpray(struct FieldActor *actor);
void WorldMap_ActivateSite138(s32 actor);
void WorldMap_CompleteTransfer(void);
void WorldMap_DisembarkCarriage(void);
void WorldMap_RunBlackOrbScene(void);
void WorldMap_RunOpeningRise(void);
void WorldMap_StopLeaderLeavingShip(void);
void WorldMap_TransferToExit102(void);
void WorldMap_TransferToExit103(void);
void WorldMap_TransferToExit104(void);
void WorldMap_TransferToExit106(void);
void WorldMap_TransferToFuneHeya(void);
void WorldMap_UpdateView(void);
void YamaRama_DisableActorCollision(void);
#endif

#if GAME_TLA
struct AnimationObject;
struct EffectPosition;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct ObjectRuntime;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct ScriptInterpreter;

extern const u8					   DriftScene_SpawnScript[];
extern const struct SceneEntrance  gDeriMuraEntrances[];
extern const struct SceneEntrance  gIdejimaEntrances[];
extern const struct SceneEvent	   gIdejimaEvents[];
extern const struct SceneEvent	   gIdejimaEventsEntrance1[];
extern const struct SceneEvent	   gIdejimaEventsWake[];
extern const u32				   gIdejimaExits[];
extern const struct ScenePlacement gIdejimaPlacements[];
extern const struct SceneEntrance  gSceneEntrances[];
extern const struct SceneEvent	   gSceneEvents[];
extern const u32				   gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct ScenePlacement gScenePlacementsEntrance99[];
extern const struct SceneRegion	   gSceneRegions[];
extern struct ObjectRuntime		   gSnapshotObjects[];
extern const u8					   Idejima_ActorDepartureScript[];
extern const u8					   LinkLobby_SlotColumns[];
extern const s32				   LinkLobby_SlotValues[];
extern const u8					   ObjectGroup_BlinkChildValues[4];
extern const u8					   ObjectMotion_StepAngleScript[];
extern u16						   Script_MainScript[];
extern const u8					   WorldMap_TerrainBehaviorTable[];
s32								   Animation_InitializeObjects(struct AnimationObject *);
void							   Animation_SetStateFlags(struct FieldActor *object, s32 flags);
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *, s32);
s32	 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
void BattleFx_ClearChildValueOnMismatch(void);
s32	 BattleFx_DispatchRequestKind(void);
void BattleFx_LoadActionEffectResources(s32 action, s32 flags);
s32	 BattleFx_Run(void);
s32	 BattleFx_SelectBattleCue(s32 row, s32 column);
void BattleFx_SetupObjectPair(s32 actor, s32 target);
s32	 BattleFx_StartBufferInterpolationFar(s32);
void BattleMotion_ProjectScaledPositionFar(s32, struct EffectPosition *);
void BattlePres_RunBurstScene(struct EffectStep *, s32);
void EffectRuntime_StopCurrentObject(void);
s32	 Event_SpawnObjectTable(s32 event_id, s32 state);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second);
void Field_BeginPaletteTransition(s32 mode);
void Field_DispatchTypeHandler(s32 kind);
void Field_EndActorSpriteEffect(s32 mode);
void FieldEvent_RunTypeHandler(void);
void Func_02000b2c(void);
void Func_02000d30(void);
void Func_02001c7c(s32, s32);
void Func_02001c84(s32, s32);
void Func_02001c8c(void);
void Func_02001c94(void);
void Func_02001c9c(void);
void Func_020023a4(void *object);
void Func_0200258c(void);
void Func_0200260c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02002a34(s32 a, s32 b);
void Func_02002a44(void);
void Func_02002a4c(void);
void Func_02002a54(void);
void Func_02002a5c(struct ObjectRuntime *object, s32 mode);
s32	 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);
void Func_080caa2c(u32 object_id);
void Object_SetSquareSprite(u32 object_id);
void Func_080e70f8(u32 object_id);
void Func_081b83c4(void);
void GameFlag_RefreshLureCapFar(void);
void Map_ApplyWorkOriginAndSpanFar(void);
s32	 Map_GetTerrainHeightFar(u8, s32, s32);
void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant);
s32	 Object_IsTargetUnset(void *arg0);
#ifndef OWN_Object_SetPartAttribute
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
#endif
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void ObjectDispatch_InitFromTable4WithArgumentFar(void *object, void *effect);
void ObjectEffect_EndContextEffect(s32 effect);
void ObjectEffect_PrepareContextEffect(s32 effect);
void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset);
void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z);
void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant);
void ObjectSystem_Configure(s32 mode);
s32	 Party_AddActiveOwnerFar(s32 event_id);
void Party_RemoveActiveOwnerFar(s32 owner);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale, u16 angle);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale, s32 angle, s32 layer);
s32	 Render_ProjectPoint(s32 *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32	 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
void UiTimedNotice_CloseIfActiveFar(void);
s32	 UiWork_IsIdleFar(s32);
#endif

#endif
