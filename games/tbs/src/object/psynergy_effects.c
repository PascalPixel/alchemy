#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "scene.h"
#include "battle_type.h"
#include "game_state.h"
#include "field_runtime.h"
#include "battle_system.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "event_work.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "field_obj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "owner_value.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "item_gift.h"
#include "battle_calc.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "battle_party.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "field_coordinate.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_effect_scripts.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/scroll_initialize_hblank_dma.h"

// 手に入れたアイテムのメッセージ (ドイツ語版はブラックオーブだけ別の文)
static inline void ItemGift_ShowGotItem(s32 item, s32 mode) {
#if defined(TBS_EDITION_DE)
  if (item == ITEM_BLACK_ORB) {
    UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, mode);
    return;
  }
#endif
  UiWork_PushValueSlotFar(item, 2);
  UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem, mode);
}

// イベントのワーク (gEventWork) のこのファイルでの見え方。共通の EventWork
// に欄のない 0xcb6 以降も持つ
struct FieldWork {
  u8 unk000[0x10];
  s32 view;
  struct FieldActor *actors[86];
  s16 action;
  s16 input;
  s16 exit;
  s16 menu;
  s16 examine;
  s16 start;
  u16 descriptor;
  s16 tile;
  s16 scene;
  s16 command;
  s16 item;
  s16 event;
  s16 count;
  s16 survived;
  s16 quantities[9];
  s16 itemArgument;
  u8 unk19c[2];
  s16 mode;
  u8 unk1a0[8];
  s32 unk1a8;
  u8 unk1ac[4];
  s32 unk1b0;
  s32 unk1b4;
  u8 unk1b8[4];
  u8 *cells;
  s32 transition;
  u16 unk1c4;
  u16 transitionActive;
  s32 transitionFrames;
  u8 unk1cc[0xaea];
  u16 busy;
  s16 effectActive;  // 効果が動いていれば 0 以外
  s16 effectFrames;  // 残りのフレーム
  s16 effectCenterX; // 効果の中心 (ドット)
  s16 effectCenterY;
  s16 unkcc0;
  u8 unkcc2[4];
  s8 resolvingAction; // エナジーの演出中
  s8 unkcc7;
  s16 queuedSound;
};

void Battle_UpdateModeFromShoulderButtons(void) {
  struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

  if (gDebugMode != 0) {
    if (gKeysPressed & KEY_L) {
      runtime->mode1cc = 0;
    }
    if (gKeysPressed & KEY_R) {
      runtime->mode1cc = -1;
    }
  }
}

void Battle_WaitMode0(s32 frames) {
  if (((struct BattleRuntime *)gEventWork)->mode1cc == 0 && frames != 0) {
    WaitFrames(frames);
  }
}

void Battle_InitializeRenderObject(void) {
  struct BattleRenderObject *object;

  object = ObjectTable_Get(gGameState.selectedActor);
  object->unk30 = 0x10000;
  object->unk34 = 0x8000;
  object->unk38 = 0x80000000;
  object->unk40 = 0x80000000;
  object->unk24 = 0;
  object->unk2c = 0;
  if (gGameState.movementMode == 1) {
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 0xC);
    return;
  }
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

void Battle_Reset(void) {
  struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

  UiTimedNotice_CloseIfActiveFar();
  Battle_InitializeRenderObject();
  if (runtime->unkcb6 != 0) {
    Battle_ResetEffectCounter();
  }
  runtime->unkcc2 = 0;
  runtime->unkcc4 = 0;
  runtime->unk1c8 = 0x10;
  runtime->mode1cc = 0;
  runtime->unk1da = 0xFFFF;
  runtime->unk1dc = -1;
  runtime->unk1de = -1;
  Scheduler_AddOrUpdateCallback(
      (const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
  GameFlag_ClearBitFar(0x132);
  runtime->objectId = gGameState.selectedActor;
  runtime->unk1f8 = 0;
}

void BattleFx_FinishAction(void) {
  Scheduler_RemoveCallback(Battle_UpdateModeFromShoulderButtons);
  Object_AttachWorkTargetToObject(gGameState.selectedActor, 1);
  GameFlag_RefreshLureCapFar();
}

void Event_NoOpHookA(void) {}

void Event_NoOpHook(void) {}

void Event_RunObjectHookAndWait(struct ScenePlacement *table) {
  Event_NoOpHook();
  ObjectTable_ResetForObject(table);
  WaitFrames(1);
  ObjectTable_Get(gGameState.selectedActor);
}

void Event_CallWithLastActiveObjectId(struct ScenePlacement *table) {
  Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

void Event_SetSceneEvents(s32 value) {
  struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

  work->events = (const struct SceneEvent *)value;
}

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value) {
  UnitGetFar(event_id);
  Party_AddActiveOwnerFar(event_id);
  if (value != 0) {
    Party_ShowJoinedMessageFar(event_id, value);
  }
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second) {
  Party_AddActiveOwnerFar(first);
  Party_AddActiveOwnerFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

s32 Event_ValidatePackedId(u32 packed_id) {
  s32 group = (packed_id >> 10) & 0xf;
  s32 entry = packed_id & 0x3ff;

  if (group > 7)
    return -1;
  if (GameFlag_TestFar(group) == 0)
    return -2;
  if (Equipment_HasValueFar(group, entry) == 0)
    return -3;
  return 0;
}

void Event_ClearInvalidPackedValues(void) {
  if (Event_ValidatePackedId(gGameState.firstShortcut) != 0)
    gGameState.firstShortcut = 0;
  if (Event_ValidatePackedId(gGameState.secondShortcut) != 0)
    gGameState.secondShortcut = 0;
}

void Party_RemoveOwnerRestored(s32 owner) {
  struct BattleUnit *vitals;
  s32 i;
  s32 count;
  s32 standing;

  Party_RemoveActiveOwnerFar(owner);
  Event_ClearInvalidPackedValues();
  vitals = UnitGetFar(owner);
  vitals->hp = vitals->maxHp;
  vitals->pp = vitals->maxPp;
  Unit_UpdateGauges(vitals);
  vitals->poison = 0;
  standing = 0;
  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    vitals = UnitGetFar(gGameState.activeOwners[i]);
    if (vitals->hp != 0)
      standing++;
  }
  if (standing == 0) {
    vitals = UnitGetFar(gGameState.selectedActor);
    vitals->hp = 1;
    Unit_UpdateGauges(vitals);
  }
}

s32 Party_CheckMemberValueTotal(s32 id) {
  s32 count;
  s32 sum = 0;
  s32 i;

  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++)
    sum += Inventory_CountItemFar(gGameState.activeOwners[i], id);

  if (sum >= count * 30) {
    UiWork_PushValueSlotFar(id, 2);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore, 1);
    UiWork_PushValueSlotFar(id, 2);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore + 1, 1);
    return -1;
  }
  return 0;
}

s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  struct EventRuntime *work;
  s32 saved;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  work = (struct EventRuntime *)gEventWork;
  saved = work->message;
  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    ItemGift_ShowGotItem(item, 1);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryAnyMore, 1);
  retry:
    UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop, 1);
    result = Shop_PickUnitItemFar(&member, &slot);
    if (result == -1) {
      if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 4, 1);
        goto retry;
      }
      UiWork_PushValueSlotFar(item, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 1, 5);
      result = Object_CallSpawnRoutineAtOrigin(1);
      UiWork_FinalizePendingCoreFar();
      if (result != 0)
        goto retry;
      Item_AdjustCounterFar(item, 1);
      UiWork_PushValueSlotFar(item, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 2, 1);
      work->message = saved;
      return owner;
    } else {
      UnitGetFar(member);
      {
        /* FAKEMATCH: the ROM loads the slot argument before the member. Only
         * the English build, without the thirty-item check below, needs the
         * copy; the direct call matches the rest. */
        s32 s = slot;

        count = Shop_GetSelectionState(member, s);
      }
#if !defined(TBS_EDITION_EN)
      if (Inventory_CountItemFar(member, item) > 29) {
        UiWork_PushValueSlotFar(member, 1);
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 7, 1);
        goto retry;
      }
#endif
      for (i = 0; i < count; i++)
        Inventory_DiscardFar(member, slot);
      owner = PartyInventory_AddFar(item);
      AudioCommand_PlayFar(83);
      if (owner == gGameState.selectedActor) {
        ItemGift_ShowGotItem(item, 3);
      } else {
        UiWork_PushValueSlotFar(item, 2);
        UiWork_PushValueSlotFar(owner, 1);
        UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 3);
      }
      work->message = saved;
      return owner;
    }
  } else {
    AudioCommand_PlayFar(83);
    ItemGift_ShowGotItem(item, 3);
    if (owner != gGameState.selectedActor) {
      UiWork_PushValueSlotFar(item, 2);
      UiWork_PushValueSlotFar(owner, 1);
#if defined(TBS_EDITION_DE)
      UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 1);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                    \
    defined(TBS_EDITION_IT)
      UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem + 1, 1);
#else
      UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem + 1, 3);
#endif
    }
    work->message = saved;
  }
  return owner;
}

//--------------------------------------------------------------------------
// try_add_and_return_owner.c
// 持ち物：アイテムを持たせて、持ち主を返す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// アイテムを持たせる
// 持てればキャラクター番号を、持ち物がいっぱいなら -1 を返す
// item   : アイテム番号
// unused : 使わない
// owner  : 持たせるキャラクター番号
//--------------------------------------------------------------------------
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 owner) {
  if (Inventory_AddItemFar(owner, item) >= 0)
    return owner;
  return -1;
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Inventory_NoOpCallback(void) {}

struct EventPromptWork {
  struct EventRuntime event;
  u8 unk200[0xac2];
  s16 resourceBase;
  s16 width;
};

void Inventory_ReservedNoOp(void) {}

void Object_WaitUntilChildValueDiffers(s32 objectId, s32 value) {
  struct ObjectRuntime *obj = ObjectTable_Get(objectId);

  if (obj != 0 && obj->animationKind == 1) {
    s32 cnt = 0;
    u8 *p = &((struct AnimationObject *)obj->animation)->lastNo;

    while (cnt <= 89) {
      WaitFrames(1);
      if (value != *p) {
        break;
      }
      cnt++;
    }
  }
}

s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 forceBottom) {
  struct EventPromptWork *work = (struct EventPromptWork *)gEventWork;
  s32 sprite = ((struct ScenePlacement *)BattleAction_FindDescriptor(
                    work->event.speaker))
                   ->sprite;
  struct UiWindow *window = (struct UiWindow *)work->event.messageWindow;
  struct UiWindow *side = (struct UiWindow *)work->event.sideWindow;
  s32 useBottom = 1;
  s32 answer;

  while (gKeysPressed != 0)
    WaitFrames(1);

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames(3);

  if (forceBottom == 0) {
    s32 bottom = window->y + window->height;

    if (side != 0) {
      s32 sideBottom = side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > 15)
      useBottom = 0;
  }

  answer = Menu_RunConfirmSelectionFar(useBottom, work->resourceBase,
                                       work->width, 0);
  if (answer != 0) {
    Object_SetModeById(actor, 4);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    Object_WaitUntilChildValueDiffers(actor, 4);
  } else {
    Object_SetModeById(actor, 3);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    Object_WaitUntilChildValueDiffers(actor, 3);
  }

  return answer;
}

s32 Object_CallSpawnRoutineAtOrigin(s32 value) {
  return Menu_RunConfirmSelectionFar(value, 0, 0, 0);
}

//--------------------------------------------------------------------------
// draw_draw_quantity_pair_with_cue.c
// メッセージ：わざを覚えさせて「覚えた」と出す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// わざを覚えさせ、効果音とメッセージを出して、送られるまで待つ
// unit : キャラクター番号（メッセージの名前 1）
// ability : わざの番号（メッセージのわざ 4）
//--------------------------------------------------------------------------
void UiText_DrawQuantityPairWithCue(s32 unit, s32 ability) {
  // 引数なしの宣言だが、r0・r1 の unit・ability がそのまま渡る
  OwnerAction_AddFar();
  AudioCommand_PlayFar(83);
  UiWork_PushValueSlotFar(unit, 1);
  UiWork_PushValueSlotFar(ability, 4);
  UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}

void Event_StartScreenTransition(void) {
  struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

  DisplayTransition_Start(runtime->value1c0, runtime->value1c8);
  runtime->status1c6 = 1;
}

void Event_FinishScreenTransition(void) {
  struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

  DisplayTransition_Finish(runtime->value1c0, runtime->value1c8);
  runtime->status1c6 = 0;
}

void Event_WaitTransitionFrames(void) {
  WaitFrames(((struct EventRuntime *)gEventWork)->value1c8);
}

void Event_SetDestination(s32 first, s32 second) {
  ((struct EventRuntime *)gEventWork)->value170 = 999;
  gGameState.scene = first;
  gGameState.entrance = second;
}

void Event_SetSavedDestination(u16 first, u16 second) {
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;
  state->value170 = 999;
  gGameState.savedScene = first;
  gGameState.savedEntrance = second;
}

void Event_SetExitRequest(u16 value) {
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;
  state->value170 = value;
}

//--------------------------------------------------------------------------
// result_request.c
// バトル効果：イベントから決まった敵との戦闘を用意する
//--------------------------------------------------------------------------

#define AERIE 98           // 頂上の嵐の背景で戦うエンカウント表
#define DJINN_NO_HOP 0x800 // ジンを跳ねさせない
#define DJINNI_0 300       // ジン 0 番の pendingDjinnEvent

//--------------------------------------------------------------------------
// エンカウント表 zone の slot 番目の敵グループとの戦闘を用意する
// ワールドマップ（mode 3）では、選択中の者の足もとの地形も引く
//--------------------------------------------------------------------------
void BattleFx_SetWeightedResult(s32 zoneNo, s32 slotNo) {
  s32 zone = zoneNo;
  s32 slot = slotNo;
  struct EventRuntime *ev = (struct EventRuntime *)gEventWork;

  ev->value17c = BattleFx_GetWeightedResult(zone, slot); // 敵グループ
  if (zone == AERIE && slot == 0)
    gGameState.special = (s32)&ResourceId_VenusAerieStormBackdrop;
  if (ev->mode19e == 3)
    BattleFx_LookupResult((u8 *)ObjectTable_Get(gGameState.selectedActor) +
                          8); // 位置
  BattleFx_SelectBattleCue(zone, slot);
}

//--------------------------------------------------------------------------
// ジン no 番との戦闘を用意する
// flag の下 8 ビットはジンのオブジェクトで、DJINN_NO_HOP がなければ跳ねさせる
// no は属性ごとに 20 ずつ（Djinn_ResolvePendingEvent）
//--------------------------------------------------------------------------
void BattleFx_SetPhaseRequest(s32 flag, s32 no) {
  struct EventRuntime *ev;
  struct GameState *gs;
  s32 skip;

  ev = (struct EventRuntime *)gEventWork;
  skip = flag & DJINN_NO_HOP;
  flag &= 0xff;

  if (skip == 0)
    EventActor_RunHopAndLeapSequence(flag);

  gs = &gGameState;
  gs->pendingDjinnEvent = (no + DJINNI_0) | skip;
  ev->value17c = BattleFx_GetPhaseResult(no); // ジンの敵グループ
  if (ev->mode19e == 3) {
    void *obj = ObjectTable_Get(gs->selectedActor);

    BattleFx_LookupResult((u8 *)obj + 8); // 位置
  }
  BattleFx_SelectBattleCue(0, 0);
}

void Party_SetFields1ceAnd1d0(s32 first, s32 second) {
  gGameState.nextScene = first;
  gGameState.nextEntrance = second;
}

void Event_SetDefeatDestination(s32 first, s32 second) {
  gGameState.defeatScene = first;
  gGameState.defeatEntrance = second;
}

void Event_RequestDestination(u16 first, u16 second) {
  struct EventRuntime *work = (struct EventRuntime *)gEventWork;
  gGameState.scene = first;
  gGameState.entrance = second;
  work->value170 = 999;
}

void BattleFx_SetQueuedSoundAndPlay(s32 soundId) {
  ((struct FieldWork *)gEventWork)->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = 0x121;
  }
  AudioCommand_PlayFar(0x12a);
  AudioCommand_PlayFar(soundId);
}

void BattleFx_PlayQueuedSound(void) {
  s16 soundId = ((struct FieldWork *)gEventWork)->queuedSound;

  if (soundId != -1)
    AudioCommand_PlayFar(soundId);
}

struct FieldActor *Object_GetById(s32 objectId) {
  struct FieldActor *object = ObjectTable_Get(objectId);

  if (object == NULL)
    return NULL;
  return object;
}

void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
  }
}

void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectMotion_SetActionCallback(object, action);
  }
}

void ObjectMotion_EnableActionAndResetMotion(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  u8 enabled;

  if (object != NULL) {
    enabled = 1;
    object->actionFlags = enabled | object->actionFlags;
    ObjectDispatch_InitFromTable6Far(object);
  }
}

void Object_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->linkedObject =
        (struct ObjectRuntime *)Object_GetById(linkedObjectId);
    ObjectMotion_SetActionCallback(object,
                                   (s32)gObjectMotion_LinkedActionScript);
  }
}

void Object_RefreshSelectorById(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_WaitForValue16Far(object);
}

void Object_SetActionCallbackAndRefreshById(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectMotion_SetActionCallback(object, action);
    ObjectDispatch_WaitForValue16Far(object);
  }
}

void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    Script_WaitForEventTimeoutFar(object);
  }
}

void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

void ObjectMotion_SetPositionAndReset(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    Script_WaitForEventTimeoutFar(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

void ObjectMotion_CenterOnColumn(u32 objectId, s32 action, s32 zOffset) {
  struct ObjectRuntime *object;
  s16 currentAngle;
  s32 snappedAngle;
  s16 angleRemainder;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    currentAngle = ((union FieldCoordinate *)&object->x)->part.pixel;
    snappedAngle = currentAngle;
    if (currentAngle < 0) {
      snappedAngle += 15;
    }
    snappedAngle >>= 4;
    snappedAngle *= 16;
    angleRemainder = currentAngle - snappedAngle;
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + ((8 - angleRemainder) << 16),
                            object->y, object->z);
    Script_WaitForEventTimeoutFar(object);
    ObjectMotion_SetActionVariant(objectId, action);
    Object_SetMoveTargetFar(object, object->x, object->y,
                            object->z + (zOffset << 16));
  }
}

void ObjectMotion_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

void ObjectMotion_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  ObjectMotion_OffsetPositionAndReset(objectId, xOffset, zOffset);
  if (object != NULL) {
    Script_WaitForEventTimeoutFar(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct ObjectRuntime *object = ObjectTable_Get(arg0);

  if (object != NULL) {
    struct ObjectRuntime *other;

    ObjectMotion_SetSpeedParameters(arg0, 0x9999, 0x4CCC);
    other = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
    if (other != NULL)
      ObjectMotion_SetHorizontalPositionWithTerrain(arg0, other->x, other->z);
    object->movementState = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    ObjectMotion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (const s32 *)&gObjectMotion_LaunchScript);
    object->action = arg3;
  }
}

void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    Script_WaitForEventTimeoutFar(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;
  s32 terrainId;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_InitFromTable6Far(object);
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    object->x = x;
    object->z = z;
    if (1 & object->flags) {
      terrainId = object->terrainId;
      tileX = x;
      if (tileX < 0) {
        tileX += 0xFFFF;
      }
      tileX >>= 0x10;
      tileZ = z;
      if (tileZ < 0) {
        tileZ += 0xFFFF;
      }
      tileZ >>= 0x10;
      terrainHeight = Map_GetTerrainHeightFar(terrainId, tileX, tileZ) << 0x10;
      object->y = (object->y - object->terrainHeight) + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_InitFromTable6Far(object);
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    object->x = x;
    object->y = y;
    object->z = z;
    if (1 & object->flags) {
      s32 terrainId = object->terrainId;
      tileX = x;
      if (tileX < 0) {
        tileX += 0xFFFF;
      }
      tileX >>= 0x10;
      tileZ = z;
      if (tileZ < 0) {
        tileZ += 0xFFFF;
      }
      terrainHeight = Map_GetTerrainHeightFar(terrainId, tileX, tileZ >> 0x10)
                      << 0x10;
      object->y = (object->y - object->terrainHeight) + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void Object_SetModeById(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_ApplyArgumentToChildrenFar(object, action);
}

void Object_SetActionById(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object,
                                           action);
}

void ObjectMotion_WaitForAnimationChange(u32 objectId) {
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  /* FAKEMATCH: volatile restores the form the bytes prove: saved lives in a
   * 4-byte frame, stored once and loaded again for each compare. Without it
   * differ 29, 26 instructions for 31 and no frame. */
  volatile s32 saved;
  s32 i;

  object = ObjectTable_Get(objectId);
  if (object != NULL && object->animationKind == 1) {
    sprite = object->animation;
    saved = sprite->lastNo;
    for (i = 0; i <= 89; i++) {
      WaitFrames(1);
      if (saved != sprite->lastNo) {
        break;
      }
    }
  }
}

void Motion_SetModeAndWaitAnimation(u32 objectId, s32 action) {
  Object_SetModeById(objectId, action);
  ObjectMotion_WaitForAnimationChange(objectId);
}

void ObjectMotion_NoOp(void) {}

void ObjectMotion_Launch(u32 objectId, s32 speed, s32 eventId) {
  struct ObjectRuntime *object;
  u8 *objectFlags;
  s32 updatedFlags;
  s32 verticalVelocity;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    objectFlags = &object->flags;
    updatedFlags = *objectFlags | 2;
    verticalVelocity = speed << 16;
    *objectFlags = updatedFlags;
    object->velocityY = verticalVelocity;
    if (speed > 5) {
      AudioCommand_PlayFar(0x99);
    } else {
      AudioCommand_PlayFar(0x98);
    }
    Battle_WaitMode0(eventId);
  }
}

void ObjectMotion_SetVariantCallback(u32 objectId, s32 variant) {
  struct ObjectRuntime *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL && variant > 0) {
    if (variant > 3) {
      variant = 3;
    }
    ObjectDispatch_InitializeFar(
        (struct DispatchObject *)object,
        (const s32 *)(gObjectMotion_VariantScripts + ((3 - variant) << 7)));
  }
}

void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant) {
  ObjectMotion_SetVariantCallback(objectId, variant);
  Object_RefreshSelectorById(objectId);
}

void BattleFx_UpdateParticleLinearMotion(struct FieldActor *particle) {
  s32 velocityX;
  s32 x;
  s32 velocityZ;
  s32 z;
  s32 velocityY;
  s32 vz;
  velocityX = particle->speed;
  x = particle->x.fixed + velocityX;
  particle->x.fixed = x;
  particle->targetX = x;
  velocityZ = particle->acceleration;
  z = particle->z.fixed + velocityZ;
  particle->z.fixed = z;
  particle->targetZ = z;
  velocityY = particle->y.fixed + 0x400;
  particle->y.fixed = velocityY;
  particle->targetY = velocityY;
  particle->speed = (velocityX - velocityX / 0x12);

  vz = velocityZ;
  if (velocityZ < 0) {
    vz += 0xF;
  }
  particle->acceleration = (velocityZ - (vz >> 4));
}

void BattleFx_SpawnBurstParticle(struct FieldActor *source, s32 optional) {
  struct FieldActor *object;
  struct FieldSprite *child;
  s32 value;

  object = (struct FieldActor *)Object_CreateFar(
      222, source->x.fixed, source->y.fixed, source->z.fixed);
  if (object != 0) {
    child = object->sprite;
    switch (Random16() & 1) {
    case 1:
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
      ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                   (const s32 *)gBattleFx_BurstParticleScriptA);
      break;
    default:
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
      ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                   (const s32 *)gBattleFx_BurstParticleScriptB);
      break;
    }

    if (optional != 0)
      ObjectGroup_SetChildValue(object, optional);

    object->motionFlags = 0;
    value = Random16() % 10 + 5;
    object->acceleration = -0x1999 * value;
    value = Random16() % 15 - 7;
    value <<= 1;
    object->speed = 0x1999 * value;
    object->timer = 0;
    object->update = BattleFx_UpdateParticleLinearMotion;
    child->flags = 0;
    child->priority = source->sprite->priority;
  }
}

void BattleFx_RunRisingObjectSequence(s32 sequenceArg, s32 modeOrFrame,
                                      s32 optionalAction) {
  s32 nextY;
  s32 baseZ;
  struct ObjectRuntime *object;
  u8 *objectFlags;

  object = (struct ObjectRuntime *)Object_GetById(sequenceArg);
  baseZ = object->z;
  if (object != 0) {
    AudioCommand_PlayFar(0x121);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, modeOrFrame);
    WaitFrames(10);
    objectFlags = &object->flags;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    {
      u8 flags = 2;
      flags |= *objectFlags;
      *objectFlags = flags;
    }
    object->velocityY = 0x40000;
    Object_SetMoveTargetFar(object, object->x, object->y, baseZ + 0xC0000);
    WaitFrames(6);
    AudioCommand_PlayFar(0xD9);
    modeOrFrame = 0;
    ObjectMotion_ArmCallback(sequenceArg, 0x5000, 0);
    *objectFlags = 0;
    do {
      nextY = object->y + 0xFFFE0000;
      object->y = nextY;
      object->targetY = nextY;
      WaitFrames(1);
      if ((optionalAction != -1) && (modeOrFrame & 1)) {
        BattleFx_SpawnBurstParticle((struct FieldActor *)object,
                                    optionalAction);
      }
      modeOrFrame++;
    } while ((u32)modeOrFrame <= 0xD);
    *objectFlags = 3;
    object->velocityY = 0x30000;
    Object_SetMoveTargetFar(object, object->x, object->y, baseZ + 0x100000);
    Script_WaitForEventTimeoutFar(object);
    for (modeOrFrame = 0;
         (u32)modeOrFrame <= 0xB3 && object->y > object->terrainHeight;
         modeOrFrame++) {
      WaitFrames(1);
    }
    WaitFrames(2);
    BattleFx_PlayQueuedSound();
  }
}

void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct ObjectRuntime *first;
  struct ObjectRuntime *second;

  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);
  if (first != 0 && second != 0) {
    first->angle = ArcTan2(second->z - first->z, second->x - first->x);
    Battle_WaitMode0(wait);
  }
}

void Object_LinkPair(s32 firstId, s32 secondId, s32 wait) {
  void *first = ObjectTable_Get(firstId);
  void *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    FacingObject_TurnPairToFaceEachOther(first, second);
    Battle_WaitMode0(wait);
  }
}

void FacingObject_TurnPairToFaceEachOther(struct FacingObject *raw_first,
                                          struct FacingObject *raw_second) {
  struct ObjectRuntime *first = (struct ObjectRuntime *)raw_first;
  struct ObjectRuntime *second = (struct ObjectRuntime *)raw_second;
  s32 toward;
  s32 away;
  s32 frame;
  s32 turning;
  s32 delta;

  if (first == NULL || second == NULL)
    return;
  toward = (u16)ArcTan2(second->z - first->z, second->x - first->x);
  away = toward + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    turning = 2;
    delta = (s16)(toward - first->angle);
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      first->angle += delta;
    } else {
      turning = 1;
    }
    delta = (s16)(away - second->angle);
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      second->angle += delta;
    } else {
      turning--;
    }
    if (turning == 0)
      break;
    WaitFrames(1);
  }
}

void ObjectTable_DestroyById(s32 index) {
  void *object = ObjectTable_Get(index);
  struct ObjectSlotTable *table = (struct ObjectSlotTable *)gEventWork;

  if (object != 0) {
    ObjectDispatch_ReleaseFar(object);
    table->slots[index] = NULL;
  }
}

void ObjectTable_DestroyNoOp(void) {}

void ObjectGroup_ConfigureChildValue(s32 objectId, s32 value) {
  s32 flags;
  struct DispatchObject *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    flags = 0x100 & value;
    if (flags != 0) {
      ((struct ScriptMotionObject *)object)->hook = (void (*)(
          struct ScriptMotionObject *))ObjectGroup_ApplyIndexedChildValue;
      return;
    }
    ((struct ScriptMotionObject *)object)->hook = NULL;
    ObjectGroup_SetChildValue((struct FieldActor *)object, value);
  }
}

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object) {
  if ((object->kind & 0xf) == 1) {
    u8 childValue;
    struct AnimationObject *container;
    u8 childCount;

    childValue = gObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
    container = object->target.child;
    childCount = container->count;
    if (childCount != 0) {
      struct AnimationEntry **entries = container->entries;
      s32 remaining = childCount;
      do {
        struct AnimationEntry *entry = *entries++;
        if (entry != 0 && entry->script != 0) {
          entry->param = childValue;
        }
        remaining--;
      } while (remaining != 0);
    }
    container->dirty = 1;
  }
}

void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 value) {
  struct DispatchObject *object = (struct DispatchObject *)actor;

  if ((object->kind & 0xf) == 1) {
    struct AnimationObject *container = object->target.child;
    u8 raw_count = container->count;

    if (raw_count != 0) {
      struct AnimationEntry **entry = container->entries;
      u32 count = raw_count;
      do {
        struct AnimationEntry *item = *entry++;
        if (item != NULL && item->script != 0) {
          item->param = value;
        }
        count--;
      } while (count != 0);
    }
    container->dirty = 1;
  }
}

void Object_SetTargetAndCallback(u32 objectId, s32 targetId,
                                 const void *callback) {
  struct ObjectRuntime *first = ObjectTable_Get(objectId);
  struct ObjectRuntime *second = ObjectTable_Get(targetId & 0xff);

  if (first != NULL && second != NULL) {
    first->linkedObject = second;
    if (!(targetId & 0x10000)) {
      first->action = 40;
      first->acceleration = second->acceleration * 2;
      first->speedLimit = second->speedLimit;
      first->unk59 = 0;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)first, callback);
  }
}

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object) {
  s32 delta = 0;

  if (object != NULL) {
    s32 target_angle = (u16)object->action;
    s32 current_angle = object->angle;
    delta = (s16)(target_angle - current_angle);
    if (delta != 0) {
      if (delta > 4096)
        delta = 2048;
      if (delta < -4096)
        delta = -2048;
      object->angle = current_angle + delta;
    }
  }
  return delta;
}

void Object_ResetTargetAndSetMode1(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->targetX = ACTOR_NO_TARGET;
    object->targetY = ACTOR_NO_TARGET;
    object->targetZ = ACTOR_NO_TARGET;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

void ObjectMotion_ArmCallback(s32 objectId, s32 angle, s32 wait) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->action = angle;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (const s32 *)gObjectMotion_StepAngleScript);
    Battle_WaitMode0(wait);
  }
}

void ObjectMotion_SetActionVariant(s32 objectId, s32 priority) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL && (object->animationKind & 0xF) == 1) {
    struct FieldSprite *sprite = object->animation;

    sprite->priority = priority;
    sprite->secondPriority = priority;
    object->unk23 &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId) {
  void *p;
  u8 flags;
  u32 shape;

  p = Object_GetById(sourceId);
  p = ((struct ObjectRuntime *)p)->animation;
  flags = ((struct FieldSprite *)p)->vramBlock;
  shape = ((struct FieldSprite *)p)->tile;

  p = Object_GetById(targetId);
  p = ((struct ObjectRuntime *)p)->animation;
  ((struct FieldSprite *)p)->vramBlock = flags;
  ((struct FieldSprite *)p)->tile = shape;
}

void ObjectVisual_ReservedNoOp(void) {}

void Event_SetMessageId(s32 value) {
  ((struct EventRuntime *)gEventWork)->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key) {
  struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
  s32 result = -1;
  struct ObjectRuntime *entry = state->actors[key & 0x0fff];

  if (entry != 0 && entry->animationKind == 1)
    result = ((struct AnimationObject *)entry->animation)->entries[0]->animId;
  return result;
}

s32 ObjectTable_FindActiveByValue(s32 value) {
  struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
  s32 result = -1;
  s32 index;
  struct ObjectRuntime *object;

  for (index = 8; index <= 65; index++) {
    object = state->actors[index];
    if (object != 0 && object->animationKind == 1 &&
        ((struct AnimationObject *)object->animation)->entries[0]->animId ==
            value) {
      result = index;
      break;
    }
  }
  return result;
}

