#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/field/takara_shima/shima.h"
#include "unit/field/common/effect/advance.h"
#include "unit/field/common/object/orbit.h"
#include "unit/field/common/effect/create.h"
#include "unit/field/common/object/staged_actor.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/slots.h"
#include "unit/field/common/script/script.h"
#include "unit/system/scheduler/callback.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "field_effect.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "field_service.h"
#include "call.h"
#include "scene_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

struct Sprite
{
	u8	pad00[9];
	u8	flags9;
	u8	pad0a[20];
	u16 angle;
	u8	pad20[6];
	u8	state26;
};

struct Effect
{
	u8			   pad00[8];
	s32			   position[3];
	u8			   pad14[4];
	s32			   accum18;
	s32			   accum1c;
	u8			   pad20[3];
	u8			   flatla3;
	u8			   pad24[12];
	s32			   rate30;
	s32			   rate34;
	u8			   pad38[12];
	s32			   velocity[3];
	struct Sprite *sprite;
	u8			   pad54;
	u8			   mode55;
	u8			   pad56[14];
	u16			   step64;
	u8			   pad66[6];
	u32			   callback;
};

typedef struct Obj
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	f14[28];
	s32 f30;
	s32 f34;
} Obj;

struct S_02000474
{
	s32 f00;
	u16 f04;
	u16 f06;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	pad14[20];
	s32 f28;
	u8	pad2c[4];
	s32 f30;
	s32 f34;
	u8	pad38[29];
	u8	f55;
};

struct V
{
	s32 a;
	s32 b;
	s32 c;
};

struct SceneObject
{
	u8	filler00[8];
	s32 x;
	s32 y;
	s32 z;
};

struct Vec
{
	s32 x;
	s32 y;
	s32 z;
};

struct EffectParams
{
	u8	filler00[0x24];
	s32 callback;
};

typedef union
{
	s32 w;
	s16 h[2];
} RecWord;

struct S_02001b14
{
	s32 pad[2];
	s32 f08;
	s32 f0c;
	s32 f10;
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/staged_actor.c"
#include "../common/effect/create.c"
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"

struct Owner
{
	u8 unk0[9];
	u8 unk9_0 : 2;
	u8 mode : 2;
	u8 unk9_4 : 4;
};

union GameStateRows
{
	s16 halves[512][1];
};

void NoOpEffectCallback(void)
{
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_TakaraShima1)
	{
		return gTakaraShimaEntrances1;
	}
	if (scene == (s32)&SceneId_TakaraShima2)
	{
		return gTakaraShimaEntrances2;
	}
	if (scene == (s32)&SceneId_TakaraShima3)
	{
		return gTakaraShimaEntrances3;
	}
	if (scene == (s32)&SceneId_TakaraShima4)
	{
		return gTakaraShimaEntrances4;
	}
	if (scene == (s32)&SceneId_TakaraShima5)
	{
		return gTakaraShimaEntrances5;
	}
	return gTakaraShimaEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gTakaraShimaExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_TakaraShima1)
	{
		return gTakaraShimaPlacements1;
	}
	if (scene == (s32)&SceneId_TakaraShima3)
	{
		return gTakaraShimaPlacements3;
	}
	if (scene <= (s32)&SceneId_TakaraShima14 && scene >= (s32)&SceneId_TakaraShima6)
	{
		return gTakaraShimaPlacements6To14;
	}
	return gTakaraShimaPlacementsOther;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/advance.c"

void SpawnRadialEffectBurst(s32 object_id)
{
	struct SceneObject *object;
	struct Vec			vec;
	struct EffectParams params;
	u32					angle_step;
	s32					angle;
	s32					x;
	s32					z;

	object = (struct SceneObject *)Object_GetById(object_id);
	params.callback = (s32)Effect_AdvanceMotion;
	for (angle_step = 0; angle_step <= 16; angle_step += 2)
	{
		angle = angle_step << 12;
		vec.x = Engine_MathCos(angle);
		vec.y = 0;
		z = Engine_MathSin(angle);
		x = vec.x;
		vec.z = z;
		x += Math_Divide(x, 3);
		vec.x = x;
		Effect_Spawn(object->x, object->y, object->z, x, vec.y, z, 0x01000001, &params);
	}
}

void InitializePrologueSceneState(void)
{

	u8 *base;

	Engine_EventBegin();
	base = (u8 *)&gGameState;
	Party_SetFields1ceAnd1d0(*(s16 *)(base + 448), 5);
	base[555] = 3;
	BattleFx_SetWeightedResult(84, 5);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_TakaraShima1)
	{
		return gTakaraShimaEvents1;
	}
	if (scene == (s32)&SceneId_TakaraShima2)
	{
		return gTakaraShimaEvents2;
	}
	if (scene == (s32)&SceneId_TakaraShima3)
	{
		return gTakaraShimaEvents3;
	}
	if (scene == (s32)&SceneId_TakaraShima4)
	{
		return gTakaraShimaEvents4;
	}
	if (scene == (s32)&SceneId_TakaraShima5)
	{
		return gTakaraShimaEvents5;
	}
	return gTakaraShimaEventsOther;
}

void StartScriptedSceneMessage(s32 message_id)
{
	((void (*)(s32 arg0))Engine_EventBegin)(message_id);
	Engine_ActorSetPosition(8, 0, 0);
	Engine_GameFlagSet(4055);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
	Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
	struct FieldActor *actor;
	s32				   scene;
	s32				   current;
	s32				   first;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	scene = gGameState.scene;
	if (scene == (s32)&SceneId_TakaraShima3)
	{
		InitializeEscapeSceneActors();
		return 0;
	}
	if (scene == (s32)&SceneId_TakaraShima5)
	{
		if (GameFlag_IsSet(0xef7) == 0)
		{
			Map_CopyCellAttributes(0, 3, 1, 1, 13, 40);
			Map_CopyCellAttributes(0, 2, 1, 1, 15, 40);
			MapObject_SetPosition(101, 0xd80000, 0x2880000);
		}
		if (gGameState.scene == scene)
		{
			if (gGameState.entrance != 5)
			{
				if (GameFlag_IsSet(0x8d1) == 0)
				{
					return 0;
				}
			}
			GameFlag_Set(0x8d1);
			Map_CopyCellAttributes(0, 1, 1, 1, 13, 30);
			MapObject_SetPosition(100, 0xd80000, 0x1e80000);
			return 0;
		}
	}
	current = gGameState.scene;
	if (current == (s32)&SceneId_TakaraShima1)
	{
		TakaraShima_RestoreRoomActors();
		Actor_Get(8)->target_x = 0x810000;
		FieldScene_RedrawActorFootprint(9);
		FieldScene_RedrawActorFootprint(10);
		if (GameFlag_IsSet(0x240) != 0)
		{
			actor = Actor_Get(11);
			if (actor != 0)
			{
				actor->collision_flags = 0;
				Object_SetMode(actor, 4);
				Engine_ActorSetSpriteFlags(actor, 0);
			}
			Field_SetMapCellCollision(0, 0x1300000, 0x1700000, 253);
		}
		if (GameFlag_IsSet(0x241) != 0)
		{
			actor = Actor_Get(12);
			if (actor != 0)
			{
				actor->collision_flags = 0;
				Object_SetMode(actor, 4);
				Engine_ActorSetSpriteFlags(actor, 0);
			}
			Field_SetMapCellCollision(0, 0x500000, 0x1700000, 253);
		}
		if (GameFlag_IsSet(0x242) != 0)
		{
			actor = Actor_Get(13);
			if (actor != 0)
			{
				actor->collision_flags = 0;
				Object_SetMode(actor, 4);
				Engine_ActorSetSpriteFlags(actor, 0);
			}
			Field_SetMapCellCollision(0, 0x600000, 0x1500000, 253);
		}
		if (GameFlag_IsSet(0x243) != 0)
		{
			actor = Actor_Get(14);
			if (actor != 0)
			{
				actor->collision_flags = 0;
				Object_SetMode(actor, 4);
				Engine_ActorSetSpriteFlags(actor, 0);
			}
			Field_SetMapCellCollision(0, 0x900000, 0x1400000, 253);
			Field_SetMapCellCollision(0, 0x2f00000, 0x1400000, 253);
		}
		if (GameFlag_IsSet(0xfd7) != 0)
		{
			return 0;
		}
		InitializeStagedActorSceneOrbitingEffect(8);
		return 0;
	}
	first = (s32)&SceneId_TakaraShima6;
	if (current == first)
	{
		if (GameFlag_IsSet(0xef4) == 0)
		{
			Map_CopyCellAttributes(0, 0, 1, 1, 37, 10);
			MapObject_SetPosition(100, 0x2580000, 0xa80000);
		}
	}
	current = gGameState.scene;
	if (current >= first)
	{
		if (current <= (s32)&SceneId_TakaraShima14)
		{
			FieldScene_RunScene3b2SequenceA();
			if (gGameState.entrance == 5)
			{
				TakaraShima_LeaveClearedRoom();
			}
		}
	}
	return 0;
}

