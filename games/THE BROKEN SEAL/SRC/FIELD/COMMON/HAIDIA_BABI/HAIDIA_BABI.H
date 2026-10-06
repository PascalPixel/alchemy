#ifndef HAIDIA_BABI_H
#define HAIDIA_BABI_H

#include "TYPES.H"
#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

enum {

    ITEM_MYTHRIL_BAG = 224
};

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

void HaidiaBabi_CloseScreenAndExit(s32 exit);
void ActorPresentation_SetTwoSceneCells(void);
void FieldScene_RunSupplementalSequenceOne();
void SceneEffect_UpdateAnchoredRiseFrame(union FieldObject *object);
void OverlayObject_UpdateArcFromAnchor(union FieldObject *object);

#endif
