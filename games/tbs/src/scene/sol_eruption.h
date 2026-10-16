#ifndef SOL_ERUPTION_H
#define SOL_ERUPTION_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum CollapseActor {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED + 1,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  ACTOR_FELIX = ACTOR_FIRST_PLACED + 5,
  ACTOR_ALEX
};

#include "facing_object.h"

typedef struct {
  u8 filler0[8];
  s32 unk8;
  u8 fillerC[4];
  s32 unk10;
  u8 filler14[16];
  s32 unk24;
  u8 filler28[4];
  s32 unk2C;
  u8 filler30[8];
  s32 unk38;
  u8 filler3C[4];
  s32 unk40;
} ViewCenter;

extern u8 *gParticleWork[];

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern struct FieldActor *gArcEffects[];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
