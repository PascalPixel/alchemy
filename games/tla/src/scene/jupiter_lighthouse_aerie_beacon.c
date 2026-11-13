/// @file
/// The scene code of overlay 0x6a5 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_aerie_beacon/.

#include "types.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000038(s32 a0) {
  Event_PlaySound(0x98);
  Actor_SetSingleChildFlags(a0, 0);
  return 0;
}

s32 Func_02000050(s32 a0) {
  Actor_SetSingleChildFlags(a0, 1);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "../field/object/park_record.c"

#include "field_event.h"


void Func_02000090(struct Actor *self) {
  struct Actor *centre = Actor_Get(31);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Math_Cosine(angle) * (self->speedLimit + 3);
  sine = Math_Sin(angle);
  z = centre->z;
  x = self->x;
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  self->targetZ = z;
  self->timer += -0x800;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// Circles object 36, wider across than deep, by the actor's speed and its
/// byte at 98.
// As ☀️'s SceneEffect_UpdateCounterDrivenOrbit does.
void Func_020000ec(struct Actor *self) {
  struct Actor *centre = Actor_Get(36);
  u16 angle = self->timer;
  s32 cosine = Math_Cosine(angle);
  s32 radius = self->speedLimit + BYTE_AT(self, 98);
  self->x = centre->x + cosine * (radius + 6);
  self->z = centre->z + Math_Sin(angle) * (BYTE_AT(self, 98) + 4);
  self->targetX = self->x;
  self->targetZ = self->z;
  self->timer += -0x800;
}
