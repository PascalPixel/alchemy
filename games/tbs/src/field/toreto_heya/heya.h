#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

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