struct UiWindow *UiText_OpenMessageAtObject(s32 arg, s32 unused) {
  struct UiRenderWork *win;
  struct EventRuntime *work;
  s32 speaker;
  s32 extra;
  s32 tail;
  s32 margin;
  s32 height;
  s32 width;
  s32 top;
  s32 left;
  s32 pos[3];
  struct UiWindow *handle;
  s32 side;
  s32 flags;
  s32 message;
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 ret;
  s32 column;
#if !EDITION_INTERNATIONAL
  s32 none;
#endif

  win = (struct UiRenderWork *)gWindowWork[0];
  work = ((struct WorkPointers *)gWindowWork)->event;
  handle = 0;
  side = 0;
  speaker = ObjectTable_ReadActiveValue(arg);
  extra = 0;
  tail = 0;
  margin = 4;
  flags = arg & 0xf000;
  message = work->message;
  arg &= 0xfff;
  x = 0;
  object = ObjectTable_Get(arg);
  work->speaker = arg;
  y = 0;
  ret = 0;
  if (work->messageBusy == 0) {
    if (object != 0) {
      if (work->mode19e == 3) {
        Render_ProjectPoint(&object->x, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        ret = 1;
        y -= 2;
      } else {
        if (Object_GetScreenPosition(arg, pos) != -1)
          ret = 1;
        else
          ret = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (arg <= 7) {
      speaker = arg;
      object = ObjectTable_Get(gGameState.selectedActor);
      if (work->mode19e == 3) {
        Render_ProjectPoint(&object->x, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        ret = 1;
      } else {
        if (Object_GetScreenPosition(gGameState.selectedActor, pos) != -1)
          ret = 1;
        else
          ret = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    }
    if (!ret) {
      left = 15;
      top = 10;
    } else {
      left = 0;
      top = 0;
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
                                         (u32 *)&height);
      left = x - width / 2;
      if (flags & 0x4000)
        top = y - height - 1;
      else if (!(flags & 0x8000) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }
    if (win->mode != 0)
      margin = 5;
    column = x;
    if (flags & 0x1000) {
      column = x - margin - 2;
      if (column < 0)
        column = 0;
    } else if (flags & 0x2000) {
      column += 2;
      if (column + margin > 29)
        column = 29 - margin;
    } else if (column <= 15) {
      column = column - margin - 2;
      if (column < 0)
        column = x + 2;
    } else {
      column += 2;
      if (column + margin > 29)
        column = x - margin - 2;
    }
#if EDITION_INTERNATIONAL
    ret = Localization_LookupEntryIdFar(speaker);
    if (ret != -1) {
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
                                         (u32 *)&height);
      message = -1;
      if (top > y)
        tail = top - 5;
      else
        tail = top + height;
      if (tail < 0)
        tail = top + height;
      else if (tail + 5 > 19)
        tail = top - 5;
      if (top < tail) {
        s32 lines = height;

        UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                        (u32 *)&height);
        message = -1;
        extra = lines - height + 1;
      }
    } else if (top < y) {
      s32 lines = height;

      UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                      (u32 *)&height);
      extra = lines - height + 1;
      message = ret;
    }
#else
    none = Localization_LookupEntryIdFar(speaker);
    ret = -1;
    if (none != ret) {
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
                                         (u32 *)&height);
      message = ret;
      if (top > y)
        tail = top - 5;
      else
        tail = top + height;
      if (tail < 0)
        tail = top + height;
      else if (tail + 5 > 19)
        tail = top - 5;
    }
    if (top < y) {
      s32 lines = height;

      UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                      (u32 *)&height);
      message = -1;
      extra = lines - height + 1;
    }
#endif
    if (column < 0)
      column = 0;
    else if (column + margin > 29)
      column = 29 - margin;
    if (win->mode != 0) {
      WaitFrames(8);
      if (extra != 0)
        handle =
            UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
      else
        handle = UiText_OpenMessageWindowFar(message, left, top, 2);
    } else {
      s32 icon = BattleFx_GetResourceId(speaker);

      if (extra != 0)
        handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                             (icon << 16) | 17);
      else
        handle =
            UiText_OpenMessageWindowFar(message, left, top, (icon << 16) | 1);
    }
    if (win->mode != 0)
      side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
    else
      side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
    while (!UiWork_IsCompleteFar())
      WaitFrames(1);
  }
  work->messageWindow = (s32)handle;
  work->sideWindow = side;
  work->message++;
  return handle;
}

void BattleEv_RunWait(s32 action, s32 flag) {
  s32 maskedAction;
  struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;
  struct UiWindow *window = UiText_OpenMessageAtObject(action, flag);
  s32 messageId;
  u32 frames;

  frames = 0;
  WaitFrames(1);
  messageId = ObjectTable_ReadActiveValue(action);
  if (action <= 7) {
    maskedAction = action & 0x0fff;

    if (BattleAction_FindDescriptor(maskedAction) == 0) {
      messageId = maskedAction;
    }
  }
  UiWork_FinalizeEntityMatchingLocalizedIdFar(messageId);

  if (runtime->messageBusy == 0) {
    while (UiWork_IsIdleFar(window) == 0) {
      WaitFrames(1);
      frames++;
      if (frames > 600 || ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) &&
                           (gKeysHeld & KEY_L) && (gKeysHeld & KEY_A))) {
        UiWork_FinalizePendingCoreFar();
      }
    }
  }

  WaitFrames(1);
}

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value) {
  BattleEv_RunWait(first, second);
  Battle_WaitMode0(value);
}

s32 BattleEventRuntime_ProcessAction(s32 objectId, s32 actionId) {
  s32 result;
  struct EventRuntime *runtime;
  struct GameState *save;

  UiText_OpenMessageAtObject(objectId, actionId);
  save = &gGameState;
  result = Inventory_PromptAndSetObjectMode(save->selectedActor, 0);
  if (result == 0) {
    BattleEv_RunWait(objectId, actionId);
    runtime = (struct EventRuntime *)gEventWork;
    runtime->message += 1;
  } else {
    runtime = (struct EventRuntime *)gEventWork;
    runtime->message += 1;
    BattleEv_RunWait(objectId, actionId);
  }
  return result;
}

void BattleEventRuntime_EmptyCallback(void) {}

void Event_ShowSpeakerMessage(s32 speaker) {
  // 位置は未初期化のまま読まれる (original bug: the position is never set)
  struct {
    s32 x, y;
  } pos;
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;

  speaker &= 0xfff;
  ObjectTable_Get(speaker);
  state->speaker = speaker;
  if (state->messageBusy == 0) {
    s32 px = pos.x;
    s32 py = pos.y;
    s32 win;

    if (py > 119)
      py += 32;
    else
      py -= 32;
    px = px < 8 ? 8 : px;
    if (px > 312)
      px = 312;
    py = py < 20 ? 20 : py;
    if (py > 220)
      py = 220;
    win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
    state->messageWindow = win;
    while (UiWork_IsIdleFar((struct UiWindow *)win) == 0)
      WaitFrames(1);
  }
  state->message++;
}

void ObjectTable_CallRefreshHook(s32 unused0, s32 unused1, s32 x, s32 y) {
  Event_ShowMessageAtPosition(unused0, unused1, x, y);
}

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;
  s32 py = y;
  s32 px = x;
  s32 minX = 8;
  s32 minY = 20;
  s32 ret;

  if (py > 119)
    py += 32;
  else
    py -= 32;

  if (x < minX)
    px = minX;
  if (px > 312)
    px = 312;
  if (py < minY)
    py = minY;
  if (py > 220)
    py = 220;

  ret = UiText_OpenMessageWindowFar(state->message, px, py, 1);
  while (UiWork_IsIdleFar((struct UiWindow *)ret) == 0)
    WaitFrames(1);
  state->message++;
}

//--------------------------------------------------------------------------
// table_run_if_active.c
// フィールド：話し手の表示の後しまつ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// アクターに絵があれば、その絵番号を話し手としてウィンドウの表示を終える
// id : アクター番号
//--------------------------------------------------------------------------
void ObjectTable_RunIfActive(s32 id) {
  s32 no;

  no = ObjectTable_ReadActiveValue(id);
  if (no != -1)
    UiWork_FinalizeEntityMatchingLocalizedIdFar(no);
}

void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 first_x, s32 first_y,
                                      s32 first_arg, s32 first_extra,
                                      s32 second, s32 second_x, s32 second_y,
                                      s32 second_arg, s32 second_extra) {
  struct EventRuntime *rt = (struct EventRuntime *)gEventWork;
  s32 id0 = ObjectTable_ReadActiveValue(first);
  s32 id1 = ObjectTable_ReadActiveValue(second);
  s32 h0;
  s32 h1;

  h0 = UiText_OpenMessageWindowFar(rt->message++, first_x, first_y,
                                   BattleFx_GetResourceId(id0) << 16);
  UiWindow_CreateWithSideObjectFar(id0, 0, first_arg, first_extra);

  h1 = UiText_OpenMessageWindowFar(rt->message++, second_x, second_y,
                                   BattleFx_GetResourceId(id1) << 16);
  UiWindow_CreateWithSideObjectFar(id1, 0, second_arg, second_extra);

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames(1);
  while ((gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
    WaitFrames(1);

  WaitFrames(1);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(id0);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(id1);
  UiWork_FinalizePendingCoreFar();
  WaitFrames(1);

  while (UiWork_IsIdleFar((struct UiWindow *)h0) == 0)
    WaitFrames(1);
  while (UiWork_IsIdleFar((struct UiWindow *)h1) == 0)
    WaitFrames(1);

  WaitFrames(1);
}

void Ui_SetRenderResultFromObject(s32 arg0) {
  struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
  u8 value;
  s32 ret;

  if (arg0 == 0x80000000) {
    *(s16 *)&state->result[0] = (value = 0);
    *(s16 *)&state->result[1] = value;
  } else {
    ret = BattleFx_GetResourceId(ObjectTable_ReadActiveValue(arg0));
    value = gUi_RenderResultValues[gGameState.unk207[5]];
    *(s16 *)&state->result[0] = ret;
    *(s16 *)&state->result[1] = value;
  }
}

void Object_AttachWorkTargetToObject(s32 id, s32 flag) {
  struct ObjectRuntime *obj;
  struct ObjectRuntime *target;
  struct EventWork *work;
  struct MapScrollWork *p;

  obj = ObjectTable_Get(id);
  work = Runtime_AllocateBlock(0x1B, 0xCCC);
  target = (struct ObjectRuntime *)work->viewCenter;
  p = gMapWork[0];
  if (obj != 0) {
    p->origin = &target->x;
    ObjectDispatch_InitFromTable4WithArgumentFar(target, obj);
    if (flag == 0) {
      target->x = obj->x;
      target->y = obj->y;
      target->z = obj->z;
      WaitFrames(1);
      if (((struct EventRuntime *)work)->mode19e != 3) {
        Map_ApplyWorkOriginAndSpanFar();
      }
    }
  }
}

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second) {
  struct EventWork *owner = Runtime_AllocateBlock(0x1b, 0xccc);
  ((struct ObjectRuntime *)owner->viewCenter)->speedLimit = first;
  ((struct ObjectRuntime *)owner->viewCenter)->acceleration = second;
}

void Motion_CamBounds(s32 requestedX, s32 requestedY, s32 requestedZ,
                      s32 useSetter) {
  s32 shouldUseSetter;
  struct EventWork *runtimeBlock;
  s32 minimumX;
  s32 minimumZ;
  s32 maximumX;
  s32 objectZOffset;
  s32 maximumZ;
  s32 positionX;
  s32 positionZ;
  s32 positionY;
  struct MapScrollWork *cameraState;
  struct ObjectRuntime *object;

  positionX = requestedX;
  positionY = requestedY;
  shouldUseSetter = useSetter;
  positionZ = requestedZ;
  runtimeBlock = Runtime_AllocateBlock(0x1B, 0xCCC);
  object = (struct ObjectRuntime *)runtimeBlock->viewCenter;
  cameraState = gMapWork[0];
  minimumX = cameraState->minX + 0x780000;
  objectZOffset = object->y;
  minimumZ = cameraState->minY + objectZOffset + 0x600000;
  maximumX = cameraState->maxX + 0xFF880000;
  maximumZ = cameraState->maxY + objectZOffset + 0xFFC00000;
  cameraState->origin = &object->x;
  ObjectDispatch_InitFromTable6Far(object);
  if (positionX == -1) {
    positionX = object->x;
  }
  if (positionY == -1) {
    positionY = object->y;
  }
  if (positionZ == -1) {
    positionZ = object->z;
  }
  if (positionX < minimumX) {
    positionX = minimumX;
  }
  if (positionZ < minimumZ) {
    positionZ = minimumZ;
  }
  if (positionX > maximumX) {
    positionX = maximumX;
  }
  if (positionZ > maximumZ) {
    positionZ = maximumZ;
  }
  if (shouldUseSetter == 0) {
    object->x = positionX;
    object->y = positionY;
    object->z = positionZ;
    WaitFrames(1U);
    if (((struct EventRuntime *)runtimeBlock)->mode19e != 3) {
      Map_ApplyWorkOriginAndSpanFar();
    }
  } else {
    Object_SetMoveTargetFar(object, positionX, positionY, positionZ);
  }
}

void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1) {
  struct ObjectRuntime *obj;

  obj = ObjectTable_Get(arg0);
  Runtime_AllocateBlock(0x1B, 0xCCC);
  if (obj != NULL) {
    Motion_CamBounds(obj->x, -1, obj->z, arg1);
  }
}

void BattleFx_CommitObjectPositionAndWait(void) {
  Script_WaitForEventTimeoutFar(
      ((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->viewCenter);
  Battle_WaitMode0(2);
}

struct FieldActor *Battle_GetWorkObject1e0(void) {
  return (s32)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))
      ->viewCenter;
}

void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target,
                                 s32 keepCurrentPosition) {
  struct ObjectRuntime *object;

  object = (struct ObjectRuntime *)((struct EventWork *)Runtime_AllocateBlock(
                                        0x1B, 0xCCC))
               ->viewCenter;
  if (target != NULL) {
    ObjectDispatch_InitFromTable4WithArgumentFar(object, NULL);
    object->linkedObject = target;
    if (keepCurrentPosition == 0) {
      object->x = target->x;
      object->y = target->y;
      object->z = target->z;
    }
  }
}

//--------------------------------------------------------------------------
// mapfx.c
// マップの表示範囲とズームの移り変わり、ほかのオブジェクトについて回るオブジェクト
//--------------------------------------------------------------------------
// スプライトの OAM 属性2 の上位バイト (bit 2〜3 が表示優先度)
#define ATTR2_HI(spr) (((u8 *)&((struct AnimationObject *)(spr))->part[0])[9])

//--------------------------------------------------------------------------
// マップの表示範囲を決める
//--------------------------------------------------------------------------
void Map_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
  struct MapScrollWork *work = gMapWork[0];

  work->minX = minX;
  work->minY = minY;
  work->maxX = maxX;
  work->maxY = maxY;
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : ズームを前の値から目標へ 1 段すすめる
// 画面中心のオブジェクトが止まっているあいだは待つ
//--------------------------------------------------------------------------
s32 BattleFx_StepRatioTransition(void) {
  struct PerspectiveWork *work;
  s16 *frames;
  s32 *from;
  s16 *step;
  s32 zoom;
  s32 delta;

  work = gMapWork[0];
  if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))
          ->viewCenter->paused != 0)
    return;
  frames = (s16 *)&work->zoomTransitionFrames;
  if (*frames == 0)
    return;
  from = &work->previousZoom;
  delta = work->zoom - *from;
  step = &work->zoomTransitionStep;
  (*step)++;
  /* FAKEMATCH: an explicit libgcc call, an ordinary call where the operator
   * makes a libcall block. Plain form measured 2026-10-13: the same sum with
   * the / operator differ 93 (84 instructions instead of 87). */
  zoom = *from + __divsi3(delta * *step, *frames);
  work->distance = Iwram_MulQ16(work->farPlane, zoom);
  gScanlinePitch = work->pitch + 1;
  if (*step == *frames) {
    *frames = 0;
    Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StepRatioTransition);
  }
}

//--------------------------------------------------------------------------
// ズームの移り変わりを始める (イベントの mode が 3 のときだけ)
// zoom   : 倍率 (16.16 固定小数)
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames) {
  struct PerspectiveWork *work = gMapWork[0];
  s32 inv;

  if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))
          ->mode != 3)
    return;
  {
    inv = Iwram_RatioMulQ14(zoom, 0x10000); // 1 / zoom
  }
  work->previousZoom = work->zoom;
  work->zoom = inv;
  work->zoomTransitionFrames = frames;
  work->zoomTransitionStep = 0;
  Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_StepRatioTransition,
                                0xc94);
}

//--------------------------------------------------------------------------
// ズームの移り変わりが終わるまで待つ (イベントの mode が 3 のとき、最長 300
// フレーム)
//--------------------------------------------------------------------------
void Event_WaitForZoomTransition(void) {
  s32 frames;
  struct PerspectiveWork *work = gMapWork[0];

  if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))
          ->mode == 3) {
    frames = 0;
    if (*(s16 *)&work->zoomTransitionFrames != 0) {
      do {
        WaitFrames(1);
        frames++;
      } while (frames < 300 && *(s16 *)&work->zoomTransitionFrames != 0);
    }
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 付いていく相手の位置に置き、y には絵の boxX と 8
// ドットを足す
//--------------------------------------------------------------------------
s32 BattleFx_CopyLinkedObjectPosition(void *self) {
  struct FieldActor *obj = self;
  struct FieldActor *link;

  link = obj->linkedObject;
  if (link != NULL) {
    obj->motionFlags = 0;
    obj->x.fixed = link->x.fixed;
    obj->y.fixed =
        link->y.fixed +
        ((s8)Resource_GetMetadataRecordFar(
             (s16)BattleAction_FindDescriptor(*(s16 *)&obj->work)->sprite)
             ->boxX
         << 16) +
        (8 << 16);
    obj->terrainHeight = link->terrainHeight;
    obj->z.fixed = link->z.fixed;
  }
  return 0;
}

//--------------------------------------------------------------------------
// id のオブジェクトについて回るオブジェクト (種類 21) を出し、wait フレーム待つ
// mode : 下位 4 ビットはオブジェクトの mode、下位 8 ビットが 6 なら音 110、
//        0x100 で表示優先度 1 (なければ相手と同じ優先度)
//--------------------------------------------------------------------------
void BattleFx_SpawnLinked(s32 id, s32 mode, s32 wait) {
  struct FieldActor *target;

  if ((mode & 0xff) == 6) {
    AudioCommand_PlayFar(110);
  }

  target = ObjectTable_Get(id);
  if (target != 0) {
    struct FieldActor *obj =
        Object_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

    if (obj != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_LinkedObjectScript);
      ObjectDispatch_ApplyArgumentToChildrenFar(obj, mode & 15);
      obj->motionFlags = 0;
      obj->timer = 0;
      obj->work = id;
      obj->update =
          (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
      obj->sprite->flags = 0;
      obj->linkedObject = target;

      if ((mode & 0x100) != 0) {
        s32 mask = 13;
        u8 attr = ATTR2_HI(obj->sprite);

        mask = -mask;
        mask &= attr;
        mask |= 4;
        ATTR2_HI(obj->sprite) = mask;
      } else {
        s32 pri = 12;
        u8 src = ATTR2_HI(target->sprite);
        u8 dst;
        s32 mask = 13;

        pri &= src;
        dst = ATTR2_HI(obj->sprite);
        mask = -mask;
        mask &= dst;
        mask |= pri;
        ATTR2_HI(obj->sprite) = mask;
      }
    }
    Battle_WaitMode0(wait);
  }
}

void BattleFx_ConfigureLinkedObject(s32 id, s32 flags) {
  struct FieldActor *object = ObjectTable_Get(id);
  struct FieldActor *child;
  struct FieldSprite *visual;
  s32 mode;

  child = 0;
  visual = 0;

  if (object == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == 2 || object->linkedObject == 0) {
      child = Object_CreateFar(209, object->x.fixed, object->y.fixed,
                               object->z.fixed);
    }
  } else {
    child = object->linkedObject;
    if (child == 0)
      return;
    ObjectDispatch_ReleaseFar(child);
    object->linkedObject = 0;
    return;
  }

  if (child == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case 1:
    ObjectDispatch_ApplyArgumentToChildrenFar(child, 1);
    object->linkedObject = child;
    child->timer = 1;
    break;
  case 2:
    ObjectDispatch_ApplyArgumentToChildrenFar(child, 2);
    ObjectDispatch_InitializeFar((struct DispatchObject *)child,
                                 (u32)gObjectMotion_WaitScript);
    child->timer = 1;
    break;
  }

  child->work = id;
  child->motionFlags = 0;
  child->update = BattleFx_CopyLinkedObjectPosition;
  visual = child->sprite;
  visual->flags = 0;
  child->linkedObject = object;

  if (flags & 0x100) {
    s32 mask = 13;
    u8 visualFlags = ((u8 *)visual)[9];

    mask = -mask;
    mask &= visualFlags;
    mask |= 4;
    ((u8 *)visual)[9] = mask;
  } else {
    s32 copiedFlags = 12;
    u8 sourceFlags = ((u8 *)object->sprite)[9];
    u8 destinationFlags;
    s32 clearMask = 13;

    copiedFlags &= sourceFlags;
    destinationFlags = ((u8 *)visual)[9];
    clearMask = -clearMask;
    clearMask &= destinationFlags;
    clearMask |= copiedFlags;
    ((u8 *)visual)[9] = clearMask;
  }
}

s32 Object_ClearChildAndCollisionFlags(void *obj) {
  ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
  ((struct ScriptObjectRuntime *)obj)->flags59 = 0;
  return 0;
}

s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object) {
  s32 step;
  struct ObjectRuntime *target;
  s32 deltaX;
  s32 deltaY;
  s32 cellX;
  s32 cellY;
  s32 newX;
  s32 newZ;
  s32 distance;

  target = object->linkedObject;
  if (target != 0) {
    deltaX = target->x - object->x;
    if (deltaX < 0)
      deltaX += 0xffff;
    cellX = deltaX >> 16;
    deltaY = target->z - object->z;
    if (deltaY < 0)
      deltaY += 0xffff;
    cellY = deltaY >> 16;
    distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
    step = object->action;
    if (distance >= step) {
      newX = object->x + (cellX << 20) / step;
      newZ = object->z + (cellY << 20) / step;
      Object_SetMoveTargetFar(object, newX, object->y, newZ);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    } else {
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    }
  }
  return 1;
}

s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object) {
  s16 turnStep;
  u16 currentAngle;
  s32 targetAngle;
  struct ObjectRuntime *target;

  target = object->linkedObject;
  if (target != NULL) {
    object->actionFlags &= 0xfe;
    targetAngle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
    currentAngle = object->angle;
    turnStep = targetAngle - currentAngle;
    if (turnStep != 0) {
      if (turnStep > 0x1000)
        turnStep = 0x1000;
      if (turnStep < -0x1000)
        turnStep = -0x1000;
      object->angle = (currentAngle + turnStep);
    }
  }
  return 1;
}

void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind) {
  switch ((kind - 1)) {
  case 0:
    kind = (s32)gObjectMotion_ActionKind1Script;
    break;
  case 1:
    kind = (s32)gObjectMotion_ActionKind2Script;
    break;
  case 2:
    kind = (s32)gObjectMotion_ActionKind3Script;
    break;
  case 3:
    kind = (s32)gObjectMotion_ActionKind4Script;
    break;
  case 4:
    kind = (s32)gObjectMotion_MoveTowardTargetScript;
    break;
  case 5:
    object->linkedObject = Object_GetById(gGameState.selectedActor);
    kind = (s32)gObjectMotion_TurnTowardLinkedScript;
    break;
  case 6:
    kind = (s32)gObjectMotion_ResetActionScript;
    break;
  default:
    break;
  }
  ObjectDispatch_InitializeFar((struct DispatchObject *)object, kind);
}

struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self,
                                                     s32 id) {
  struct ObjectRuntime *entry;
  struct ObjectRuntime *found;
  s32 cnt;
  s32 best;
  s32 dy;
  s32 dx;
  s32 dz;
  s32 dist;
  s32 angle;
  s32 turn;

  found = NULL;
  best = 40;
  entry = gObjectSlots;
  for (cnt = 0; cnt < 64; cnt++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->animationKind != 1)
      continue;
    dy = entry->y - self->y;
    if (dy >= 0) {
      if (dy > 0x2fffff)
        continue;
    } else {
      if (self->y - entry->y > 0x2fffff)
        continue;
    }
    dx = (entry->x - self->x) / 0x10000;
    dz = (entry->z - self->z) / 0x10000;
    dist = Iwram_Sqrt(dx * dx + dz * dz);
    if (dist >= best)
      continue;
    angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
    if (dist > 23) {
      turn = (s16)(angle - self->angle);
      if (turn < -0x2fff)
        continue;
      if (turn > 0x2fff)
        continue;
    }
    found = entry;
    best = dist;
  }
  if (found == NULL)
    return NULL;
  if (((struct AnimationObject *)found->animation)->entries[0]->animId != id)
    return NULL;
  return found;
}

#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_TARGET ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_ABOVE ((struct MapCell *)(Ram_MapCellBuffer - 0x200))

s32 Field_TryJumpForward(void) {
  struct ObjectRuntime *leader = Object_GetById(gGameState.selectedActor);
  s32 result = -1;
  s32 angle = (leader->angle + 0x2000) & 0xc000;
  u8 *flags = &leader->flags;
  u8 saved = *flags;
  struct FieldStepWork *work = gEventWork;
  s32 childMode = 1;
  struct FieldPosition position;
  struct FieldPosition *pos;
  struct ObjectRuntime *target;

  pos = &position;
again:
  pos->x = (leader->x & 0xfff00000) + 0x80000;
  pos->y = leader->y;
  pos->z = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 1)
    return -1;
  pos->x = (leader->x & 0xfff00000) + 0x80000;
  pos->y = leader->y;
  pos->z = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x200000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 0) {
    if (leader->animationKind == 1)
      childMode = ((struct FieldSprite *)leader->animation)->flags;
    Battle_Reset();
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
    WaitFrames(6);
    AudioCommand_PlayFar(152);
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildFlagsFar(leader, childMode & 0xfe);
    ObjectMotion_SetPositionAndCommit(
        gGameState.selectedActor,
        ((union FieldCoordinate *)&pos->x)->part.pixel,
        ((union FieldCoordinate *)&pos->z)->part.pixel);
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
    ObjectDispatch_SetSingleChildFlagsFar(leader, childMode);
    if ((target = Object_FindNearestFacingTarget(leader, 207)) != NULL ||
        (target = Object_FindNearestFacingTarget(leader, 205)) != NULL) {
      ObjectDispatch_ApplyArgumentToChildrenFar(target, 7);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(2);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(10);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
      WaitFrames(4);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
    } else {
      WaitFrames(6);
    }
    *flags = saved;
    BattleFx_FinishAction();
    if (work != NULL)
      work->encounterSteps += Iwram_MulQ16(work->encounterRate, 0x200000);
    if (GetMapCellCollisionFar(leader->terrainId, pos->x, pos->z) == 0xf9) {
      ObjectDispatch_ApplyArgumentToChildrenFar(leader, 1);
      WaitFrames(6);
      goto again;
    }
    result = 0;
  }
  return result;
}

s32 FieldEffect_UpdateGridPlacement(void) {
  struct ObjectRuntime *object = Object_GetById(gGameState.selectedActor);
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  Battle_Reset();

  if (gGameState.movementMode == 0) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_TARGET[index].collisionCode) {
      /* FAKEMATCH: frame local: the six-word probe keeps the native 24-byte
       * frame though only its first three words are the position. Plain forms
       * measured 2026-10-13: one struct FieldPosition or s32[3] differ 4 (a
       * 12-byte frame); two FieldPositions match as well. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      position->x = object->x;
      position->y = object->y - 0x100000;
      position->z = object->z;
      result = CheckMapPositionCellOccupiedFar((const s32 *)position);
      if (result != 0)
        goto failure;

      ObjectMotion_SetPositionAndCommit(gGameState.selectedActor, gridX, gridZ);
      object->speedLimit = 0x10000;
      ObjectMotion_ArmCallback(gGameState.selectedActor, 0xc000, 0);
      Object_RefreshSelectorById(gGameState.selectedActor);
      object->actionFlags = 1;
      object->flags = 0;
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 13);
      Object_SetMoveTargetFar(object, gridX << 16, object->y - 0x100000,
                              (gridZ << 16) + 0x100000);
      ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selectedActor);
      gGameState.movementMode = 1;
    } else {
      goto failure;
    }
  } else {
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 10);
    object->flags = 3;
    object->velocityY = 0x40000;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar(object, 1);
    Battle_WaitMode0(6);
    gGameState.movementMode = 0;
    object->actionFlags = 1;
    object->angle = 0xc000;
  }

  BattleFx_FinishAction();
  return 0;

failure:
  BattleFx_FinishAction();
  return -1;
}

s32 battle_owner_69(void) {
  struct ObjectRuntime *object = Object_GetById(gGameState.selectedActor);
  s16 childMode = 1;
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  Battle_Reset();

  if (object->animationKind == 1)
    childMode = ((struct FieldSprite *)object->animation)->flags;

  if (gGameState.movementMode == 0) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_ABOVE[index].collisionCode) {
      /* FAKEMATCH: frame local: the six-word probe keeps the native 24-byte
       * frame though only its first three words are the position. Plain forms
       * measured 2026-10-13: one struct FieldPosition or s32[3] differ 4 (a
       * 12-byte frame); two FieldPositions match as well. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      position->x = object->x;
      position->y = object->y;
      position->z = object->z;
      result = CheckMapPositionCellOccupiedFar((const s32 *)position);
      if (result != 0)
        goto failure;

      object->actionFlags = 0;
      ObjectMotion_SetPositionAndCommit(gGameState.selectedActor, gridX, gridZ);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
      WaitFrames(4);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
      object->velocityY = 0x40000;
      WaitFrames(4);
      object->flags = 0;
      childMode &= 0xfe;
      ObjectDispatch_SetSingleChildFlagsFar(object, childMode);
      object->speedLimit = 0x10000;
      object->velocityY = 0;
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
      WaitFrames(4);
      gGameState.movementMode = 1;
      object->actionFlags = 1;
      WaitFrames(8);
    } else {
      goto failure;
    }
  } else {
    object->flags = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 11);
    Object_SetMoveTargetFar(object, gridX << 16, object->y + 0x80000,
                            (gridZ << 16) - 0x100000);
    ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selectedActor);
    object->flags = 3;
    childMode |= 1;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar(object, childMode);
    Battle_WaitMode0(4);
    gGameState.movementMode = 0;
    object->actionFlags = 1;
  }

  BattleFx_FinishAction();
  return 0;

failure:
  BattleFx_FinishAction();
  return -1;
}

s32 Object_GetScreenPosition(s32 objectId, s32 *position) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  s32 *camera;
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  /* FAKEMATCH: the view position read as two adjacent words through one
   * pointer, so both loads share one base. Plain forms measured 2026-10-13:
   * viewX and viewY read through gMapWork[0], or through a struct MapScrollWork
   * pointer, differ 28 (61 instructions instead of 59). */
  camera = &((struct MapScrollWork *)gMapWork[0])->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / 0x10000;
  *position = z / 0x10000;
  if ((object->animationKind & 15) == 1)
    *position -=
        (s8)Resource_GetMetadataRecordFar(
            ((struct AnimationObject *)object->animation)->entries[0]->animId)
            ->boxX;
  return 0;
}

void ObjectEffect_ReservedNoOp941DC(void) {}

s32 Scene_FadeColorFromWhite(s32 unused) {
  struct EventRuntime *work = gEventWork;
  s32 i;
  s32 c;

  AudioCommand_PlayFar(gGameState.sceneCue);
  AudioCommand_PlayFar(288);
  AudioCommand_PlayFar(147);
  if (work->mode19e == 3) {
    Io_Write16(0x7fff, (u16 *)0x050001e6);
    DisplayTransition_Finish(0x401, 16);
    work->status1c6 = 0;
    WaitFrames(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16((c << 10) | (c << 5) | c, (u16 *)0x050001e6);
      WaitFrames(1);
    }
  } else {
    Io_Write16(0x7fff, &PLTT_BACKDROP);
    DisplayTransition_Finish(0x207, 16);
    work->status1c6 = 0;
    WaitFrames(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16((c << 10) | (c << 5) | c, &PLTT_BACKDROP);
      WaitFrames(1);
    }
  }
}