void FieldScene_RunScene3b2SequenceA(void)
{
	struct FieldActor *actor;
	s32				   cleared;

	actor = Actor_Get(8);
	cleared = GameFlag_IsSet((((union GameStateRows *)&gGameState)->halves[224][0]
							  + (0x8d2 - (s32)&SceneId_TakaraShima6)));
	if (cleared != 0)
	{
		Actor_SetPosition(8, 0x28a0000, 0xa80000);
		actor->y.fixed = -0x200000;
		Engine_ActorSetSpriteFlags(Actor_Get(8), 0);
		Engine_ActorSetSpritePriority(8, 3);
		actor->motion_flags = 0;
		actor->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		Map_CopyCellAttributes(42, 10, 1, 1, 40, 10);
	}
	else
	{
		Object_GetById(8)->motion_flags = cleared;
	}
}

void FieldScene_HandleEscapeColumn(void)
{
	struct FieldActor *entity;
	s16				  *slot;
	s32				   column;
	u8				   zero;

	entity = Object_GetById(8);
	column = entity->x.fixed >> 20;
	if (column != 40)
	{
		return;
	}

#if EDITION_INTERNATIONAL
	{
		s32 off = 448;
		slot = (s16 *)((u8 *)((s16 *)&gGameState) + off);
	}
	if (Engine_GameFlagIsSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6)) != 0)
	{
		return;
	}
#endif

	entity->motion_flags = 3;
#if !EDITION_INTERNATIONAL
	zero = 0;
#endif

	Engine_EventWait(8);
	SpawnRadialEffectBurst(8);
	Engine_AudioPlayCue(136);
	Engine_EventWait(40);

#if EDITION_INTERNATIONAL
	Engine_ActorSetSpriteFlags((u8 *)Object_GetById(8), 0);
#else
	Engine_ActorSetSpriteFlags((u8 *)Object_GetById(8), zero);
#endif
	Engine_ActorSetSpritePriority(8, 3);

#if EDITION_INTERNATIONAL
	entity->motion_flags = 0;
#else
	entity->motion_flags = zero;
#endif
	{
		s32 flags = 2;

		flags |= entity->priority_flags;
		entity->priority_flags = flags;
	}

	Engine_MapCopyCellAttributes(42, 10, 1, 1, column, 10);

#if !EDITION_INTERNATIONAL
	{
		s32 off = 448;
		slot = (s16 *)((u8 *)((s16 *)&gGameState) + off);
	}
#endif
	Engine_GameFlagSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6));
}

void RunPrologueSceneSetup(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	FieldScene_HandleEscapeColumn();
}

void StartSceneScript37(void)
{
	Engine_MapAnimateCells(TakaraShima_EntranceCells, 37, 7);
	Engine_AudioPlayCue(183);
	Engine_EventRequestExit(4);
}

void TakaraShima_RunAreaEntry(void)
{
	if (Engine_GameFlagIsSet(gGameState.scene + (0x8c8 - (s32)&SceneId_TakaraShima6)) == 0)
	{
		Engine_EventBegin();
		Party_SetFields1ceAnd1d0(gGameState.scene, 5);
		((u8 *)((s16(*)[1]) & gGameState))[0x22b] = 3;
		switch (gGameState.scene - (s32)&SceneId_TakaraShima6)
		{
		case 0:
			BattleFx_SetWeightedResult(63, 0);
			break;
		case 1:
			BattleFx_SetWeightedResult(63, 1);
			break;
		case 2:
			BattleFx_SetWeightedResult(63, 2);
			break;
		case 3:
			BattleFx_SetWeightedResult(63, 3);
			break;
		case 4:
			BattleFx_SetWeightedResult(84, 0);
			break;
		case 5:
			BattleFx_SetWeightedResult(84, 1);
			break;
		case 6:
			BattleFx_SetWeightedResult(84, 2);
			break;
		case 7:
			BattleFx_SetWeightedResult(84, 3);
			break;
		case 8:
			BattleFx_SetWeightedResult(84, 4);
			break;
		}
		Engine_EventEnd();
	}
	else
	{
		Engine_MapAnimateCells((s32)TakaraShima_EntranceCells, 44, 7);
		Engine_AudioPlayCue(183);
		Engine_EventRequestExit(3);
	}
}

void TakaraShima_LeaveClearedRoom(void)
{
	u32 i;
	s32 record;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set((((union GameStateRows *)&gGameState)->halves[224][0]
				  + (0x8c8 - (s32)&SceneId_TakaraShima6)));
	Engine_EventWait(30);
	Map_AnimateCells(((const u16 *)TakaraShima_EntranceCells), 44, 7);
	Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	Engine_EventRequestExit(3);
	Engine_EventEnd();
}

void ConfigureSceneActor11(s32 actor_id)
{
	s32				   a = 0x1300000;
	s32				   b = 0x1700000;
	struct FieldActor *p = Object_GetById(11);

	if (p != 0)
	{
		p->collision_flags = 0;
	}
	Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
	SetMapCellCollision(0, a, b, 253);
	Engine_GameFlagSet(576);
}

void ConfigureSceneActor12(s32 actor_id)
{
	s32				   a = 0x500000;
	s32				   b = 0x1700000;
	struct FieldActor *p = Object_GetById(12);

	if (p != 0)
	{
		p->collision_flags = 0;
	}
	Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
	SetMapCellCollision(0, a, b, 253);
	Engine_GameFlagSet(577);
}

void ConfigureSceneActor13(s32 actor_id)
{
	s32				   a = 0x600000;
	s32				   b = 0x1500000;
	struct FieldActor *p = Object_GetById(13);

	if (p != 0)
	{
		p->collision_flags = 0;
	}
	Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
	SetMapCellCollision(0, a, b, 253);
	Engine_GameFlagSet(578);
}

void ConfigureSceneActor14(s32 actor_id)
{
	s32				   a = 0x900000;
	s32				   b = 0x1400000;
	s32				   c = 0x2f00000;
	s32				   d = 0x1400000;
	struct FieldActor *p = Object_GetById(14);

	if (p != 0)
	{
		p->collision_flags = 0;
	}
	Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
	SetMapCellCollision(0, a, b, 253);
	SetMapCellCollision(0, c, d, 253);
	Engine_GameFlagSet(579);
}

void ShowForgetEverythingMessage(void)
{
	Engine_GameFlagSet(2244);
	{
		s32 k4 = 8, k5 = 21;

		Engine_MapCopyCellAttributes(0, 0, 1, 1, k4, k5);
	}
}

void ShowHelpYouForgetMessage(void)
{
	Engine_GameFlagSet(2245);
}

void ShowDamagedDoorMessage(void)
{
	Engine_GameFlagSet(2246);
}

void ShowSaveMyLifeMessage(void)
{
	Engine_GameFlagSet(2247);
}

void TakaraShima_RestoreRoomActors(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(0x8c4) != 0)
	{
		Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
		Actor_SetPosition(15, 0x3c80000, 0x3c80000);
	}
	else
	{
		record = Actor_Get(15);
		record->scale_y = 0x19999;
	}
	if (GameFlag_IsSet(0x8c5) != 0)
	{
		Actor_SetPosition(16, 0x3c80000, 0x3c80000);
	}
	else
	{
		record = Actor_Get(16);
		record->scale_y = 0x19999;
	}
	if (GameFlag_IsSet(0x8c6) != 0)
	{
		Actor_SetPosition(17, 0x3c80000, 0x3c80000);
	}
	else
	{
		record = Actor_Get(17);
		record->scale_y = 0x19999;
	}
	if (GameFlag_IsSet(0x8c7) != 0)
	{
		Actor_SetPosition(18, 0x3c80000, 0x3c80000);
	}
	else
	{
		record = Actor_Get(18);
		record->scale_y = 0x19999;
	}
}

