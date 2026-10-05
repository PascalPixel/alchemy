#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "CONFIGURED_EFFECT_SPAWN.H"

extern s16 *ToretoHeya_PaletteBuffer;
extern u8 ToretoHeya_ActionTable2[];
extern u8 ToretoHeya_ActionTable1[];
extern u8 ToretoHeya_SparkOrigin[];
extern u8 ToretoHeya_SparkCounter[];

void ToretoHeya_HandleFloorSwitch(int, int, int, int);
void ToretoHeya_PlayGesture();
void PartyInventory_FindOwner();
void Object_SetActionCallbackAndRefreshById();
void ToretoHeya_SpawnSwirlSparks();
void ToretoPalette_ApplyTint(void);
void ToretoHeya_AdvanceEffectMotion();