void ObjectEffect_PrepareContextEffect(s32 value) {
  u32 zero;
  u8 kind;
  struct ObjectRuntime *object;
  struct AnimationObject *context;
  struct AnimationEntry *effect;

  object = ObjectTable_Get(gGameState.selectedActor);
  context = object->animation;
  effect = (struct AnimationEntry *)ResourceMetadata_RegisterFar(context, 27);
  zero = 0;
  kind = 15;

  context->flags = zero;
  effect->param = kind;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = ACTOR_NO_TARGET;
  object->targetZ = ACTOR_NO_TARGET;
  ObjectDispatch_ApplyArgumentToChildrenFar(object, value);
  WaitFrames(18);
}

void ObjectEffect_BeginContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(0x1A);
  GameFlag_SetBitFar(0x120);
}

void ObjectEffect_BeginContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(0x19);
  GameFlag_SetBitFar(0x121);
}

void ObjectEffect_EndContextEffect(s32 arg0) {
  s32 zero;
  s32 mask;
  struct ObjectRuntime *obj = ObjectTable_Get(gGameState.selectedActor);
  struct AnimationObject *ctx = obj->animation;
  struct AnimationEntry *eff =
      (struct AnimationEntry *)ResourceMetadata_RegisterFar(ctx, 27);

  zero = 0;
  mask = 0xfff00000;
  ctx->flags = zero;
  eff->param = 15;
  obj->x = (obj->x & mask) + 0x80000;
  obj->z &= mask;
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, arg0);
  WaitFrames(30);
  ctx->count = 1;
  ResourceMetadata_ClearRecordFar(ctx->entries[1]);
  ctx->entries[1] = (void *)zero;
  ctx->flags = 1;
  obj->acceleration = 0x10000;
  obj->speedLimit = 0x10000;
  Object_SetMoveTargetFar(obj, obj->x, obj->y, obj->z + 0x80000);
  Script_WaitForEventTimeoutFar(obj);
}

s32 ObjectEffect_RunPendingFlagEvent(void) {
  s32 result = 0;
  s32 flag = 0x120;

  if (GameFlag_TestFar(flag) != 0) {
    ObjectEffect_EndContextEffect(24);
    GameFlag_ClearBitFar(flag);
    result = 1;
  } else {
    flag = 0x121;
    if (GameFlag_TestFar(flag) != 0) {
      ObjectEffect_EndContextEffect(23);
      GameFlag_ClearBitFar(flag);
      result = 2;
    } else {
      flag = 0x122;
      if (GameFlag_TestFar(flag) != 0) {
        s32 id;
        struct ObjectRuntime *obj;

        GameFlag_ClearBitFar(flag);
        id = gGameState.selectedActor;
        obj = ObjectTable_Get(id);
        obj->y += 0x00a00000;
        Motion_CamBounds(-1, -1, -1, 0);
        while (obj->y + obj->velocityY > obj->terrainHeight) {
          WaitFrames(1);
        }
        AudioCommand_PlayFar(159);
        obj->y = obj->terrainHeight;
        ObjectDispatch_ApplyArgumentToChildrenFar(obj, 22);
        Battle_WaitMode0(15);
        Object_AttachWorkTargetToObject(id, 1);
        result = 3;
      }
    }
  }
  return result;
}

void DisplayScroll_ArmHBlankDma(void) {
  volatile u16 *dma;
  struct DisplayScrollWork *state;
  u32 *source;
  u16 status;
  u32 control;
  volatile u32 *destination;

  state = gHBlankScrollWork;
  source = (u32 *)state->rows[state->page];

  dma = REG_DMA0;
  destination = (volatile u32 *)0x04000014;
  status = dma[5];
  control = status & (u16)~DMA_START_HBLANK_REPEAT;
  dma[5] = control;
  status = dma[5];
  control = status & (u16)~DMA_ENABLE;
  dma[5] = control;
  (void)dma[5];

  *destination = *source++;
  *destination = *source++;
  *destination = *source++;

  control = 0xa6600003;
  Dma_Set(source, destination, control, (volatile u32 *)dma);
}

void DisplayScroll_BuildAndSwapHBlankPage(void) {
  struct DisplayScrollWork *work = gHBlankScrollWork;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  u16 *row;
  s32 step;
  s32 phase;
  s32 amp;
  s32 i;
  u16 d;

  row = &work->rows[work->page ^ 1][0][0].x;
  step = work->stepX;
  phase = (work->frame + (u16)y3) * work->freqX;
  if (step == 0) {
    for (i = 0; i != 160; i++) {
      row[i * 6] = x3;
      row[i * 6 + 2] = x2;
      row[i * 6 + 4] = x1;
    }
  } else {
    u16 a = x3;
    u16 b = x2;
    u16 c = x1;

    amp = work->ampX;
    for (i = 0; i != 160; i++) {
      d = Iwram_MulQ16(gDisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
      *row = a + d;
      row += 2;
      *row = b + d;
      row += 2;
      *row = c + d;
      row += 2;
      phase += step;
    }
  }
  row = &work->rows[work->page ^ 1][0][0].y;
  step = work->stepY;
  phase = (work->frame + (u16)y3) * work->freqY;
  if (step == 0) {
    for (i = 0; i != 160; i++) {
      row[i * 6] = y3;
      row[i * 6 + 2] = y2;
      row[i * 6 + 4] = y1;
    }
  } else {
    u16 a = y3;
    u16 b = y2;
    u16 c = y1;

    amp = work->ampY;
    for (i = 0; i != 160; i++) {
      d = Iwram_MulQ16(gDisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
      *row = a + d;
      row += 2;
      *row = b + d;
      row += 2;
      *row = c + d;
      row += 2;
      phase += step;
    }
  }
  work->frame++;
  work->page ^= 1;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../scene/common/scroll_initialize_hblank_dma.h"

void DisplayScroll_DisarmHBlankDma(void) {
  volatile u16 *dma0;

  Scheduler_RemoveCallback(DisplayScroll_ArmHBlankDma);
  Scheduler_RemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
  dma0 = REG_DMA0;
  dma0[5] = 0xC5FF & dma0[5];
  dma0[5] = 0x7FFF & dma0[5];
  (void)dma0[5];
}

struct GroundParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  s32 entry;
  s32 tileBase;
  struct GroundParticle particles[32];
  s32 unk408;
  s32 stopped;
};

void FieldFx_UpdateGroundParticles(void) {
  struct MapScrollWork *view = gMapWork[0];
  struct GroundParticleWork *work = gMapWork[21];
  s32 *camera = &view->viewX;
  s32 cameraX = camera[0];
  s32 cameraZ = camera[1];
  struct GroundParticle *particle = work->particles;
  u32 i;

  for (i = 0; i < 32; i++, particle++) {
    s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_TestFar(0x166))
      particle->timer++;
    frame = &gFieldFx_GroundParticleFrames[5 * particle->timer];
    x = (particle->posX - cameraX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - cameraZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Runtime_PushSlotEntry(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *leader = view->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
      particle->timer = 16;
    }
  }
}

void BattleFx_UpdateStormFlash(void) {
  struct FieldBlendWork *work = gFieldBlendWork[0];
  struct BattleEffectBuffers *target = gFieldBlendWork[2];
  s16 *timer = &work->timer;

  if (*timer < 0)
    return;
  if (GameFlag_TestFar(358))
    work->timer = 128;
  switch ((*timer)--) {
  case 0:
    if (work->enabled != 0) {
      u32 a = Random16();
      u32 b = Random16();
      work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
      if (work->loud != 0)
        AudioCommand_PlayFar(172);
      else
        AudioCommand_PlayFar(171);
    }
  case 5:
  case 10:
    BattleFx_ApplyColorToTargetBuffer(work, 1);
    Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    BattleFx_ApplyColorToTargetBuffer(work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}

void FieldFx_StartGroundParticles(void) {
  struct GroundParticleWork *work;
  s32 *leader;
  struct GroundParticle *particle;
  u32 *words;
  u32 i;

  /* FAKEMATCH: one pointer, words, holds the fill's zero, the decode buffer and
   * each particle's cell cursor, all three in r6 as the native keeps them. A
   * plain fill = 0 takes r3 (differ 4); a cell cursor of its own reallocates
   * the loop and drops a stack slot (differ 58). 2026-10-13: an inline
   * allocate-then-clear helper (Runtime_AllocateBlock, then Dma_Fill32(0,
   * block, size)) differs 22: the fill moves before the sound call. */
  work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
  leader = ((struct MapScrollWork *)gMapWork[0])->origin;
  BattleFx_SetQueuedSoundAndPlay(170);
  words = 0;
  particle = work->particles;
  Dma_Fill32((u32)words, work, sizeof(struct GroundParticleWork));
  words = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_GroundParticleTiles, words);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramBlock_LoadCached(work->entry, 0x300, words);
  Runtime_ReleaseHeapBlock(14);
  for (i = 0; i < 32; i++) {
    s32 x;
    s32 z;

    words = (u32 *)particle;
    *words++ = 0;
    *words++ = 0x40000400;
    *words = 0xd400;
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
    particle->timer = (i & 15) + 1;
    particle++;
  }
  Io_Write16(0x3f00, &REG_BLDCNT);
  Io_Write16(0x1008, &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Scheduler_AddOrUpdateCallback(FieldFx_UpdateGroundParticles, 0xc80);
}

union DustWord {
  u32 value;
  void *link;
};

void FieldMotes_Update(void) {
  struct GroundParticleWork *work = WORK_SLOT(29);
  struct MapScrollWork *map = WORK_SLOT(8);
  u32 spawned = 0;
  u32 i;
  struct GroundParticle *mote = work->particles;

  for (i = 0; i < 32; i++, mote++) {
    s32 x;
    s32 y;

    if (--mote->timer != 0xffff) {
      s32 *camera = &map->viewX;
      s32 cameraX = camera[0];
      s32 cameraZ = camera[1];
      u16 age = mote->timer;
      u32 a;
      u32 b;

      if (GameFlag_TestFar(0x166)) {
        mote->timer++;
        mote->delay--;
      }
      a = Random16();
      b = Random16();
      x = ((mote->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1) - 1;
      y = ((mote->posZ - mote->posY - cameraZ) >> 16) - age;
      if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
        if (mote->timer < 60) {
          mote->tile = work->tileBase + 16;
          mote->delay += 3;
        } else if (mote->timer < 90) {
          mote->tile = work->tileBase + 8;
          mote->delay++;
        } else {
          mote->tile = work->tileBase;
        }
        if ((gFrameCount >> 3) & 1)
          mote->tile += 4;
        mote->x = x;
        mote->y = y - (mote->delay >> 2);
        mote->shape = 0;
        mote->size = 1;
        Runtime_PushSlotEntry(mote, 240);
      } else {
        mote->timer = 0;
      }
    }
    if (spawned < 8 && mote->timer == 0) {
      s32 *leader = map->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      mote->posZ = y;
      mote->posX = x;
      mote->posY = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}

void FieldMotes_Start(void) {
  struct GroundParticleWork *work;
  struct GroundParticle *mote;
  u8 *tiles;
  u32 i;
  u32 size;

  size = 0x410;
  work = Runtime_AllocateBlock(29, size);
  mote = work->particles;
  /* FAKEMATCH: reusing the allocation size for the cleared word keeps the zero
   * in r1, the register size was passed in; a plain zero = 0 takes r3 (differ
   * 4). 2026-10-13: an allocate-then-clear helper, as an inline function or a
   * statement macro (Runtime_AllocateBlock, then Dma_Fill32(0, block, size)),
   * differs 10: the zero takes r3 and mote is formed after the fill. */
  size = 0;
  Dma_Fill32(size, work, sizeof(*work));
  tiles = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_MoteTiles, tiles);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramBlock_LoadCached(work->entry, 0x300, tiles);
  Runtime_ReleaseHeapBlock(14);
  for (i = 0; i < 32; i++) {
    s32 *pos = ((struct MapScrollWork *)gMapWork[0])->origin;
    /* FAKEMATCH: reusing the integer field cursor for the height x argument
     * keeps the stores in r1. A plain s32 * cursor with x of its own puts the
     * cursor in r2 and the leader's base in r0 (differ 16). */
    u32 p = (u32)mote;
    s32 z;
    *(s32 *)p = 0;
    p += sizeof(s32);
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = Map_GetTerrainHeightFar(0, (s32)p >> 16, z >> 16) << 16;
    mote->timer = (i & 15) + 1;
    mote++;
  }
  Io_Write16(0x3f00, &REG_BLDCNT);
  Io_Write16(0x1008, &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Scheduler_AddOrUpdateCallback((s32)(FieldMotes_Update), 0xc80);
}

void FieldEffect_UpdateSparkles(void) {
  /* FAKEMATCH: needed to match: an unsigned-long frame read keeps size and flip
   * stores together. The plain volatile read differs by 3: it splits the two
   * bit-field stores. */
  struct GroundParticleWork *work;
  u32 spawned;
  u32 i;
  struct MapScrollWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct GroundParticle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = WORK_SLOT(29);
  spawned = 0;
  map = WORK_SLOT(8);
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->particles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_TestFar(0x166) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gFieldFx_SparkleFrames[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->posY - cameraZ) / 0x10000 +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
        frames = *(unsigned long *)&gFrameCount;
        sparkle->y = sy;
        sparkle->affineIndex = ((frames >> 1) & 1) << 3;
        Runtime_PushSlotEntry(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i <= 31);
}

void FieldEffect_InitSparkles(void) {
  u32 size = 0x410;
  struct GroundParticleWork *work = Runtime_AllocateBlock(29, size);
  struct GroundParticle *p = work->particles;
  u8 *buf;
  u32 i;
  s32 clear;

  /* FAKEMATCH: needed to match: reusing the allocation size for the fill word
   * keeps zero in r1; a separate helper value differs by four instructions.
   * 2026-10-13: an allocate-then-clear helper, as an inline function or a
   * statement macro (Runtime_AllocateBlock, then Dma_Fill32(0, block, size)),
   * differs 10: the zero takes r3 and p is formed after the fill. */
  size = 0;
  Dma_Fill32(size, work, sizeof(*work));
  buf = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_SparkleTiles, buf);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramBlock_LoadCached(work->entry, 0x200, buf);
  Runtime_ReleaseHeapBlock(14);
  i = 0;
  clear = 0;
  do {
    struct MapScrollWork *view = gMapWork[0];
    s32 *leader;
    /* FAKEMATCH: needed to match: an integer field cursor keeps the view load
     * order; reusing it for the height x argument keeps the leader in r2.
     * Separate temporaries differ by 14 instructions. */
    u32 attr;
    s32 z;

    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(union DustWord);
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(union DustWord);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = Map_GetTerrainHeightFar(0, (s32)attr >> 16, z >> 16) << 16;
    p->timer = (i & 15) + 1;
    i++;
    p++;
  } while (i < 32);
  Scheduler_AddOrUpdateCallback((s32)(FieldEffect_UpdateSparkles), 0xc80);
}

void BattleFx_StartTwelveFrameBlend(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;

  work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  buffers = gPaletteBufferWork;
  Dma_Fill32(0, work, sizeof(struct FieldBlendWork));
  BattleFx_BuildBuffer(0x10003, buffers, work, 1);
  BattleFx_BuildBuffer(0x10005, buffers, work->to, 1);
  BattleFx_InterpolateBuffers(work->to, work->from, work->delta, 12);
  BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

void BattleFx_SetBlock30ValuesMaxZero(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 0x7FFF;
  state->enabled = 0;
}

void BattleFx_SetBlock30Values12Zero(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

void BattleFx_SetBlock30Values128One(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

void BattleFx_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  s16 *target;

  work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  buffers = gPaletteBufferWork;
  Dma_Fill32(0, work, sizeof(struct FieldBlendWork));
  BattleFx_BuildBuffer(from, buffers, work, 1);
  target = work->to;
  BattleFx_BuildBuffer(to, buffers, target, 1);
  BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
  BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

//--------------------------------------------------------------------------
// motion_move_halfway_toward_target.c
// オブジェクト：つながった相手への追従
//--------------------------------------------------------------------------

// ほかのモジュールの関数 (共通ヘッダーにまだ宣言が無いもの)

// このファイルの関数 (定義より前で使うもの)

// FieldActor と同じ物体。位置をひとつのベクトルとして渡すための見え方
struct Object {
  u8 pad0[6];
  u16 angle;
  struct EffectVector pos;
  u8 pad1[14];
  u8 kind;
  u8 pad2[0x2d];
  struct AnimationObject *sprite;
};

//--------------------------------------------------------------------------
// つながったオブジェクトとの距離を、x・y・z とも半分つめる
// obj : 動かすオブジェクト
//--------------------------------------------------------------------------
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj) {
  struct ObjectRuntime *target = obj->linkedObject;

  obj->x += (target->x - obj->x) / 2;
  obj->y += (target->y - obj->y) / 2;
  obj->z += (target->z - obj->z) / 2;
}

//--------------------------------------------------------------------------
// hop.c
// 跳ねて分身を引く演出、ジン加入の演出、シーンのオブジェクトの再配置
//--------------------------------------------------------------------------

struct ActorDescriptor {
  s16 action;
  s16 scene; // 配置先のシーン番号 - 252
  u8 pad4[4];
  s32 x; // 配置座標 (16.16 固定小数)
  s32 y;
  s32 z;
};

#define CLONE_MAX 8 // 分身の数

//--------------------------------------------------------------------------
// 後ろへ 3 回跳ね、分身を引いて前へ跳ぶ
// id : 跳ねるオブジェクト
//--------------------------------------------------------------------------
void EventActor_RunHopAndLeapSequence(s32 id) {
  struct FieldActor *actor;
  struct FieldActor *copy;
  struct FieldActor *prev;
  void *res;
  s32 dir;
  s32 back;
  s32 dist;
  s32 kind;
  s32 value;
  s32 i;
  struct FieldActor *copies[CLONE_MAX];

  // 選択中のオブジェクトの向きを 90 度単位に丸める
  actor = Object_GetById(id);
  dir = (Object_GetById(gGameState.selectedActor)->facing + 0x2000) & 0xc000;
  Battle_Reset();
  WaitFrames(10);
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(id, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(id, 1);
  back = dir + 0x8000;
  WaitFrames(20);
  ObjectMotion_ArmCallback(id, back, 0);
  WaitFrames(10);
  actor->sprite->priority = 0;
  actor->facing = back;
  Object_GetById(id)->steeringFlags &= ~1;
  actor->motionFlags = 2;

  // 向いた方の逆へ 16 ドットずつ 3 回跳ねる
  dist = 16 << 16;
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             dist, dir);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 4, 0);
  Script_WaitForEventTimeoutFar(actor);
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             dist, dir);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 4, 0);
  Script_WaitForEventTimeoutFar(actor);
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             dist, dir);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 4, 0);
  Script_WaitForEventTimeoutFar(actor);
  WaitFrames(20);

  // 同じスプライトの分身を作り、ひとつ前を追いかけるようにつなぐ
  kind = ((struct AnimationObject *)actor->sprite)->entries[0]->animId;
  value = 9;
  if (kind == 90)
    value = 2;
  if (kind == 92)
    value = 10;
  if (kind == 91)
    value = 9;
  res = NULL;
  prev = actor;
  for (i = 0; i < CLONE_MAX; i++) {
    copy = (struct FieldActor *)Object_CreateFar(
        kind, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->steeringFlags |= 1;
      copy->update = (void *)ObjectMotion_MoveHalfwayTowardTarget;
      copy->facing = actor->facing;
      copy->sprite->priority = 0;
      Animation_ApplyChildValuesFar(copy, value);
      ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
      ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
      res = Object_ReplaceResourceEntry(copy->sprite, res);
      copy->linkedObject = prev;
      prev = copy;
    }
  }

  // 向いた方へ 64 ドット跳び、動きを止める
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             64 << 16, dir + 0x8000);
  AudioCommand_PlayFar(136);
  ObjectMotion_Launch(id, 12, 0);
  WaitFrames(24);
  actor->motionFlags = 0;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  actor->velocityY = 0;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  ObjectDispatch_ApplyArgumentToChildrenFar(actor, 0);
  actor->sprite->priority = 2;
  BattleFx_FinishAction();
}

//--------------------------------------------------------------------------
// ジンを仲間に加え、属性ごとの演出のあとに加入メッセージを出す
// slot  : 演出の対象 (-1 で演出なし)
// element   : ジンの属性
// djinn : 属性内のジン番号
//--------------------------------------------------------------------------
void BattleFx_RunPageEffectForSlot(s32 slot, s32 element, void *djinn) {
  struct FieldWork *work;
  s32 owner;

  // 持ち主を決める (フラグ 366 のときは 0 番にし、入手フラグだけ立てる)
  work = (struct FieldWork *)gEventWork;
  if (GameFlag_TestFar(366) != 0) {
    owner = 0;
    GameFlag_SetBitFar((u8 *)djinn + element * 20 + 48); // ジン入手フラグ
  } else {
    owner = Djinn_AddToLeastLoadedOwnerFar(element, djinn);
  }

  if (owner >= 0) {
    Battle_Reset();
    Battle_SetObjectFlag5bWhenMode3();
    if (slot != -1) {
      if (work->mode == 3)
        BattleEffect_ClearOutOfBoundsObjects();

      if (element == 0) // ヴィーナス
        BattleFx_RunVenusDjinnCapture(slot);
      else if (element == 1) // マーキュリー
        BattleFx_RunMercuryDjinnCapture(slot);
      else if (element == 2)
        BattleEffect_RunPhasedRadialParticleSequence(slot);
      else if (element == 3)
        BattleFx_FinishHeavyImpact(slot);

      work->actors[slot] = 0;
      if (work->mode == 3)
        BattleEffect_ClearAllObjects();
    }

    Djinn_ShowJoinedMessageFar(owner, element, djinn);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_FinishAction();
  }
}

//--------------------------------------------------------------------------
// 保留中のジンイベントのシーン (300〜380) に属するオブジェクトを置きなおす
// 0x800 が立っていれば、選択中のオブジェクトから z を 32 ドット引いた位置へ
//--------------------------------------------------------------------------
void FieldObject_PlaceSceneActors(void) {
  s32 flags = gGameState.pendingDjinnEvent & 0xf000;
  u32 scene = gGameState.pendingDjinnEvent & 0xfff;
  s32 id;

  if (GameFlag_TestFar(0x109) != 0 && flags == 0) {
    flags = scene & 0x800;
    scene &= 0x7ff;
    if (scene - 300 <= 80 && gGameState.sceneChangeReason > 0) {
      for (id = 8; id <= 65; id++) {
        struct ActorDescriptor *desc =
            (struct ActorDescriptor *)BattleAction_FindDescriptor(id);
        struct ObjectRuntime *obj;

        if (desc == NULL || desc->scene - 48 != scene - 300)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->flags = 3;
          Object_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct ObjectRuntime *selection =
              ObjectTable_Get(gGameState.selectedActor);

          Object_SetPositionAndResetMotionFar(obj, selection->x, selection->y,
                                              selection->z - (32 << 16));
        }
        ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
      }
    }
  }
}

struct MapActor {
  s16 id;
  s16 event;
};

void Djinn_ResolvePendingEvent(s32 capture) {
  struct GameState *st = &gGameState;
  s16 *pending = &st->pendingDjinnEvent;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (capture == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < 300 || flag > 380)
        return;
      if (st->sceneChangeReason > 0 && st->sceneChangeReason != 999)
        return;
      GameFlag_SetBitFar(flag - 172);
      *pending = kind;
    } else if (kind == 0x1000) {
      if (st->sceneChangeReason == 1)
        GameFlag_SetBitFar(flag);
      *pending = capture;
    }
    return;
  }
  if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= 300 && flag <= 380) {
      flag &= 0x7ff;
      if (st->sceneChangeReason > 0) {
        s32 djinn = flag - 300;
        s32 element = djinn / 20;
        s32 index = djinn % 20;
        s32 actor;

        for (actor = 8; actor <= 65; actor++) {
          struct MapActor *found =
              (struct MapActor *)BattleAction_FindDescriptor(actor);
          if (found != 0 && found->event - 48 == flag - 300) {
            WaitFrames(40);
            BattleFx_RunPageEffectForSlot(actor, element, (void *)index);
            break;
          }
        }
      }
    }
  }
  gGameState.pendingDjinnEvent = 0;
}

// 演出スロット 24 個を動かす
static __inline__ void UpdateAllEffectSlots(void) {
  struct EffectSlot *slot;
  s32 n;

  slot = ((struct BattleFxScene *)gEffectWork)->slots;
  n = 23;
  do {
    n -= 1;
    EffectSlot_Update(slot);
    slot++;
  } while (n >= 0);
}

void BattleFx_UpdateAllSlots(void) { UpdateAllEffectSlots(); }

void BattleFx_InitializeSlots(void) {
  void *work;

  work = Runtime_AllocateHeapBlock(56, sizeof(struct BattleFxScene));
  Dma_Fill32(0, work, sizeof(struct BattleFxScene));
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllSlots, 3200);
}

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct EffectSlot *slot;
  s32 i;

  Scheduler_RemoveCallback(BattleFx_UpdateAllSlots);
  for (i = 0; i < 24; i++) {
    slot = &scene->slots[i];
    if (slot->active != 0)
      BattleFx_ClearOwnedSlot(slot);
  }
  Runtime_ReleaseHeapBlock(56);
  WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}

void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor) {
  struct MotionObject *target;
  struct EffectVector pos;

  target = (struct MotionObject *)Object_GetById(gGameState.selectedActor);
  if (actor->state == 0) {
    actor->orbitRadius += 1;
    actor->orbitAngle += 1;
    if ((s16)actor->age == 60) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 1) {
    actor->orbitAngle += 1;
    if ((s16)actor->age == 40) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 2) {
    actor->orbitAngle += 1;
    pos.x = target->x;
    pos.y = target->y + 0x140000;
    pos.z = target->z;
    Camera_WorldToScreen(&pos);
    actor->originX += (pos.x - actor->originX) / 8;
    actor->originZ += (pos.z - actor->originZ) / 8;
    if ((s16)actor->age == 40) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 3) {
    actor->orbitRadius -= 1;
    actor->orbitAngle += 1;
    if ((s16)actor->age == 60) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 4) {
    BattleFx_ClearOwnedSlot(actor);
  }
  pos.x = actor->originX;
  pos.z = actor->originZ;
  Vector_AddPolarOffset(actor->orbitRadius << 16, actor->orbitAngle << 11,
                        &pos);
  actor->x = pos.x;
  actor->z = pos.z;
}

//--------------------------------------------------------------------------
// heavy_impact_finish.c
// フィールド効果：ジンが仲間になる演出 (属性3)
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ジンが跳ねて光の粒になり、操作キャラへ集まる
// djinni_id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_FinishHeavyImpact(s32 djinniId) {
  struct EffectVector pos;
  struct MotionObject *djinni;
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  s32 scale;
  s32 cnt;

  djinni = (struct MotionObject *)Object_GetById(djinniId);
  if (djinni == NULL)
    return;
  BattleFx_InitializeSlots();
  scene = gEffectWork;
  Shop_RestoreSceneTilesFar(0x20118c);
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  WaitFrames(20);

  // 回転してから高く跳ねる
  AudioCommand_PlayFar(140);
  djinni->update = BattleFx_AdvanceSpinAngle;
  WaitFrames(40);
  AudioCommand_PlayFar(153);
  ObjectMotion_Launch(djinniId, 12, 22);

  // ジンの画面位置から光の粒を 24 個出す
  pos.x = djinni->x;
  pos.y = djinni->y;
  pos.z = djinni->z;
  Camera_WorldToScreen(&pos);
  ObjectDispatch_ReleaseFar(djinni);
  AudioCommand_PlayFar(164);
  slot = scene->slots;
  cnt = 23;
  do {
    EffectSlot_Initialize(slot, 284, pos.x, pos.z);
    EffectSlot_SetCallback(
        slot, BattleFx_RunAngledApproachPhases); // 回ってから操作キャラへ
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object,
                                              (Random16() * 7) >> 16);
    scale = Random16() / 3 + 0x10000; // 1.0〜1.33 倍
    slot->scaleY = scale;
    slot->scaleX = scale;
    cnt--;
    WaitFrames(1);
    slot++;
  } while (cnt >= 0);

  // 操作キャラを 0x4000 (90°) の向きへ回し、動作 28 にする
  WaitFrames(60);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(40);
  AudioCommand_PlayFar(164);
  WaitFrames(100);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

// FieldActor と同じ物体。0x64 を符号なし、0x66 を符号つきで読む (FieldActor
// の型ではバイトが変わる)
struct DescendingOrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 angle;
};

// FieldActor と同じ物体。FieldActor に欄のない 0x5e (lifetime) を使う
struct CaptureObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  s32 gravity;
  u8 unk2c[4];
  s32 speed;
  u8 unk34[33];
  u8 mode;
  u8 unk56[5];
  u8 visible;
  u8 unk5c[2];
  u16 lifetime;
  u8 unk60[4];
  u16 timer;
  u16 angle;
  struct FieldActor *linkedObject;
  void *callback;
};

void BattleFx_SetObjectAlternatingWords(u8 *object) {
  u32 *table = gBattleFx_PulseScales;
  u32 index = ((u32)gFrameTick >> 2) & 1;
  u32 value = index[table];
  ((struct MotionObject *)object)->scaleX = value;
  ((struct MotionObject *)object)->scaleY = value;
}

void BattleFx_AdvanceAngleWithRamp(void *obj) {
  u32 step;

  step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
  ((struct MotionObject *)obj)->angle =
      ((struct MotionObject *)obj)->angle + step + 0x1000;
  if (step < 0x1000U) {
    FIELD_AT_OFFSET(obj, s16 *, 0x64) = FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1;
  }
}

// 毎フレームの更新 : 45 度ずつ回しながら拡大率に step を足し、0x3000
// を下回ったら消す
#define DEFINE_SHRINK_AND_DESTROY(name, step)                                  \
  void name(void *obj) {                                                       \
    s32 scale;                                                                 \
                                                                               \
    scale = ((struct MotionObject *)obj)->scaleX + step;                       \
    ((struct MotionObject *)obj)->scaleY =                                     \
        ((struct MotionObject *)obj)->scaleY + step;                           \
    ((struct MotionObject *)obj)->angle =                                      \
        ((struct MotionObject *)obj)->angle + 0x2000;                          \
    ((struct MotionObject *)obj)->scaleX = scale;                              \
    if (scale < 0x3000) {                                                      \
      ObjectDispatch_ReleaseFar(obj);                                          \
    }                                                                          \
  }

DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroySlow, 0xFFFFFE40)

void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect) {
  struct Object *owner;
  struct EffectVector position;
  s32 state;

  owner = (struct Object *)Object_GetById(gGameState.selectedActor);
  state = effect->state;

  if (state == 0) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset(0x780000,
                          ((Random16() * 3 << 11) >> 16) -
                              ((Random16() * 3 << 11) >> 16) + 0xc000,
                          &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x50000;
    effect->maxSpeed = 0x50000;
    effect->flag42 = state;
    effect->state++;

    ((struct AnimationObject *)effect->object)->part[0].priority =
        owner->sprite->part[0].priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(134);
  } else if (state == 1) {
    if ((s16)effect->age == 3) {
      ((struct AnimationObject *)effect->object)->part[0].priority = 0;
      effect->flags = 4;
    }
    if (BattleFx_HasReachedTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (BattleFx_HasReachedTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct AnimationObject *)effect->object)->part[0].priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == 3) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = owner->pos.x;
    position.y = owner->pos.y + 0x140000;
    position.z = owner->pos.z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(145);
  } else if (state == 4) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      effect->state--;
  } else if (state == 5) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}

