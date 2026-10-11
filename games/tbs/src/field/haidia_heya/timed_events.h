#ifndef GUARD_TIMED_EVENTS_H
#define GUARD_TIMED_EVENTS_H

#include "types.h"

#define NULL ((void *)0)

#include "facing_object.h"
#include "field_event.h"
#include "field_scene.h"

enum ValeHouseFlag
{
	FLAG_GOT_FAREWELL_HERB = 0x81b
};

enum
{
	ITEM_HERB = 180
};

typedef struct
{
	u8	pad_to_position[8];
	s32 x;
	s32 y;
	u8	pad_to_scale[8];
	s32 scale_x;
	s32 scale_y;
	u8	pad_to_velocity[0x44];
	s16 velocity_x;
	s16 velocity_y;
} ScaledMotion;

union SceneActor
{
	s32 words[26];
	u16 halfwords[52];
};

union SceneField
{
	u16 value;
	u8	bytes[2];
} __attribute__((packed, aligned(2)));

extern s32					 gValeFaceTargetScript[];
extern s32					 gValeActor10Script[];
extern s32					 gValePartyScript[];
extern s32					 gValeTimedActorScript[];
extern struct SceneEntrance	 gValeHouseEntrances[];
extern u32					 gValeHouseExits[];
extern u32					 gValeHouseLateExits[];
extern struct ScenePlacement gValeHousePlacements[];
extern struct ScenePlacement gValeHousePlacementsAfterLeaving[];
extern struct ScenePlacement gValeHouseReturnPlacements[];
extern struct SceneEvent	 gValeHouseLateEvents[];
extern struct SceneEvent	 gValeHouseEvents[];
extern struct SceneEvent	 gValeHouseEventsAfterLeaving[];
extern struct SceneEvent	 gValeHouseReturnEvents[];

#endif
