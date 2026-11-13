#ifndef ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_ENTRANCE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct GlowSprite;
struct GlowState;

void VenusLighthouseEntrance_EmitDust(struct Actor *object, s32 x, s32 y,
                                      s32 z);
void VenusLighthouseEntrance_StartDust(void);
void VenusLighthouseEntrance_StopDust(void);
extern u32 gVenusLighthouseEntrance_BirdLands[];

void EffectSprite_DrawAtWorldPosition(struct GlowSprite *sprite,
                                      struct GlowState *state);

void VenusLighthouseEntrance_UpdateBlastColumn(void);
void VenusLighthouseEntrance_UpdateDust(void);

void VenusLighthouseEntrance_BirdsFlyOff(void);

void VenusLighthouseEntrance_ThievesAttack(void);

void VenusLighthouseEntrance_ThiefWaitsForBackup(void);

void VenusLighthouseEntrance_PointToMeetingSpot(void);

#endif