void BattleEffect_RunPhasedRadialParticleSequence(s32 arg) {
  s8 *objectPointer;
  struct EffectVector position;
  s32 value;
  s32 remaining;
  struct FieldActor *sourceObject;
  void *effectSlots;
  struct FieldActor *targetObject;
  void *slot;
  s8 *slotState;

  sourceObject = Object_GetById(arg);
  targetObject = Object_GetById(gGameState.selectedActor);
  if (sourceObject == NULL)
    return;

  BattleFx_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201090);
  WaitFrames(30);
  ObjectMotion_ArmCallback(arg, 0x4000, 0);
  WaitFrames(20);
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(arg, 1);
  AudioCommand_PlayFar(174);
  Motion_SetVarCbAndRefresh(arg, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(arg, 1);
  WaitFrames(20);
  AudioCommand_PlayFar(140);
  sourceObject->update = (void *)BattleFx_AdvanceAngleWithRamp;
  sourceObject->timer = 0;
  WaitFrames(80);
  sourceObject->update = (void *)BattleFx_ShrinkObjectAndDestroySlow;
  ObjectDispatch_ApplyArgumentToChildrenFar(sourceObject, 3);

  position.x = sourceObject->x.fixed;
  position.y = sourceObject->y.fixed;
  position.z = sourceObject->z.fixed;
  Camera_WorldToScreen(&position);

  slot = (u8 *)effectSlots + 88;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, 284, position.x, position.z);
    EffectSlot_SetCallback(slot,
                           (void *)BattleEffect_UpdatePhasedRadialParticle);
    EffectSlot_SetObjectMode(slot, 7);
    objectPointer = slot;
    value = *(s32 *)objectPointer;
    ObjectGroup_SetChildValueUnlessFifteenFar((void *)value, 10);
    value = Random16() / 3 + 0x10000;
    ((struct EffectSlot *)slot)->scaleY = value;
    ((struct EffectSlot *)slot)->scaleX = value;
    remaining--;
    WaitFrames(1);
    slot = (u8 *)slot + 72;
  } while (remaining >= 0);

  WaitFrames(60);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  {
    s32 nextState;

    WaitFrames(20);
    nextState = 2;
    slotState = effectSlots;
    slotState += 152;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  WaitFrames(60);
  targetObject->update = (void *)BattleFx_SetObjectAlternatingWords;
  WaitFrames(100);

  {
    s32 nextState;

    slotState = effectSlots;
    nextState = 5;
    slotState += 152;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  WaitFrames(10);
  slot = NULL;
  targetObject->update = slot;
  targetObject->scaleX = 0x10000;
  targetObject->scaleY = 0x10000;
  WaitFrames(30);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroyFast, 0xFFFFFC00)

void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg) {
  struct EffectVector local;
  struct DescendingOrbitObject *other;
  s32 raw;
  s16 value;
  s32 y;

  other =
      (struct DescendingOrbitObject *)Object_GetById(gGameState.selectedActor);
  raw = arg->timer - 1;
  arg->timer = raw;
  value = arg->timer;
  local.x = other->x;
  local.z = other->z;
  Vector_AddPolarOffset(value * 0x6666, (value << 11) + arg->angle, &local);
  arg->x = local.x;
  arg->z = local.z;
  y = arg->y + 0xFFFF0000;
  arg->y = y;
  if (y < other->y + 0x140000)
    ObjectDispatch_ReleaseFar(arg);
}

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect) {
  struct EffectVector position;
  struct ObjectRuntime *source;
  s32 state;
  u32 random;

  source = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  state = effect->state;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;

    random = Random16() * 10 + 0xa0000;
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    effect->originX = position.x;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(0x780000, 0xc000, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x10000;
    effect->maxSpeed = 0x50000;
    effect->flag42 = state;
    effect->state++;

    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(0x90);
  } else if (state == 1) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}

void BattleFx_RunVenusDjinnCapture(s32 arg) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  void *effectSlots;
  u8 *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  s32 position[3];

  leader = (struct CaptureObject *)Object_GetById(gGameState.selectedActor);
  djinni = (struct CaptureObject *)Object_GetById(arg);
  if (djinni == NULL)
    return;

  BattleFx_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201204);
  WaitFrames(30);
  djinni->visible = 0;
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(arg, 4, 15);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(arg, 4, 15);
  WaitFrames(30);
  djinni->callback = (void *)BattleFx_AdvanceSpinAngle;
  AudioCommand_PlayFar(153);
  ObjectMotion_Launch(arg, 8, 22);
  AudioCommand_PlayFar(140);
  Runtime_SetWorkTripleIfNonNegativeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = (void *)BattleFx_ShrinkObjectAndDestroyFast;
  ObjectDispatch_ApplyArgumentToChildrenFar(djinni, 3);
  WaitFrames(90);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(30);
  Runtime_SetWorkTripleIfNonNegativeFar(0x19999, 0x19999, 0x10000);

  position[0] = djinni->x;
  position[1] = djinni->y;
  position[2] = djinni->z;
  Camera_WorldToScreen((struct EffectVector *)position);

  slot = (u8 *)effectSlots + 88;
  remaining = 23;
  do {
    EffectSlot_Initialize((struct EffectSlot *)slot, 284, position[0],
                          position[2]);
    EffectSlot_SetCallback((struct EffectSlot *)slot,
                           (void *)BattleFx_UpdateRadialSpread);
    EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(*(void **)slot, 11);
    ((struct EffectSlot *)slot)->scaleX = 0x8000;
    ((struct EffectSlot *)slot)->scaleY = Random16() + 0x18000;
    remaining--;
    WaitFrames(1);
    slot += 72;
  } while (remaining >= 0);

  WaitFrames(140);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 152;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  WaitFrames(20);
  Runtime_SetWorkTripleIfNonNegativeFar(1, 1, 1);
  WaitFrames(30);

  for (remaining = 0; remaining <= 23; remaining++) {
    position[0] = leader->x;
    position[1] = leader->y + 0x780000;
    position[2] = leader->z;
    djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1],
                                                  position[2]);
    if (djinni != NULL) {
      scale = Random16() / 3 + 0x10000;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = (void *)BattleFx_UpdateDescendingOrbitObject;
      djinni->mode = 0;
      ObjectDispatch_ApplyArgumentToChildrenFar(djinni, 7);
      Animation_ApplyChildValuesFar(djinni, 11);
    }
  }

  WaitFrames(100);
  AudioCommand_PlayFar(288);
  WaitFrames(1);
  AudioCommand_PlayFar(151);
  position[0] = leader->x;
  position[1] = leader->y + 0x120000;
  position[2] = leader->z;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1],
                                                  position[2]);
    if (djinni == NULL)
      break;
    djinni->scaleY = 0x9999;
    djinni->scaleX = 0x9999;
    djinni->mode = 2;
    djinni->gravity = 0x50000;
    djinni->baseY = djinni->y;
    djinni->speed = Random16() + 0x16666;
    Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)djinni,
                                               0x200000, Random16());
    Animation_ApplyChildValuesFar(djinni, 11);
    djinni->lifetime = 8;
    ObjectDispatch_InitializeFar((struct DispatchObject *)djinni,
                                 gBattleFx_CommonParticleScript);
  }

  WaitFrames(15);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect) {
  struct ObjectRuntime *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y + Random16() * 5 + 0xf0000;
    position.z = source->z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->x = position.x;
    effect->z = position.z - 0x640000;
    effect->acceleration = 0x30000;
    effect->maxSpeed = Random16() * 3 + 0x30000;
    effect->scaleX = 0x10000;
    effect->scaleY = 0x10000;
    effect->flag42 = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (BattleFx_HasReachedTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = 0x10000;
      effect->acceleration = 0;
      effect->maxSpeed = Random16() + 0x23333;
      effect->scaleX = 0x8000;
      effect->scaleY = 0x8000;
      AudioCommand_PlayFar(143);

      if (*statePointer == 1)
        (*statePointer)--;
      else
        (*statePointer)++;
      effect->callbackDelay = 6;
    }
  } else if (state == 3) {
    BattleFx_ClearOwnedSlot(effect);
  }
}

void BattleFx_HalveDistanceToTarget(struct FieldActor *object) {
  struct FieldActor *target = object->linkedObject;

  object->x.fixed += (target->x.fixed - object->x.fixed) / 2;
  object->y.fixed += (target->y.fixed - object->y.fixed) / 2;
  object->z.fixed += (target->z.fixed - object->z.fixed) / 2;
}

//--------------------------------------------------------------------------
// run_mercury_djinn_capture.c
// フィールド演出：水のジンが仲間になるとき、跳ねて残像を引いて昇り、粒を降らせる
//--------------------------------------------------------------------------

#define AFTERIMAGE_MAX 8 // 残像の数
#define PARTICLE_MAX 24  // 降らせる粒の数 (scene->slots)

//--------------------------------------------------------------------------
// 画面を青く染め、ジンを 3 回跳ねさせてから 8 つの残像を引いて 120 ドット
// 昇らせて消す。selectedActor を mode 28 にしたあと、そのまわりに粒を
// 1 フレームに 1 つずつ 24 個降らせ、最後に色を戻す
// id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_RunMercuryDjinnCapture(s32 id) {
  struct FieldActor *djinni;
  struct FieldActor *copy;
  struct FieldActor *previous;
  struct FieldSprite *resource;
  void *scene;
  struct FieldActor **cursor;
  struct FieldActor **list;
  u8 *slot;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct FieldActor *copies[AFTERIMAGE_MAX];
  struct EffectVector position;

  djinni = Object_GetById(id);
  if (djinni == NULL)
    return;

  // 画面の色を青 (R4 G4 B16) で染める
  BattleFx_InitializeSlots();
  scene = gEffectWork;
  Shop_RestoreSceneTilesFar(PALETTE_TRANSFORM_TINT | (16 << 10) | (4 << 5) | 4);
  WaitFrames(30);
  djinni->paused = 0;
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(id, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(id, 1);
  WaitFrames(20);

  // 3・5・7 ドット/フレームで 3 回跳ねる
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 3, 14);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 5, 16);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 7, 18);
  WaitFrames(20);

  // 残像：同じ絵を 8 つ作り、それぞれ一つ前を半分ずつ追いかけさせる
  kind = ((struct AnimationObject *)djinni->sprite)->entries[0]->animId;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = AFTERIMAGE_MAX - 1; remaining >= 0; remaining--) {
    copy = (struct FieldActor *)Object_CreateFar(
        kind, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->steeringFlags |= 1;
      copy->update = (void *)BattleFx_HalveDistanceToTarget;
      copy->facing = djinni->facing;
      Animation_ApplyChildValuesFar(copy, 9);
      ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
      ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
      resource = Object_ReplaceResourceEntry(copy->sprite, resource);
      copy->linkedObject = previous;
      previous = copy;
    }
  }

  // 8 ドットずつ 15 フレーム昇らせ、ジンと残像を消す
  resourceId = resource->vramBlock;
  AudioCommand_PlayFar(153);
  djinni->motionFlags = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y.fixed += 8 << 16;
    WaitFrames(1);
  }
  ObjectDispatch_ReleaseFar(djinni);
  list = copies;
  for (remaining = AFTERIMAGE_MAX - 1; remaining >= 0; remaining--)
    ObjectDispatch_ReleaseFar(*list++);
  if (resourceId != 96)
    Resource_ResetEntry(resourceId);
  WaitFrames(10);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(20);

  // 粒 (種類 240) を 1 フレームに 1 つずつ出す。動きは
  // BattleFx_UpdateRandomTargetParticle
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  slot = (u8 *)scene + 88; // scene->slots[0]
  remaining = PARTICLE_MAX - 1;
  do {
    EffectSlot_Initialize((struct EffectSlot *)slot, 240, position.x,
                          position.z);
    EffectSlot_SetCallback((struct EffectSlot *)slot,
                           BattleFx_UpdateRandomTargetParticle);
    EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(*(void **)slot, 9);
    remaining--;
    WaitFrames(1);
    slot += 72; // sizeof(struct EffectSlot)
  } while (remaining >= 0);

  // 120 フレーム後、動いている粒 (active) の state を 2 にする
  WaitFrames(120);
  {
    s32 nextState;

    nextState = 2;
    slotState = scene;
    slotState += 152; // scene->slots[0].state
    remaining = PARTICLE_MAX - 1;
    do {
      if (slotState[5] != 0) // active
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  // 色を戻す
  WaitFrames(50);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

//--------------------------------------------------------------------------
// dispatch_type_handler.c
// フィールド：演出処理の呼び分け
//--------------------------------------------------------------------------
// 物体や座標は関数ごとの構造体で渡すので、引数は void * で受ける

//--------------------------------------------------------------------------
// kind に応じた演出の処理を呼ぶ
// kind : 0 対象を指定せずに実行、1 指定の対象 (secondObjectId) に実行、2
// 終わり、3 種類ごとの処理
//--------------------------------------------------------------------------
void Field_DispatchTypeHandler(s32 kind) {
  if (kind == 0)
    BattleFx_Run();
  else if (kind == 1)
    BattleFx_DispatchRequestKind();
  else if (kind == 2)
    BattleFx_ClearChildValueOnMismatch();
  else if (kind == 3)
    FieldEvent_RunTypeHandler();
}

//--------------------------------------------------------------------------
// effect.c
// フィールド効果：種類ごとの振り分けと、効果のイベント処理
//--------------------------------------------------------------------------

#define HEAP_SLOT_FX 56 // エナジー演出のワーク (gWorkSlot[56] は gEffectWork)

void _call_via_r3(s32, s32);

//--------------------------------------------------------------------------
// 効果の種類 (animation) ごとに本体を走らせる
//--------------------------------------------------------------------------
void BattleFx_Run(void) {
  struct BattleFxScene *scene;
  struct BattleRuntime *runtime;
  s32 kind;
  s32 targetId;
  s32 objId;

  scene = *(gWorkSlot + HEAP_SLOT_FX);
  runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
  kind = scene->animation;
  targetId = scene->secondObjectId;

  switch (kind) {
  case 1:
    BattleFx_RunItemBreakSequence();
    return;
  case 7:
    RunBattleEffect07();
    return;
  case 11:
    RunBattleEffect11();
    return;
  case 4:
    RunBattleEffect04();
    return;
  case 5:
    RunBattleEffect05();
    return;
  case 14:
    RunBattleEffect14();
    return;
  case 6:
    BattleFx_RunOrbitingParticles();
    return;
  case 3:
    RunBattleEffect03();
    return;
  case 12:
    BattleFx_RunBurstParticles();
    return;
  case 13:
    RunBattleEffect13();
    return;
  case 9: // 前に止めた対象を戻し、近くの対象を止める
    if (gGameState.pausedObject != -1) {
      BattleFx_ResumeObject(gGameState.pausedObject);
      gGameState.pausedObject = -1;
    }

    objId =
        BattleEffect_SelectNearbyTargetObject(gGameState.selectedActor, kind);
    objId = BattleFx_FilterObjectIdByFlags(objId);
    if (BattleFx_FindDescriptorWithOverride(objId) != 0) {
      BattleFx_SetupObjectPair(gGameState.selectedActor, objId);
      BattleFx_MarkChildAndRunFallbackTransition(objId);
      BattleEffect_PauseObject(objId);
      gGameState.pausedObject = objId;
    } else {
      BattleEffect_RunFallbackObjectTransition();
    }
    return;
  case 2: // 波と色の効果 (種類 8 の効果が動いていれば先に止める)
    if (*(s16 *)&runtime->unkcb8[0] != 0)
      ResetSceneTransitionEffect();
    RunSceneTransitionEffect(scene->firstObjectId, targetId);
    return;
  case 8:
    RunBattleEffect08();
    return;
  case 10:
    BattleFx_RunFlashingCallbackSequence();
    return;
  case 15:
    BattleFx_RunEffect15();
    return;
  case 16:
    RunBattleEffect16();
    return;
  }
}

//--------------------------------------------------------------------------
// 対象が決まっているときの振り分け
//--------------------------------------------------------------------------
void BattleFx_DispatchRequestKind(void) {
  struct BattleFxScene *scene = *(gWorkSlot + HEAP_SLOT_FX);
  struct BattleRuntime *runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
  s32 kind = scene->animation;
  s32 targetId = scene->secondObjectId;

  scene->enabled = 0;
  switch (kind) {
  case 2:
    if (*(s16 *)&runtime->unkcb8[0] != 0)
      ResetSceneTransitionEffect();
    if (gGameState.pausedObject != scene->secondObjectId)
      ((struct ObjectRuntime *)scene->child)->movementState = 1;
    RunSceneTransitionEffect(scene->firstObjectId, targetId);
    break;
  case 1: // 方向キーで対象を押す
    BattleFx_SteerPushedTarget(targetId);
    break;
  case 7:
    BattleEffect_RunTargetedItemBreak(targetId);
    break;
  case 11:
    BattleFx_SteerLiftedTarget(targetId);
    break;
  case 4:
    BattleFx_CallEffect04(targetId);
    break;
  case 5:
    BattleFx_CallEffect05(targetId);
    break;
  case 6:
    BattleFx_StartOrbitingParticles(targetId);
    break;
  case 12:
    BattleFx_RunBurstParticleMainObject(targetId);
    break;
  case 9:
    if (gGameState.pausedObject != -1) {
      BattleFx_ResumeObject(gGameState.pausedObject);
      gGameState.pausedObject = -1;
    }
    BattleEffect_PauseObject(targetId);
    gGameState.pausedObject = targetId;
    BattleFx_MarkChildAndRunFallbackTransition(targetId);
    break;
  case 3:
    BattleFx_CallEffect03AndStop(targetId);
    break;
  case 14:
    BattleFx_CallEffect14(targetId);
    break;
  case 13:
    BattleFx_RunEffect13Hook(targetId);
    break;
  case 8:
    RunBattleEffect08();
    break;
  case 10:
    BattleFx_RunFlashingCallbackSequence();
    break;
  case 15:
    BattleFx_RunEffect15();
    break;
  case 16:
    RunBattleEffect16();
    break;
  }
}

//--------------------------------------------------------------------------
// 種類 2 の後始末：効果を終え、止めていない対象の movementState を戻す
//--------------------------------------------------------------------------
void BattleFx_ClearChildValueOnMismatch(void) {
  struct BattleFxScene *scene = gEffectWork;

  if (scene->animation == 2) {
    BattleFx_FinishSceneAndReleaseHeapBlock();
    if (gGameState.pausedObject != scene->secondObjectId) {
      ((struct ObjectRuntime *)scene->child)->movementState = 0;
    }
  }
}

//--------------------------------------------------------------------------
// 種類 8, 10, 16 の追加処理
//--------------------------------------------------------------------------
void FieldEvent_RunTypeHandler(void) {
  u32 kind;

  kind = ((struct BattleFxScene *)gEffectWork)->animation;
  switch (kind) {
  case 8:
    ResetSceneTransitionEffect();
    return;
  case 10:
    MapEvent_RunTileTriggerSequence();
    return;
  case 16:
    FieldEvent_ShowStatusMessage();
    return;
  }
}

//--------------------------------------------------------------------------
// 効果に結びついたイベントを実行する
// event  : オフセット 8 の値が 0x10000
// 未満ならメッセージ番号、それ以上なら処理の番地 param  : 処理へ渡す値
// (使った者など) target : 対象 フラグ 322
// が立っていれば「何も起こらない」を出す
//--------------------------------------------------------------------------
s32 BattleFx_RunEventAction(void *event, s32 param, s32 target) {
  s32 script;

  if (event != NULL) {
    script = ((struct EffectDescriptor *)event)->result;
    if (script != 0) {
      if (script < 0x10000) {
        Battle_Reset();
        Event_SetMessageId(((struct EffectDescriptor *)event)->result);
        BattleEv_RunWait(target, 0);
        BattleFx_FinishAction();
      } else {
        _call_via_r3(param, target);
      }
    }
    if (GameFlag_TestFar(322) != 0) {
      Battle_Reset();
      UiText_ShowPositionedMessageAndWaitFar(MsgNothingHappens, 1);
      BattleFx_FinishAction();
    }
  }
  return 0;
}

//--------------------------------------------------------------------------
// 各パーツのパレットをフレーム数 % 6 にする (毎フレーム呼ぶと色が巡る)
//--------------------------------------------------------------------------
void ObjectGroup_ApplyRandomChildValues(void *owner) {
  s32 kind;
  void *anim;
  s32 initialCount;
  s32 cnt;
  void **entry;
  void *part;
  /* FAKEMATCH: volatile, as gFrameCount is declared (frame.h): each *frameCount
   * is a load of its own through the pointer local. A plain pointer differs 16;
   * gFrameCount read directly in the loop differs 6; 46 instructions every way.
   */
  volatile u32 *frameCount;
  s32 value;

  kind = ((struct ObjectRuntime *)owner)->animationKind;
  if (kind == 1) {
    anim = ((struct ObjectRuntime *)owner)->animation;
    if (anim != 0 &&
        (((struct AnimationObject *)anim)->displayFlags & kind) == 0) {
      initialCount = ((struct AnimationObject *)anim)->count;
      if (initialCount != 0) {
        frameCount = &gFrameCount;
        entry = (void **)((struct AnimationObject *)anim)->entries;
        cnt = initialCount;
        do {
          part = *entry++;
          value = *frameCount % 6;
          cnt--;
          ((struct AnimationEntry *)part)->param = value;
        } while (cnt != 0);
      }
      ((struct AnimationObject *)anim)->dirty = 1;
    }
  }
}

//--------------------------------------------------------------------------
// オブジェクトを向き angle へ magnitude だけずらした位置に置く
//--------------------------------------------------------------------------
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle) {
  s32 pos[3];

  if (object != 0) {
    pos[0] = object->x;
    pos[1] = object->y;
    pos[2] = object->z;
    Vector_AddPolarOffset(magnitude, angle, pos);
    Object_SetMoveTargetFar(object, pos[0], pos[1], pos[2]);
  }
}

// 以下では物体を関数ごとの構造体で渡す

s32 ResourceTable_CountFreeBlocks(void) {
  u8 *marker = gResourceBlockOwners;
  s32 freeCount = 0;
  s32 remaining = 0x200;

  do {
    if (*marker++ == 0xff) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
}

void *Object_ReplaceResourceEntry(void *src, void *alt) {
  void *ret;
  struct FieldSprite *obj;

  obj = src;
  ret = NULL;
  if (obj != NULL) {
    if (alt == NULL) {
      obj->unk1d0 = 1;
    } else {
      Resource_ResetEntry(obj->vramBlock);
      obj->vramBlock = ((struct FieldSprite *)alt)->vramBlock;
      obj->unk1d0 = 1;
      obj = alt;
    }
    ret = obj;
  }
  return ret;
}

//--------------------------------------------------------------------------
// arcupdat.c
// エナジー演出：演出用の物体、ほかの物体の一括設定、ふくらむ弧の動き
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 演出用の物体を出す
// アニメのない物体は消して NULL。高さは使う者に合わせ、優先度は 0
//--------------------------------------------------------------------------
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct BattleFxScene *work = gEffectWork;
  struct ObjectRuntime *obj;

  obj = Object_CreateFar(kind, x, y, z);
  if (obj != NULL) {
    if (obj->animationKind == 0) {
      ObjectDispatch_ReleaseFar(obj);
      return NULL;
    }
    obj->terrainHeight =
        ((struct ObjectRuntime *)work->mainObject)->terrainHeight;
    obj->flags = 4;
    obj->unk23 = 4;
    ((struct AnimationObject *)obj->animation)->part[0].priority = 0;
    ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
  }
  return obj;
}

//--------------------------------------------------------------------------
// 主人公と except 以外のすべての物体に、動きの状態と値を設定する
//--------------------------------------------------------------------------
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state,
                                    s32 value) {
  s16 *player;
  struct ObjectRuntime *obj;
  s32 id;

  id = 0;
  player = (s16 *)gPlayerObjectId;
  do {
    obj = ObjectTable_Get(id);
    if (id != *player && obj != NULL && obj != except) {
      obj->movementState = state;
      ObjectDispatch_ApplyValueToChildrenFar(obj, value);
    }
    id++;
  } while (id <= OBJ_LAST);
}

//--------------------------------------------------------------------------
// 弧 A：32 フレームでふくらんでしぼみ、毎フレーム 1 ドット上がる
// z は物体の 9 ドット先から、小さいほど 5 ドットまで離れる
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj) {
  struct FieldActor *link;
  s32 v;

  link = obj->linkedObject;
  v = ++obj->timer;
  if (v > 31) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
    return;
  }
  v = Trig_Sin(v << 10);
  obj->scaleX = v;
  obj->scaleY = v;
  obj->x.fixed = link->x.fixed;
  obj->y.fixed += 0x10000;
  obj->z.fixed = link->z.fixed + (0x10000 - v) * 5 + 0x90000;
}

//--------------------------------------------------------------------------
// 弧 B：A の上下を反転したもの。z は物体の 16 ドット先から、小さいほど 5
// ドットまで寄る
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj) {
  struct FieldActor *link;
  s32 v;

  link = obj->linkedObject;
  v = ++obj->timer;
  if (v > 31) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
    return;
  }
  v = Trig_Sin(v << 10);
  obj->scaleX = v;
  obj->scaleY = -v;
  obj->x.fixed = link->x.fixed;
  obj->y.fixed += 0x10000;
  obj->z.fixed = link->z.fixed - (0x10000 - v) * 5 + 0x100000;
}

//--------------------------------------------------------------------------
// actsetup.c
// エナジー演出：ワークの準備、使う者と対象の記録、ふくらむ弧の物体
//--------------------------------------------------------------------------
// スプライト：OAM の三つの属性と管理情報
struct ArcSprite {
  u8 pad00[4];
  u16 y : 8;
  u16 affine : 1;
  u16 doubleSize : 1;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 color : 1; // 1 = 256 色
  u16 shape : 2;
  u16 x : 9;
  u16 unused : 3;
  u16 hflip : 1;
  u16 vflip : 1;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 pad0a[18];
  u8 slot; // VRAM ブロック
  u8 active : 1;
  u8 displayFlags : 7;
  u8 pad1e[8];
  u8 flags;
  u8 pad27;
  struct AnimationEntry *child;
};

//--------------------------------------------------------------------------
// 物体の位置に二つの弧を出す (B は上下を反転)
// 弧は 32 フレームでふくらんでしぼみ、物体についていく
//--------------------------------------------------------------------------
void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *source = scene->mainObject;
  struct FieldActor *objs[2];
  struct FieldActor *obj;
  struct ArcSprite *spr;
  s32 i;

  for (i = 0; i <= 1; i++) {
    obj = (struct FieldActor *)Object_CreateFar(
        26, linked->x.fixed, linked->y.fixed, linked->z.fixed);
    objs[i] = obj;
    if (obj == 0)
      continue;
    obj->terrainHeight = linked->terrainHeight;
    spr = obj->sprite;
    obj->motionFlags = 0;
    obj->timer = 0;
    obj->linkedObject = linked;
    obj->scaleX = obj->scaleY = 0x1999; // 0.1 倍
    if (spr == 0)
      continue;
    // 演出のタイルを使う 16 色の横長スプライト
    AnimationObjects_SelectAnimationFar((struct AnimationObject *)spr, 0);
    spr->flags = 0;
    Resource_ResetEntry(spr->slot);
    spr->slot = scene->tileSlot;
    spr->active = 1;
    spr->tile = (gVramBlockCache[spr->slot].offset >> 5) & 0x3ff;
    spr->color = 0;
    spr->shape = 1;
    spr->size = 2;
    spr->child->frame = 0;
  }
  // B の弧は最前面、A の弧は使う者と同じ優先度
  objs[0]->update = BattleFx_UpdateScaledArcObjectB;
  objs[0]->sprite->priority = 0;
  objs[1]->update = BattleFx_UpdateScaledArcObjectA;
  objs[1]->sprite->priority = source->sprite->priority;
  objs[1]->priorityFlags = 2;
}

//--------------------------------------------------------------------------
// 2 フレームごとに色 7 と 0 を切り替え、16 フレームごとに弧を出す
//--------------------------------------------------------------------------
void BattleFx_FlickerObjectAndTick(s32 obj) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 7);
  } else {
    Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 0);
  }
  if ((gFrameCount & 15) == 0) {
    BattleFx_SpawnScaledArcObjects(obj);
  }
}

//--------------------------------------------------------------------------
// 2 フレームごとに色 0-5 を順に回し、16 フレームごとに弧を出す
//--------------------------------------------------------------------------
void BattleFx_CycleObjectValueByCounter(s32 obj) {
  if ((gFrameCount & 1) != 0) {
    s32 col = (gFrameCount >> 1) % 6;

    Animation_ApplyChildValuesFar((struct DispatchObject *)obj, col);
  }
  if ((gFrameCount & 15) == 0) {
    BattleFx_SpawnScaledArcObjects(obj);
  }
}

//--------------------------------------------------------------------------
// 演出スロット 24 個を動かす
//--------------------------------------------------------------------------
void BattleFx_UpdateAllEffectSlots(void) { UpdateAllEffectSlots(); }

//--------------------------------------------------------------------------
// エナジー演出のワークを用意し、わざの演出を準備する
// 演出中にもう一度呼ばれたら、わざだけを入れ替える
//--------------------------------------------------------------------------
void BattleFx_LoadActionEffectResources(s32 abilityId, s32 mode) {
  struct FieldWork *ev = *(gWorkSlot + HEAP_SLOT_EVENT);
  struct MapScrollWork *map = *(gWorkSlot + 8);
  struct BattleFxScene *work;
  s32 running;

  running = ev->resolvingAction;
  if (running == 0) {
    work = Runtime_AllocateHeapBlock(HEAP_SLOT_FX, sizeof *work);
    Dma_Fill32(running, work, sizeof *work);
  } else {
    work = *(gWorkSlot + HEAP_SLOT_FX);
  }
  work->action = abilityId;
  work->animation = BattleActGet(abilityId)->type0c;
  running = ev->resolvingAction;
  if (running != 0)
    return;
  work->freeBlocks = 512 - ResourceTable_CountFreeBlocks();
  work->mode = mode;
  work->visible = 1;
  work->enabled = 1;
  work->active = 1;
  work->messageMode = 1;
  // マップのゆれの大きさと減り方を取っておき、演出の後で戻す
  work->savedX = map->shakeX;
  work->savedY = map->shakeY;
  work->savedZ = map->shakeDecay;
  if (gGameState.map == (s32)&SceneId_MercuryEntrance)
    work->lit = 1;
  if (gGameState.map == (s32)&SceneId_MercuryLighthouse2)
    work->lit = 1;
  BattleFx_SetupObjectPair(gGameState.selectedActor, -1);
  if (work->animation != 8)
    ev->unkcc0 = running;
  work->tileSlot = Resource_FindFreeEntry();
  VramBlock_LoadCached((s16)work->tileSlot, 256, gBattleFx_SparkTiles);
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllEffectSlots, 0xc80);
}

//--------------------------------------------------------------------------
// 使う者と対象の物体を記録する
// 対象がなければ、使う者の向きに 16 ドット先を演出の位置にする
//--------------------------------------------------------------------------
void BattleFx_SetupObjectPair(s32 userId, s32 targetId) {
  struct ScriptMotionObject *user;
  struct ScriptMotionObject *target;
  s32 dir;
  struct BattleFxScene *work;

  work = gEffectWork;
  work->firstObjectId = userId;
  user = ObjectTable_Get((s16)userId);
  work->secondObjectId = targetId;
  work->mainObject = user;
  target = ObjectTable_Get((s16)targetId);
  dir = (user->facing + 0x2000) & 0xc000; // 四方向にまるめる
  work->child = target;
  work->angle = dir;
  if (target != NULL) {
    // 対象の動きと色を、あとで戻すために取っておく
    work->savedCallback = (void *)target->hook;
    work->savedScript = (const s32 *)target->script;
    {
      struct AnimationObject *anim =
          ((struct ObjectRuntime *)target)->animation;
      u8 pal = anim->entries[0]->param;

      work->savedPalette = pal;
    }
    work->x = target->x;
    work->z = target->z;
    work->y = target->y;
    return;
  }
  work->x = user->x;
  work->z = user->z;
  work->y = user->y;
  Vector_AddPolarOffset(0x100000, dir, &work->x);
}

//--------------------------------------------------------------------------
// 使う者の動きを止めて、色を戻す
//--------------------------------------------------------------------------
void EffectRuntime_StopCurrentObject(void) {
  struct BattleFxScene *work = gEffectWork;
  struct ScriptMotionObject *user = work->mainObject;

  user->hook = NULL;
  Animation_ApplyChildValuesFar(user, 0);
  WaitFrames(1);
}

struct BattleEffectPosition {
  u8 reserved00[4];
  s32 x;
  s32 y;
  s32 z;
};

#define BATTLE_OBJECT_SLOTS ((struct ScriptMotionObject *)gObjectSlots)

