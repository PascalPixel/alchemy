#ifndef TLA_IZUMO_RUINS_H
#define TLA_IZUMO_RUINS_H

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "input.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "frame.h"
#include "scene_tables.h"

struct IzumoRuinsEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[0x56];
  s32 field1ac;
  u8 unk1b0[0xb0c];
  struct Actor *objectCbc;
};

struct IzumoRuinsAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[8];
  u16 angle;
};

struct IzumoRuinsMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct IzumoRuinsMapWork {
  u8 unk000[0x108];
  struct IzumoRuinsMapLayer layers[3];
};

struct IzumoRuinsPlacement {
  u16 id;
  u16 unk02;
  s16 flag;
};

struct IzumoRuinsEffectWork {
  u8 unk00[26];
  s16 objectId;
};

extern u8 gMapCellBuffer[];
extern char SceneId_IzumoRuinsFirstRoom;
extern char SceneId_IzumoRuinsSecondRoom;

void Pillars_Pound(u16 *list, s32 mode, s32 count);
void Actor_SetCellShapeAt(s32 a, s32 b, s32 owner);
s32 MapGrid_GetCellWord(u32 layer, s32 x, s32 y);
s32 MapGrid_SetCellWord(u32 layer, s32 x, s32 y, s32 value);
void IzumoRuins_ShowPillarState(struct Actor *object);
void Actor_SelectAnimation(struct Actor *object, s32 value);
void Event_End(void);
void Actor_DisableById(s32 id);
void FlagBlock_ApplyFlags(const void *data);
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);

extern s16 gIzumoRuins_FirstRoomMovablePillars[];
extern s16 gIzumoRuins_SecondRoomMovablePillars[];
extern s16 gIzumoRuins_FirstRoomPoundPillars[];
extern s16 gIzumoRuins_SecondRoomPoundPillars[];
extern s16 gIzumoRuins_Puddles[];
extern u32 gIzumoRuins_FirstRoomRegions[];
extern u32 gIzumoRuins_SecondRoomRegions[];
extern struct ActorPlacement gIzumoRuins_ActorPlacements[];
extern struct ActorPlacement gIzumoRuins_FirstRoomActorPlacements[];
extern struct ActorPlacement gIzumoRuins_SecondRoomActorPlacements[];
extern struct Trigger gIzumoRuins_FirstRoomTriggers[];
extern struct Trigger gIzumoRuins_SecondRoomTriggers[];

void IzumoRuins_PlacePillars(u16 *list);
void Rubble_Spray(s32 mode);
void Rubble_Burst(void);
void Map_CenterOnFollowedPoint(void);
s32 Actor_GetCellIndex(struct Actor *object);
void Actor_PlaceBlocks(const void *data);
void FieldEffect_StartBlankOverlay(s32 value);
void FlagBlock_PlaceAll(const void *data);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#endif