void RunSceneVectorTransition(void)
{
	struct V6 transition;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&transition) != 0)
	{
		((void (*)(struct V6))SceneActor_MoveAndRedraw)(transition);
	}
	Engine_EventEnd();
}

void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset)
{
	Obj *p;
	Obj *q;
	s32	 x;
	s32	 y;

	p = Object_GetById(gGameState.selected_actor);
	q = Object_GetById(actor_id);
	Engine_EventBegin();
	{
		x = ((p->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
		y = ((p->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

		p->f30 = 0x10000;
		p->f34 = 0x8000;
		Engine_ObjectSetPosition((struct FieldActor *)p, x, p->f0c, y);
	}
	Object_SetMode(p, 27);
	{
		x = ((q->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
		y = ((q->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

		q->f30 = 0x10000;
		q->f34 = 0x8000;
		Engine_ObjectSetPosition((struct FieldActor *)q, x, q->f0c, y);
	}
	if (x_offset < 0 || z_offset < 0)
	{
		Object_SetMode(q, 4);
	}
	else
	{
		Object_SetMode(q, 3);
	}
	Engine_AudioPlayCue(226);
	Object_CommitPosition(p);
	Engine_AudioPlayCue(288);
	Engine_EventEnd();
}

void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused)
{
	struct S_020009dc *p = (struct S_020009dc *)Object_GetById(actor_mode);

	if (p != 0)
	{
		s32 v;

		Engine_ActorSetSpritePriority(actor_mode, 3);
		v = 2;
		v |= p->f23;
		p->f23 = v;
		{
			s32 k5 = p->f10 >> 20;
			s32 k4 = (p->f08 >> 20) - 1;

			Engine_MapCopyCellAttributes(grid_value, grid_attribute, 3, 1, k4, k5);
		}
	}
}

void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused)
{
	struct S_020009dc *p = (struct S_020009dc *)Object_GetById(actor_mode);

	if (p != 0)
	{
		s32 v;

		Engine_ActorSetSpritePriority(actor_mode, 3);
		v = 2;
		v |= p->f23;
		p->f23 = v;
		{
			s32 k4 = p->f08 >> 20;
			s32 k5 = (p->f10 >> 20) - 1;

			Engine_MapCopyCellAttributes(grid_value, grid_attribute, 1, 3, k4, k5);
		}
	}
}

void SetSceneActorModes(int actor_id)
{
	Engine_ActorSetAnimation(actor_id, 1);
	Engine_ActorSetAnimation(actor_id, 2);
}

void InitializeEscapeSceneActors(void)
{
	MarkGridLeftOfSceneActor(8, 0x11, 0x1E, 0x15);
	MarkGridLeftOfSceneActor(0xA, 0x11, 0x1F, 0x16);
	MarkGridAboveSceneActor(0xB, 0x14, 0x1E, 0x17);
	MarkGridAboveSceneActor(0xC, 0x15, 0x1E, 0x18);
	MarkGridAboveSceneActor(0xD, 0x16, 0x1E, 0x19);
	MarkGridAboveSceneActor(0xF, 0x17, 0x1E, 0x1A);
	MarkGridLeftOfSceneActor(0x11, 0, 0x1E, 0x1F);
	MarkGridLeftOfSceneActor(0x12, 0, 0x1F, 0x20);
	MarkGridLeftOfSceneActor(9, 0, 0x20, 0x21);
	MarkGridAboveSceneActor(0x13, 4, 0x1E, 0x22);
	MarkGridAboveSceneActor(0xE, 5, 0x1E, 0x23);
	MarkGridAboveSceneActor(0x10, 6, 0x1E, 0x24);
}

void ActivateSceneActor8(void)
{
	SetSceneActorModes(8);
}

void ActivateSceneActor10(void)
{
	SetSceneActorModes(10);
}

void ActivateSceneActor11(void)
{
	SetSceneActorModes(11);
}

void ActivateSceneActor12(void)
{
	SetSceneActorModes(12);
}

void ActivateSceneActor13(void)
{
	SetSceneActorModes(13);
}

void ActivateSceneActor15(void)
{
	SetSceneActorModes(15);
}

void ActivateSceneActor17(void)
{
	SetSceneActorModes(17);
}

void ActivateSceneActor18(void)
{
	SetSceneActorModes(18);
}

void ActivateSceneActor9(void)
{
	SetSceneActorModes(9);
}

void ActivateSceneActor19(void)
{
	SetSceneActorModes(19);
}

void ActivateSceneActor14(void)
{
	SetSceneActorModes(14);
}

void ActivateSceneActor16(void)
{
	SetSceneActorModes(16);
}

s32 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested)
{
	u8 *state = &actor->f55;
	s32 saved_state = *state;
#if EDITION_INTERNATIONAL
	struct V destination;

	destination.a = (actor->f08 & 0xfff00000) + 0x80000;
	destination.b = actor->f0c;
	destination.c = (actor->f10 & 0xfff00000) + 0x80000;
	{
		s32 direction = (actor->f06 + 0x2000) & 0xc000;

		Vector_AddPolarOffset(0x200000, direction, &destination);
	}
	if (Object_CheckMovementCollision(actor, &destination) == 0)
	{
#else
	if (Object_CheckMovementCollision(actor, requested) == 0)
	{
#endif
		s32 t;

		Engine_EventBegin();
		Object_SetMode(actor, 6);
		Engine_TaskWait(6);
		Engine_AudioPlayCue(152);
		Object_SetMode(actor, 7);
		actor->f30 = 0x30000;
		actor->f34 = 0x20000;
		actor->f28 = 0x40000;
		t = 126;
		t &= *state;
		*state = (u8)t;
		Engine_ActorSetSpriteFlags(actor, 0);
		{
#if EDITION_INTERNATIONAL
			s16 *coordinates = (s16 *)&destination;
#else
			s16 *coordinates = (s16 *)requested;
#endif

			Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, coordinates[1], coordinates[5]);
		}
		Object_SetMode(actor, 6);
		Engine_ActorSetSpriteFlags(actor, 1);
		*state = (u8)saved_state;
		Engine_EventEnd();
		return 1;
	}
	return 0;
}

s32 CheckActorPathSouth(void)
{
	struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct V		   destination;

	destination.a = actor->f08;
	destination.b = actor->f0c;
	destination.c = actor->f10 + -0x200000;
	return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathNorth(void)
{
	struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct V		   destination;

	destination.a = actor->f08;
	destination.b = actor->f0c;
	destination.c = actor->f10 + 0x200000;
	return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathWest(void)
{
	struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct V		   destination;

	destination.a = actor->f08 + -0x200000;
	destination.b = actor->f0c;
	destination.c = actor->f10;
	return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathEast(void)
{
	struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct V		   destination;

	destination.a = actor->f08 + 0x200000;
	destination.b = actor->f0c;
	destination.c = actor->f10;
	return TryPushBlockingSceneActor(actor, &destination);
}

void UpdateEscapeRouteForActorPositions(void)
{
	s32 actor_x = ((Obj *)Object_GetById(8))->f08 >> 20;
	s32 actor_z = ((Obj *)Object_GetById(8))->f10 >> 20;
	s32 actor_12_x = ((Obj *)Object_GetById(12))->f08 >> 20;
	s32 actor_15_x = ((Obj *)Object_GetById(15))->f08 >> 20;

	if (actor_z == 19)
	{
		if (actor_12_x == 24)
		{
			PositionSceneActorPair(8, 0, -80);
		}
		else if (actor_15_x == 24)
		{
			PositionSceneActorPair(8, 0, -112);
			PositionSceneActorPair(8, 0, -32);
		}
		else
		{
			PositionSceneActorPair(8, 0, -80);
			PositionSceneActorPair(8, 0, -112);
		}
	}
	else if (actor_z == 14)
	{
		if (actor_12_x == 24)
		{
			return;
		}
		if (actor_15_x == 24)
		{
			PositionSceneActorPair(8, 0, -64);
		}
		else
		{
			PositionSceneActorPair(8, 0, -112);
		}
	}
	else if (actor_z == 10)
	{
		if (actor_15_x == 24)
		{
			return;
		}
		PositionSceneActorPair(8, 0, -48);
	}
	else
	{
		CheckActorPathSouth();
		return;
	}
	Engine_TaskWait(2);
	{
		s32 route_end_z = ((Obj *)Object_GetById(8))->f10 >> 20;
		s32 route_x = actor_x - 1;

		Engine_MapCopyCellAttributes(route_x, actor_z, 3, 1, route_x, route_end_z);
	}
	Engine_MapCopyCellAttributes(0, 0, 3, 1, actor_x - 1, actor_z);
}

void UpdateActor8ReturnRoute(void)
{
	s32 x = ((Obj *)Object_GetById(8))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(8))->f10 >> 20;
	s32 z = ((Obj *)Object_GetById(12))->f08 >> 20;

	if (y == 7)
	{
		if (z == 24)
		{
			PositionSceneActorPair(8, 0, 48);
		}
		else
		{
			PositionSceneActorPair(8, 0, 80);
			PositionSceneActorPair(8, 0, 112);
		}
	}
	else if (y == 10)
	{
		if (z == 24)
		{
			return;
		}
		PositionSceneActorPair(8, 0, 144);
	}
	else if (y == 14)
	{
		PositionSceneActorPair(8, 0, 80);
	}
	else
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(8))->f10 >> 20;
		s32 m = x - 1;

		Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
	}
	Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10RetreatRoute(void)
{
	s32 x = ((Obj *)Object_GetById(10))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(10))->f10 >> 20;
	s32 z = ((Obj *)Object_GetById(13))->f08 >> 20;
	s32 w = ((Obj *)Object_GetById(15))->f08 >> 20;

	if (y == 18)
	{
		if (w >= 31 && w <= 33)
		{
			PositionSceneActorPair(10, 0, -128);
		}
		else if (z >= 31 && z <= 33)
		{
			PositionSceneActorPair(10, 0, -128);
		}
		else
		{
			PositionSceneActorPair(10, 0, -112);
			PositionSceneActorPair(10, 0, -64);
		}
	}
	else if (y == 10)
	{
		if (w >= 31 && w <= 33)
		{
			return;
		}
		if (z >= 31 && z <= 33)
		{
			return;
		}
		PositionSceneActorPair(10, 0, -48);
	}
	else if (y == 7)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(10))->f10 >> 20;
		s32 m = x - 1;

		Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
	}
	Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10AdvanceRoute(void)
{
	s32 x = ((Obj *)Object_GetById(10))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(10))->f10 >> 20;

	if (y != 18)
	{
		if (y == 10)
		{
			PositionSceneActorPair(10, 0, 128);
		}
		else
		{
			PositionSceneActorPair(10, 0, 112);
			PositionSceneActorPair(10, 0, 64);
		}
		Engine_TaskWait(2);
		{
			s32 k = ((Obj *)Object_GetById(10))->f10 >> 20;
			s32 m = x - 1;

			Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
		}
		Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
	}
}

void UpdateActor11WestRoute(void)
{
	s32 x = ((Obj *)Object_GetById(11))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(11))->f10 >> 20;

	if (x != 30)
	{
		if (x == 34)
		{
			if ((((Obj *)Object_GetById(10))->f10 >> 20) == 18)
			{
				return;
			}
			PositionSceneActorPair(11, -64, 0);
		}
		else if (x == 36)
		{
			if ((((Obj *)Object_GetById(10))->f10 >> 20) == 18)
			{
				PositionSceneActorPair(11, -32, 0);
			}
			else
			{
				PositionSceneActorPair(11, -96, 0);
			}
		}
		Engine_TaskWait(2);
		{
			s32 k = ((Obj *)Object_GetById(11))->f08 >> 20;
			s32 m = y - 1;

			Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
		}
		Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
	}
}

void UpdateActor11EastRoute(void)
{
	s32 x = ((Obj *)Object_GetById(11))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(11))->f10 >> 20;

	if (x != 36)
	{
		if (x == 30)
		{
			if ((((Obj *)Object_GetById(10))->f10 >> 20) == 18)
			{
				return;
			}
			PositionSceneActorPair(11, 96, 0);
		}
		else if (x == 34)
		{
			PositionSceneActorPair(11, 32, 0);
		}
		Engine_TaskWait(2);
		{
			s32 k = ((Obj *)Object_GetById(11))->f08 >> 20;
			s32 m = y - 1;

			Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
		}
		Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
	}
}

void UpdateActor12WestRoute(void)
{
	s32 x = ((Obj *)Object_GetById(12))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(12))->f10 >> 20;

	if (x == 36)
	{
		PositionSceneActorPair(12, -96, 0);
		PositionSceneActorPair(12, -96, 0);
	}
	else if (x == 34)
	{
		PositionSceneActorPair(12, -96, 0);
		PositionSceneActorPair(12, -64, 0);
	}
	else if (x == 24)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(12))->f08 >> 20;
		s32 m = y - 1;

		Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
	}
	Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor12EastRoute(void)
{
	s32 x = ((Obj *)Object_GetById(12))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(12))->f10 >> 20;

	if (x == 24)
	{
		PositionSceneActorPair(12, 96, 0);
		PositionSceneActorPair(12, 96, 0);
	}
	else if (x == 34)
	{
		PositionSceneActorPair(12, 32, 0);
	}
	else if (x == 36)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(12))->f08 >> 20;
		s32 m = y - 1;

		Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
	}
	Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13WestRoute(void)
{
	s32 x = ((Obj *)Object_GetById(13))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(13))->f10 >> 20;
	s32 z = ((Obj *)Object_GetById(10))->f10 >> 20;
	s32 w = ((Obj *)Object_GetById(15))->f08 >> 20;

	if (x == 36)
	{
		if (w == 34)
		{
			PositionSceneActorPair(13, -16, 0);
		}
		else if (z == 7)
		{
			PositionSceneActorPair(13, -32, 0);
		}
		else if (w == 30)
		{
			PositionSceneActorPair(13, -80, 0);
		}
		else
		{
			PositionSceneActorPair(13, -96, 0);
			PositionSceneActorPair(13, -80, 0);
		}
	}
	else if (x == 35)
	{
		if (w == 34)
		{
			return;
		}
		if (z == 7)
		{
			PositionSceneActorPair(13, -16, 0);
		}
		else if (w == 30)
		{
			PositionSceneActorPair(13, -64, 0);
		}
		else
		{
			PositionSceneActorPair(13, -80, 0);
			PositionSceneActorPair(13, -80, 0);
		}
	}
	else if (x == 34)
	{
		if (z == 7)
		{
			return;
		}
		if (w == 30)
		{
			PositionSceneActorPair(13, -48, 0);
		}
		else
		{
			PositionSceneActorPair(13, -144, 0);
		}
	}
	else if (x == 31)
	{
		if (w == 30)
		{
			return;
		}
		PositionSceneActorPair(13, -96, 0);
	}
	else if (x == 25)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(13))->f08 >> 20;
		s32 m = y - 1;

		Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
	}
	Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13EastRoute(void)
{
	s32 x = ((Obj *)Object_GetById(13))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(13))->f10 >> 20;

	Object_GetById(15);
	if (x == 25)
	{
		PositionSceneActorPair(13, 96, 0);
		PositionSceneActorPair(13, 80, 0);
	}
	else if (x == 31)
	{
		PositionSceneActorPair(13, 80, 0);
	}
	else if (x == 34)
	{
		PositionSceneActorPair(13, 32, 0);
	}
	else if (x == 35)
	{
		PositionSceneActorPair(13, 16, 0);
	}
	else if (x == 36)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(13))->f08 >> 20;
		s32 m = y - 1;

		Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
	}
	Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15WestRoute(void)
{
	s32 x = ((Obj *)Object_GetById(15))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(15))->f10 >> 20;
	s32 z = ((Obj *)Object_GetById(8))->f10 >> 20;
	s32 w = ((Obj *)Object_GetById(10))->f10 >> 20;

	if (x == 35)
	{
		if (w == 7)
		{
			PositionSceneActorPair(15, -16, 0);
		}
		else if (z == 7)
		{
			PositionSceneActorPair(15, -112, 0);
		}
		else
		{
			PositionSceneActorPair(15, -96, 0);
			PositionSceneActorPair(15, -80, 0);
		}
	}
	else if (x == 34)
	{
		if (w == 7)
		{
			return;
		}
		PositionSceneActorPair(15, -96, 0);
		PositionSceneActorPair(15, -64, 0);
	}
	else if (x == 33)
	{
		PositionSceneActorPair(15, -144, 0);
	}
	else if (x == 31)
	{
		PositionSceneActorPair(15, -80, 0);
	}
	else if (x == 30)
	{
		PositionSceneActorPair(15, -96, 0);
	}
	else if (x == 24)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(15))->f08 >> 20;
		s32 m = y - 1;

		Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
	}
	Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15EastRoute(void)
{
	s32 x = ((Obj *)Object_GetById(15))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(15))->f10 >> 20;
	s32 z = ((Obj *)Object_GetById(10))->f10 >> 20;
	s32 w = ((Obj *)Object_GetById(13))->f08 >> 20;

	if (x == 24)
	{
		if (z == 7 || w == 31)
		{
			PositionSceneActorPair(15, 96, 0);
		}
		else if (w == 34)
		{
			PositionSceneActorPair(15, 64, 0);
			PositionSceneActorPair(15, 80, 0);
		}
		else if (w == 35)
		{
			PositionSceneActorPair(15, 80, 0);
			PositionSceneActorPair(15, 80, 0);
		}
		else
		{
			PositionSceneActorPair(15, 80, 0);
			PositionSceneActorPair(15, 96, 0);
		}
	}
	else if (x == 30 || w == 31)
	{
		if (z == 7)
		{
			return;
		}
		if (w == 34)
		{
			PositionSceneActorPair(15, 48, 0);
		}
		else if (w == 35)
		{
			PositionSceneActorPair(15, 64, 0);
		}
		else
		{
			PositionSceneActorPair(15, 80, 0);
		}
	}
	else if (x == 33)
	{
		if (w == 34)
		{
			return;
		}
		if (w == 35)
		{
			PositionSceneActorPair(15, 16, 0);
		}
		else
		{
			PositionSceneActorPair(15, 32, 0);
		}
	}
	else if (x == 34)
	{
		PositionSceneActorPair(15, 16, 0);
	}
	else if (x == 35)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(15))->f08 >> 20;
		s32 m = y - 1;

		Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
	}
	Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor17SouthRoute(void)
{
	s32 x = ((Obj *)Object_GetById(17))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(17))->f10 >> 20;
	s32 z = ((Obj *)Object_GetById(19))->f08 >> 20;

	if (y == 19)
	{
		if (z >= 3 && z <= 5)
		{
			PositionSceneActorPair(17, 0, -16);
		}
		else
		{
			PositionSceneActorPair(17, 0, -64);
		}
	}
	else if (y == 18)
	{
		if (z >= 3 && z <= 5)
		{
			return;
		}
		PositionSceneActorPair(17, 0, -48);
	}
	else if (y == 15)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(17))->f10 >> 20;
		s32 m = x - 1;

		Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
	}
	Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor17NorthRoute(void)
{
	s32 x = ((Obj *)Object_GetById(17))->f08 >> 20;
	s32 y = ((Obj *)Object_GetById(17))->f10 >> 20;

	if (y == 15)
	{
		PositionSceneActorPair(17, 0, 64);
	}
	else if (y == 18)
	{
		PositionSceneActorPair(17, 0, 16);
	}
	else if (y == 19)
	{
		return;
	}
	Engine_TaskWait(2);
	{
		s32 k = ((Obj *)Object_GetById(17))->f10 >> 20;

		s32 m = x - 1;

		Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
	}
	Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

#define IN_COLUMNS(x) ((x) >= 6 && (x) <= 8)

void TakaraShima_SettlePushedBlockNorth(void)
{
	s32 cell_x;
	s32 cell_z;
	s32 x19;
	s32 x14;
	s32 x16;

	cell_x = Object_GetById(18)->x.fixed >> 20;
	cell_z = Object_GetById(18)->z.fixed >> 20;
	x19 = Object_GetById(19)->x.fixed >> 20;
	x14 = Object_GetById(14)->x.fixed >> 20;
	x16 = Object_GetById(16)->x.fixed >> 20;
	if (cell_z == 19)
	{
		if (IN_COLUMNS(x19))
		{
			PositionSceneActorPair(18, 0, -16);
		}
		else if (IN_COLUMNS(x14))
		{
			PositionSceneActorPair(18, 0, -64);
		}
		else if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(18, 0, -112);
		}
		else
		{
			PositionSceneActorPair(18, 0, -64);
			PositionSceneActorPair(18, 0, -96);
		}
	}
	else if (cell_z == 18)
	{
		if (IN_COLUMNS(x19))
		{
			return;
		}
		if (IN_COLUMNS(x14))
		{
			PositionSceneActorPair(18, 0, -48);
		}
		else if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(18, 0, -96);
		}
		else
		{
			PositionSceneActorPair(18, 0, -144);
		}
	}
	else if (cell_z == 15)
	{
		if (IN_COLUMNS(x14))
		{
			return;
		}
		if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(18, 0, -48);
		}
		else
		{
			PositionSceneActorPair(18, 0, -96);
		}
	}
	else if (cell_z == 14)
	{
		if (IN_COLUMNS(x14))
		{
			return;
		}
		if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(18, 0, -32);
		}
		else
		{
			PositionSceneActorPair(18, 0, -80);
		}
	}
	else if (cell_z == 12)
	{
		if (IN_COLUMNS(x16))
		{
			return;
		}
		PositionSceneActorPair(18, 0, -48);
	}
	else if (cell_z == 11)
	{
		if (IN_COLUMNS(x16))
		{
			return;
		}
		PositionSceneActorPair(18, 0, -32);
	}
	else if (cell_z == 9)
	{
		return;
	}
	Engine_TaskWait(2);
	Engine_MapCopyCellAttributes(
		cell_x - 1, cell_z, 3, 1, cell_x - 1, Object_GetById(18)->z.fixed >> 20);
	Engine_MapCopyCellAttributes(0, 0, 3, 1, cell_x - 1, cell_z);
}

