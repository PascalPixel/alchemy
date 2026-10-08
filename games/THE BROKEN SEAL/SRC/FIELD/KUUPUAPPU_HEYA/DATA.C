#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"
#include "OBJCMD.H"
#include "text/MSG_IDS.H"

void ActorPresentation_SetSceneCellByAngle(void);
void FieldScene_RunActorEighteenConditionalScene(void);
void FieldScene_RunActorNineteenAngleDialogue(void);
void FieldScene_RunActorTwentyAngleDialogue(void);
void FieldScene_RunActorTwentyThreeAngleDialogue(void);
void FieldScene_RunFlag856DialogueBranch(void);
void FieldScene_RunObjectTwentySixPositionCheck(void);
void FieldScene_RunOpeningSequenceHead(void);
void FieldScene_RunOpeningSequenceSecond(void);
void FieldScene_RunOpeningSequenceThird(void);
void FieldScene_RunScene383SequenceB(void);
void FieldScene_RunScene383SequenceC(void);
void FieldScene_RunSetupSequence(void);
void FieldScene_RunSteps107And250(void);
void FieldScene_RunVaultClosingSequence(void);
void FieldScene_SelectActorPair(void);
void KuupuappuHeya_OfferBone(void);
void KuupuappuHeya_PoseDialogueActors(void);
void KuupuappuHeya_RunActor25Response(void);
void KuupuappuHeya_TalkAboutIvan(void);
void SceneActor_StepActor24AnimationByFacing(void);
void SceneDialogue_RunActor10Line(void);
void SceneDialogue_RunActor11Line(void);
void SceneDialogue_RunActor14Line(void);
void SceneDialogue_RunActor16FlaggedLine(void);
void SceneDialogue_RunActor9FlaggedLine(void);
void SceneDialogue_RunActorEighteenBranchedDialogue(void);
void SceneDialogue_RunActorElevenDialogue(void);
void SceneDialogue_RunActorElevenFlaggedDialogue(void);
void SceneDialogue_RunActorNineFlaggedDialogue(void);
void SceneDialogue_RunActorTwelveFlaggedDialogue(void);
void SceneDialogue_ShowEmptyBarrel(void);
void SceneDialogue_ShowEmptyChest(void);
void SceneDialogue_ShowLine124EOr135E(void);
void SceneDialogue_ShowLine128E(void);
void SceneDialogue_ShowLine12BB(void);
void SceneState_BranchOnSlotZeroFacingAndFlag855(void);
void SceneState_SetValue123Mode11(void);

u32 KuupuappuHeya_Stops[] = {
	4, 1, 0x801e, 0xffff, 6, 2, 0x8000, 0xffff,
	8, 3, 0x8001, 0x401f, 10, 4, 0x8002, 0xffff,
	12, 5, 0x8003, 0xffff, 14, 6, 0x8004, 0xffff,
	16, 7, 0x8005, 0x4022, 18, 8, 0x8006, 0xffff,
	20, 9, 0x8007, 0xffff, 22, 10, 0x8008, 0xffff,
	24, 0x400b, 0x8009, 0xffff, 536, 0x400c, 0xc00a, 0xffff,
	1048, 0x400d, 0xc00b, 0xffff, 1560, 0x400e, 0xc00c, 0xffff,
	2072, 0x800f, 0xc00d, 0xffff, 2070, 0x8010, 14, 0xffff,
	2068, 0x8011, 15, 0xffff, 2066, 0x8012, 16, 0xffff,
	2064, 0x8013, 17, 0xc024, 2062, 0x8014, 18, 0xffff,
	2060, 0x8015, 19, 0xffff, 2058, 0x8016, 20, 0xffff,
	2056, 0x8017, 21, 0xc021, 2054, 0x8018, 22, 0xffff,
	2052, 0x8019, 23, 0xffff, 2050, 0x801a, 24, 0xffff,
	2048, 0xc01b, 25, 0xffff, 1536, 0xc01c, 0x401a, 0xffff,
	1024, 0xc01d, 0x401b, 0xffff, 512, 30, 0x401c, 0xffff,
	258, 0, 0x801d, 0xffff, 520, 0x4020, 0xc002, 0xffff,
	1032, 0x4021, 0xc01f, 0xffff, 1544, 0x4016, 0xc020, 0xffff,
	528, 0x4023, 0xc006, 0xffff, 1040, 0x4024, 0xc022, 0xffff,
	1552, 0x4012, 0xc023, 0xffff,
};

s32 KuupuappuHeya_ActionTable[] = {
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(180),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KuupuappuHeya_Villager11ExitScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(400)),
	SC_SYNC,
	SC_MOVE(PIXELS(800), 0, PIXELS(400)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(20),
	SC_END,
};

