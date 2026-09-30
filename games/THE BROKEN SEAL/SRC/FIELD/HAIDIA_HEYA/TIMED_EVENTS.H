#ifndef GUARD_TIMED_EVENTS_H
#define GUARD_TIMED_EVENTS_H

/*
 * A run of unnamed leaf owners in resource_376: scene data getters, actor
 * dialogue beats, numbered scene steps, and the long presentation
 * sequence.
 */

#include "TYPES.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum ValeHouseFlag {
    FLAG_GOT_FAREWELL_HERB = 0x81b
};

enum {
    /* Message 0x182 + 180. */
    ITEM_HERB = 180
};

typedef struct {
    u8 pad_to_position[8];
    s32 x;
    s32 y;
    u8 pad_to_scale[8];
    s32 scale_x;
    s32 scale_y;
    u8 pad_to_velocity[0x44];
    s16 velocity_x;
    s16 velocity_y;
} ScaledMotion;

union SceneActor {
    s32 words[26];
    u16 halfwords[52];
};

/*
 * The fields at 100 and 102 are consecutive halfwords, so this byte and
 * halfword view needs two-byte layout rather than the default four.
 */
union SceneField {
    u16 value;
    u8 bytes[2];
} __attribute__((packed, aligned(2)));

extern u8 gValeFaceTargetScript[];
extern u8 gValeActor10Script[];
extern u8 gValePartyScript[];
extern u8 gValeTimedActorScript[];
extern u8 gValeHouseEntrances[];
extern u8 gValeHouseExits[];
extern u8 gValeHouseLateExits[];
extern u8 gValeHousePlacements[];
extern u8 gValeHousePlacementsAfterLeaving[];
extern u8 gValeHouseReturnPlacements[];
extern u8 gValeHouseLateEvents[];
extern u8 gValeHouseEvents[];
extern u8 gValeHouseEventsAfterLeaving[];
extern u8 gValeHouseReturnEvents[];

s32 ArcTan2(s32, s32);
void SceneEvents_UpdateInView(void *);
void Object_SetTargetAndCallback();
void BattleFx_SetBlock30ValuesMaxZero(void);
void Map_ClearLayerEntryFlag();
void BattleFx_StartTwelveFrameBlend();
void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30Values128One();
s32 Battle_GetWorkObject1e0();
s32 Object_SetActionCallbackAndRefreshById();
void Scene_UpdateTimedActor(void);
void Scene_GiveFarewellHerb(void);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

#endif
