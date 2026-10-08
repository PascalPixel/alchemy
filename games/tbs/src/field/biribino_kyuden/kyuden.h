#ifndef KYUDEN_H
#define KYUDEN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "facing_object.h"

#define NULL ((void *)0)

enum {
    FLAG_REWARD_TAKEN = 0x84e
};

extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern const u8 Kyuden_FacingActions[];
extern const u8 Kyuden_PacingActions[];

s32 ArcTan2(s32, s32);
void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
void Object_RefreshSelectorById();
void Object_SetTargetAndCallback();
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32, s32, s32);

void FieldScene_RunChestStep(s32 flag);
void FieldScene_RunSlotSubjectBranch(s32 actor, s32 item, s32 flag);
void Kyuden_RunKolimaRequest(void);
void FieldScene_RunPalaceGreeting(void);
void ConfigurePrimarySceneChannels(void);
void ConfigureSecondarySceneChannels(void);
void RunEventScript02(void);
void BiribinoKyuden_RunActorRowScene(void);

#endif
