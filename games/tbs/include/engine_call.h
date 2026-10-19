#ifndef ALCHEMY_ENGINE_CALL_H
#define ALCHEMY_ENGINE_CALL_H

// Engine call layer: one typed inline for each of the thirteen main-image
// routines below that the scene, menu and debug overlays import; every overlay
// call of them goes through it. Main-image code calls them as ordinary
// functions; only the shared scene/common/scroll_initialize_hblank_dma.h, which
// object/psynergy_effects.c includes as lemurian_ship.c does, reaches the layer
// there. Overlays call main-image routines through these inline functions;
// their calls resolve to import stubs. Passing arguments through inline
// parameters rematerializes constants at each call. field_service.h includes
// this layer; rooms without the scene command layer use it directly.

#include "types.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "unit/window/window.h"
#include "unit/game/game.h"
#include "unit/import.h"

struct FieldActor;
struct FieldPosition;
struct MenuSprite;

// Register a main-image scheduler callback. The scene layer's Task_AddCallback
// and FieldScheduler_AddOrUpdateCallback also use this wrapper, so overlays
// import this routine under one name.
static inline s32 FieldScheduler_AddCallback(SchedulerCallback callback,
                                             s32 priority) {
  return Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void FieldUiTextResource_Initialize(struct MenuSprite *cursor,
                                                  s32 *slot) {
  UiTextResource_Initialize(cursor, slot);
}

static inline void FieldUiTextResource_SetPosition(struct MenuSprite *cursor,
                                                   s32 x, s32 y) {
  UiTextResource_SetPosition(cursor, x, y);
}

static inline void FieldUi_AdjustValueWithoutLimit(s32 message, u16 *buffer) {
  Ui_AdjustValueWithoutLimit(message, buffer);
}

static inline void FieldMap_SetViewBounds(s32 minX, s32 minY, s32 maxX,
                                          s32 maxY) {
  Map_SetViewBounds(minX, minY, maxX, maxY);
}

static inline void FieldBattleFx_RunRisingObjectSequence(s32 actor, s32 mode,
                                                         s32 action) {
  Actor_JumpDownLedge(actor, mode, action);
}

static inline s32
FieldObject_CheckMovementCollision(struct FieldActor *actor,
                                   struct FieldPosition *position) {
  return Object_CheckMovementCollision((struct ObjectRuntime *)actor, position);
}

static inline s32 FieldGameFlag_GetByte(s32 flag) {
  return GameFlag_GetByte(flag);
}

static inline void FieldGameFlag_SetByte(s32 flag, s32 value) {
  GameFlag_SetByte(flag, value);
}

static inline s32 FieldParty_ListActiveOwners(s16 *owners) {
  return Party_ListActiveOwners(owners);
}

static inline s32 FieldUnit_AddHp(s32 owner, s32 delta) {
  return Unit_AddHp(owner, delta);
}

// Decode type-01 compressed data. Scenes and the title and clear menus use
// this wrapper. The same decode in main-image field/lucky_dice.c uses call.h's
// tagged Value2 instead.
static inline s32 FieldResource_DecodeType01(const void *source,
                                             void *destination) {
  return Resource_DecodeType01(source, destination);
}

static inline void DisplayScroll_StartHBlankDma(s32 mode, s32 freqX, s32 stepX,
                                                s32 ampX, s32 freqY, s32 stepY,
                                                s32 ampY) {
  Engine_DisplayScrollStartHBlankDma(mode, freqX, stepX, ampX, freqY, stepY,
                                     ampY);
}

#endif
