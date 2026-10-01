/* Declarations shared by McCoy's palace sources in Bilibin (resource_38d). */
#ifndef KYUDEN_H
#define KYUDEN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"

#define NULL ((void *)0)

enum {
    FLAG_REWARD_TAKEN = 0x84e
};

/* The scene's tables and action tables, laid out after the code. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern const u8 Kyuden_FacingActions[];
extern const u8 Kyuden_PacingActions[];

/* Main-image code reached through the overlay's import veneers. Old-style
 * declarations: the scenes pass their arguments as plain words. */
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

/* Moves the next dialogue line on by amount messages. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

#endif
