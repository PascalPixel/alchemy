#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)

#include "scene_effect_sequence.h"
#include "iwram_call.h"

struct PeriodicEffectConfig {
    s32 kind;
    s32 variant;
    u8 pad_08[16];
    u16 id;
    u8 pad_1a[2];
    u8 *data;
    u8 pad_20[8];
};

extern u8 ShianMura_EffectConfig[];
extern u8 gShianMuraEntrances[];
extern u8 gShianMuraExits[];
extern u8 gShianMuraEvents[];
extern u8 ShianMura_ActionTable[];

s32 ShianMura_WatchGateTrigger();
void Actor_UpdatePresentationFlag(void);
extern u8 gShianMuraPlacements[];
extern u8 ShianMura_GateSteps1[];
extern u8 ShianMura_GateSteps2[];
extern u8 ShianMura_GateSteps3[];
extern u8 ShianMura_GateSteps4[];
extern u8 ShianMura_GateSteps5[];

void BattleFx_RunPageEffectForSlot();
void Effect_Spawn();
void BattleFx_PlayQueuedSound();