#undef IN_COLUMNS

void AdvanceActor18AlongEscapeRoute(void)
{
	s32 column;
	s32 row;
	s32 companion19Column;
	s32 companion14Column;

	s32 permuted_5;
	permuted_5 = Object_GetById(18)->x.fixed >> 20;
	row = Object_GetById(18)->z.fixed >> 20;
	column = permuted_5;
	companion19Column = Object_GetById(19)->x.fixed >> 20;
	companion14Column = Object_GetById(14)->x.fixed >> 20;

	if (row == 9)
	{
		if ((u32)(companion14Column - 6) <= 2)
		{
			goto transition32;
		}
		if ((u32)(companion19Column - 6) <= 2)
		{
			goto transition80;
		}
		PositionSceneActorPair(18, 0, 64);
		PositionSceneActorPair(18, 0, 96);
	}
	else if (row == 11)
	{
		if ((u32)(companion14Column - 6) <= 2)
		{
			return;
		}
		if ((u32)(companion19Column - 6) <= 2)
		{
			PositionSceneActorPair(18, 0, 48);
		}
		else
		{
			PositionSceneActorPair(18, 0, 128);
		}
	}
	else if (row == 12)
	{
		if ((u32)(companion19Column - 6) <= 2)
		{
		transition32:
			PositionSceneActorPair(18, 0, 32);
		}
		else
		{
			PositionSceneActorPair(18, 0, 112);
		}
	}
	else if (row == 14)
	{
		if ((u32)(companion19Column - 6) <= 2)
		{
			return;
		}
	transition80:
		PositionSceneActorPair(18, 0, 80);
	}
	else if (row == 15)
	{
		PositionSceneActorPair(18, 0, 64);
	}
	else if (row == 18)
	{
		PositionSceneActorPair(18, 0, 16);
	}
	else if (row == 19)
	{
		return;
	}

	Engine_TaskWait(2);

	column -= 1;
	Engine_MapCopyCellAttributes(column, row, 3, 1, column, Object_GetById(18)->z.fixed >> 20);
	Engine_MapCopyCellAttributes(0, 0, 3, 1, column, row);
}

