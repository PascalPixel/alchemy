#ifndef TLA_GABOMBA_STATUE_CHAMBER_H
#define TLA_GABOMBA_STATUE_CHAMBER_H

#include "types.h"
#include "system/system.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "system/frame.h"

struct GabombaStatueChamberObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[2];
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

void FieldPsynergy_RunHandler(s32 type);

#include "field/field_sequence_event.h"

void Actor_MoveToObject(s32 actor, s32 target);

// ---- this overlay

void GabombaStatueChamber_DropIntoHole(s32 x, s32 z);

extern struct ActorPlacement gGabombaStatueChamber_Room1Placements[];
extern struct ActorPlacement gGabombaStatueChamber_Room3Placements[];
extern struct ActorPlacement gGabombaStatueChamber_Placements[];
extern struct Trigger gGabombaStatueChamber_Room1Triggers[];
extern struct Trigger gGabombaStatueChamber_Room2Triggers[];
extern struct Trigger gGabombaStatueChamber_Room3Triggers[];
extern struct Trigger gGabombaStatueChamber_Triggers[];
extern u32 gGabombaStatueChamber_Room1Regions[];

#endif