void BattleEffect_CleanupSceneObjects(void) {
  struct BattleFxScene **sceneCell;
  struct BattleFxScene *scene;
  struct FieldWork *runtime;
  struct BattleEffectPosition *position;
  struct EffectSlot *sceneObject;
  s32 remaining;

  sceneCell = (struct BattleFxScene **)&gEffectWork;
  scene = *sceneCell;
  runtime = *(struct FieldWork **)((u8 *)sceneCell - 116);
  position = *(struct BattleEffectPosition **)((u8 *)sceneCell - 192);
  for (remaining = 0; remaining < 24; remaining++) {
    sceneObject = &scene->slots[remaining];
    if (sceneObject->active != 0)
      BattleFx_ClearOwnedSlot(sceneObject);
  }

  if (runtime->resolvingAction == 0) {
    s32 waited = 0;
    void (*firstActiveUpdate)(void) =
        (void (*)(void))BattleFx_UpdateScaledArcObjectB;
    void (*secondActiveUpdate)(void) =
        (void (*)(void))BattleFx_UpdateScaledArcObjectA;
    s32 active;

    do {
      struct ScriptMotionObject *slot = BATTLE_OBJECT_SLOTS;

      active = 0;
      remaining = 0;
      while (remaining <= 63) {
        void (*update)(void) = (void (*)(void))slot->hook;

        if (update == firstActiveUpdate || update == secondActiveUpdate) {
          active = 1;
          break;
        }
        remaining++;
        slot++;
      }

      if (active != 0) {
        waited++;
        WaitFrames(1);
      }
    } while (active != 0 && waited <= 29);

    runtime->unkcc7 = 0;
    Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
    Resource_ResetEntry((s16)scene->tileSlot);
    position->x = scene->savedX;
    position->y = scene->savedY;
    position->z = scene->savedZ;
    if (scene->animation != 8)
      runtime->unkcc0 = 1;
    BattleFx_PlayQueuedSound();
    Runtime_ReleaseHeapBlock(0x38);
  }
}

void EventObject_Initialize(void) {
  s32 eventValue;
  s32 eventIndex;
  struct FieldActor *eventObject;
  struct BattleFxScene *eventState;
  void *renderState;

  eventState = *(void **)Ram_EffectWork;
  renderState = *(void **)Ram_EventWork;
  eventObject = eventState->mainObject;
  eventValue = (s16)eventState->action;
  eventIndex = eventValue;
  ObjectDispatch_ApplyArgumentToChildrenFar(eventObject, 0x14);
  eventObject->targetX = eventObject->x.fixed;
  eventObject->targetY = eventObject->y.fixed;
  eventObject->targetZ = eventObject->z.fixed;
  eventObject->velocityX = 0;
  eventObject->velocityY = 0;
  eventObject->velocityZ = 0;
  if (eventState->visible != 0) {
    AudioCommand_PlayFar(212);
    eventObject->update = (void *)BattleFx_FlickerObjectAndTick;
  }
  if (eventState->active != 0) {
    ObjectGroup_SetActionForOthers(eventObject, 1, 0);
    UiWork_PushValueSlotFar(eventIndex, 4);
    if ((s8)eventState->mode != 0) {
      UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce,
                                             eventState->messageMode);
    } else {
      UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce,
                                             eventState->messageMode);
    }
    ObjectGroup_SetActionForOthers(eventObject, 0, 0x10);
  }
  if (GameFlag_TestFar(0x140) != 0) {
    if (eventState->visible != 0) {
      eventObject->update = (void *)BattleFx_CycleObjectValueByCounter;
    }
    ObjectDispatch_ApplyArgumentToChildrenFar(eventObject, 0x15);
  } else {
    EffectRuntime_StopCurrentObject();
  }
  ((struct FieldWork *)renderState)->unkcc7 = 1;
}

void BattleEffect_InitializeSharedScene(void) {
  u8 *scene;
  struct BattleEffectBuffers *work;
  s32 no;

  scene = *(gWorkSlot + HEAP_SLOT_EVENT);
  work = *(gWorkSlot + 32);
  Dma_Set(&work->target[0x2a0], scene + 0x776, 0x84000150, REG_DMA3);
  if (((struct FieldWork *)scene)->effectActive == 0)
    Dma_Set(work->target, scene + 0x236, 0x84000150, REG_DMA3);
  Dma_Set(work->target, work->current, 0x84000000 | (sizeof work->current / 4),
          REG_DMA3);

  no = gFrameCount & 7;
  if (GameFlag_TestFar(0x148))
    no = 0;
  if (GameFlag_TestFar(0x149))
    no = 1;
  if (GameFlag_TestFar(0x14a))
    no = 2;
  if (GameFlag_TestFar(0x14b))
    no = 3;
  if (GameFlag_TestFar(0x14c))
    no = 4;
  if (GameFlag_TestFar(0x14d))
    no = 5;
  if (GameFlag_TestFar(0x14e))
    no = 6;
  if (GameFlag_TestFar(0x14f))
    no = 7;
  BattleFx_ApplyColorToTargetBuffer(gBattleFx_TargetColors[no], 1);
  BattleFx_StartBufferInterpolation(8);
}

void BattleFx_PrepareBufferInterpolation(void) {
  s32 value;
  u8 *state;

  state = (u8 *)gEventWork;
  value = (s32)(state + 0x236);
  BattleFx_ApplyColorToTargetBuffer(value, 2);
  if (((struct FieldWork *)state)->effectActive != 0) {
    BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  } else {
    BattleFx_ApplyColorToTargetBuffer(value, 1);
  }
  BattleFx_StartBufferInterpolation(8);
}

void Camera_WorldToScreen(struct EffectVector *position) {
  struct EventRuntime **data = (struct EventRuntime **)&gEventWork;

  if ((*data)->mode19e == 3) {
    struct EffectVector result;

    Render_ProjectPoint(position, &result);
    position->x = result.x << 16;
    position->z = result.y << 16;
    position->y = 0;
  } else {
    struct MapScrollWork *state = *(struct MapScrollWork **)((u8 *)data - 76);
    s32 x = state->viewX & 0xffff0000;
    s32 z = state->viewY & 0xffff0000;

    position->x -= x;
    position->z -= position->y;
    position->z -= z;
    position->y = 0;
  }
}

// 以下では座標を関数ごとの構造体で渡す

// RunSceneTransitionEffect が 0x298 バイト確保して初期化する
struct WaveFxWork {
  u16 lines[2][162]; // ラインごとのずれ (2 面を交互に書く)
  u16 phase;         // 波の位相
  u8 page;           // 書き込む面
  s8 red;            // 色相から求めた色 (赤・緑・青、各 4〜11)
  s8 green;
  s8 blue;
  u16 hue;      // 色相 (0〜359 度)
  u16 sourceId; // 使い手
  u16 targetId; // 対象
  u8 delay;     // 開始までの待ち
  u8 counter;   // 粒を出す周期 (0〜60)
};

void RunSceneTransitionEffect(s32 x, s32 y) {
  struct WaveFxWork *work;

  work = Runtime_AllocateBlock(22, 0x298);
  BattleEffect_InitializeSharedScene();
  Dma_Fill32(0, work, 0x298);
  work->hue = ((u32)(gFrameCount << 1) % (u32)360);
  BattleFx_AdvanceHueCycle();
  BattleFx_ApplyColorToTargetBuffer(
      (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
  BattleFx_StartBufferInterpolation(8);
  work->sourceId = x;
  work->targetId = y;
  work->delay = 8;
  Ui_FillBank15PaletteGrey();
  Scheduler_AddOrUpdateCallback((void (*)(void))BattleFx_UpdateWaveScene,
                                0xc80);
}

//--------------------------------------------------------------------------
// finish_scene_and_release_heap_block.c
// フィールド効果：ラインごとの波と色の変化、対象から使い手へ流れる粒
//--------------------------------------------------------------------------

struct WaveFxSprite {
  u32 unk00[2];
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
  u8 unk0a[0x1c];
  u8 palette;
};

//--------------------------------------------------------------------------
// 効果を止めて使い手の色を戻し、作業領域を返す
//--------------------------------------------------------------------------
void BattleFx_FinishSceneAndReleaseHeapBlock(void) {
  void *work;

  work = *(void **)((u32)&gBattleBgFxWork);
  Ui_SetBank15PaletteAndClearRenderMode();
  Scheduler_RemoveCallback(BattleFx_UpdateWaveScene);
  Animation_ApplyChildPalette(
      Object_GetById(((struct WaveFxWork *)work)->sourceId), 1);
  BattleFx_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(22);
}

//--------------------------------------------------------------------------
// 毎フレームの処理 (スケジューラから呼ばれる)
//--------------------------------------------------------------------------
void BattleFx_UpdateWaveScene(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
  u16 *line;
  u32 i;
  struct FieldActor *src;
  struct FieldActor *dst;
  struct FieldActor *obj;
  struct WaveFxSprite *sprite;
  struct EffectVector from;
  struct EffectVector to;
  struct AnimationMetadata *meta;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  // 160 ライン分のずれを ±4 ドットの正弦波で作り、面を切り替える
  line = work->lines[work->page ^ 1];
  for (i = 0; i < 160; i++)
    *line++ = Trig_Sin(((u32)((work->phase + i * 8) << 16) / (u32)160)) >> 14;
  work->phase += 4;
  work->page ^= 1;
  if (work->page != 0) {
    BattleFx_ApplyColorToTargetBuffer(
        (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
    BattleFx_StartBufferInterpolation(1);
    BattleFx_AdvanceHueCycle();
  }
  Animation_ApplyChildPalette(Object_GetById(work->sourceId), 0);

  // 周期の 0, 8, 16 で、対象から使い手へ向かう粒を出す
  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    src = Object_GetById(work->sourceId);
    dst = Object_GetById(work->targetId);
    if (src != 0 && dst != 0) {
      // メタデータの boxX を高さに使い、そこから 2 ドット下
      from.x = src->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)BattleAction_FindDescriptor(work->sourceId)->sprite);
      from.y = src->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      from.z = src->z.fixed;
      to.x = dst->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)BattleAction_FindDescriptor(work->targetId)->sprite);
      to.y = dst->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      to.z = dst->z.fixed;
      obj = Object_CreateFar(281, to.x, to.y, to.z);
      if (obj != 0) {
        sprite = obj->sprite;
        obj->motionFlags = 0;
        obj->speed = 0xa3d7; // 0.64 倍
        obj->acceleration = 0xa3d7;
        obj->facing = ArcTan2(from.z - to.z, from.x - to.x);
        obj->update = BattleFx_SetCallbackWhenTargetUnset;
        sprite->palette = 0;
        sprite->mode = 1;
        Object_SetMoveTargetFar(obj, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter == 0)
    AudioCommand_PlayFar(130);
  if (++work->counter > 60)
    work->counter = 0;
}

//--------------------------------------------------------------------------
// hue.c
// 背景の走査線スクロール DMA、色相の巡回、BG パレット 15 の切り替え
//--------------------------------------------------------------------------

#define GREY28 0x739c // 灰色 (赤・緑・青とも 28)

//--------------------------------------------------------------------------
// BG0 の横スクロールを走査線ごとに書き換える HBlank DMA を張りなおす
//--------------------------------------------------------------------------
void BattleFx_ArmBg0HBlankDma(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
  if (!work->delay) {
    u16 *line = work->lines[work->page];
    volatile u16 *dma = REG_DMA0;
    dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
    dma[5] &= 0x7fff; // 止める
    (void)dma[5];
    // HBlank ごとに 16 ビットを 1 回 (リピート、転送先リロード)
    Dma_Set(line, REG_BG0HOFS, 0xa2600001, (volatile u32 *)dma);
  }
}

//--------------------------------------------------------------------------
// 色相を 4 度すすめ、その色を求める
//--------------------------------------------------------------------------
void BattleFx_AdvanceHueCycle(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
  s32 r = 0;
  s32 g = 0;
  s32 b = 0;
  s32 offset;
  u8 *pr;
  u8 *pg;
  u8 *pb;

  BattleFx_ComputeHueChannels(work->hue << 16, &r, &g, &b);

  // 0〜31 を 4〜11 に縮める (offset は rgb の位置から 1 ずつ進む)
  offset = 0x28b;
  pr = (u8 *)work + offset;
  *pr = (r >> 18) + 4;
  offset += 1;
  pg = (u8 *)work + offset;
  *pg = (g >> 18) + 4;
  offset += 1;
  pb = (u8 *)work + offset;
  *pb = (b >> 18) + 4;

  work->hue += 4;

  *pr &= 0x1f;
  *pg &= 0x1f;
  *pb &= 0x1f;

  if (work->hue >= 360) {
    work->hue = 0;
  }
}

//--------------------------------------------------------------------------
// 色相から赤・緑・青を求める (各 0〜31、16.16 固定小数)
// hue : 色相 (度、16.16 固定小数)
//--------------------------------------------------------------------------
void BattleFx_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b) {
  *r = BattleFx_HueChannelRamp(hue + (120 << 16), 0, 31 << 16);
  *g = BattleFx_HueChannelRamp(hue, 0, 31 << 16);
  *b = BattleFx_HueChannelRamp(hue - (120 << 16), 0, 31 << 16);
}

//--------------------------------------------------------------------------
// 色相に対する色の強さ
// 0〜60 度で上がり、60〜180 度は high、180〜240 度で下がり、ほかは low
//--------------------------------------------------------------------------
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high) {
  angle = Fixed_Remainder(angle, 360 << 16);
  if (angle < 60 << 16)
    return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, angle));
  if (angle >= 60 << 16 && angle < 180 << 16)
    return high;
  if (angle >= 180 << 16 && angle < 240 << 16)
    return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, (240 << 16) - angle));
  return low;
}

//--------------------------------------------------------------------------
// 16.16 固定小数の剰余 (divisor が 0 なら 0)
//--------------------------------------------------------------------------
s32 Fixed_Remainder(s32 value, s32 divisor) {
  s32 quot;

  if (divisor == 0)
    return 0;
  if (divisor & 0xf0000000)
    divisor = -divisor;
  quot = Iwram_RatioMulQ14(divisor, value);
  return value - Iwram_MulQ16(quot & 0xffff0000,
                              divisor); // 商の整数部 × divisor を引く
}

//--------------------------------------------------------------------------
// 移動の目標がなければ、目標なしのスクリプトを始める
//--------------------------------------------------------------------------
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *obj) {
  s32 ty;
  s32 tx;

  tx = obj->targetX;
  if (tx == 0x80000000) // 目標なし
  {
    ty = obj->targetY;
    if ((ty == tx) && (obj->targetZ == ty)) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_UntargetedObjectScript);
    }
  }
}

//--------------------------------------------------------------------------
// BG パレット 15 の色 1, 3〜12 を灰色にし、走査線スクロールを始める
//--------------------------------------------------------------------------
void Ui_FillBank15PaletteGrey(void) {
  volatile u16 *p;

  ((struct UiRenderWork *)gWindowWork[0])->mode = 1;
  p = &BG_PLTT_COLOR(15, 1);
  *p = GREY28;
  p += 2;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg0HBlankDma, 0x480);
}

//--------------------------------------------------------------------------
// 走査線スクロールを止め、BG パレット 15 を戻す
//--------------------------------------------------------------------------
void Ui_SetBank15PaletteAndClearRenderMode(void) {
  void *work;

  work = gWindowWork[0];
  Scheduler_RemoveCallback(BattleFx_ArmBg0HBlankDma);
  BG_PLTT_COLOR(15, 1) = 0x7fff;  // 白
  BG_PLTT_COLOR(15, 3) = 0;       // 黒
  BG_PLTT_COLOR(15, 11) = 0x294a; // 灰 (10)
  BG_PLTT_COLOR(15, 12) = 0x5294; // 灰 (20)
  PaletteGlow_UpdateFar(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  ((struct UiRenderWork *)work)->mode = 0;
}

//--------------------------------------------------------------------------
// pattern.c
// バトル効果：十字キーの入力を向きに変える
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 押している十字キーの向き（0x10000 で 1 周）を返す
// 右 0、下 0x4000、左 0x8000、上 0xc000
// 何も押していないとき、斜めのときは 0xffff
//--------------------------------------------------------------------------
s32 BattleFx_GetCycledTableWord(u32 unused) {
  return gBattleFx_CyclePatternWords[(gKeysHeld >> 4) &
                                     15]; // 右・左・上・下の 4 ビット
}

//--------------------------------------------------------------------------
// fragment.c
// 演出：アイテムがこわれる演出の本体の動き
//--------------------------------------------------------------------------
// FieldActor と同じ物体。FieldActor に欄のない 0x5e を読むための見え方
struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 gravity;
  u8 reserved4c[9];
  u8 motionFlags;
  u8 reserved56[8];
  u16 unk5e;
};

//--------------------------------------------------------------------------
// こわれたアイテムの本体の update
// 向く相手がいれば、そちらへ 1 回に 0x1000（22.5 度）まで曲がる
// そして、まわりにかけらの粒をひとつ出す
// src : 本体のオブジェクト
//--------------------------------------------------------------------------
void BattleFx_UpdateItemBreakFragment(struct FieldActor *src) {
  struct FieldActor *target;
  struct EffectVector pos;
  struct ItemBreakFragmentObject *obj;
  s32 turn;
  s32 dist;

  target = src->linkedObject;
  if (target != 0) {
    s32 dx = target->x.fixed - src->x.fixed;
    s32 dz = target->z.fixed - src->z.fixed;

    if (dx != 0 || dz != 0) {
      turn = (s16)(ArcTan2(dz, dx) - src->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      src->facing += turn;
    }
    src->steeringFlags = 0;
  }

  // 粒は本体から y を 8〜24 ずらし、ランダムな向きに半径 0〜48
  // 散らした位置に出す
  pos.x = src->x.fixed;
  pos.y = src->y.fixed - (Random16() << 4) - 0x80000;
  pos.z = src->z.fixed;
  dist = Random16() * 3;
  dist <<= 4;
  Vector_AddPolarOffset(dist, Random16(), &pos);

  obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->motionFlags = 2;
    obj->gravity = 0x1999;
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
    obj->unk5e = 12;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }
}

//--------------------------------------------------------------------------
// fieldfx.c
// フィールド効果：方向キーで対象を押す処理、出現と消滅の粒、範囲の見張り
//--------------------------------------------------------------------------

#define CELL_MASK 0xfff00000 // 16 ドットのマス

//--------------------------------------------------------------------------
// 対象のそばに押すオブジェクトを出し、方向キーの向きへ対象を 16 ドット押す
// A, B, L, R で取りやめる。押せないときは 16 フレームごとにエラー音
// 押す先が使い手と重なるときは、使い手の位置しだいで押せないか、使い手も動かす
//--------------------------------------------------------------------------
void BattleFx_SteerPushedTarget(s32 targetId) {
  struct EffectVector position;
  struct EffectVector beyond;
  struct BattleFxScene *work;
  struct FieldActor *user;
  struct FieldActor *target;
  struct FieldActor *pusher;
  struct FieldActor *hit;
  s32 x;
  s32 z;
  s32 angle;
  s32 moved;
  s32 dir;
  s32 keys;
  s32 cellX;
  s32 cellZ;
  struct EffectVector *p;

  work = gEffectWork;
  user = work->mainObject;
  target = work->child;
  angle = work->angle + 0x8000; // work->angle の反対向き
  moved = 0;
  if (target == NULL)
    return;
  BattleEffect_InitializeSharedScene();
  user->linkedObject = target;
  ObjectDispatch_InitializeFar((struct DispatchObject *)user,
                               gBattleFx_SourceHoldScript);
  pusher = BattleFx_StartItemBreak(user);
  if (pusher == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }

  // 押すオブジェクトは対象の 16 ドット上、angle の向きへ 16 ドットの位置
  pusher->linkedObject = target;
  p = &position;
  p->x = target->x.fixed;
  p->y = target->y.fixed + (16 << 16);
  p->z = target->z.fixed;
  Vector_AddPolarOffset(16 << 16, angle, p);
  Object_SetMoveTargetFar(pusher, p->x, p->y, p->z);
  BattleFx_SnapScaleToFull((struct MotionObject *)pusher);
  pusher->speed = 4 << 16;
  pusher->acceleration = 0x8000;
  pusher->motionFlags = 4;
  target->update = ObjectGroup_ApplyRandomChildValues; // 押す間は色を巡らせる
  target->speed = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = moved;
  target->terrainId = 2;
  for (;;) {
    WaitFrames(1);
    keys = gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS);
    if (keys != 0)
      break;

    // 方向キーが離れていれば押すオブジェクトを元の位置へ戻す
    dir = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
    if (dir == 0xffff) {
      position.x = target->x.fixed;
      position.y = target->y.fixed + (16 << 16);
      position.z = target->z.fixed;
      Vector_AddPolarOffset(16 << 16, angle, &position);
      Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 1);
      pusher->velocityX = 0;
      pusher->velocityY = 0;
      pusher->velocityZ = 0;
      continue;
    }

    // 押すオブジェクトを押す向きへ 2 ドット寄せ、対象の 16 ドット先と 32
    // ドット先を調べる
    position.x = target->x.fixed;
    position.y = target->y.fixed + (16 << 16);
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &position);
    Vector_AddPolarOffset(2 << 16, dir, &position);
    Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Script_WaitForEventTimeoutFar(pusher);
    position.x = target->x.fixed;
    position.y = target->y.fixed;
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, dir, &position);
    beyond.x = target->x.fixed;
    beyond.y = target->y.fixed;
    beyond.z = target->z.fixed;
    Vector_AddPolarOffset(32 << 16, dir, &beyond);
    if (Map_CheckObjectStepFar(target, &position) > 0) {
      ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
      if ((gFrameCount & 15) == 0)
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      continue;
    }
    hit = ScriptObject_FindOverlappingEntryFar(target, p);
    if (hit != NULL) {
      if (hit != user) {
        ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
        continue;
      }

      // 押す先が使い手と重なる：使い手のマスが押す先なら押せない。使い手が 32
      // ドット先の マスにいれば、そこが通れるときだけ使い手もそこへ動かす
      cellX = user->x.fixed & CELL_MASK;
      cellZ = user->z.fixed & CELL_MASK;
      if (cellX == (p->x & CELL_MASK) && cellZ == (p->z & CELL_MASK)) {
        ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
        continue;
      }
      if (cellX == (beyond.x & CELL_MASK) && cellZ == (beyond.z & CELL_MASK)) {
        if (Map_GetCellAttributeLowNibbleFar(user->terrainId, beyond.x,
                                             beyond.z) != 0) {
          ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
          if ((gFrameCount & 15) == 0)
            AudioCommand_PlayFar(SOUND_MENU_ERROR);
          continue;
        }
        moved = 1;
      }
    }

    // 押す：押すオブジェクトの動作は angle と押す向きの差 (90° 単位) で選ぶ
    AudioCommand_PlayFar(175);
    x = position.x;
    z = position.z;
    ObjectDispatch_ApplyArgumentToChildrenFar(
        pusher, gBattleFx_QuadrantModes[(u16)(angle - dir) >> 14]);
    WaitFrames(15);
    target->paused = 0;
    target->speed = 0x3333;
    target->acceleration = 0x3333;
    Object_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speed = 0x3333;
    pusher->acceleration = 0x3333;
    Vector_AddPolarOffset(16 << 16, dir, &position);
    Object_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16),
                            position.z);
    if (moved == 1) {
      Object_GetById(work->firstObjectId)->steeringFlags &= 0xfe;
      user->speed = 0x3333;
      user->acceleration = 0x3333;
      Object_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
    }
    Script_WaitForEventTimeoutFar(target);
    target->x.fixed = x;
    target->z.fixed = z;
    target->velocityX = 0;
    target->velocityZ = 0;
    break;
  }

  // 対象の色、スクリプト、毎フレームの処理を元に戻し、押すオブジェクトを消す
  Animation_ApplyChildValuesFar(target, work->savedPalette);
  ObjectDispatch_InitializeFar((struct DispatchObject *)target,
                               work->savedScript);
  target->update = work->savedCallback;
  EffectRuntime_StopCurrentObject();
  if (moved == 1)
    (Object_GetById(work->firstObjectId))->steeringFlags |= 1;
  BattleFx_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(pusher);
}

//--------------------------------------------------------------------------
// 粒の動き：元の位置から 30 ドット先へ飛び、また元の位置へ戻って消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot) {
  struct EffectVector position;
  s8 *statePtr = &slot->state;
  s32 state;

next_state:
  state = *statePtr;
  if (state == 0) {
    u32 angle;

    position.x = slot->originX;
    position.z = slot->originZ;
    angle = Random16();
    Vector_AddPolarOffset(30 << 16, (u16)angle, &position);
    slot->targetX = position.x;
    slot->targetZ = position.z;
    slot->acceleration = 4 << 16;
    slot->maxSpeed = 4 << 16;
    slot->flag42 = state;
    (*statePtr)++;
    return;
  }

  if (state == 1) {
    if (BattleFx_HasReachedTarget(slot) == 0) {
      (*statePtr)++;
      goto next_state;
    }
    return;
  }

  if (state == 2) {
    slot->targetX = slot->originX;
    slot->targetZ = slot->originZ;
    slot->maxTurnStep = 0x400;
    slot->flag42 = 1;
    (*statePtr)++;
    return;
  }

  if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
    BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 使い手のそばにオブジェクトを出し、30 フレーム見せてから粒にして消す
//--------------------------------------------------------------------------
void BattleFx_RunItemBreakSequence(void) {
  s32 work[3];
  struct BattleFxScene *scene;
  void *object;

  scene = gEffectWork;
  object = scene->mainObject;

  /* FAKEMATCH: the one-pass block keeps the mainObject load below the
   * frame's sub sp. Measured: plain, the scheduler lifts the load
   * above it (differ 2). */
  do {
    BattleEffect_InitializeSharedScene();
  } while (0);
  object = BattleFx_StartItemBreak(object);
  BattleFx_SnapScaleToFull(object);
  if (object != 0) {
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 4);
    WaitFrames(30);
  }
  BattleFx_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(object);
}

//--------------------------------------------------------------------------
// object の 16 ドット上にオブジェクト 215 を出し、向いている側
// (90° 単位) へ 16 ドット動かす。周りに粒 8 個を散らす
// 戻り値：出したオブジェクト (出せなければ 0)
//--------------------------------------------------------------------------
void *BattleFx_StartItemBreak(void *object) {
  struct MotionObject *src = object;
  struct MotionObject *parent;
  struct MotionObject *child;
  s32 angle;
  s32 dist;
  s32 speed;
  s32 i;
  u32 spread;
  u32 jitter;

  angle = (src->angle + 0x2000) & 0xc000; // いちばん近い 90° の向き
  parent = Object_Spawn(215, src->x, src->y + (16 << 16), src->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000; // 0.25 倍から
  parent->scaleX = 0x4000;
  parent->update = BattleFx_UpdateItemBreakFragment;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(parent, 3);
  Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  // 粒：向きの ±45° へ 8〜32 ドット、速さ 1〜2 ドット
  for (i = 7; i >= 0; i--) {
    child = Object_Spawn(285, src->x, src->y + (16 << 16), src->z);
    if (child != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)child,
                                   gBattleFx_FragmentScript);
      speed = Random16() + 0x10000;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->verticalMotionStrength = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Motion_SetTargetPositionFromMagnitudeAngle(
          child, dist, ((jitter - Random16()) >> 3) + src->angle);
    }
  }
  AudioCommand_PlayFar(SOUND_ITEM_BREAK);
  return parent;
}

//--------------------------------------------------------------------------
// 拡大率を 1/16 刻みで等倍まで上げ、位置を確定する
//--------------------------------------------------------------------------
void BattleFx_SnapScaleToFull(struct MotionObject *obj) {
  s32 next;
  s32 scale;

  if (obj != NULL) {
    scale = obj->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + 0x1000;
        scale = next;
      } while (next <= 0xffff);
      obj->scaleX = next;
      obj->scaleY = next;
    }
    Script_WaitForEventTimeoutFar(obj);
  }
}

//--------------------------------------------------------------------------
// 31 フレームかけて回りながら上り、縮んでから粒 8 個になって消える
//--------------------------------------------------------------------------
void UpdateRisingParticleBurst(void *object) {
  struct MotionObject *src = object;
  struct MotionObject *child;
  s32 i;
  s32 dist;
  u32 spread;

  AudioCommand_PlayFar(154);
  for (i = 30; i >= 0; i--) {
    src->y += 1 << 16;
    src->angle += 0x2000; // 45°
    src->scaleX += -0x800;
    src->scaleY += -0x800;
    WaitFrames(1);
  }
  for (i = 7; i >= 0; i--) {
    child = Object_Spawn(285, src->x, src->y, src->z);
    if (child != 0) {
      s32 speed;
      /* FAKEMATCH: a block-local 1.0, set before the block's calls, keeps
       * 0x10000 in one register that loop.c hoists for both uses.
       * Measured: written as the constant at both uses, or with speed
       * holding the call plus the constant, it stays unhoisted (89
       * instructions for 93, differ 24); the acceleration stored before
       * Random16() written inline hoists it but moves that store above
       * the call (differ 4). */
      s32 unit = 0x10000;

      ObjectDispatch_InitializeFar((struct DispatchObject *)child,
                                   gBattleFx_FragmentScript);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->verticalMotionStrength = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  AudioCommand_PlayFar(131);
  ObjectDispatch_ReleaseFar(src);
}

//--------------------------------------------------------------------------
// 64 個のオブジェクトのうち、最初のパーツが絵 200 のものへ value を渡す
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyValueToKind200Children(s32 value) {
  struct ObjectRuntime *object = gObjectSlots;
  s32 cnt = 63;

  do {
    if (object->script != NULL) {
      u8 kind = object->animationKind;

      if (kind == 1) {
        struct AnimationObject *anim = object->animation;
        struct AnimationEntry *entry = anim->entries[0];

        if (entry->animId == 200) {
          entry->param = value;
          anim->dirty = kind;
        }
      }
    }
    cnt--;
    object++;
  } while (cnt >= 0);
}

//--------------------------------------------------------------------------
// 効果の中心から操作キャラが 60 ドット離れるか、残り時間が尽きたら終了を頼む
// (中心と時間は RunBattleEffect08 が gEventWork の 0xcb8〜 に置く)
//--------------------------------------------------------------------------
void FieldEffect_WatchLeaderDistance(void) {
  struct FieldWork *work;
  struct ObjectRuntime *actor;
  s16 *timer;
  s32 dx;
  s32 dz;

  work = (struct FieldWork *)gEventWork;
  actor = ObjectTable_Get(gGameState.selectedActor);
  if (work->unkcc0 != 0) // 時間を減らすか
  {
    timer = &work->effectFrames;
    if (*timer != 0)
      (*timer)--;
  }

  // x は 0xd105 (約 0.82) 倍して比べる
  dx = Iwram_MulQ16(work->effectCenterX - actor->x / 0x10000, 0xd105);
  dz = work->effectCenterY - (actor->z - actor->y) / 0x10000;
  if (dx * dx + dz * dz >= 3600 || work->effectFrames == 0) {
    u16 *slot = (u16 *)&work->command; // psynergyRequest
    u32 request = 0x2090;

    *slot = request;
  }
}

//--------------------------------------------------------------------------
// markers.c
// 戦闘エフェクト 8 番：選択中のオブジェクトを中心に画面を切りかえ、印を出す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 戦闘エフェクト 8 番
// 選択中のオブジェクトの位置を中心に画面切りかえを始め、
// 合うイベントがあれば動かしてから、まわりに印を出す
//--------------------------------------------------------------------------
void RunBattleEffect08(void) {
  struct FieldWork *scene;
  struct DisplayTransitionState *work;
  struct FieldActor *target;
  s32 arg;
  void *ev;
  s32 frame;

  scene = WORK_SLOT(HEAP_SLOT_EVENT);
  ObjectDispatch_ApplyValueToKind200Children(6);
  DisplayTransition_InitializeBattleEffectState(8);
  work = WORK_SLOT(HEAP_SLOT_TRANSITION);
  target = ObjectTable_Get(gGameState.selectedActor);
  work->centreX = target->x.fixed;
  work->centreY = target->z.fixed - target->y.fixed;
  BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
  BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  BattleFx_StartBufferInterpolation(1);
#if !defined(TBS_EDITION_EN)
  scene->effectActive = 0; // 英語版にはない
#endif
  WaitFrames(1);
  ev = BattleFx_FindMatchingEvent(0x50000005, 8, &arg);
  if (ev != NULL)
    BattleFx_RunEventAction(ev, gGameState.selectedActor, arg);
  AudioCommand_PlayFar(131);

  // イベントのワークに中心 (ドット) と時間を入れ、近くに印を出す
  scene->effectActive = 1;
  scene->effectCenterX = work->centreX / 0x10000;
  scene->effectCenterY = work->centreY / 0x10000;
  scene->effectFrames = 600;
  scene->unkcc0 = 1;
  FieldEffect_SpawnNearbyMarkers();
  frame = 0;
  do {
    WaitFrames(1);
    work->value = frame;
    frame++;
  } while (frame <= 18);
  Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}

//--------------------------------------------------------------------------
// burstobj.c
// エナジー演出：イマジンの打ち切り、演出 12 の粒、演出 7 で対象を持ち上げる
//--------------------------------------------------------------------------

#define PSYNERGY_EV_4 0x40000005 // エナジーのイベント、段階 4
#define FX_REVEAL 8              // イマジンの演出
#define PARTICLE_OBJ 284         // 粒の物体
#define FALL_OBJ 217             // 落ちる物体
#define FALL_LAST 4              // 落ちる物体は 0-4 の 5 個

struct BurstParticleVector {
  s32 values[3]; // x, y, z
};

// 粒のスプライトの属性
struct BurstParticleVisual {
  u8 reserved00[5];
  u8 flagsALow : 5;
  u8 flip : 1;
  u8 flagsAHigh : 2;
  u8 reserved06;
  u8 flagsBLow : 6;
  u8 flagsBHigh : 2;
  union {
    struct {
      u16 shape : 10;
      u16 unused : 6;
    } bits;
    struct {
      u8 lo;
      u8 highLow : 4;
      u8 palette : 4;
    } bytes;
  } attributes;
};

struct BurstParticleVisualGroup {
  struct BurstParticleVisual primary;
  struct BurstParticleVisual child;
};

//--------------------------------------------------------------------------
// イマジンの効果中なら打ち切る
// 見張りをやめ、色を黒から 30 フレームで戻し、イマジンの段階 4
// のイベントを実行する
//--------------------------------------------------------------------------
void ResetSceneTransitionEffect(void) {
  // gEffectWork はスロット 56。0x74 前がイベントワーク (27)、0x64 前がスロット
  // 31
  struct BattleFxScene **cell = &gEffectWork;
  struct BattleFxScene *work = *cell;
  struct DisplayTransitionState *scene =
      *(struct DisplayTransitionState **)((u8 *)cell - 0x64);
  struct FieldWork *state = *(struct FieldWork **)((u8 *)cell - 0x74);
  s16 zero;
  s32 tid;
  void *ev;

  if (state->effectActive != 0) {
    AudioCommand_PlayFar(SOUND_SCENE_TRANSITION);
    Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

    zero = 0;
    state->effectActive = zero;
    state->effectFrames = zero;
    ObjectDispatch_ApplyValueToKind200Children(0);

    BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 1);
    BattleFx_StartBufferInterpolation(1);
    BattleFx_ApplyColorToSourceBuffer(0, 0);
    BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 0);
    BattleFx_StartBufferInterpolation(30);
    WaitFrames(1);

    ev = BattleFx_FindMatchingEvent(PSYNERGY_EV_4, FX_REVEAL, &tid);
    if (ev != NULL)
      BattleFx_RunEventAction(ev, gGameState.selectedActor, tid);

    if (work->childOption == 0) {
      scene->nextMode = 0;
      scene->duration = 1;
      scene->step = 1;
      WaitFrames(10);
    }
  }
}