#undef IN_COLUMNS
#define IN_COLUMNS(x) ((x) >= 9 && (x) <= 11)

void TakaraShima_SettleSecondBlockNorth(void)
{
	s32 cell_x;
	s32 cell_z;
	s32 x19;
	s32 x14;
	s32 x16;

	cell_x = Object_GetById(9)->x.fixed >> 20;
	cell_z = Object_GetById(9)->z.fixed >> 20;
	x19 = Object_GetById(19)->x.fixed >> 20;
	x14 = Object_GetById(14)->x.fixed >> 20;
	x16 = Object_GetById(16)->x.fixed >> 20;
	if (cell_z == 19)
	{
		if (IN_COLUMNS(x19))
		{
			PositionSceneActorPair(9, 0, -16);
		}
		else if (IN_COLUMNS(x14))
		{
			PositionSceneActorPair(9, 0, -64);
		}
		else if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(9, 0, -112);
		}
		else
		{
			PositionSceneActorPair(9, 0, -80);
			PositionSceneActorPair(9, 0, -96);
		}
	}
	else if (cell_z == 18)
	{
		if (IN_COLUMNS(x19))
		{
			return;
		}
		if (IN_COLUMNS(x14))
		{
			PositionSceneActorPair(9, 0, -48);
		}
		else if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(9, 0, -96);
		}
		else
		{
			PositionSceneActorPair(9, 0, -96);
			PositionSceneActorPair(9, 0, -64);
		}
	}
	else if (cell_z == 15)
	{
		if (IN_COLUMNS(x14))
		{
			return;
		}
		if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(9, 0, -48);
		}
		else
		{
			PositionSceneActorPair(9, 0, -112);
		}
	}
	else if (cell_z == 14)
	{
		if (IN_COLUMNS(x14))
		{
			return;
		}
		if (IN_COLUMNS(x16))
		{
			PositionSceneActorPair(9, 0, -32);
		}
		else
		{
			PositionSceneActorPair(9, 0, -96);
		}
	}
	else if (cell_z == 12)
	{
		if (IN_COLUMNS(x16))
		{
			return;
		}
		PositionSceneActorPair(9, 0, -64);
	}
	else if (cell_z == 11)
	{
		if (IN_COLUMNS(x16))
		{
			return;
		}
		PositionSceneActorPair(9, 0, -48);
	}
	else if ((u32)cell_z <= 9)
	{
		return;
	}
	Engine_TaskWait(2);
	Engine_MapCopyCellAttributes(
		cell_x - 1, cell_z, 3, 1, cell_x - 1, Object_GetById(9)->z.fixed >> 20);
	Engine_MapCopyCellAttributes(0, 0, 3, 1, cell_x - 1, cell_z);
}

