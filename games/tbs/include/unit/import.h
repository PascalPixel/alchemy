#ifndef ALCHEMY_UNIT_IMPORT_H
#define ALCHEMY_UNIT_IMPORT_H

/// @file
/// The main-image routines scene overlays import.

#include "types.h"

struct FieldPosition;
struct ObjectRuntime;
struct SceneEvent;
struct ScenePlacement;
struct UiWindow;

void Effect_SoundAndFlash(void);
s32 Engine_BattleFxPlayCueAndStartEmitterOnTarget(s32 effect, s32 target,
                                                  s32 mode);
void Engine_BattleFxRunPageEffectForSlot(s32 slot, s32 element, s32 djinn);
void Engine_BattleFxSetWeightedResult(s32 zone_no, s32 slot_no);
struct UiWindow *Engine_DebugCreateWindow(s32 x, s32 y, s32 width, s32 height,
                                          s32 attrs);
void Engine_DebugFinalizeWindow(struct UiWindow *work, s32 release);
void Engine_DisplayScrollStartHBlankDma(s32 mode, s32 freqX, s32 stepX,
                                        s32 ampX, s32 freqY, s32 stepY,
                                        s32 ampY);
s32 Engine_InnCheckIn(s32 mode, s32 objectId);
void Engine_InventorySnapshotRestore(void);
void Engine_MapSetWorkFlagBits9To11(u32 v);
void Engine_OwnerRecalculateRatios(s32 owner_id);
s32 Engine_PartyInventoryFindOwner(s32 item_id);
void Engine_EventSetNextDestination(s32 first, s32 second);
void Engine_ResetSceneEffectCounter(void);
s32 Engine_ShopConfirmAct(s32 unitId);
void Engine_UiTextDrawQuantity(u32 value, u32 kind);
void Event_LoadAreaScript(const struct SceneEvent *events);
s32 GetPartyMemberCount(void);
void InventorySnapshot_Take(void);
s32 DjinnMenu_Open(void);
s32 Object_CheckMovementCollision(struct ObjectRuntime *object,
                                  struct FieldPosition *position);
void SceneEvents_UpdateInView(struct ScenePlacement *entry);
void UiText_DrawQuantity(u32 value, u32 kind);

#endif