//--------------------------------------------------------------------------
// 対象のある演出 12：対象のまわりに粒を出し、対象を足元の優先度にする
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticleMainObject(s32 unused) {
  struct FieldActor *target;

  target = ((struct BattleFxScene *)gEffectWork)->child;
  if (target != NULL) {
    BattleEffect_SpawnBurstParticleField();
    ObjectDispatch_ApplyArgumentToChildrenFar(target, 2);
    target->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlagsFar(target, 0);
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    WaitFrames(10);
    AudioCommand_PlayFar(SOUND_RECOVERY);
    WaitFrames(40);
    BattleFx_PrepareBufferInterpolation();
  }
}

//--------------------------------------------------------------------------
// 対象のない演出 12：粒を出したあと、まわり 4-10 ドットに落ちる物体を 5 個出す
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticles(void) {
  struct BattleFxScene *work = gEffectWork;
  struct BurstParticleVector pos;
  struct BurstParticleVector *p;
  s32 n;

  BattleEffect_SpawnBurstParticleField();
  AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
  p = &pos;
  n = FALL_LAST;
  do {
    struct FieldActor *obj;
    s32 dist;

    p->values[0] = work->x;
    p->values[2] = work->z;
    dist = (Random16() * 6) + 0x40000;
    Vector_AddPolarOffset(dist, Random16(), p);
    p->values[1] = work->y;
    obj = Object_Spawn(FALL_OBJ, p->values[0], p->values[1], p->values[2]);
    if (obj != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_BurstParticleObjectScript);
      obj->motionFlags = ACTOR_FALLS;
    }
    WaitFrames(((Random16() * 2) >> 16) + 2);
    n--;
  } while (n >= 0);
  WaitFrames(30);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 使う者の前から粒を 24 個、2 フレームおきに出し、演出の位置のまわりへ飛ばす
// 出る位置：向き 0x4000 は 10 ドット上、0xc000 は 24 ドット上、ほかは 10
// ドット上の 10 ドット前
//--------------------------------------------------------------------------
void BattleEffect_SpawnBurstParticleField(void) {
  struct BattleFxScene *work;
  struct FieldActor *user;
  s32 pos[3];
  s32 n;

  work = gEffectWork;
  user = work->mainObject;
  BattleEffect_InitializeSharedScene();

  for (n = 0; n < PARTICLE_MAX; n++) {
    struct FieldActor *obj;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    s32 dist;

    if (work->angle == 0x4000) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + 0xa0000;
      pos[2] = user->z.fixed;
    } else if (work->angle == 0xc000) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + 0x180000;
      pos[2] = user->z.fixed;
    } else {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + 0xa0000;
      pos[2] = user->z.fixed;
      Vector_AddPolarOffset(0xa0000, work->angle, pos);
    }

    // 子のスプライトに、向きと形と色を写す
    obj = Object_Spawn(PARTICLE_OBJ, pos[0], pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
    child->flip = visual->flip;
    child->flagsAHigh = visual->flagsAHigh;
    child->flagsBHigh = visual->flagsBHigh;
    child->attributes.bits.shape = visual->attributes.bits.shape;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (obj != 0) {
      obj->scaleY = 0xb333; // 0.7 倍
      obj->scaleX = 0xb333;
      obj->acceleration = 0x18000;
      obj->speed = 0x18000;
      obj->motionFlags = 0;
      Animation_ApplyChildValuesFar(obj, 11);
      ObjectDispatch_ApplyArgumentToChildrenFar(obj, 7);
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_CommonParticleScript + 1);
      ObjectDispatch_SetSingleChildFlagsFar(obj, 1);

      // 行き先：演出の位置から 4-10 ドット
      pos[0] = work->x;
      pos[1] = work->y;
      pos[2] = work->z;
      if (work->angle == 0xc000)
        Vector_AddPolarOffset(0xe0000, work->angle, pos);
      dist = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(dist, Random16(), pos);
      Object_SetMoveTargetFar(obj, pos[0], pos[1], pos[2]);
    }
    AudioCommand_PlayFar(131);
    WaitFrames(2);
  }
  WaitFrames(8);
}

//--------------------------------------------------------------------------
// 対象のある演出 7
// 左右 32 ドットに物体を出して 14 ドット動かし、対象といっしょに 32
// ドット持ち上げる
//--------------------------------------------------------------------------
void BattleEffect_RunTargetedItemBreak(s32 unused) {
  struct BattleFxScene *work;
  struct DispatchObject *user;
  struct FieldActor *target;
  struct MotionObject *pair[2];
  s32 pos[3];
  s32 x, y, z;
  s32 n;
  s32 startY;

  work = gEffectWork;
  target = work->child;
  user = work->mainObject;
  if (target == NULL)
    return;

  BattleEffect_InitializeSharedScene();
  user->argument = (s32)target;
  ObjectDispatch_InitializeFar(user, gBattleFx_CommonParticleScript + 3);

  x = work->x;
  pos[0] = x;
  y = work->y + 0x100000; // 16 ドット上
  pos[1] = y;
  z = work->z;
  pos[2] = z;
  pair[0] = BattleFx_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
  pair[1] = BattleFx_SpawnItemBreakMode3(pos[0] - 0x200000, pos[1], pos[2], 0);

  WaitFrames(15);
  for (n = 0; n < 2; n++) {
    struct MotionObject *obj = pair[n];
    if (obj != NULL)
      Motion_SetTargetPositionFromMagnitudeAngle(obj, 0xe0000, obj->angle);
  }

  Script_WaitForEventTimeoutFar(pair[0]);
  target->update =
      (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
  AudioCommand_PlayFar(130);
  target->motionFlags = 4;

  // 1/4 ドットずつ 32 ドット上げる
  startY = target->y.fixed;
  if (pair[0] != NULL && pair[1] != NULL && startY <= startY + 0x200000) {
    do {
      pair[0]->y += 0x4000;
      pair[1]->y += 0x4000;
      target->y.fixed += 0x4000;
      WaitFrames(1);
    } while (target->y.fixed <= startY + 0x200000);
  }

  UpdateRisingParticleBurst(pair[0]);
  UpdateRisingParticleBurst(pair[1]);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// anchor.c
// エナジー演出 7：左右の二つの物体と、飛び散る破片
//--------------------------------------------------------------------------
// 破片の数
#define ANCHOR_SHARD_MAX 24

//--------------------------------------------------------------------------
// 対象のない演出の本体：演出の位置の 16 ドット上、左右 32 ドットに spawn で
// 物体を出し、それぞれの向きへ 24 ドット動かしてから、破片を 24 個、
// 8-32 ドット先へ散らす
//--------------------------------------------------------------------------
#define DEFINE_ITEM_BREAK_EFFECT(name, spawn)                                  \
  void name(void) {                                                            \
    struct BattleFxScene *work;                                                \
    struct MotionObject *obj;                                                  \
    struct MotionObject *pair[2];                                              \
    struct ScatterPosition pos;                                                \
    struct MotionObject **walk;                                                \
    s32 n;                                                                     \
    s32 dist;                                                                  \
                                                                               \
    work = gEffectWork;                                                        \
    BattleEffect_InitializeSharedScene();                                      \
    pos.x = work->x;                                                           \
    pos.y = work->y + 0x100000;                                                \
    pos.z = work->z;                                                           \
    pair[0] = spawn(pos.x + 0x200000, pos.y, pos.z, 0x8000);                   \
    pair[1] = spawn(pos.x - 0x200000, pos.y, pos.z, 0);                        \
                                                                               \
    WaitFrames(15);                                                            \
    walk = pair;                                                               \
    for (n = 0; n < 2; n++) {                                                  \
      obj = *walk++;                                                           \
      if (obj != 0)                                                            \
        Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle); \
    }                                                                          \
                                                                               \
    Script_WaitForEventTimeoutFar(pair[0]);                                    \
    AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);                                  \
    for (n = 0; n < ANCHOR_SHARD_MAX; n++) {                                   \
      pos.x = work->x;                                                         \
      pos.y = work->y + 0x100000;                                              \
      pos.z = work->z;                                                         \
      obj = Object_Spawn(285, pos.x, pos.y, pos.z);                            \
      if (obj != 0) {                                                          \
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj,             \
                                     gBattleFx_FragmentScript);                \
        obj->speedLimit = Random16() + 0x20000;                                \
        obj->acceleration = 0x20000;                                           \
        obj->motionFlags = 0;                                                  \
        dist = Random16() * 24 + 0x80000;                                      \
        Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());     \
      }                                                                        \
    }                                                                          \
                                                                               \
    ObjectDispatch_ReleaseFar(pair[0]);                                        \
    ObjectDispatch_ReleaseFar(pair[1]);                                        \
    BattleFx_PrepareBufferInterpolation();                                     \
  }

// 対象のない演出 7
DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect07, BattleFx_SpawnItemBreakMode3)

//--------------------------------------------------------------------------
// 物体 215 を出し、回しながら 1/4 から等倍まで 24 フレームで大きくする
// angle : 大きくなったあとの向き
//--------------------------------------------------------------------------
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z,
                                                  s32 angle) {
  struct MotionObject *obj;
  s32 scale;

  AudioCommand_PlayFar(SOUND_ITEM_BREAK);
  obj = Object_Spawn(215, x, y, z);
  if (obj != NULL) {
    obj->scaleX = obj->scaleY = 0x4000;
    obj->acceleration = obj->speedLimit = 0x30000; // 3 ドット/フレーム
    ((struct FieldSprite *)obj->records)->priority = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
    scale = obj->scaleX;
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        obj->scaleY = scale;
        obj->scaleX = scale;
        obj->angle += 0x2000; // 1/8 回転
        WaitFrames(1);
        scale = obj->scaleX;
      } while (scale <= 0xffff);
    }
    obj->angle = angle;
  }
  return obj;
}

//--------------------------------------------------------------------------
// impact.c
// 散って集まる粒の動きと、重い衝撃の破片
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 粒の動き : 原点のまわりへ散ってから、主オブジェクトのそばへ集まって消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot) {
  struct BattleFxScene *scene = gEffectWork;
  struct EffectVector pos;
  s32 state;

again:
  state = slot->state;
  if (state == 0) {
    // 原点から 25 ドット、でたらめな方向へ
    pos.x = slot->originX;
    pos.z = slot->originZ;
    Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 3 << 16;
    slot->maxSpeed = 3 << 16;
    slot->flag42 = 0;
    slot->state++;
    return;
  }

  if (state == 1) {
    // 着いたらすぐ次へ
    if (BattleFx_HasReachedTarget(slot) == 0) {
      slot->state++;
      goto again;
    }
    return;
  }

  if (state == 2) {
    // 主オブジェクトの 16 ドット上から scene->angle へ 8 ドット、
    // 画面座標にして 4 ドットの揺らぎを足す
    struct MotionObject *obj = scene->mainObject;

    pos.x = obj->x;
    pos.y = obj->y + (16 << 16);
    pos.z = obj->z;
    Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->maxTurnStep = 0x800;
    slot->flag42 = 1;
    slot->state++;
    return;
  }

  if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
    BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 重い衝撃 : y を 32 ドット下げた位置に 0x11b を置き、破片を 12 個まき散らす
// src : 衝撃の位置になるオブジェクト
//--------------------------------------------------------------------------
s32 SpawnHeavyImpactEffect(struct MotionObject *src) {
  struct EffectVector pos;
  struct ObjectRuntime *obj;
  struct ObjectRuntime *frag;
  s32 i;

  AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
  pos.x = src->x;
  pos.y = src->y;
  pos.z = src->z;
  obj = Object_Spawn(0x11b, pos.x, pos.y - (32 << 16), pos.z);
  if (obj != NULL) {
    obj->flags = 0;
    obj->unk5e = 20;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }

  // 破片 : 速さ 1〜2 ドット、8〜32 ドット先のでたらめな方向へ
  for (i = 0; i < 12; i++) {
    frag = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
    if (frag != NULL) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)frag,
                                   gBattleFx_FragmentScript);
      frag->speedLimit = Random16() + (1 << 16);
      frag->acceleration = 1 << 16;
      frag->flags = 0;
      Motion_SetTargetPositionFromMagnitudeAngle(
          frag, Random16() * 24 + (8 << 16), Random16());
    }
  }
  return 0;
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 4 番を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_CallEffect04(s32 unused) { RunBattleEffect04(); }

//--------------------------------------------------------------------------
// orbit.c
// フィールドのエナジー演出：4 番（押し出す弾）、6 番（渦を巻いて集まる粒）
//--------------------------------------------------------------------------

#define ABOVE (16 << 16) // 16 ドット上

//--------------------------------------------------------------------------
// obj の 16 ドット上の位置
//--------------------------------------------------------------------------
static __inline__ void PosAbove(struct FieldActor *obj,
                                struct EffectVector *pos) {
  pos->x = obj->x.fixed;
  pos->y = obj->y.fixed + ABOVE;
  pos->z = obj->z.fixed;
}

// FieldActor と同じ物体。FieldActor に欄のない 0x5e を使う
struct SparkObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[24];
  s32 anchorX; // ゆれの中心（FieldActor の targetX の位置）
  u8 unk3c[12];
  s32 gravity; // FieldActor と同じ位置
  u8 unk4c[9];
  u8 motionFlags; // FieldActor と同じ位置
  u8 unk56[8];
  u16 unk5e;
  u8 unk60[4];
  s16 phase; // ゆれの位相（128 フレームで 1 周）
  u8 unk66[6];
  void (*update)(struct SparkObject *);
};

//--------------------------------------------------------------------------
// エナジー 4 番 : 使う者の前に弾を出して大きくし、残像 3 つと前へ飛ばす
// 当たった物（scene->child）は 16 ドット押し出す
//--------------------------------------------------------------------------
void RunBattleEffect04(void) {
  struct FieldActor *target;
  s32 ctx;
  struct FieldActor *copies[4];
  struct EffectVector pos;
  struct FieldActor *obj;
  struct FieldActor *copy;
  struct FieldActor *src;
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  struct AnimationObject *res;
  void *ev;
  s32 scale;
  s32 i;
  u8 resSlot;

  scene = gEffectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(130);
  slot = scene->slots;

  // 使う者の頭上から、光を 2 フレームおきに 12 個はじけさせる
  for (i = 0; i <= 11; i++) {
    src = scene->mainObject;
    PosAbove(src, &pos);
    Camera_WorldToScreen(&pos);
    EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 9);
    slot->scaleY = 0xb333; // 0.7 倍
    slot->scaleX = 0xb333;
    WaitFrames(2);
    slot++;
  }

  // 弾：使う者の向きに 8 ドット先で、0.25 倍から等倍まで大きくする
  src = scene->mainObject;
  pos.x = src->x.fixed;
  pos.y = src->y.fixed + ABOVE;
  pos.z = src->z.fixed;
  Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
  obj = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
  if (obj == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }
  obj->scaleY = 0x4000;
  obj->scaleX = 0x4000;
  obj->facing = scene->angle;
  obj->speed = 4 << 16;
  obj->acceleration = 4 << 16;
  obj->motionFlags = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 5);
  Animation_ApplyChildValuesFar(obj, 3);
  scale = obj->scaleX;
  if (scale < 0x10000) {
    do {
      scale += 0x500;
      obj->scaleY = scale;
      obj->scaleX = scale;
      WaitFrames(1);
      scale = obj->scaleX;
    } while (scale <= 0xffff);
  }
  WaitFrames(3);

  // 残像 3 つ。絵の資源はひとつを分けあう
  res = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] =
        Object_Spawn(0xd7, obj->x.fixed, obj->y.fixed, obj->z.fixed);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = scene->angle;
      copy->speed = 4 << 16;
      copy->acceleration = 4 << 16;
      copy->motionFlags = 0;
      ObjectDispatch_ApplyArgumentToChildrenFar(copy, 5);
      Animation_ApplyChildValuesFar(copy, 2);
      res = Object_ReplaceResourceEntry((struct AnimationObject *)copy->sprite,
                                        res);
    }
  }
  resSlot = res->slot;

  // 行き先：scene->enabled なら使う者の 56 ドット先、ほかは scene の位置
  if (scene->enabled != 0) {
    src = scene->mainObject;
    pos.x = src->x.fixed;
    pos.y = src->y.fixed + ABOVE;
    pos.z = src->z.fixed;
    Vector_AddPolarOffset(56 << 16, scene->angle, &pos);
  } else {
    pos.x = scene->x;
    pos.y = scene->y + ABOVE;
    pos.z = scene->z;
  }
  Object_SetMoveTargetFar(obj, pos.x, pos.y, pos.z);
  ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                               gBattleFx_BurstParticleObjectScript + 4);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      WaitFrames(3);
      Object_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      ObjectDispatch_InitializeFar((struct DispatchObject *)copy,
                                   gBattleFx_CommonParticleScript + 1);
    }
  }

  // 弾のスクリプトが終わるまで、60 フレームまで待つ
  for (i = 0; i <= 59 && ((struct ScriptMotionObject *)obj)->script != NULL;
       i++)
    WaitFrames(1);

  // 当たった物を、ぶつからなければ 16 ドット押し出す（childOption なら跳ねる）
  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = 8 << 16;
    pos.x = target->x.fixed;
    pos.y = target->y.fixed;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, scene->angle, &pos);
    if (Map_CheckObjectStepFar(target, &pos) == 0 &&
        ScriptObject_CheckOverlapFar(target, &pos) == 0) {
      target->acceleration = 1 << 16;
      target->speed = 1 << 16;
      Object_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  // 応じるマップのイベントがあれば動かす
  ev = BattleFx_FindMatchingEvent(0x50000005, 4, &ctx);
  if (ev != NULL)
    BattleFx_RunEventAction(ev, gGameState.selectedActor, ctx);
  WaitFrames(10);
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(20);
  if (resSlot != 96)
    Resource_ResetEntry(resSlot);
}