#undef IN_COLUMNS

void AdvanceActor9AlongEscapeRoute(void)
{
	s32 column;
	s32 row;
	s32 companion19Column;
	s32 companion14Column;
	s32 companion16Column;

	s32 permuted_6;
	permuted_6 = Object_GetById(9)->x.fixed >> 20;
	row = Object_GetById(9)->z.fixed >> 20;
	column = permuted_6;
	companion19Column = Object_GetById(19)->x.fixed >> 20;
	companion14Column = Object_GetById(14)->x.fixed >> 20;
	companion16Column = Object_GetById(16)->x.fixed >> 20;

	if (row == 8)
	{
		if ((u32)(companion16Column - 9) <= 2)
		{
			return;
		}
		if ((u32)(companion14Column - 9) <= 2)
		{
			goto transition48;
		}
		if ((u32)(companion19Column - 9) > 2)
		{
			PositionSceneActorPair(9, 0, 80);
		}
		PositionSceneActorPair(9, 0, 96);
	}
	else if (row == 11)
	{
		if ((u32)(companion14Column - 9) <= 2)
		{
			return;
		}
		if ((u32)(companion19Column - 9) <= 2)
		{
		transition48:
			PositionSceneActorPair(9, 0, 48);
		}
		else
		{
			PositionSceneActorPair(9, 0, 128);
		}
	}
	else if (row == 12)
	{
		if ((u32)(companion14Column - 9) <= 2)
		{
			return;
		}
		if ((u32)(companion19Column - 9) <= 2)
		{
			PositionSceneActorPair(9, 0, 32);
		}
		else
		{
			PositionSceneActorPair(9, 0, 112);
		}
	}
	else if (row == 14)
	{
		if ((u32)(companion19Column - 9) <= 2)
		{
			return;
		}
		PositionSceneActorPair(9, 0, 80);
	}
	else if (row == 15)
	{
		PositionSceneActorPair(9, 0, 64);
	}
	else if (row == 18)
	{
		PositionSceneActorPair(9, 0, 16);
	}

	Engine_TaskWait(2);

	column -= 1;
	Engine_MapCopyCellAttributes(column, row, 3, 1, column, Object_GetById(9)->z.fixed >> 20);
	Engine_MapCopyCellAttributes(0, 0, 3, 1, column, row);
}

void TakaraShima_SettlePushedBlockWest(void)
{
	s32 cell_x;
	s32 cell_z;
	s32 z17;
	s32 z18;
	s32 z9;

	cell_x = Object_GetById(19)->x.fixed >> 20;
	cell_z = Object_GetById(19)->z.fixed >> 20;
	z17 = Object_GetById(17)->z.fixed >> 20;
	z18 = Object_GetById(18)->z.fixed >> 20;
	z9 = Object_GetById(9)->z.fixed >> 20;
	if (cell_x == 3)
	{
		return;
	}
	if (cell_x == 13)
	{
		if (z9 == 15)
		{
			PositionSceneActorPair(19, -16, 0);
		}
		else if (z18 == 15)
		{
			PositionSceneActorPair(19, -64, 0);
		}
		else if (z17 == 15)
		{
			PositionSceneActorPair(19, -112, 0);
		}
		else
		{
			PositionSceneActorPair(19, -112, 0);
			PositionSceneActorPair(19, -48, 0);
		}
	}
	else if (cell_x == 6)
	{
		if (z17 == 15)
		{
			return;
		}
		PositionSceneActorPair(19, -48, 0);
	}
	else if (cell_x == 5)
	{
		PositionSceneActorPair(19, -32, 0);
	}
	else if (cell_x == 8)
	{
		if (z18 == 15)
		{
			return;
		}
		if (z17 == 15)
		{
			PositionSceneActorPair(19, -32, 0);
		}
		else
		{
			PositionSceneActorPair(19, -80, 0);
		}
	}
	else if (cell_x == 9)
	{
		if (z18 == 15)
		{
			return;
		}
		if (z17 == 15)
		{
			PositionSceneActorPair(19, -48, 0);
		}
		else
		{
			PositionSceneActorPair(19, -96, 0);
		}
	}
	else if (cell_x == 12)
	{
		if (z9 == 15)
		{
			return;
		}
		if (z18 == 15)
		{
			PositionSceneActorPair(19, -48, 0);
		}
		else if (z17 == 15)
		{
			PositionSceneActorPair(19, -96, 0);
		}
		else
		{
			PositionSceneActorPair(19, -144, 0);
		}
	}
	Engine_TaskWait(2);
	Engine_MapCopyCellAttributes(
		cell_x, cell_z - 1, 1, 3, Object_GetById(19)->x.fixed >> 20, cell_z - 1);
	Engine_MapCopyCellAttributes(0, 0, 1, 3, cell_x, cell_z - 1);
}

void TakaraShima_SettlePushedBlock(void)
{
	s32 cell_x;
	s32 cell_z;
	s32 z18;
	s32 z9;

	cell_x = Object_GetById(19)->x.fixed >> 20;
	cell_z = Object_GetById(19)->z.fixed >> 20;
	z18 = Object_GetById(18)->z.fixed >> 20;
	z9 = Object_GetById(9)->z.fixed >> 20;
	if (cell_x == 3)
	{
		if (z18 == 15)
		{
			PositionSceneActorPair(19, 32, 0);
		}
		else if (z9 == 15)
		{
			PositionSceneActorPair(19, 80, 0);
		}
		else
		{
			PositionSceneActorPair(19, 112, 0);
			PositionSceneActorPair(19, 48, 0);
		}
	}
	else if (cell_x == 5)
	{
		if (z18 == 15)
		{
			return;
		}
		if (z9 == 15)
		{
			PositionSceneActorPair(19, 48, 0);
		}
		else
		{
			PositionSceneActorPair(19, 128, 0);
		}
	}
	else if (cell_x == 6)
	{
		if (z9 == 15)
		{
			PositionSceneActorPair(19, 32, 0);
		}
		else
		{
			PositionSceneActorPair(19, 112, 0);
		}
	}
	else if (cell_x == 8)
	{
		if (z9 == 15)
		{
			return;
		}
		PositionSceneActorPair(19, 80, 0);
	}
	else if (cell_x == 9)
	{
		PositionSceneActorPair(19, 64, 0);
	}
	else if (cell_x == 12)
	{
		PositionSceneActorPair(19, 16, 0);
	}
	Engine_TaskWait(2);
	Engine_MapCopyCellAttributes(
		cell_x, cell_z - 1, 1, 3, Object_GetById(19)->x.fixed >> 20, cell_z - 1);
	Engine_MapCopyCellAttributes(0, 0, 1, 3, cell_x, cell_z - 1);
}

