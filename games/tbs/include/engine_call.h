#ifndef ALCHEMY_ENGINE_CALL_H
#define ALCHEMY_ENGINE_CALL_H

// Engine call layer: one typed inline for each of the thirteen main-image
// routines below that the scene, menu and debug overlays import; every overlay
// call of them goes through it. Main-image code calls them as ordinary
// functions; only the shared scene/common/scroll_initialize_hblank_dma.h, which
// object/psynergy_effects.c includes as lemurian_ship.c does, reaches the layer
// there. 本体の関数を呼ぶ層: オーバーレイは本体のルーチンをこのインライン関数を
// 通して呼ぶ (オーバーレイでは呼び出しは import スタブへ行く)。
// 引数はインラインの仮引数を通るので、呼び出しごとに定数を作り直す。
// field_service.h がこれを取り込む;
// シーンの命令層を使わない部屋はこれだけを使う。

#include "types.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "unit/window/window.h"
#include "unit/game/game.h"
#include "unit/import.h"

struct FieldActor;
struct FieldPosition;
struct MenuSprite;

// 本体のスケジューラにコールバックを登録する。シーン層の Task_AddCallback と
// FieldScheduler_AddOrUpdateCallback もこれを通る
// (オーバーレイはこのルーチンを一つの名前で呼ぶ)。
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

static inline void FieldUi_AdjustValueWithoutLimit(s32 message, s32 buffer) {
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

// 型 01
// の圧縮データを展開する。シーンのほかタイトルとクリアのメニューもこれを取り込む。
// 本体の field/camera.c の同じ展開は call.h のタグ付きの Value2 で呼ぶ。
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