//--------------------------------------------------------------------------
// 毎フレームの処理：8 フレームごとに 2 フレームだけアニメ 2 にして光らせる
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle) {
  s32 frame = gFrameCount & 7;
  if (frame == 0) {
    Animation_ApplyChildValuesFar(particle, 2);
  } else if (frame == 2) {
    Animation_ApplyChildValuesFar(particle, 0);
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：1/16 ずつ縮み、1/16 以下で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle) {
  s32 scale;

  if (particle != NULL) {
    scale = particle->scaleX - 0x1000;
    particle->scaleY -= 0x1000;
    particle->scaleX = scale;
    if (scale <= 0x1000)
      ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                   gBattleFx_CommonParticleScript);
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：target のまわりを左まわりに渦を巻いて寄る
// 半径は残りフレーム × 2 ドット、1 フレームに 0x800 まわる
// work は始めの向き、timer は残りフレーム。0 で縮んで消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle) {
  struct EffectVector pos;
  s16 life;
  s32 cnt;

  if (particle != NULL) {
    cnt = (u16)particle->timer - 1;
    particle->timer = cnt;
    life = cnt;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset(life << 17, (s16)particle->work + (life << 11),
                            &pos);
      particle->x.fixed = pos.x;
      particle->y.fixed = pos.y;
      particle->z.fixed = pos.z;
    } else {
      particle->update =
          (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
    }
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：左まわりと同じで、右まわり
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle) {
  struct EffectVector pos;
  s16 life;
  s32 cnt;

  if (particle != NULL) {
    cnt = (u16)particle->timer - 1;
    particle->timer = cnt;
    life = cnt;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset(life << 17, (s16)particle->work - (life << 11),
                            &pos);
      particle->x.fixed = pos.x;
      particle->y.fixed = pos.y;
      particle->z.fixed = pos.z;
    } else {
      particle->update =
          (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
    }
  }
}

//--------------------------------------------------------------------------
// エナジー 6 番をかけられた物（scene->child）から始める
// childMode があれば、物は変えずに粒だけ出す
//--------------------------------------------------------------------------
void BattleFx_StartOrbitingParticles(s32 unused) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *target = scene->child;

  if (target != 0) {
    if (scene->childMode != 0) {
      scene->enabled = 1;
    }
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    BattleFx_RunOrbitingParticles();
  }
}

//--------------------------------------------------------------------------
// エナジー 6 番 : 粒 16 個が scene の位置へ渦を巻いて集まり、
// 物を 16 回光らせる。childOption がなければ物を mode 2 にする
//--------------------------------------------------------------------------
void BattleFx_RunOrbitingParticles(void) {
  s32 ctx;
  struct EffectVector pos;
  struct EffectVector *p;
  struct BattleFxScene *scene;
  struct FieldActor *target;
  struct FieldActor *particle;
  void *ev;
  s32 cnt;

  scene = gEffectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(115);

  // 3 フレームおきに粒を出す。大きさ 0.5〜1 倍、まわる向きはでたらめ
  // 中心は scene の位置から 2〜6 ドットずらす
  p = &pos;
  cnt = 15;
  do {
    particle = Object_Spawn(0xe8, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 dist;

      scale = (Random16() >> 1) + 0x8000;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update =
            (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleLeft;
      else
        particle->update =
            (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->work = Random16();
      Animation_ApplyChildValuesFar(particle, 9);

      p->x = scene->x;
      p->y = scene->y;
      p->z = scene->z;
      dist = (Random16() << 2) + (2 << 16);
      Vector_AddPolarOffset(dist, Random16(), p);
      particle->targetX = p->x;
      particle->targetY = p->y;
      particle->targetZ = p->z;
    }

    WaitFrames(3);
    cnt--;
  } while (cnt >= 0);

  WaitFrames(10);
  AudioCommand_PlayFar(115);
  WaitFrames(50);

  if (target != NULL && scene->enabled == 0) {
    AudioCommand_PlayFar(212);

    // 5 フレームごとに 1 フレーム光る
    cnt = 15;
    do {
      Animation_ApplyChildValuesFar(target, 7);
      WaitFrames(1);
      Animation_ApplyChildValuesFar(target, 0);
      WaitFrames(4);
      cnt--;
    } while (cnt >= 0);

    if (scene->childOption == 0) {
      AudioCommand_PlayFar(220);
      ObjectDispatch_ApplyArgumentToChildrenFar(target, 2);
    }

    target->update =
        (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleMain;
    ev = BattleFx_FindMatchingEvent(0x50000005, 6, &ctx);
    if (ev != NULL) {
      BattleFx_RunEventAction(ev, gGameState.selectedActor, ctx);
    }
    WaitFrames(20);
  }

  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 毎フレームの処理：anchorX を中心に左右 4 ドット、128 フレームで 1 往復ゆれる
//--------------------------------------------------------------------------
void BattleFx_SwayParticle(struct SparkObject *obj) {
  s16 *phase;

  phase = &obj->phase;
  obj->x = obj->anchorX + Iwram_MulQ16(4 << 16, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 10 ドット上を中心に、渦を巻いて寄る
// 半径は残りフレームと同じドット数。timer が 0 で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *obj) {
  struct BattleFxScene *scene = gEffectWork;
  struct EffectVector pos;
  s16 life;
  s32 cnt;

  if (obj != NULL) {
    cnt = (u16)obj->timer - 1;
    obj->timer = cnt;
    life = cnt;
    if (life != 0) {
      pos.x = scene->x;
      pos.y = scene->y + (10 << 16);
      pos.z = scene->z;
      Vector_AddPolarOffset(life << 16, (s16)obj->work + (life << 11), &pos);
      obj->x.fixed = pos.x;
      obj->y.fixed = pos.y;
      obj->z.fixed = pos.z;
    } else {
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 16 ドット上で左右 6 ドット（64
// フレーム）ゆれ、 3 フレームごとに、ゆれながらゆっくり落ちる粒を出す phase が
// -1 なら、その場から動かない
//--------------------------------------------------------------------------
void BattleFx_RunSparkEmitter(struct SparkObject *obj) {
  struct BattleFxScene *scene;
  s16 *phase;
  struct SparkObject *spark;
  s32 pos[3];

  phase = &obj->phase;
  scene = gEffectWork;
  if (*phase != -1) {
    obj->x = scene->x + Iwram_MulQ16(6 << 16, Trig_Sin(*phase << 10));
    obj->y = scene->y + ABOVE;
    obj->z = scene->z;
    (*phase)++;
    *phase = (*phase + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    // 2 ドット上から、でたらめな向きに 0〜6 ドットずらす
    pos[0] = obj->x;
    pos[1] = obj->y + (2 << 16);
    pos[2] = obj->z;
    Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
    spark = Object_Spawn(0x11d, pos[0], pos[1], pos[2]);
    if (spark != 0) {
      spark->update = BattleFx_SwayParticle;
      spark->scaleX = spark->scaleY = 0x9999; // 0.6 倍
      spark->motionFlags = ACTOR_FALLS;
      spark->gravity = 458;
      spark->phase = Random16() >> 9;
      spark->anchorX = spark->x;
      Animation_ApplyChildValuesFar(spark, 9);
      spark->unk5e = 72;
      ObjectDispatch_InitializeFar((struct DispatchObject *)spark,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

//--------------------------------------------------------------------------
// call_effect_03_and_stop.c
// 演出 03：渦を巻いて集まる 8 個の粒と、火花を出して飛び去る 1 個
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 演出 03 を流し、いまのオブジェクトを止める
//--------------------------------------------------------------------------
void BattleFx_CallEffect03AndStop(s32 unused) {
  RunBattleEffect03();
  EffectRuntime_StopCurrentObject();
}

//--------------------------------------------------------------------------
// 演出 03
// 8 個の粒が渦を巻いて中心へ集まり、中心に現れた 1 個が火花を出してから
// 遠くへ飛び去る
//--------------------------------------------------------------------------
void RunBattleEffect03(void) {
  struct BattleFxScene *scn = gEffectWork;
  struct FieldActor *target = scn->mainObject;
  struct FieldActor *object;
  struct FieldSprite *last;
  u8 marker;
  s32 i;

  BattleEffect_InitializeSharedScene();

  // 渦の粒：1 フレームに 1 個ずつ、1/8 周ずらして出す
  last = 0;
  for (i = 0; i < 8; i++) {
    object = Object_Spawn(233, target->x.fixed, target->y.fixed + 0x200000,
                          target->z.fixed);
    if (object != 0) {
      object->scaleY = 0xb333; // 0.7 倍
      object->scaleX = 0xb333;
      object->update = BattleFx_UpdateShrinkingOrbitObject;
      object->timer = 120;
      object->work = i << 13;
      object->motionFlags = 4;
      last = Object_ReplaceResourceEntry(object->sprite, last);
    }
    WaitFrames(1);
  }

  marker = last->vramBlock;
  AudioCommand_PlayFar(130);
  WaitFrames(110);

  // 中心の 1 個
  target = Object_Spawn(233, 0, 0, 0);
  object = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    target->x.fixed = scn->x;
    target->y.fixed = scn->y + 0x100000;
    target->z.fixed = scn->z;
    target->motionFlags = 4;
    Animation_ApplyChildValuesFar(target, 7);
  }

  AudioCommand_PlayFar(131);
  WaitFrames(12);
  if (target != 0) {
    // 2 フレームごとに子の値を切り替える（4 回に 1 回は 10）
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH: needed to match: the ROM keeps object as a second register
       * copy of the spark (r5 beside r6); this redundant reassignment stops
       * gcse copy-propagating object = target away, and loop then drops it
       * (differ 1 without it). */
      target = object;
      if (i & 3)
        Animation_ApplyChildValuesFar(object, 9);
      else
        Animation_ApplyChildValuesFar(object, 10);
      WaitFrames(2);
    }
  }

  // 火花
  Animation_ApplyChildValuesFar(object, 0);
  AudioCommand_PlayFar(84);
  if (object != 0) {
    target->update = BattleFx_RunSparkEmitter;
    target->timer = 0;
    if (scn->enabled != 0)
      WaitFrames(128);
    else
      WaitFrames(192);
  }

  // 飛び去る
  if (target != 0) {
    target->timer = -1;
    target->speed = 0x50000;
    target->acceleration = 0x6666;
    target->steeringFlags = 0;
    Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
    Script_WaitForEventTimeoutFar(target);
    ObjectDispatch_ReleaseFar(target);
  }
  if (marker != 0x60)
    Resource_ResetEntry(marker);
  if (scn->finishCallback != 0)
    scn->finishCallback();
  BattleFx_PrepareBufferInterpolation();
}

#define CLOAK_TILE 0xfb  // 隠れていられるタイルの属性
#define CLOAK_END 0x2092 // 隠れていられないときの psynergyRequest

//--------------------------------------------------------------------------
// 毎フレームの処理 : 選択中のオブジェクトの足もとのタイルが
// CLOAK_TILE でなければ、CLOAK_END を要求する
// イベントの mode が 3 なら 32 ドット四方の gMapBlocks、
// ほかは 16 ドット四方の地形のマップを見る
//--------------------------------------------------------------------------
void CheckObjectMapTile(void) {
  u32 slot;
  struct EventRuntime *ev;
  struct ObjectRuntime *obj;
  u8 *tile;
  s32 x;
  s32 y;

  slot = (u32)&gEventWork;
  ev = gEventWork;
  obj = ObjectTable_Get(gGameState.selectedActor);
  tile = (u8 *)*(struct MapState **)(slot - 76); // gMapWork[0]

  if (ev->mode19e == 3) {
    u32 tx;
    u32 ty;

    x = obj->x;
    if (x < 0)
      x += 0x1fffff;
    tx = (x >> 21) & 31;

    y = obj->z;
    if (y < 0)
      y += 0x1fffff;
    ty = (y >> 21) & 31;

    tile = (u8 *)gMapBlocks + ((tx + (ty << 5)) << 2);
  } else {
    if (obj->terrainId <= 2) {
      tile = (u8 *)((struct MapState *)tile)->layers[obj->terrainId].cells;
    } else
      tile = (u8 *)gMapCellBuffer;

    x = obj->x;
    if (x < 0)
      x += 0xfffff;
    {
      u32 tx = x >> 20;

      y = obj->z;
      if (y < 0)
        y += 0xfffff;

      tile = (u8 *)(tile + ((tx + ((y >> 20) << 7)) << 2));
    }
  }

  if (tile[2] != CLOAK_TILE)
    ((struct EventWork *)ev)->psynergyRequest = CLOAK_END;
}

//--------------------------------------------------------------------------
// 姿を戻す : 確認を止め、点滅させて cloaked を消す
//--------------------------------------------------------------------------
void MapEvent_RunTileTriggerSequence(void) {
  struct ObjectRuntime *obj;
  struct AnimationObject *anim;
  struct AnimationEntry *entry;
  u32 i;

  obj = ObjectTable_Get(gGameState.selectedActor);
  anim = obj->animation;
  entry = anim->entries[0];

  AudioCommand_PlayFar(154);
  Scheduler_RemoveCallback(CheckObjectMapTile);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
  ((struct ScriptMotionObject *)obj)->hook = 0;

  for (i = 0; i < 5; ++i) {
    entry->param = 7;
    anim->dirty = 1;
    anim->flags = 2;
    WaitFrames(2);
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
  }

  for (i = 0; i < 5; ++i) {
    entry->param = 7;
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
    entry->param = 0;
    anim->dirty = 1;
    WaitFrames(2);
  }

  anim->flags = 1;
  gGameState.cloaked = 0;
}

//--------------------------------------------------------------------------
// 隠れているなら、足もとの確認を始める
//--------------------------------------------------------------------------
void BattleFx_ScheduleCloakedMapCheck(void) {
  if (gGameState.cloaked != 0) {
    Scheduler_AddOrUpdateCallback((SchedulerCallback)CheckObjectMapTile, 0xc80);
  }
}

//--------------------------------------------------------------------------
// 姿を隠す : 10 回点滅させて cloaked を立て、足もとを確かめる
// その場で隠れられなければ、すぐに姿を戻す
//--------------------------------------------------------------------------
void BattleFx_RunFlashingCallbackSequence(void) {
  struct EventWork *ev = gEventWork;
  struct ObjectRuntime *obj = ObjectTable_Get(gGameState.selectedActor);
  struct AnimationObject *anim = obj->animation;
  struct AnimationEntry *entry = anim->entries[0];
  u32 n;
  void (*check)(void);

  AudioCommand_PlayFar(130);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
  ((struct ScriptMotionObject *)obj)->hook = 0;
  n = 0;
  do {
    entry->param = 7;
    anim->dirty = 1;
    anim->flags = 2;
    WaitFrames(2);
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
    n++;
  } while (n <= 9);
  n = 0;
  entry->param = n;
  anim->flags = 2;
  anim->dirty = 1;
  check = CheckObjectMapTile;
  Scheduler_AddOrUpdateCallback(check, 0xc80);
  gGameState.cloaked = 1;
  check();
  if (ev->psynergyRequest == CLOAK_END) {
    MapEvent_RunTileTriggerSequence();
    ev->psynergyRequest = n;
  }
}

// FieldActor と同じ物体。FieldActor に欄のない 0x5e (phase) を使う
struct Effect05Object {
  u8 reserved00[8];
  s32 x, y, z;
  s32 altitude; // 落ちて止まる高さ
  s32 scaleX, scaleY;
  u8 reserved20[8];
  s32 velocityY;
  u8 reserved2c[4];
  s32 speed;
  u8 reserved34[0x1c];
  struct WaveFxSprite *sprite;
  u8 reserved54;
  u8 flag;
  u8 reserved56[8];
  s16 phase;
  u8 reserved60[0xc];
  void *callback; // 毎フレームの処理
};

struct Effect05State {
  s32 angle;
  s32 x, y, z; // 玉の行き先の足もと
  struct Object *target;
  s32 initialized;
  u8 reserved18[8];
  u8 highArc;
  u8 reserved21[0x13];
  u8 variant;
  u8 reserved35[0x10];
  u8 alternate;
};

//--------------------------------------------------------------------------
// 毎フレームの処理 : 落ちる粒が altitude に着いたら、小さな粒を 3 つ散らす
//--------------------------------------------------------------------------
void BattleFx_SpawnRandomAngleTriplet(struct Effect05Object *obj) {
  s32 i;
  struct Effect05Object *p;
  s32 phase = 2;
  s32 phase2;
  s16 *pp;

  if (obj->y <= obj->altitude) {
    obj->phase = phase;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
    p = NULL;
    obj->callback = p;

    // 半分の大きさで、上へ 1 ドットの速さで跳ね、32 ドット先のでたらめな方向へ
    for (i = 0; i <= 2; i++) {
      p = Object_Spawn(0xf0, obj->x, obj->y, obj->z);
      if (p == NULL) {
        break;
      }
      p->scaleY = 0x8000;
      p->scaleX = 0x8000;
      p->flag = 2;
      p->velocityY = 1 << 16;
      p->speed = Random16() + 0x13333;
      Motion_SetTargetPositionFromMagnitudeAngle(p, 32 << 16, Random16());
      pp = &p->phase;
      phase2 = 6;
      *pp = phase2;
      ObjectDispatch_InitializeFar((struct DispatchObject *)p,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 左右にゆれながらゆっくり落ち、地面に着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *obj) {
  s32 r;

  obj->y -= 0x4ccc;
  r = Random16();
  obj->x += r - Random16();
  if (obj->y <= obj->terrainHeight) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }
}

void BattleFx_CallEffect05(s32 unused) { RunBattleEffect05(); }

//--------------------------------------------------------------------------
// 0〜10 段で start から end へ
//--------------------------------------------------------------------------
static __inline__ s32 Lerp10(s32 start, s32 end, s32 step) {
  return start + step * (end - start) / 10;
}

//--------------------------------------------------------------------------
// 光の玉を対象の 16 ドット上から行き先の 32 ドット上 (variant なら 80 ドット上)
// へ 10 フレームで運びながら大きくし、粒を降らせてから戻す
// 行き先は、まだ決まっていなければ対象から angle の向きへ 16 ドット
//--------------------------------------------------------------------------
void RunBattleEffect05(void) {
  struct EffectVector pos;
  struct EffectVector start;
  struct EffectVector end;
  struct Effect05State *work = gEffectWork;
  struct Object *target = work->target;
  struct Effect05Object *orb;
  struct EffectVector *from;
  struct EffectVector *to;
  s32 i;
  s32 steps = 11;
  s32 count;
  s32 y;
  struct Effect05Object *mote;

  orb = Object_Spawn(0xef, i = 0, 0, 0);
  if (orb == 0)
    return;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(138);
  if (work->initialized == 0) {
    work->x = target->pos.x;
    work->z = target->pos.z;
    Vector_AddPolarOffset(16 << 16, work->angle,
                          (struct EffectVector *)&work->x);
    work->y = Map_GetTerrainHeightFar(0, work->x, work->z);
  }
  from = &start;
  from->x = target->pos.x;
  from->y = target->pos.y + (16 << 16);
  from->z = target->pos.z;
  to = &end;
  to->x = work->x;
  y = work->y;
  to->y = y + (32 << 16);
  to->z = work->z;
  if ((s8)work->variant != 0)
    to->y = y + (80 << 16);

  // 運びながら 1/4 から等倍まで大きくする
  for (; i < steps; i++) {
    s32 scale;

    {
      struct EffectVector *a = &start;
      struct EffectVector *b = &end;
      s32 e, t;

      e = b->x;
      t = a->x;
      t += i * (e - t) / 10;
      orb->x = t;
      e = b->y;
      t = a->y;
      t += i * (e - t) / 10;
      orb->y = t;
      e = b->z;
      t = a->z;
      t += i * (e - t) / 10;
      orb->z = t;
    }
    scale = Lerp10(0x4000, 0x10000, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    WaitFrames(1);
  }
  WaitFrames(10);

  if ((s8)work->alternate == 0) {
    // 玉のまわり 3〜8 ドットから粒を 6 フレームおきに落とす (最後は玉の真下)
    count = 10;
    if ((s8)work->highArc == 0)
      count = 24;
    for (i = 0; i < count; i++) {
      pos.x = orb->x;
      pos.y = orb->y;
      pos.z = orb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
      if (i == count - 1) {
        WaitFrames(25);
        pos.x = orb->x;
        pos.y = orb->y;
        pos.z = orb->z;
      }
      mote = Object_Spawn(0xf0, pos.x, pos.y, pos.z);
      if (mote != 0) {
        mote->altitude = pos.y - (32 << 16);
        mote->callback = (void *)BattleFx_SpawnRandomAngleTriplet;
        mote->flag = 2;
      }
      AudioCommand_PlayFar(132);
      WaitFrames(6);
    }
    WaitFrames(10);
  } else {
    // 玉のまわり 3〜8 ドットから、ゆれて落ちる粒を 6 フレームおきに出す
    if ((s8)work->highArc != 0)
      count = 10;
    else
      count = 30;
    for (i = 0; i < count; i++) {
      pos.x = orb->x;
      pos.y = orb->y;
      pos.z = orb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
      mote = Object_Spawn(0x11c, pos.x, pos.y, pos.z);
      if (mote != 0) {
        mote->callback = (void *)BattleFx_UpdateDriftingFallObject;
        mote->flag = 0;
        mote->sprite->mode = 2;
        ObjectDispatch_ApplyArgumentToChildrenFar(mote, 8);
        Animation_ApplyChildValuesFar(mote, 7);
      }
      WaitFrames(6);
    }
    WaitFrames(70);
  }

  // 等倍から 1/4 まで縮めながら、対象の上へ戻す
  i = 0;
  do {
    s32 scale;

    {
      struct EffectVector *a = &end;
      struct EffectVector *b = &start;
      s32 e, t;

      e = b->x;
      t = a->x;
      t += i * (e - t) / 10;
      orb->x = t;
      e = b->y;
      t = a->y;
      t += i * (e - t) / 10;
      orb->y = t;
      e = b->z;
      t = a->z;
      t += i * (e - t) / 10;
      orb->z = t;
    }
    scale = Lerp10(0x10000, 0x4000, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    WaitFrames(1);
    i++;
  } while (i < steps);
  ObjectDispatch_ReleaseFar(orb);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// particles.c
// バトル効果：持ち上げた物のまわりに落ちる粒
//--------------------------------------------------------------------------

struct FallScene {
  u8 reserved00[20];
  struct FieldActor *child; // 持ち上げている物
};

LAYOUT_SIZE_GUARD(FallPos_Size, struct EffectVector, 0x0c);
LAYOUT_OFFSET_GUARD(FallScene_Child, struct FallScene, child, 0x14);

//--------------------------------------------------------------------------
// 毎フレームの処理：持ち上げている物のまわりに、落ちる粒をひとつ出す
// 高さは物から 8〜24 ドット上、横はでたらめな向きに 0〜48 ドット
//--------------------------------------------------------------------------
void BattleFx_SpawnFallingParticles(void) {
  struct FieldActor *src;
  struct EffectVector pos;
  struct ItemBreakFragmentObject *obj;
  u32 dist;

  src = ((struct FallScene *)gEffectWork)->child;
  pos.x = src->x.fixed;
  pos.y = src->y.fixed - Random16() * 16 + (24 << 16);
  pos.z = src->z.fixed;

  dist = Random16() * 3;
  Vector_AddPolarOffset(dist * 16, Random16(), &pos);

  obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->motionFlags = ACTOR_FALLS;
    obj->gravity = 0x1999; // 重力 0.1 ドット
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
    obj->unk5e = 12;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }
}

//--------------------------------------------------------------------------
// lift_drop.c
// 対象を 2 つの持ち手で持ち上げ、方向キーで動かして落とす
//--------------------------------------------------------------------------

#define SHARD_MAX 20 // 破片の数

//--------------------------------------------------------------------------
// 足もとを 16 ドット上げて、pos へ動いたときにぶつかるかを調べる
//--------------------------------------------------------------------------
static __inline__ s32 ObjProbeUp(struct FieldActor *obj,
                                 struct EffectVector *pos) {
  s32 hit;

  obj->terrainHeight += 16 << 16;
  hit = Map_CheckObjectStepFar(obj, pos);
  obj->terrainHeight -= 16 << 16;
  return hit;
}

//--------------------------------------------------------------------------
// 対象を左右の持ち手で 24 ドットの高さまで持ち上げ、方向キーの向きへ
// 16 ドット動かして落とす (A・B・L・R ならその場で落とす)
// 行き先がふさがっていれば、ブザーを鳴らして待つ
//--------------------------------------------------------------------------
void BattleFx_SteerLiftedTarget(s32 id) {
  struct BattleFxScene *work;
  struct FieldActor *src;
  struct FieldActor *target;
  struct FieldActor *shards[SHARD_MAX];
  struct EffectVector pos;
  struct FieldActor *hands[2];
  struct FieldActor *obj;
  struct FieldActor *right;
  s32 i;
  s32 angle;
  s32 dist;
  struct EffectVector *p;

  work = gEffectWork;
  src = work->mainObject;
  target = work->child;
  if (target == NULL)
    return;
  BattleEffect_InitializeSharedScene();
  src->linkedObject = target;
  ObjectDispatch_InitializeFar((struct DispatchObject *)src,
                               gBattleFx_SourceHoldScript);

  // 持ち手を左右 32 ドットに出し、対象の左右 16 ドットへ動かす
  /* FAKEMATCH: the position is reached through this pointer everywhere
   * but in the steering loop's first part, which names the position
   * itself. Measured: the position named everywhere differs 10; the
   * pointer everywhere differs 561 (514 instructions for 531). */
  p = &pos;
  p->x = work->x;
  p->y = work->y + (16 << 16);
  p->z = work->z;
  obj = BattleFx_SpawnItemBreakMode1(p->x + (32 << 16), p->y, p->z, 0x8000);
  hands[0] = obj;
  right = BattleFx_SpawnItemBreakMode1(p->x - (32 << 16), p->y, p->z, 0);
  hands[1] = right;
  if (hands[0] == NULL || right == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }
  WaitFrames(15);
  p->x = target->x.fixed;
  p->y = target->y.fixed + (16 << 16);
  p->z = target->z.fixed;
  Object_SetMoveTargetFar(obj, p->x + (16 << 16), p->y, p->z);
  Object_SetMoveTargetFar(right, p->x - (16 << 16), p->y, p->z);
  Script_WaitForEventTimeoutFar(obj);
  Script_WaitForEventTimeoutFar(right);
  obj->x.fixed = p->x + (16 << 16);
  obj->velocityX = 0;
  right->x.fixed = p->x - (16 << 16);
  right->velocityX = 0;

  // 地面から 24 ドットの高さまで持ち上げる
  target->update = ObjectGroup_ApplyRandomChildValues;
  Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
  AudioCommand_PlayFar(130);
  target->motionFlags = 4;
  ObjectDispatch_SetSingleChildFlagsFar(target, 0);
  if (hands[0] != NULL && hands[1] != NULL) {
    while (target->y.fixed - target->terrainHeight <= 24 << 16) {
      obj->y.fixed += 0x6000;
      right->y.fixed += 0x6000;
      target->y.fixed += 0x6000;
      WaitFrames(1);
    }
  }
  hands[0]->speed = 4 << 16;
  hands[0]->acceleration = 0x8000;
  hands[1]->speed = 4 << 16;
  hands[1]->acceleration = 0x8000;
  target->speed = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = 0;
  target->terrainId = 2;

  //------------------------------------------------------------------
  // 方向キーで動かす
  //------------------------------------------------------------------
  for (;;) {
    WaitFrames(1);
    if ((gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
      break;

    // 方向キーなし : 持ち手を対象の左右に戻す
    angle = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
    if (angle == 0xffff) {
      pos.x = target->x.fixed;
      pos.y = target->y.fixed + (16 << 16);
      pos.z = target->z.fixed;
      Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
      Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 1);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 1);
      continue;
    }

    // 持ち手をその向きへ 2 ドット寄せ、16 ドット先の地面を調べる
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + (16 << 16);
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(2 << 16, angle, &pos);
    Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
    Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
    Script_WaitForEventTimeoutFar(hands[0]);
    Script_WaitForEventTimeoutFar(hands[1]);
    pos.x = target->x.fixed;
    pos.y = target->terrainHeight;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &pos);
    i = ScriptObject_CheckOverlapFar(target, &pos);
    /* FAKEMATCH: entering the blocked path from both tests keeps it
     * ahead of the drop path, as the ROM lays them out. Measured: the two
     * tests as one || differ 407 (536 instructions for 531, the
     * allocation changing from the prologue on), under for (;;) or
     * while (1). */
    if (i != 0)
      goto blocked;
    if (ObjProbeUp(target, p) > 0) {
    blocked: // ふさがっている : 16 フレームごとにブザー
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
      if ((gFrameCount & 15) == 0)
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      continue;
    }
    // 空いている : 対象をそこへ動かす
    {
      struct FieldActor *second;
      s32 x;
      s32 z;

      AudioCommand_PlayFar(175);
      x = p->x;
      z = p->z;
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
      WaitFrames(15);
      target->paused = i;
      target->speed = 0x3333;
      target->acceleration = 0x3333;
      Object_SetMoveTargetFar(target, p->x, p->y, p->z);
      hands[0]->speed = 0x3333;
      hands[0]->acceleration = 0x3333;
      second = hands[1];
      second->speed = 0x3333;
      second->acceleration = 0x3333;
      Object_SetMoveTargetFar(hands[0], p->x + (16 << 16), p->y, p->z);
      Object_SetMoveTargetFar(second, p->x - (16 << 16), p->y, p->z);
      Script_WaitForEventTimeoutFar(target);
      target->x.fixed = x;
      target->z.fixed = z;
      target->velocityX = i;
      target->velocityZ = i;
      WaitFrames(10);
      break;
    }
  }

  //------------------------------------------------------------------
  // 対象の位置から破片を飛ばして持ち手を消し、対象を跳ねさせて落とす
  //------------------------------------------------------------------
  ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
  ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
  Scheduler_RemoveCallback(BattleFx_SpawnFallingParticles);
  AudioCommand_PlayFar(135);
  WaitFrames(15);
  AudioCommand_PlayFar(135);
  WaitFrames(15);
  p->x = target->x.fixed;
  p->y = target->y.fixed + (16 << 16);
  p->z = target->z.fixed;
  {
    struct FieldActor **walk = shards;

    // 破片 : 速さ 2〜3 ドット、8〜32 ドット先のでたらめな方向へ
    for (i = 0; i < SHARD_MAX; i++) {
      obj = Object_Spawn(0x11d, p->x, p->y, p->z);
      *walk++ = obj;
      if (obj != NULL) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                     gBattleFx_FragmentScript);
        obj->speed = Random16() + (2 << 16);
        obj->acceleration = 2 << 16;
        obj->motionFlags = 0;
        dist = Random16() * 24 + (8 << 16);
        Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
      }
    }
  }
  AudioCommand_PlayFar(131);
  ObjectDispatch_ReleaseFar(hands[0]);
  ObjectDispatch_ReleaseFar(hands[1]);

  // 対象の見た目と動きを戻し、上へ 10 ドットの速さで跳ねさせる
  Animation_ApplyChildValuesFar(target, work->savedPalette);
  ObjectDispatch_InitializeFar((struct DispatchObject *)target,
                               work->savedScript);
  target->update = work->savedCallback;
  target->motionFlags = 3;
  target->velocityY = 10 << 16;
  target->bounce = 0x3333;
  target->terrainId = 0;
  src->update = 0;
  Animation_ApplyChildValuesFar(src, 0);
  if (work->childOption != 0) {
    // 上がりきって落ちきるまで (それぞれ最長 90 フレーム) 待ち、輪を出す
    for (i = 0; i < 90 && target->velocityY >= 0; i++)
      WaitFrames(1);
    WaitFrames(1);
    for (i = 0; i < 90 && target->velocityY < 0; i++)
      WaitFrames(1);
    BattleFx_SpawnRadialParticleRing(target);
    BattleFx_PrepareBufferInterpolation();
    WaitFrames(30);
  } else {
    BattleFx_PrepareBufferInterpolation();
  }
}

DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect11, BattleFx_SpawnItemBreakMode1)

struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z,
                                                  s32 angle) {
  struct MotionObject *obj;
  s32 scale;

  AudioCommand_PlayFar(SOUND_ITEM_BREAK);
  obj = Object_Spawn(215, x, y, z);
  if (obj != NULL) {
    obj->scaleY = 0x4000;
    obj->scaleX = 0x4000;
    obj->speedLimit = 0x30000;
    obj->acceleration = 0x30000;
    obj->autoFaceMotion = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
    scale = obj->scaleX;
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        obj->scaleY = scale;
        obj->scaleX = scale;
        obj->angle += 0x2000;
        WaitFrames(1);
        scale = obj->scaleX;
      } while (scale <= 0xffff);
    }
    obj->angle = angle;
  }
  return obj;
}

// FieldActor と同じ物体。0x64 を符号なし、0x66 を符号つきで読む (FieldActor
// の型ではバイトが変わる)
struct OrbitEffect {
  u8 unk00[6];
  u16 angle; // 進む向き・回る角度
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;                  // 経過フレーム
  s16 pause;                  // 向きを大きく変える残りフレーム
  struct OrbitEffect *anchor; // 回る中心
  void (*update)(struct OrbitEffect *);
};

// ここに共通ソースが入る（ROM の並びのとおり）
#define EFFECT_SPAWN_RESIDENT
#include "../scene/common/effect_spawn.h"
#undef EFFECT_SPAWN_RESIDENT

//--------------------------------------------------------------------------
// 粒子の毎フレーム処理
// 速度で進み、x と z の速度を少しずつ落とし、拡大率と回転を変える
//--------------------------------------------------------------------------
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;

  effect->x += effect->velocityX;
  effect->y += effect->velocityY;
  effect->z += effect->velocityZ;
  effect->velocityX -= effect->velocityX / 18;
  effect->velocityZ -= effect->velocityZ / 16;
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

//--------------------------------------------------------------------------
// origin の位置から、水平に輪を描いて広がる粒子を 17 個出す
// x 方向の速度は z 方向の 1.5 倍
//--------------------------------------------------------------------------
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions opt;
  s32 vel[3];
  s32 vz;
  s32 angle;
  u32 i;

  opt.palette = 0;
  opt.update = BattleFx_UpdateParticleMotionAndScale;
  opt.startScaleX = 0xcccc; // 0.8 倍
  opt.startScaleY = 0xcccc;
  i = 0;
  do {
    angle = i << 12; // 1/16 周ずつ
    vel[0] = Trig_Cos(angle) * 3 / 2;
    vel[1] = 0;
    vz = Trig_Sin(angle);
    vel[2] = vz;
    Effect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1], vz,
                         0x01090001, &opt);
    i++;
  } while (i <= 16);
}

//--------------------------------------------------------------------------
// 中心を離れてさまよう（毎フレーム）
// 1 フレームに 2〜3 ドット進み、ときどき向きを大きく変える
// 101 フレームで共通の粒子スクリプトへ移る
//--------------------------------------------------------------------------
void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect) {
  s32 len;
  s32 angle;
  s32 dx;
  s32 dz;

  len = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(len, Trig_Cos(angle));
  dz = Iwram_MulQ16(len, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) // 1/32 の確率
  {
    effect->pause = ((Random16() << 4) >> 16) + 8; // 8〜23 フレーム
  }
  if (++effect->timer == 101)
    ObjectDispatch_InitializeFar((struct DispatchObject *)effect,
                                 gBattleFx_CommonParticleScript);
}

//--------------------------------------------------------------------------
// 中心のまわりを半径 8 ドットで回る（毎フレーム）
// 121 フレーム回ったら、さまよう動きに切り替える
//--------------------------------------------------------------------------
void BattleFx_CircleAnchor(struct OrbitEffect *effect) {
  struct OrbitEffect *anchor;
  s32 angle;
  s32 r;
  s32 dx;
  s32 dz;

  anchor = effect->anchor;
  r = 0x80000;
  angle = effect->angle;
  dx = Iwram_MulQ16(r, Trig_Cos(angle));
  dz = Iwram_MulQ16(r, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->angle += 0x800;
  if (++effect->timer == 121) {
    effect->update = BattleFx_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = 0x30000;
    effect->angle = Random16();
  }
}

//--------------------------------------------------------------------------
// 毎フレーム縮め、0.5 倍を切ったら位置を戻して処理を外す
//--------------------------------------------------------------------------
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj) {
  s32 scale;

  scale = obj->scaleX - 0x80;
  obj->scaleY = scale;
  obj->scaleX = scale;
  if (scale < 0x8000) {
    Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
    ((struct ScriptMotionObject *)obj)->hook = 0;
  }
}

void BattleFx_CallEffect14(s32 unused) { RunBattleEffect14(); }

//--------------------------------------------------------------------------
// radial.c
// フィールドのエナジー演出：14 番（運ばれる玉と、まわりを回る粒）ほか
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// エナジー 14 番 : 玉を使う者から scene の位置（4 ドット上）まで 10
// フレームで運び、 0.25 倍から大きくする。scene->enabled
// がなければ、かけられた物を縮め、 玉のまわりに 6 フレームおきに粒を 16
// 個出して回らせる
//--------------------------------------------------------------------------
void RunBattleEffect14(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *src = scene->mainObject;
  struct FieldActor *target = scene->child;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  struct FieldActor *ball;
  s32 step;
  struct FieldActor *particle;
  s32 steps = 11;

  step = 0;
  from.x = src->x.fixed;
  from.y = src->y.fixed;
  from.z = src->z.fixed;
  to.x = scene->x;
  to.y = scene->y - (4 << 16);
  to.z = scene->z;
  ball = Object_Spawn(0xda, 0, 0, 0);
  if (ball == 0) {
    return;
  }
  BattleEffect_InitializeSharedScene();
  ObjectDispatch_ApplyArgumentToChildrenFar(ball, 2);
  do {
    s32 scale;
    ball->x.fixed = from.x + step * (to.x - from.x) / 10;
    ball->y.fixed = from.y + step * (to.y - from.y) / 10;
    ball->z.fixed = from.z + step * (to.z - from.z) / 10;
    scale = 0x4000 + step * 0x10ccc / 10; // 0.25 倍から 1.3 倍へ
    ball->scaleX = scale;
    ball->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ball->scaleX = 0x1b333; // 横 1.7 倍
  ball->scaleY = 0x14ccc; // 縦 1.3 倍
  AudioCommand_PlayFar(163);
  WaitFrames(20);
  if (scene->enabled == 0) {
    if (target != 0)
      target->update = BattleFx_ShrinkObjectScaleUntilHalf;

    // 粒は玉の 4 ドット上から 0.8 ドットずつ高く、3〜8 ドット離して出す
    step = 0;
    do {
      s32 dist;

      pos.x = ball->x.fixed;
      pos.y = ball->y.fixed + step * 0xcccc + (4 << 16);
      pos.z = ball->z.fixed;
      dist = Random16() * 5 + (3 << 16);
      Vector_AddPolarOffset(dist, Random16(), &pos);
      particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->update = BattleFx_CircleAnchor;
        particle->linkedObject = ball;
        particle->timer = 0;
        particle->work = 0;
        particle->facing = Random16();
      }
      WaitFrames(6);
      step++;
    } while (step <= 15);
    WaitFrames(20);
    WaitFrames(120);
  }
  ObjectDispatch_ApplyArgumentToChildrenFar(ball, 1);
  WaitFrames(30);
  AudioCommand_PlayFar(136);
  WaitFrames(20);
  ObjectDispatch_ReleaseFar(ball);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// スロットの粒の動き（毎フレーム）
// state 0 : 出た所から 40〜70 ドット離れた所へ向かう
// state 1 : 着くまで待ち、着いたら state 2 へ
// state 2 : scene の位置の 8 ドット上（画面の位置）のまわり 4 ドットへ向かう
// state 3 : 着いたらスロットを空ける
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect) {
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *st;
  s16 angle;
  s32 state;

  scene = gEffectWork;
  st = &effect->state;
top:
  state = *st;
  if (state == 0) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 4 << 16;
    effect->maxSpeed = 4 << 16;
    effect->flag42 = state;
    *st = *st + 1;
    return;
  }
  if (state == 1) {
    if (BattleFx_HasReachedTarget(effect) != 0)
      return;
    *st = *st + 1;
    goto top;
  }
  if (state == 2) {
    pos.x = scene->x;
    pos.y = scene->y + (8 << 16);
    pos.z = scene->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->flag42 = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
    BattleFx_ClearOwnedSlot(effect);
}

//--------------------------------------------------------------------------
// かけられた物（scene->child）に paused を立て、
// BattleEffect_RunFallbackObjectTransition の演出を動かす
//--------------------------------------------------------------------------
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused) {
  struct FieldActor *target = ((struct BattleFxScene *)gEffectWork)->child;

  target->paused = 1;
  BattleEffect_RunFallbackObjectTransition();
}

//--------------------------------------------------------------------------
// fallback.c
// フィールド効果：対象のない場所へ飛ばして散らす
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 使い手から効果の位置へオブジェクトを 10 フレームで飛ばし、16
// 個の粒にして散らす
//--------------------------------------------------------------------------
void BattleEffect_RunFallbackObjectTransition(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct MotionObject *user = scene->mainObject;
  struct MotionObject *obj;
  struct EffectSlot *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = user->y;
  obj = Object_Spawn(250, 0, 0, 0);
  step = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
  if (obj == 0)
    return;

  // 使い手の 16 ドット上から、効果の位置の 8 ドット上へ
  BattleEffect_InitializeSharedScene();
  from.x = user->x;
  from.y = user->y + (16 << 16);
  from.z = user->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
  for (;;) {
    s32 steps = 11;
    s32 scale;

    obj->x = from.x + (to.x - from.x) * step / 10;
    obj->y = from.y + (to.y - from.y) * step / 10;
    obj->z = from.z + (to.z - from.z) * step / 10;
    scale = BattleFx_Interpolate(0x10000, 0x4000, step); // 0.25 倍から等倍へ
    obj->scaleX = scale;
    obj->scaleY = scale;
    WaitFrames(1);
    step++;
    if (step >= steps)
      break;
  }

  WaitFrames(5);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_HIGH);

  // その周り 4 ドットに粒を置き、外へ飛ばしてから効果の位置へ戻す
  slot = &scene->slots[0];
  for (i = 0; i < 16; i++) {
    pos.x = obj->x;
    pos.y = obj->y + (8 << 16);
    pos.z = obj->z;
    Camera_WorldToScreen(&pos.x);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
    EffectSlot_Initialize(slot, 285, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 7);
    slot++;
  }

  pos.x = obj->x;
  pos.y = obj->y + (8 << 16);
  pos.z = obj->z;
  WaitFrames(8);
  ObjectDispatch_ReleaseFar(obj);
  WaitFrames(4);
  WaitFrames(30);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// object_pause.c
// オブジェクトを止めて表の値をでたらめに当て続け、もとの処理に戻す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 止めているあいだの毎フレームの処理 : 表の 8 つからでたらめに選んで当てる
//--------------------------------------------------------------------------
void BattleEffect_SetRandomTableValueOnObject(void *obj) {
  s8 *table = gBattleFx_RandomChildValues;
  s32 r = Random16();
  Animation_ApplyChildValuesFar(obj, table[(u32)(r * 8) >> 16]);
}

//--------------------------------------------------------------------------
// id のオブジェクトを止める
// 毎フレームの処理と、動いていれば最初のアニメーションの param を
// gGameState に覚えておく
//--------------------------------------------------------------------------
void BattleEffect_PauseObject(s32 id) {
  struct FieldActor *obj;
  struct AnimationEntry *entry;

  obj = ObjectTable_Get(id);
  if (obj != NULL) {
    gGameState.pausedUpdate = (u32)obj->update;
    gGameState.pausedPose = 0;
    if (obj->active == 1) {
      entry = ((struct AnimationObject *)obj->sprite)->entries[0];
      if (entry != NULL) {
        gGameState.pausedPose = entry->param;
      }
    }
    obj->update =
        (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject;
    obj->paused = 1;
    ObjectDispatch_ApplyValueToChildrenFar(obj, 0);
  }
}

//--------------------------------------------------------------------------
// id のオブジェクトを、覚えておいた毎フレームの処理と param に戻す
//--------------------------------------------------------------------------
void BattleFx_ResumeObject(s32 id) {
  struct FieldActor *obj = ObjectTable_Get(id);
  if (obj != 0) {
    if (obj->update == (void (*)(union FieldObject *))
                           BattleEffect_SetRandomTableValueOnObject) {
      struct GameState *state = &gGameState;
      obj->update = (void (*)(union FieldObject *))state->pausedUpdate;
      state->pausedUpdate = 0;
      Animation_ApplyChildValuesFar(obj, state->pausedPose);
    }
    obj->paused = 0;
    ObjectDispatch_ApplyValueToChildrenFar(obj, 16);
  }
}

//--------------------------------------------------------------------------
// 今の演出定義の flags が 0xff なら、対象なし (-1) を返す
// id : 対象オブジェクト番号
//--------------------------------------------------------------------------
s32 BattleFx_FilterObjectIdByFlags(s32 id) {
  if (BattleFx_GetFlags(id) == 0xff)
    return -1;
  return id;
}

//--------------------------------------------------------------------------
// run_effect13_hook.c
// エナジー演出 13：オブジェクト 215 を目標まで伸ばし、押し下げて戻す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 対象への演出 (BattleFx_DispatchRequestKind から)
//--------------------------------------------------------------------------
void BattleFx_RunEffect13Hook(s32 unused) { RunBattleEffect13(); }

//--------------------------------------------------------------------------
// オブジェクト 215 を使う者の 16 ドット上から目標へ 10 段で運びながら
// 1/4 倍から等倍にし、20 ドット押し下げて child を (-9, -9) へ移し、
// 19.5 ドット上げてから縮めて戻す
// 目標は scene->enabled なら使う者の前 32 ドット、ほかは scene の位置で、
// どちらも 32 ドット上
//--------------------------------------------------------------------------
void RunBattleEffect13(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct MotionObject *actor = scene->mainObject;
  struct MotionObject *child = scene->child;
  struct MotionObject *object;
  struct FixedPointPosition spawn;
  struct FixedPointPosition origin;
  struct FixedPointPosition target;
  s32 step;
  s32 i;
  s32 steps = 11;

  origin.x = actor->x;
  origin.y = actor->y + (16 << 16);
  origin.z = actor->z;
  if (scene->enabled) {
    target.x = actor->x;
    target.y = actor->y + (32 << 16);
    target.z = actor->z;
    Vector_AddPolarOffset(32 << 16, scene->angle, &target.x);
  } else {
    target.x = scene->x;
    target.y = scene->y + (32 << 16);
    target.z = scene->z;
  }
  spawn.x = scene->x;
  spawn.y = scene->y + (32 << 16);
  spawn.z = scene->z;
  object = Object_Spawn(215, spawn.x, spawn.y, spawn.z);
  if (object == 0)
    return;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(138);
  object->angle = actor->angle;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
  Animation_ApplyChildValuesFar(object, 1);

  // 目標へ伸ばす
  step = 0;
  do {
    s32 scale;

    object->x = origin.x + (target.x - origin.x) * step / 10;
    object->y = origin.y + (target.y - origin.y) * step / 10;
    object->z = origin.z + (target.z - origin.z) * step / 10;
    scale = BattleFx_Interpolate(0x10000, 0x4000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  WaitFrames(10);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
  WaitFrames(15);

  // 2 ドットずつ 10 フレーム押し下げる
  for (i = 0; i < 10; i++) {
    object->y -= 2 << 16;
    WaitFrames(1);
  }
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
  AudioCommand_PlayFar(132);
  if (child != 0)
    Object_SetPositionAndResetMotionFar(child, -(9 << 16), child->y,
                                        -(9 << 16));
  WaitFrames(20);

  // 1.5 ドットずつ 13 フレーム上げる
  for (i = 0; i < 13; i++) {
    object->y += 0x18000;
    WaitFrames(1);
  }
  WaitFrames(10);
  AudioCommand_PlayFar(114);

  // 使う者のところへ縮めて戻す
  step = 0;
  do {
    s32 scale;

    object->x = target.x + (origin.x - target.x) * step / 10;
    object->y = target.y + (origin.y - target.y) * step / 10;
    object->z = target.z + (origin.z - target.z) * step / 10;
    scale = BattleFx_Interpolate(0x4000, 0x10000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ObjectDispatch_ReleaseFar(object);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// launch.c
// 戦闘エフェクト 15 番：回って縮む主オブジェクトと、扇状に飛び出す粒
//--------------------------------------------------------------------------

#define SLOT_MAX 24 // エフェクト枠の数 (粒の数)

//--------------------------------------------------------------------------
// 毎フレームの更新 : 0x1000 + timer × 0x50 ずつ回し、timer × 0x50 が 0x1000 に
// 達するまで timer を増やして速める
//--------------------------------------------------------------------------
void BattleFx_UpdateSpinAngle(struct FieldActor *obj) {
  u32 step;

  step = obj->timer * 0x50;
  obj->facing = obj->facing + step + 0x1000;
  if (step < 0x1000)
    obj->timer = obj->timer + 1;
}

//--------------------------------------------------------------------------
// 毎フレームの更新 : 45 度ずつ回りながら縮み、y を 1 ドットずつ増やす
// 拡大率が 0x3000 を下回ったら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDescendingObject(struct FieldActor *obj) {
  u32 scale;

  obj->scaleY -= 0x280;
  scale = obj->scaleX - 0x280;
  obj->facing += 0x2000;
  obj->y.fixed += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000)
    obj->active = 0;
}

//--------------------------------------------------------------------------
// 粒の動き : 主オブジェクトの画面位置 (z + 8 ドット) から、0xc000 の向きの ±45
// 度へ 120 ドット飛ぶ。状態 1 は着いたら飛びなおし、状態 2 は着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot) {
  struct EffectVector pos;
  struct FieldActor *src;
  s32 state;
  u32 r;

  src = ((struct BattleFxScene *)gEffectWork)->mainObject;
  state = slot->state;

  if (state == 0) {
    pos.x = src->x.fixed;
    pos.y = src->y.fixed;
    pos.z = src->z.fixed;
    Camera_WorldToScreen(&pos);

    slot->x = pos.x;
    slot->z = pos.z + (8 << 16);
    slot->originZ = slot->z;
    slot->originX = slot->x;
    pos.z = slot->originZ;
    pos.x = slot->originX;

    r = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 5 << 16;
    slot->maxSpeed = 5 << 16;
    slot->flag42 = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      AudioCommand_PlayFar(246);
  } else if (state == 1) {
    if (BattleFx_HasReachedTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (BattleFx_HasReachedTarget(slot) == 0)
      BattleFx_ClearOwnedSlot(slot);
  }
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 15 番
// 主オブジェクトを回して縮めながら、粒を 1 フレームに 1 つずつ 24 個飛ばす
//--------------------------------------------------------------------------
void BattleFx_RunEffect15(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *obj = scene->mainObject;
  struct EffectVector pos;
  struct EffectSlot *slot;
  u8 last;
  u32 i;

  Battle_Reset();
  Motion_CamBounds(-1, -1, -1, 0);
  BattleEffect_InitializeSharedScene();
  WaitFrames(10);
  ObjectMotion_ArmCallback(scene->firstObjectId, 0x4000, 0);
  WaitFrames(30);
  obj->update =
      (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
  AudioCommand_PlayFar(131);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 28);
  WaitFrames(40);

  // 回りはじめる
  AudioCommand_PlayFar(220);
  Animation_ApplyChildValuesFar(obj, 0);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
  obj->update = (void (*)(union FieldObject *))BattleFx_UpdateSpinAngle;
  obj->timer = 0;
  WaitFrames(70);

  // 縮みはじめ、その画面位置から粒を飛ばす
  ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
  obj->motionFlags = 0;
  obj->update = (void (*)(union FieldObject *))BattleFx_UpdateDescendingObject;
  obj->targetX = ACTOR_NO_TARGET;
  pos.x = obj->x.fixed;
  pos.y = obj->y.fixed;
  pos.z = obj->z.fixed;
  Camera_WorldToScreen(&pos);
  i = 0;
  slot = scene->slots;
  do {
    s32 scale;
    EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object,
                                              (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333; // 1.2〜1.7 倍
    slot->scaleY = scale;
    slot->scaleX = scale;
    i++;
    WaitFrames(1);
    slot++;
  } while (i < SLOT_MAX);
  WaitFrames(70);

  // 飛んでいる粒を、着いたら消える状態にする
  i = 0;
  last = 2;
  do {
    if (scene->slots[i].active != 0)
      scene->slots[i].state = last;
    i++;
  } while (i < SLOT_MAX);
  WaitFrames(40);
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(10);
}

void BattleFx_UpdateDescendingParticlePositiveArc(struct FieldActor *particle) {
  struct FieldActor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.map == (s32)&SceneId_Clear)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleFx_UpdateDescendingParticleNegativeArc(struct FieldActor *particle) {
  struct FieldActor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.map == (s32)&SceneId_Clear)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed - ((0x10000 - scale) * 5) + 0x100000;
}

struct DescendingArcSprite {
  u8 padding00[5];
  u8 flags05_0 : 5;
  u8 flags05_5 : 1;
  u8 layer : 2;
  u8 padding06;
  u8 flags07_0 : 6;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10;
      u16 flags08_10 : 6;
    } half;
    struct {
      u8 tileLow;
      u8 flags09_0 : 2;
      u8 variant : 2;
      u8 flags09_4 : 4;
    } byte;
  } attr;
  u8 padding0c[16];
  u8 resource;
  u8 active : 1;
  u8 flags1d1 : 7;
  u8 padding1e[8];
  u8 animation;
  u8 padding27;
  struct AnimationEntry *frame;
};

void BattleFx_SpawnDescendingArcParticles(struct FieldActor *arc) {
  struct FieldActor *source = arc;
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *leader = scene->mainObject;
  struct FieldActor *objects[2];
  struct FieldActor *object;
  struct DescendingArcSprite *sprite;
  s32 i;
  u8 zero;

  for (i = 0; i <= 1; i++) {
    object = Object_CreateFar(26, source->x.fixed, source->y.fixed + 0x400000,
                              source->z.fixed);
    objects[i] = object;
    if (object == NULL)
      continue;
    object->terrainHeight = source->terrainHeight;
    sprite = object->sprite;
    object->motionFlags = 0;
    object->timer = 0;
    zero = 0;
    object->linkedObject = source;
    object->scaleY = 0x6666;
    object->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    AnimationObjects_SelectAnimationFar(sprite, 0);
    sprite->animation = zero;
    Resource_ResetEntry(sprite->resource);
    sprite->resource = scene->arcTileSlot;
    sprite->active = 1;
    sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
    sprite->flags05_5 = 0;
    sprite->layer = 1;
    sprite->size = 2;
    sprite->frame->frame = zero;
  }
  objects[0]->update = (void *)BattleFx_UpdateDescendingParticleNegativeArc;
  ((struct DescendingArcSprite *)objects[0]->sprite)->attr.byte.variant = 0;
  objects[1]->update = (void *)BattleFx_UpdateDescendingParticlePositiveArc;
  ((struct DescendingArcSprite *)objects[1]->sprite)->attr.byte.variant =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.variant;
}

//--------------------------------------------------------------------------
// arcstate.c
// エナジー演出 16 (プルーフ)：使う者の動きと、効果が切れたときのメッセージ
//--------------------------------------------------------------------------

#define PSYNERGY_AVOID 150      // プルーフ
#define ITEM_SACRED_FEATHER 236 // せいなるはね

//--------------------------------------------------------------------------
// 演出の終わり：使う者の表示の flags を 5 フレームの周期で 3 と 1 に切り替える
//--------------------------------------------------------------------------
void BattleFx_UpdateEffect16State(void) {
  struct ArcEffectObject *obj;
  struct AnimationObject *rec;
  struct AnimationEntry *entry;
  s32 phase;

  obj = ObjectTable_Get(gGameState.selectedActor);
  rec = obj->records;
  entry = rec->entries[0];
  phase = gFrameCount % 5;
  if (phase == 0) {
    rec->dirty = 1;
    rec->flags = 3;
  } else if (phase == 2) {
    entry->param = 0; // 色を戻す
    rec->dirty = 1;
    rec->flags = 1;
  }
}

//--------------------------------------------------------------------------
// 演出中の使う者：5 フレームごと (ワールドマップでは 7) に降る弧の粒を出し、
// phase 1 では回る
//--------------------------------------------------------------------------
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *obj) {
  s16 phase;
  s32 cnt;

  phase = obj->phase;
  cnt = obj->step++;

  if (gGameState.map == (s32)&SceneId_Clear) {
    if (cnt % 7 == 0)
      BattleFx_SpawnDescendingArcParticles(obj);
  } else if (cnt % 5 == 0) {
    BattleFx_SpawnDescendingArcParticles(obj);
  }

  if (phase == 1)
    obj->angle += 0xc00;
}

//--------------------------------------------------------------------------
// プルーフ (せいなるはね) の効果が切れた
//--------------------------------------------------------------------------
void FieldEvent_ShowStatusMessage(void) {
  gGameState.avoidDistance = 0;
  if (gGameState.avoidByItem == 0) {
    UiWork_PushValueSlotFar(PSYNERGY_AVOID, 4);
    UiText_ShowPositionedMessageAndWaitFar(MsgAbilityWoreOff, 1);
    return;
  }
  UiWork_PushValueSlotFar(ITEM_SACRED_FEATHER, 2);
  UiText_ShowPositionedMessageAndWaitFar(MsgItemWoreOff, 1);
}

void RunBattleEffect16(void) {
  struct BattleFxScene *scene;
  struct ArcEffectObject *object;
  struct AnimationObject *group;
  struct AnimationEntry *entry;
  u32 angle;
  s32 slot;
  s32 count;

  scene = gEffectWork;
  object = scene->mainObject;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  slot = Resource_FindFreeEntry();
  {
    s16 *tileSlot = &scene->arcTileSlot;
    s32 zero = 0;

    *tileSlot = slot;
    VramBlock_LoadCached((s16)slot, 0x100, gBattleFx_ArcSparkTiles);
    gGameState.avoidDistance = 0x09600000;
    gGameState.avoidByItem = GameFlag_TestFar(0x145);
    Animation_ApplyChildValuesFar(object, zero);
    object->callback = BattleFx_UpdatePairedArcSpawner;
    object->phase = zero;
    object->step = zero;
  }
  AudioCommand_PlayFar(0x8c);
  WaitFrames(15);
  object->phase = 1;
  WaitFrames(10);
  for (count = 0; count < 20; count++) {
    entry->param = 7;
    group->dirty = 1;
    WaitFrames(2);
    group->dirty = 1;
    entry->param = 0;
    group->flags = 1;
    WaitFrames(3);
  }
  object->callback = NULL;
  object->angle = angle;
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
  WaitFrames(15);
  AudioCommand_PlayFar(0xae);
  WaitFrames(55);
  Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
  if (gGameState.cloaked != 0)
    ObjectDispatch_SetSingleChildFlagsFar(object, 2);
  else
    ObjectDispatch_SetSingleChildFlagsFar(object, 1);
  Animation_ApplyChildValuesFar(object, 0);
  Resource_ResetEntry(scene->arcTileSlot);
  UiText_ShowPositionedMessageAndWaitFar(MsgMonstersAttackLess, 1);
}

void EffectSlot_Update(struct EffectSlot *effect) {
  if (effect->active != 0) {
    effect->age++;
    if (effect->callbackDelay != 0)
      effect->callbackDelay--;
    else if (effect->callback != 0)
      effect->callback(effect);
    if (effect->active != 0) {
      if (effect->updateMotion != 0)
        EffectSlot_UpdateMotion(effect);
      if (effect->render != 0)
        BattleFx_DrawScaledObject(effect);
    }
  }
}

void BattleFx_DrawScaledObject(struct EffectSlot *object) {
  struct AnimationObject *sprite;
  s32 offset;
  s32 scale[2];
  s32 position[4];

  sprite = object->object;
  offset = 0;
  if (object->flags & 4)
    offset = 0x1fc0000 - object->z;
  scale[0] = Iwram_MulQ16(object->scaleX, sprite->scale);
  scale[1] = Iwram_MulQ16(object->scaleY, sprite->scale);
  position[0] = object->x;
  position[1] = offset;
  position[2] = object->z + offset;
  position[3] = 0;
  if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000 &&
      object->z < 0xe00000)
    Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void EffectSlot_UpdateMotion(struct EffectSlot *effect) {
  s32 dx;
  s32 dz;
  s32 distance;
  s32 ix;
  s32 iz;
  s32 speed;
  s16 angle;
  s32 heading;
  s32 facing;
  s16 turn;

  if (effect->targetX == EFFECT_NO_TARGET)
    return;
  dx = effect->targetX - effect->x;
  dz = effect->targetZ - effect->z;
  if (effect->stopAtTarget != 0) {
    ix = dx / 0x10000;
    iz = dz / 0x10000;
    distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
    if (distance < 0x800000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    if (distance <= 0x80000) {
      EffectSlot_SetPosition(effect, effect->targetX, effect->targetZ);
      return;
    }
  }
  angle = ArcTan2(dz, dx);
  if (effect->flag42 != 0) {
    heading = effect->heading;
    turn = angle - heading;
    if ((turn >= 0 ? turn : -turn) >= effect->maxTurnStep) {
      if (turn < 0) {
        if (-turn > effect->maxTurnStep)
          turn = -effect->maxTurnStep;
      } else if (turn > effect->maxTurnStep) {
        turn = effect->maxTurnStep;
      }
      angle = turn + heading;
    }
  }
  facing = (u16)angle;
  effect->heading = facing;
  speed = effect->speed + effect->acceleration;
  if (speed > effect->maxSpeed)
    speed = effect->maxSpeed;
  angle = facing;
  effect->speed = speed;
  effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect) {
  u32 value;

  if (effect->stopAtTarget == 0) {
    return 0;
  }
  value = effect->targetX ^ 0x80000000;
  return ((0u - value) | value) >> 31;
}

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z) {
  effect->targetX = EFFECT_NO_TARGET;
  effect->targetZ = EFFECT_NO_TARGET;
  effect->x = x;
  effect->z = z;
  effect->speed = 0;
}

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode) {
  AnimationObjects_SelectAnimationFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect,
                            EffectCallback callback) {
  effect->callback = callback;
  effect->callbackDelay = 0;
  effect->age = 0;
  effect->state = 0;
}

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z) {
  s32 unit;
  s8 *object;

  Dma_Fill32(0, effect, sizeof *effect);
  object = ResourceObject_CreateFar(kind);
  effect->object = object;
  if (object != NULL)
    object[9] &= ~0x0c;
  EffectSlot_SetPosition(effect, x, z);
  /* FAKEMATCH: register choice only: 1.0 is held in a local set before
     maxSpeed, so it lives beside 2.0 and takes r2, which lets sched2
     load the object ahead of the three unit stores, as native. Plain
     forms measured (2026-10-13): the chain with the constant, three
     separate stores, or the chain before maxSpeed give 1.0 r3 and the
     reload after the stores (differ 12). */
  unit = 0x10000;
  effect->maxSpeed = 0x20000;
  effect->acceleration = effect->scaleY = effect->scaleX = unit;
  effect->originX = x;
  effect->originZ = z;
  ((s8 *)effect->object)[38] = 0;
  effect->stopAtTarget = 1;
  effect->flag42 = 1;
  effect->updateMotion = 1;
  effect->render = 1;
  effect->active = 1;
  effect->randomValue = Random16();
  effect->flags = 4;
  EffectSlot_SetObjectMode(effect, 1);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot) {
  if (slot->object)
    ResourceObject_ReleaseFar(slot->object);
  Dma_Fill32(0, slot, sizeof *slot);
}

// ワールドマップの作業領域（gMapCellBuffer に置く）
struct WorldMapWork {
  u16 vramBlock;           // 印の OBJ キャラクタの VRAM ブロック
  u16 windowSlot;          // もうひとつの VRAM ブロック（終わりに返す）
  union FieldCoordinate x; // カーソル（ドット）
  union FieldCoordinate z;
  u8 padding0c[6];
  s16 shown; // いま地名ウインドウに出している印（-1 はなし）
  u8 padding14[4];
  s32 speed; // カーソルの速さ
  s32 window;
  struct AnimationSpritePart markers[67];
};

void BattleFx_SetupResourcesAndWindow(void) {
  u32 *oam;
  u32 *pal;
  struct WorldMapWork *work;
  struct FieldActor *object;
  s32 attr;
  u32 i;
  u32 tile;
  u32 attr2;

  pal = Runtime_BumpAllocateAlternatePool(32);
  work = (struct WorldMapWork *)gMapCellBuffer;
  oam = (u32 *)work->markers;
  object = Object_GetById(gGameState.selectedActor);
  work->vramBlock = Resource_FindFreeEntry();
  Dma_Fill32(0, pal, 0x80);
  pal[0] = 255;
  pal[1] = 0x1ff;
  pal[2] = 272;
  pal[8] = 68;
  pal[9] = 324;
  pal[10] = 272;
  pal[16] = 119;
  pal[17] = 0x177;
  pal[18] = 272;
  pal[24] = 0xff0;
  pal[25] = 0xffff;
  pal[26] = 0x1ffff;
  pal[27] = 0x11ff0;
  pal[28] = 0x1100;
  attr = VramBlock_LoadCached(work->vramBlock, 128, pal) | 0x400;
  for (i = 0; i < 66; i++) {
    *oam++ = 0;
    *oam++ = 0;
    *oam++ = attr;
  }
  MemFree(pal);
  if (GameFlag_TestFar(284)) {
    work->x.fixed = 240 << 15;
    work->z.fixed = 160 << 15;
  } else {
    work->x.fixed = (((object->x.fixed - 0x10000000) >> 16) * 240 / 4096) << 16;
    work->z.fixed = ((object->z.part.pixel * 160) / 4096) << 16;
  }
  work->windowSlot = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramBlockCache[work->windowSlot].offset;
  work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
  work->shown = -1;
  work->speed = 0x10000;
  *oam++ = 0;
  *oam++ = 0x40000000;
  attr2 = 0x400;
  attr2 |= tile >> 5;
  *oam = attr2;
}

//--------------------------------------------------------------------------
// cleanup_resources_and_window.c
// ワールドマップ：地名の印・カーソル・地名ウインドウ
//--------------------------------------------------------------------------

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
// キーを離すとカーソルが近くの地名へ寄る
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 // 地名を出す距離の 2 乗
#define WORLD_MAP_NAME_LEFT 3   // 地名ウインドウを印から左へずらす量
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

#define FLAG_NOT_ON_MAP 284 // いまいる場所が地図にのっていない

//--------------------------------------------------------------------------
// ワールドマップを閉じる：VRAM ブロックを返し、地名ウインドウを消す
//--------------------------------------------------------------------------
void BattleFx_CleanupResourcesAndWindow(void) {
  struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

  Resource_ResetEntry(work->vramBlock);
  Resource_ResetEntry(work->windowSlot);
  UiWork_FinalizeFar(work->window, 2);
}

//--------------------------------------------------------------------------
// ワールドマップの毎フレーム処理（スケジューラのコールバック）
// フラグの立った場所に印を出し、十字キーでカーソルを動かし、
// カーソルにいちばん近い印の地名をウインドウに出す
// L・R を押している間はカーソルを現在地に合わせる
//--------------------------------------------------------------------------
void Map_UpdateWorldMapMarkers(void) {
  s32 leader;
  const s32 *place;
  s32 tileBase;
  s32 blend;
  s32 best;
  s32 bestDist;
  s32 cursorX;
  s32 bestX;
  s32 bestY;
  s32 message;
  s32 bestMessage;
  s32 height;
  s32 width;
  s32 pos[3];
  struct WorldMapWork *work;
  struct FieldActor *obj;
  s32 cursorY;
  s32 tile;
  s32 mode;
  s32 flag;
  s32 i;
  struct AnimationSpritePart *marker;
  s32 x;
  s32 y;
  s32 angle;

#if WORLD_MAP_CURSOR_SNAPS
  leader = gGameState.selectedActor;
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  marker = work->markers;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->vramBlock].offset >> 5;
#else
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  leader = gGameState.selectedActor;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->vramBlock].offset >> 5;
  marker = work->markers;
#endif
  best = -1;
  bestDist = WORLD_MAP_NAME_RANGE;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

  //------------------------------------------------------------------
  // カーソル
  //------------------------------------------------------------------
  if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS)) {
    // L・R：現在地
    obj = ObjectTable_Get(gGameState.selectedActor);
    if (obj != NULL) {
      cursorX = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
      cursorY = obj->z.part.pixel * 160 / 4096;
    }
  } else {
    // 十字キー：押し続けると速くなる
    angle = gWorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
    if (angle != 0xffff) {
      pos[0] = work->x.fixed;
      pos[1] = 0;
      pos[2] = work->z.fixed;
      Vector_AddPolarOffset(work->speed, angle, pos);
      if (pos[0] < 0x100000) // x は 16〜239 ドット
        pos[0] = 0x100000;
      if (pos[0] > 0xef0000)
        pos[0] = 0xef0000;
      if (pos[2] < 0) // y は 0〜141 ドット
        pos[2] = 0;
      if (pos[2] > 0x8d0000)
        pos[2] = 0x8d0000;
      work->x.fixed = pos[0];
      work->z.fixed = pos[2];
      if (work->speed < 0x60000)
        work->speed += 0x2000;
    } else {
      work->speed = 0x10000;
    }
    cursorX = work->x.part.pixel;
    cursorY = work->z.part.pixel;
  }

  //------------------------------------------------------------------
  // 印：0 番は現在地、11〜65 番は gWorldMap_PlaceMarkers の場所
  // 表は（フラグ, 地名）の組で、フラグの上位 16bit はキャラクタ番号
  //------------------------------------------------------------------
  for (i = 0; i <= 65; i++) {
    if (i != leader && i <= 10)
      continue;
    if (i == 0) {
      if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
        continue;
      tile = 0;
      flag = leader;
      mode = 0;
    } else {
      flag = *place++;
      message = *place++;
      if (flag == 0)
        continue;
#if WORLD_MAP_CURSOR_SNAPS
      if (flag == -1)
        break;
      tile = flag >> 16;
      mode = 1;
#else
      tile = flag >> 16;
      mode = 1;
      if (flag == -1)
        break;
#endif
    }
    if (!GameFlag_TestFar(flag))
      continue;
    obj = ObjectTable_Get(i);
    if (obj == NULL)
      continue;

    // ワールドの 4096 ドット四方を 240x160 に縮める
    x = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
    y = obj->z.part.pixel * 160 / 4096;
    marker->objectMode = mode;
    marker->tile = tileBase + tile;
    marker->x = x - 1;
    marker->y = y - 1;
    width = x - cursorX;
    height = y - cursorY;
    if (width * width + height * height < bestDist) {
      bestMessage = message;
      best = i;
      bestDist = width * width + height * height;
      bestX = x;
      bestY = y;
    }
    if (i != 0 || (gFrameCount & 15) <= 7) // 現在地は 8 フレームごとに点滅
      Runtime_PushSlotEntry(marker++, y);
  }
#if WORLD_MAP_CURSOR_SNAPS
  // キーを離していれば、カーソルをいちばん近い印へ寄せる
  if (gKeysHeld == 0) {
    if (best != -1) {
      if (bestDist <= 1) {
        work->x.fixed = bestX << 16;
        work->z.fixed = bestY << 16;
        work->speed = 0x10000;
      } else {
        if (work->speed < 0x20000)
          work->speed = 0x20000;
        angle = ArcTan2(bestY - cursorY, bestX - cursorX);
        pos[0] = work->x.fixed;
        pos[1] = 0;
        pos[2] = work->z.fixed;
        Vector_AddPolarOffset(work->speed, (u16)angle, pos);
        work->x.fixed = pos[0];
        work->z.fixed = pos[2];
        if (work->speed < 0x60000)
          work->speed += 0x2000;
      }
      cursorX = work->x.part.pixel;
      cursorY = work->z.part.pixel;
    } else {
      work->speed = 0x10000;
    }
  }
#endif

  // いちばん近い印を囲む枠（点滅）
  if (best != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + 3;
    marker->x = bestX - 2;
    marker->y = bestY - 2;
    Runtime_PushSlotEntry(marker, bestY);
  }

  // カーソル
  marker = &work->markers[66];
  marker->affineIndex = 0;
  marker->x = cursorX - 17;
  marker->y = cursorY + 1;
  Runtime_PushSlotEntry(marker, 246);

  //------------------------------------------------------------------
  // 地名ウインドウ：印が変わったときだけ書き直す
  //------------------------------------------------------------------
  if (work->shown != best) {
    work->shown = best;
    RenderOutput_PrepareForRedrawFar(work->window);
    if (best != -1) {
      if (best == 0)
        bestMessage = MsgPresentLocation;
      else
        bestMessage =
            Scene_GetAreaNameIndex(bestMessage, 1) + MsgDebugEntryName;
      UiText_MeasureResourceEntriesFar(bestMessage, &width, &height);
      cursorX = bestX;
      cursorY = bestY;
      cursorX -= WORLD_MAP_NAME_LEFT;
      cursorY -= 11;
      if (cursorX + width > 240) {
        // 右へはみ出すなら、印の左上へ回す
        cursorX = 232 - width;
        cursorY = bestY - 20;
      }
      if (cursorY < 0)
        cursorY = 0;
      UiText_DrawResourceFar(bestMessage, work->window, cursorX, cursorY);
    }
  }

  // 印の半透明を明滅させる
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

void Map_ShowWorldMap(s32 picture) {
  struct MapState *map = WORK_SLOT(8);
  struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);
  struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
  s32 resource = (s32)&ResourceId_WorldMapPicture;
  struct MapAnimation *layer;
  u8 savedFlags[16];
  s32 i;
  u8 *graphics;
  s32 savedStatus;
  s16 blend;

  if (field->mode != 3)
    return;
  field->viewCenter->paused = 1;
  savedStatus = field->transitionFrames;
  field->transitionFrames = 6;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  layer = map->anim;
  for (i = 0; i < 16; i++) {
    savedFlags[i] = layer->paused;
    layer->paused = 1;
    layer++;
  }
  menu->suspended = 1;
  WaitFrames(1);
  blend = REG_BLDALPHA;
  graphics = Resource_GetTableEntry(resource);
  Dma_Set(graphics, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | (448 / 4),
          REG_DMA3);
  BG_PLTT[0] = 0;
  Resource_DecodeType01(graphics + 448, gMapCellBuffer);
  Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          DMA_ENABLE32 | DMA_32BIT | (240 * 160 / 4), REG_DMA3);
  Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
  Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                 DISPCNT_OBJ_1D_MAP,
             &REG_DISPCNT);
  BattleFx_SetupResourcesAndWindow();
  Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
  if (GameFlag_TestFar(284))
    UiText_ShowPositionedMessageAndWaitFar(MsgNotOnMap, 1);
  do {
    WaitFrames(1);
  } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
  Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
  BattleFx_CleanupResourcesAndWindow();
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  Map_LoadAreaGraphicsFar();
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, 0x20000);
  layer = map->anim;
  for (i = 0; i < 16; i++) {
    layer->paused = savedFlags[i];
    layer++;
  }
  menu->suspended = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  field->transitionFrames = savedStatus;
  field->viewCenter->paused = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void) {
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  u32 id;

  // the selected actor is looked up and the result discarded, as the ROM does
  Object_GetById(gGameState.selectedActor);
  object = (struct ObjectRuntime *)gEventWork->viewCenter;
  x = object->x;
  left = x - (320 << 16);
  right = x + (320 << 16);
  y = object->z;
  top = y - (600 << 16);
  bottom = y + (400 << 16);

  id = 8;
  do {
    object = ObjectTable_Get(id);

    if (object != 0) {
      s32 ox = object->x;
      s32 oy = object->z;

      if (ox < left || ox > right || oy < top || oy > bottom) {
        object->animationKind = 0;
      } else {
        object->animationKind = 1;
      }
    }
    id++;
  } while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void) {
  struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);

  if (field->mode == 3) {
    Scheduler_EnableUnmaskedOverlayCallbacks();
    BattleFx_UpdateObjectVisibilityBounds();
    Battle_InitializeRenderObject();
    BattleFx_ScheduleRatioTransition(0x9d89, 6);
    if (gKeysHeld & KEY_L) {
      do {
        WaitFrames(1);
      } while (gKeysHeld & KEY_L);
    }
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Scheduler_DisableOverlayCallbacks();
  }
}