void RetreatActor14AlongEscapeRoute(void)
{
	s32 column;
	s32 row;
	s32 companion18Row;
	s32 companion9Row;
	s32 rowM1;

	s32 permuted_7;
	column = Object_GetById(14)->x.fixed >> 20;
	permuted_7 = Object_GetById(14)->z.fixed >> 20;
	companion18Row = Object_GetById(18)->z.fixed >> 20;
	row = permuted_7;
	companion9Row = Object_GetById(9)->z.fixed >> 20;

	if (column == 13)
	{
		if ((u32)(companion9Row - 12) <= 2)
		{
			PositionSceneActorPair(14, -16, 0);
		}
		else if ((u32)(companion18Row - 12) <= 2)
		{
			PositionSceneActorPair(14, -64, 0);
		}
		else
		{
			PositionSceneActorPair(14, -112, 0);
		}
	}
	else if (column == 12)
	{
		if ((u32)(companion9Row - 12) <= 2)
		{
			return;
		}
		if ((u32)(companion18Row - 12) <= 2)
		{
			PositionSceneActorPair(14, -48, 0);
		}
		else
		{
			PositionSceneActorPair(14, -96, 0);
		}
	}
	else if (column == 9)
	{
		if ((u32)(companion18Row - 12) <= 2)
		{
			return;
		}
		PositionSceneActorPair(14, -48, 0);
	}
	else if (column == 8)
	{
		if ((u32)(companion18Row - 12) <= 2)
		{
			return;
		}
		PositionSceneActorPair(14, -32, 0);
	}
	else if (column == 6)
	{
		return;
	}

	Engine_TaskWait(2);

	rowM1 = row - 1;
	Engine_MapCopyCellAttributes(column, rowM1, 1, 3, Object_GetById(14)->x.fixed >> 20, rowM1);
	Engine_MapCopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor14AlongEscapeRoute(void)
{
	s32 column;
	s32 row;
	s32 companion18Row;
	s32 companion9Row;
	s32 rowM1;

	s32 permuted_7;
	column = Object_GetById(14)->x.fixed >> 20;
	permuted_7 = Object_GetById(14)->z.fixed >> 20;
	companion18Row = Object_GetById(18)->z.fixed >> 20;
	row = permuted_7;
	companion9Row = Object_GetById(9)->z.fixed >> 20;

	if (column == 6)
	{
		if ((u32)(companion9Row - 12) <= 2)
		{
			PositionSceneActorPair(14, 32, 0);
		}
		else if ((u32)(companion18Row - 12) <= 2)
		{
			PositionSceneActorPair(14, 64, 0);
		}
		else
		{
			PositionSceneActorPair(14, 112, 0);
		}
	}
	else if (column == 8)
	{
		if ((u32)(companion9Row - 12) <= 2)
		{
			return;
		}
		PositionSceneActorPair(14, 80, 0);
	}
	else if (column == 9)
	{
		if ((u32)(companion9Row - 12) <= 2)
		{
			return;
		}
		PositionSceneActorPair(14, 64, 0);
	}
	else if (column == 12)
	{
		PositionSceneActorPair(14, 16, 0);
	}
	else if (column == 13)
	{
		return;
	}

	Engine_TaskWait(2);

	rowM1 = row - 1;
	Engine_MapCopyCellAttributes(column, rowM1, 1, 3, Object_GetById(14)->x.fixed >> 20, rowM1);
	Engine_MapCopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void RetreatActor16AlongEscapeRoute(void)
{
	s32 column;
	s32 row;
	s32 companion18Row;
	s32 companion9Row;
	s32 rowM1;

	s32 permuted_7;
	column = Object_GetById(16)->x.fixed >> 20;
	permuted_7 = Object_GetById(16)->z.fixed >> 20;
	companion18Row = Object_GetById(18)->z.fixed >> 20;
	row = permuted_7;
	companion9Row = Object_GetById(9)->z.fixed >> 20;

	if (column == 13)
	{
		if ((u32)(companion9Row - 9) <= 2)
		{
			PositionSceneActorPair(16, -16, 0);
		}
		else if ((u32)(companion18Row - 9) <= 2)
		{
			PositionSceneActorPair(16, -64, 0);
		}
		else
		{
			PositionSceneActorPair(16, -112, 0);
		}
	}
	else if (column == 12)
	{
		if ((u32)(companion9Row - 9) <= 2)
		{
			return;
		}
		if ((u32)(companion18Row - 9) <= 2)
		{
			PositionSceneActorPair(16, -48, 0);
		}
		else
		{
			PositionSceneActorPair(16, -96, 0);
		}
	}
	else if (column == 9)
	{
		if ((u32)(companion18Row - 9) <= 2)
		{
			return;
		}
		PositionSceneActorPair(16, -48, 0);
	}
	else if (column == 8)
	{
		PositionSceneActorPair(16, -32, 0);
	}
	else if (column == 6)
	{
		return;
	}

	Engine_TaskWait(2);

	rowM1 = row - 1;
	Engine_MapCopyCellAttributes(column, rowM1, 1, 3, Object_GetById(16)->x.fixed >> 20, rowM1);
	Engine_MapCopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor16AlongEscapeRoute(void)
{
	s32 column;
	s32 row;
	s32 companionRow;

	s32 permuted_6;
	column = Object_GetById(16)->x.fixed >> 20;
	permuted_6 = Object_GetById(16)->z.fixed >> 20;
	companionRow = Object_GetById(9)->z.fixed >> 20;
	row = permuted_6;

	if (column == 6)
	{
		if ((u32)(companionRow - 9) <= 2)
		{
			PositionSceneActorPair(16, 32, 0);
		}
		else
		{
			PositionSceneActorPair(16, 112, 0);
		}
	}
	else if (column == 8)
	{
		if ((u32)(companionRow - 9) <= 2)
		{
			return;
		}
		PositionSceneActorPair(16, 80, 0);
	}
	else if (column == 9)
	{
		PositionSceneActorPair(16, 64, 0);
	}
	else if (column == 12)
	{
		PositionSceneActorPair(16, 16, 0);
	}
	else if (column == 13)
	{
		return;
	}

	Engine_TaskWait(2);

	row -= 1;
	Engine_MapCopyCellAttributes(column, row, 1, 3, Object_GetById(16)->x.fixed >> 20, row);
	Engine_MapCopyCellAttributes(0, 0, 1, 3, column, row);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/orbit.c"

s16 StagedActor_DirectionSteps[] = {
	0, 16,
	0, 16,
	0, 16,
	16, 0,
	16, 0,
	16, 0,
	16, 0,
	0, -16,
	0, -16,
	0, -16,
	0, -16,
	-16, 0,
	-16, 0,
	-16, 0,
	-16, 0,
	0, 16,
};

u32 StagedActor_FootprintKinds[] = {
	207, 205, 228, 229, 298, 297,
};

s32 StagedActor_FootprintBounds[] = {
	-32, -8, 32, 8,
	-8, -32, 8, 32,
	-32, -16, 32, 0,
	-8, -32, 8, 32,
	-32, -8, 32, 8,
	-8, -32, 8, 32,
};

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

u16 TakaraShima_EntranceCells[] = {
	MAP_CELL_STEP(58, 23, 1, 2, 5), MAP_CELL_STEP(59, 23, 1, 2, 5), MAP_CELL_STEPS_END,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
	(const s32 *)0xffff0000,
	(const s32 *)120,
	(const s32 *)0x40000098,
	(const s32 *)0xffff0000,
	(const s32 *)-1,
	(const s32 *)0xffff,
	(const s32 *)0xffff,
	0,
	0,
	0,
	0,
	0,
};

struct SceneEntrance gTakaraShimaEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraShimaEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
	{ 2, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraShimaEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
	{ 2, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
	{ 99, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraShimaEntrances4[] = {
	{ 1, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u16 gTakaraShimaEntrances5[] = {
	MAP_CELL_STEP(1, 65535, 312, 0, 656), MAP_CELL_STEP(32768, 0, 65535, 65535, 65535), MAP_CELL_STEPS_END,
	MAP_CELL_STEP(0, 5, 65535, 216, 0), MAP_CELL_STEP(544, 49152, 0, 65535, 65535), MAP_CELL_STEPS_END,
	MAP_CELL_STEPS_END,
};

struct SceneEntrance gTakaraShimaEntrancesOther[] = {
	{ 1, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
	{ 2, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
	{ 3, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
	{ 4, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
	{ 5, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
	{ SCENE_TABLE_END },
};

u32 gTakaraShimaExits[] = {
	SCENE_EXITS(113),
	SCENE_EXIT(1, 2, 76),
	SCENE_EXIT(2, 114, 1),
	SCENE_EXIT(0, 114, 0),
	SCENE_EXIT(1, 113, 2),
	SCENE_EXIT(2, 126, 1),
	SCENE_EXIT(0, 123, 0),
	SCENE_EXIT(1, 134, 3),
	SCENE_EXIT(2, 134, 4),
	SCENE_EXIT(0, 124, 0),
	SCENE_EXIT(1, 134, 2),
	SCENE_EXIT(2, 125, 1),
	SCENE_EXIT(0, 125, 0),
	SCENE_EXIT(1, 124, 2),
	SCENE_EXIT(0, 126, 0),
	SCENE_EXIT(1, 114, 2),
	SCENE_EXIT(2, 127, 1),
	SCENE_EXIT(3, 115, 1),
	SCENE_EXIT(4, 115, 2),
	SCENE_EXIT(0, 127, 0),
	SCENE_EXIT(1, 126, 2),
	SCENE_EXIT(2, 128, 1),
	SCENE_EXIT(3, 116, 1),
	SCENE_EXIT(4, 116, 2),
	SCENE_EXIT(0, 128, 0),
	SCENE_EXIT(1, 127, 2),
	SCENE_EXIT(2, 129, 1),
	SCENE_EXIT(3, 117, 1),
	SCENE_EXIT(4, 117, 2),
	SCENE_EXIT(0, 129, 0),
	SCENE_EXIT(1, 128, 2),
	SCENE_EXIT(2, 130, 1),
	SCENE_EXIT(3, 118, 1),
	SCENE_EXIT(4, 118, 2),
	SCENE_EXIT(0, 130, 0),
	SCENE_EXIT(1, 129, 2),
	SCENE_EXIT(2, 131, 1),
	SCENE_EXIT(3, 119, 1),
	SCENE_EXIT(4, 119, 2),
	SCENE_EXIT(0, 131, 0),
	SCENE_EXIT(1, 130, 2),
	SCENE_EXIT(2, 132, 1),
	SCENE_EXIT(3, 120, 1),
	SCENE_EXIT(4, 120, 2),
	SCENE_EXIT(0, 132, 0),
	SCENE_EXIT(1, 131, 2),
	SCENE_EXIT(2, 133, 1),
	SCENE_EXIT(3, 121, 1),
	SCENE_EXIT(4, 121, 2),
	SCENE_EXIT(0, 133, 0),
	SCENE_EXIT(1, 132, 2),
	SCENE_EXIT(2, 134, 1),
	SCENE_EXIT(3, 122, 1),
	SCENE_EXIT(4, 122, 2),
	SCENE_EXIT(0, 134, 0),
	SCENE_EXIT(1, 133, 2),
	SCENE_EXIT(2, 124, 1),
	SCENE_EXIT(3, 123, 1),
	SCENE_EXIT(4, 123, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement gTakaraShimaPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraShimaPlacements1[] = {
	{ 0x16, 0xfd7, 7,
		PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(144), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(352), 0, PIXELS(376), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(336), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(352), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraShimaPlacements3[] = {
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(408), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(168), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(520), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(72), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(120), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraShimaPlacements6To14[] = {
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(632), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gTakaraShimaEventsOther[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraShima_RunAreaEntry) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(StartSceneScript37) },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(RunPrologueSceneSetup) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_HandleEscapeColumn) },
	{ SEARCH(SEARCH_CHEST), 100, 0xef4, 0x500004 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraShimaEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraShimaEvents1[] = {
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, 0xfd7, EVENT_SCRIPT(StartScriptedSceneMessage) },
	{ PSYNERGY_ON_ACTOR(0xc), 11, 0x240, EVENT_SCRIPT(ConfigureSceneActor11) },
	{ PSYNERGY_ON_ACTOR(0xc), 12, 0x241, EVENT_SCRIPT(ConfigureSceneActor12) },
	{ PSYNERGY_ON_ACTOR(0xc), 13, 0x242, EVENT_SCRIPT(ConfigureSceneActor13) },
	{ PSYNERGY_ON_ACTOR(0xc), 14, 0x243, EVENT_SCRIPT(ConfigureSceneActor14) },
	{ PSYNERGY_ON_ACTOR(0x4e), 15, 0x8c4, EVENT_SCRIPT(ShowForgetEverythingMessage) },
	{ PSYNERGY_ON_ACTOR(0x4e), 16, 0x8c5, EVENT_SCRIPT(ShowHelpYouForgetMessage) },
	{ PSYNERGY_ON_ACTOR(0x4e), 17, 0x8c6, EVENT_SCRIPT(ShowDamagedDoorMessage) },
	{ PSYNERGY_ON_ACTOR(0x4e), 18, 0x8c7, EVENT_SCRIPT(ShowSaveMyLifeMessage) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(RunSceneVectorTransition) },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(RunSceneVectorTransition) },
	{ 0x8413, 100, 0xec0, FIND_COINS(1) },
	{ 0xc413, 100, 0xec0, FIND_COINS(1) },
	{ 0xa413, 100, 0xec0, FIND_COINS(1) },
	{ 0x8413, 101, 0xec1, FIND_ITEM(0xc3) },
	{ 0xc413, 101, 0xec1, FIND_ITEM(0xc3) },
	{ 0xa413, 101, 0xec1, FIND_ITEM(0xc3) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraShimaEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 41, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 42, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 43, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateEscapeRouteForActorPositions) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor8ReturnRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor10RetreatRoute) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor10AdvanceRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor11WestRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor11EastRoute) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor12WestRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor12EastRoute) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor13WestRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor13EastRoute) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor15WestRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor15EastRoute) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor17SouthRoute) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor17NorthRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraShima_SettlePushedBlockNorth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor18AlongEscapeRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraShima_SettleSecondBlockNorth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor9AlongEscapeRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraShima_SettlePushedBlockWest) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraShima_SettlePushedBlock) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(RetreatActor14AlongEscapeRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor14AlongEscapeRoute) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(RetreatActor16AlongEscapeRoute) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor16AlongEscapeRoute) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor8) },
	{ EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor10) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor11) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor12) },
	{ EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor13) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor15) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor17) },
	{ EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor18) },
	{ EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor9) },
	{ EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor19) },
	{ EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor14) },
	{ EVENT_TOUCH, 36, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor16) },
	{ SEARCH(SEARCH_CHEST), 100, 0xee2, FIND_COINS(999) },
	{ SEARCH(SEARCH_CHEST), 101, 0xee3, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ SEARCH(SEARCH_CHEST), 102, 0xee4, FIND_ITEM(0xb) },
	{ SEARCH(SEARCH_CHEST), 103, 0xee5, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraShimaEvents4[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraShimaEvents5[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_TOUCH, 10, 0x8d1, EVENT_SCRIPT(InitializePrologueSceneState) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
	{ SEARCH(SEARCH_CHEST), 100, 0xee6, FIND_ITEM(0x53) },
	{ SEARCH(SEARCH_CHEST), 101, 0xef7, 0x500007 },
	{ SCENE_EVENTS_END },
};