s32 KuupuappuHeya_Villager12ExitScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(416)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(20),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptA[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(744), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptB[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(6),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(744), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptC[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(792), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(440)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(840), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(856), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptD[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(808), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(840), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(856), 0, PIXELS(424)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptE[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(840), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(104), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptB[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(80), 0, PIXELS(640)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptC[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptD[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(232), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptE[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(632)),
	SC_SYNC,
	SC_MOVE(PIXELS(232), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptF[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(104), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptG[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(80), 0, PIXELS(640)),
	SC_SYNC,
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptH[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptI[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(232), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptJ[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(632)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(232), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptK[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(648)),
	SC_SYNC,
	SC_MOVE(PIXELS(72), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptL[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptM[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(648)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptN[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptO[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptP[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(120), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptQ[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptR[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

struct SceneEntrance KuupuappuHeya_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0 },
	{ 6, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0 },
	{ 7, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0 },
	{ 8, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0 },
	{ 9, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0 },
	{ 10, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0 },
	{ 11, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0 },
	{ 12, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0 },
	{ 13, CONDITION_ALWAYS, 648, 0, 424, FACING_SOUTH, 0, 560, 320, 800, 480, 0 },
	{ 14, CONDITION_ALWAYS, 704, 0, 408, FACING_WEST, 0, 560, 320, 800, 480, 0 },
	{ 15, CONDITION_ALWAYS, 768, 0, 408, FACING_EAST, 0, 712, 288, 952, 496, 0 },
	{ 16, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
	{ 17, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
	{ SCENE_TABLE_END },
};

s16 KuupuappuHeya_Regions[] = {
	639, 33, 407, 657, 51, 425, -1, 13,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 KuupuappuHeya_Messages[] = {
	SCENE_EXITS(21),
	SCENE_EXIT(5, 20, 5),
	SCENE_EXIT(6, 20, 6),
	SCENE_EXIT(7, 20, 7),
	SCENE_EXIT(8, 20, 8),
	SCENE_EXIT(9, 20, 9),
	SCENE_EXIT(10, 20, 10),
	SCENE_EXIT(11, 21, 11),
	SCENE_EXIT(12, 21, 12),
	SCENE_EXIT(13, 20, 13),
	SCENE_EXIT(14, 21, 14),
	SCENE_EXIT(15, 21, 15),
	SCENE_EXIT(16, 9, 12),
	SCENE_EXITS_END,
};

struct ScenePlacement KuupuappuHeya_SceneTableA[] = {
	{ ACTOR_GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ ACTOR_IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, 0x0, 3,
		PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_WANDER,
		PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_WANDER,
		PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(752), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_STAND,
		PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x66, 0x0, ACTOR_STAND,
		PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x75, 0x0, ACTOR_STAND,
		PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_WANDER,
		PIXELS(536), 0, PIXELS(672), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(632), 0, PIXELS(136), FACING_SOUTH, 3, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(664), 0, PIXELS(632), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_STAND,
		PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x3f, 0x854, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
	{ 0x40, 0x854, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xdf, 0x0, ACTOR_STAND,
		PIXELS(692), 0, PIXELS(411), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement KuupuappuHeya_SceneTableB[] = {
	{ ACTOR_GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ ACTOR_IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(160), 0, PIXELS(136), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x75, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(504), 0, PIXELS(456), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x3f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
	{ 0x40, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x40, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, 0x0, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, 0x0, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, 0x0, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xfb, CONDITION_ALWAYS, 7,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xdd, CONDITION_ALWAYS, 7,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKuupuappuHeyaEvents[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetValue123Mode11) },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
	{ EVENT_TOUCH, 10, IF_FLAG_SET(0x856), EVENT_SCRIPT(FieldScene_RunOpeningSequenceThird) },
	{ EVENT_TOUCH, 11, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 12, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 13, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 14, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 15, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 16, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 17, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 18, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 20, IF_FLAG_SET(0x300), EVENT_SCRIPT(FieldScene_RunScene383SequenceB) },
	{ EVENT_TOUCH, 21, IF_FLAG_SET(0x852), EVENT_SCRIPT(FieldScene_RunSteps107And250) },
	{ EVENT_RAISED, 201, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SelectActorPair) },
	{ EVENT_RAISED, 202, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SelectActorPair) },
	{ EVENT_RAISED, 203, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SelectActorPair) },
	{ EVENT_RAISED, 250, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine12BB) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningSequenceHead) },
	{ 0x202, 40, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetSceneCellByAngle) },
	{ EVENT_TALK, 2, IF_FLAG_SET(0x250), 0 },
	{ EVENT_TALK, 2, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSetupSequence) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThisRockWasBlownHereAll) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor9FlaggedLine) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMyNeighborCreepsMeOutHe) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorElevenDialogue) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorTwelveFlaggedDialogue) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuEverSinceTheEruptionGrandpaKeeps) },
	{ EVENT_TALK, 14, 0x856, MSGNO(MsgKuupuappuICouldHelpIvanIfI) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYoureHelpingIvanYouDontLook) },
	{ EVENT_TALK, 15, 0x856, EVENT_SCRIPT(KuupuappuHeya_TalkAboutIvan) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuSinceYoureHelpingIvanIllTell) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFlag856DialogueBranch) },
	{ EVENT_TALK, 17, 0x856, MSGNO(MsgKuupuappuMyHusbandTheMayorCantBelieve) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuReallyYoureGoingToHelpIvan) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine128E) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorNineteenAngleDialogue) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyAngleDialogue) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_BranchOnSlotZeroFacingAndFlag855) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheGuestsUpstairsWerentInTheir) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyThreeAngleDialogue) },
	{ EVENT_TALK, 24, IF_FLAG_SET(0x250), 0 },
	{ EVENT_TALK, 25, IF_FLAG_SET(0x250), 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, IF_FLAG_SET(0x250), 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, IF_FLAG_SET(0x250), 0 },
	{ EVENT_TALK, 24, 0x853, MSGNO(MsgKuupuappuThatKidIvanHeCreepsMe) },
	{ EVENT_TALK, 25, 0x853, MSGNO(MsgKuupuappuWeHaventDoneAnythingYoureA) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_StepActor24AnimationByFacing) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuHeya_RunActor25Response) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheOtherSparklingRocksAllCrumbled) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorNineFlaggedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuHeyMaybeMyNeighborIsThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorElevenFlaggedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine124EOr135E) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYouNeverBoughtMeAnythingWorth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheseGuysLookReliableIGuess) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWonderWhatSortOfThing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor16FlaggedLine) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMaybeTheyllFindOurStolenFamily) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorEighteenBranchedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuAllOurBestWeaponsWereStolen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWonderIfTheyLeftUs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKuupuappuHahhWereSureToSeeA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuItWouldBeAwfulIfOur) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThatVolcanoCostUsALot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x418, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_StepActor24AnimationByFacing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x419, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuHeya_RunActor25Response) },
	{ PSYNERGY_ON_ACTOR(0x8c), 26, 0x859, EVENT_SCRIPT(FieldScene_RunObjectTwentySixPositionCheck) },
	{ SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyBarrel) },
	{ SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
	{ 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledHistoryOf)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledGemsOf)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKuupuappuHeyaEventsEntrances15To17[] = {
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningSequenceSecond) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunVaultClosingSequence) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyChest) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyChest) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyChest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuAhhHelpMe) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKuupuappuHeyaEventsFlag855[] = {
	{ SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorEighteenConditionalScene) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningSequenceHead) },
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningSequenceSecond) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuICanSleepWellNowThat) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor9FlaggedLine) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor10Line) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11Line) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorTwelveFlaggedDialogue) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuJustAsIThoughtNothingWas) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor14Line) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIvanLearnedALotByTalking) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene383SequenceC) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYoureThePersonWhoGotOur) },
	{ EVENT_TALK, 18, 0x250, EVENT_SCRIPT(KuupuappuHeya_OfferBone) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorNineteenAngleDialogue) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyAngleDialogue) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_BranchOnSlotZeroFacingAndFlag855) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuImShockedThoseGuestsOfOurs) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyThreeAngleDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuEverybodysAfterMyRockTheyWant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorNineFlaggedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheReasonLunpaTurnedBadIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorElevenFlaggedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine124EOr135E) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWishIHadSomethingWorth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhenIGrowUpIWant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWishIHadWingsLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor16FlaggedLine) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuImSoRelievedNowThatWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorEighteenBranchedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMyWeaponsWereFoundInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuItMustBeSnowingUpIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhyAreThesePeopleSoAnxious) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhoCouldHaveKnownTheyWere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKuupuappuOfCourseIAlwaysSuspectedThey) },
	{ SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
	{ 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledHistoryOf)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledGemsOf)) },
	{ SCENE_EVENTS_END },
};

s32 *KuupuappuHeya_StepActions[] = {
	KuupuappuHeya_PairScriptC,
	KuupuappuHeya_PairScriptD,
	KuupuappuHeya_PairScriptJ,
	KuupuappuHeya_PairScriptH,
	KuupuappuHeya_PairScriptF,
	KuupuappuHeya_PairScriptB,
};

s32 *KuupuappuHeya_IdleActions[] = {
	KuupuappuHeya_PairScriptG,
	KuupuappuHeya_PairScriptA,
	KuupuappuHeya_PairScriptC,
	KuupuappuHeya_PairScriptE,
	KuupuappuHeya_PairScriptI,
	KuupuappuHeya_PairScriptH,
};

s32 *KuupuappuHeya_ResponseScripts[] = {
	KuupuappuHeya_PairScriptM,
	KuupuappuHeya_PairScriptQ,
	KuupuappuHeya_PairScriptP,
	KuupuappuHeya_PairScriptL,
	KuupuappuHeya_PairScriptO,
	KuupuappuHeya_PairScriptK,
	KuupuappuHeya_PairScriptN,
	KuupuappuHeya_PairScriptR,
};
