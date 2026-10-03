#ifndef HAIDIA_BABI_H
#define HAIDIA_BABI_H

#include "TYPES.H"
#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Vale house scene: the hooks its entry veneers export, the villagers'
 * dialogue, the Mythril Bag scene and its sparkle effects, and the Psynergy
 * steps.
 */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

enum {
    /* Message 0x182 + 224. */
    ITEM_MYTHRIL_BAG = 224
};

/* The scene's tables, in the overlay's read-only data. */
extern s32 gHaidiaBabiEntrances[];
extern s32 gHaidiaBabiExits2[];
extern s32 gHaidiaBabiExits[];
extern s32 gHaidiaBabiPlacements4[];
extern s32 gHaidiaBabiPlacements3[];
extern s32 gHaidiaBabiPlacements2[];
extern s32 gHaidiaBabiPlacements[];
extern s32 gHaidiaBabiEvents6[];
extern s32 gHaidiaBabiEvents5[];
extern s32 gHaidiaBabiEvents4[];
extern s32 gHaidiaBabiEvents3[];
extern s32 gHaidiaBabiEvents2[];
extern s32 gHaidiaBabiEvents[];

void SceneState_SetWork448To521AndRun(s32 exit);
void ActorPresentation_SetTwoSceneCells(void);
void FieldScene_RunSupplementalSequenceOne();
void SceneEffect_UpdateAnchoredRiseFrame(union FieldObject *object);
void OverlayObject_UpdateArcFromAnchor(union FieldObject *object);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

#endif
