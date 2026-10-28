// Draft, differ 212 of 707: examines or talks to object id (bit 12 for no
// opening message). Objects past 63 are containers, their row of
// Func_080ceba8's table (from 64, eight bytes: type, a no-hint flag, the
// flag that marks it opened, the contents), or object 79, the one container
// whose contents are the event work's 0xd34: types 128 and 129 (opening
// messages 0xdc5), 130 to 132 (0xdc9), 132 (0xdde) and 133 (0xde4). An opened
// container (GameFlag_IsConditionActive) gives what it holds: type 130 a trap
// that drains the party, 132 the runes (Func_080ea14c), 129 a
// battle with an enemy group of zone 104 or 105, a Mimic-like shape a
// message, coins (bit 15 of the contents) or an item; an empty one shows
// 0xdf0 and up. Any other object runs its talk script (Func_080ccd78: the
// paused object's stage-7 one first), turned toward the leader by its
// placement's kind, with the leader stopped and linked to it.
// Left: the native reads each row field with its own offset from the row
// address (0xfffffe00 to 0xfffffe06; written as a halfword array the
// allocation shifts, 528) and keeps the no-hint flag at sp 4; the message
// names stand for 0xdc4, 0xdef, 0xe0f to 0xe1c, 0xde5 and 0x15b5.
#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "object_runtime.h"
#include "scene_ids.h"

struct ActorPlacement {
  u8 unk00[22];
  s8 kind;
};

extern char MsgCheckedContainer, MsgContainerEmpty, MsgContainerHint,
    MsgContainerTrap, MsgContainerTrapEnd, MsgContainerMonster, MsgSarcophagus,
    MsgContainerShape, MsgFoundCoins, MsgInventoryFull, MsgFoundItem,
    MsgFoundItemFor;

struct ContainerRow {
  s16 type;
  s16 noHint;
  s16 flag;
  s16 contents;
};

struct TalkEvent {
  u8 unk00[8];
  s32 script;
};

void Marker_MoveLeaderAwayFromEmitter(void);
extern const u8 gObjectMotion_StepAngleScript[];

struct ActorPlacement *Scene_FindActorPlacement(s32 id);
struct ObjectRuntime *Object_GetById(s32 id);
struct ContainerRow *Func_080ceba8(void);
void EventRuntime_Start(void);
void EventRuntime_End(void);
void EventRuntime_PrepareCurrentObject(void);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void UiText_DrawQuantity(s32 value, s32 kind);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_IsConditionActive(s32 condition);
void ObjectEvent_SetMode2(s32 id);
void ObjectEvent_SetMode4WithCue(s32 id);
void ObjectEvent_SetMode5WithCue(s32 id);
void ObjectEvent_SetMode6WithCue(s32 id);
struct ObjectRuntime *ObjectEvent_GetObject(s32 id);
void ObjectEvent_Remove(s32 id);
void ObjectEvent_Spring(s32 id);
void Func_080cf350(struct ObjectRuntime *object);
void Audio_PlayCue(s32 cue);
void BattleParty_ApplyDrain(s32 amount, s32 unused);
void UiWindow_CreateWithLayoutBoundsFar(s32 layout);
s32 Party_CountActiveOwnersFar(void);
void PartyStatus_CloseFar(void);
void EventRuntime_Wait(s32 frames);
void EventRuntime_ResolvePendingActions(s32 skip);
void Func_080ea14c(s32 targetId);
void Func_08038340(s32 value);
void Func_080ad2f8(s32 value);
u16 Encounter_GetEnemyGroupAtSlot(s32 zone, s32 slot);
void BattleEffect_SelectBattleCue(s32 zone, s32 group);
s32 Object_GetSpriteShape(s32 id);
s32 Party_FindOwnerWithActionFar(s32 action);
struct ObjectRuntime *BattleEffect_StartRandomParticleEmitter(s32 id, s32 item);
void WaitFrames(s32 frames);
void Party_AdjustCoinsFar(s32 amount);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void Object_Destroy(struct ObjectRuntime *object);
void Object_DestroyIfPresent(struct ObjectRuntime *object);
s32 PartyInventory_AddFar(s32 item);
struct TalkEvent *Func_080ccd78(s32 kind, s32 target);
s32 BattleEffect_GetFlags(s32 id);
u32 Random16(void);
void EventRuntime_RunMessage(s32 actor, s32 message);
void ObjectDispatch_SetAnimationSpeedFar(struct ObjectRuntime *object,
                                         s32 value);
void Object_LinkPair(s32 first, s32 second, s32 mode);
void UiWork_SetBusyFlagsFar(s32 flags);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const void *script);
void BattleEffect_ResumeObject(s32 id);

s32 Func_080ccec8(s32 id) {
  struct ActorPlacement *placement;
  s32 result;
  struct ObjectRuntime *object;
  s32 savedAngle;
  s32 paused;
  s32 busyFlags;
  s32 silent;
  u8 *event;
  struct ObjectRuntime *leader;
  struct ObjectRuntime *shown;
  struct ContainerRow *row;
  struct TalkEvent *talk;
  u8 *busy;
  s32 found;
  s32 type;
  s32 noHint;
  s32 flag;
  s32 contents;
  s32 index;
  s32 slot;
  u8 *work;

  placement = Scene_FindActorPlacement(id);
  result = -1;
  silent = id & 0x1000;
  savedAngle = 0;
  paused = 0;
  busyFlags = 0;
  event = Ram_HeapSlots->eventWork;
  id &= 0xfff;
  object = Object_GetById(id);
  found = 0;
  if (id > 63) {
    if (id != 79) {
      row = Func_080ceba8();
      if (row != NULL) {
        type = row[id - 64].type;
        index = id;
        found = 1;
        noHint = row[id - 64].noHint;
        flag = row[id - 64].flag;
        contents = row[id - 64].contents;
      }
    } else {
      noHint = result;
      type = 131;
      flag = result;
      contents = *(s16 *)(event + 0xd34);
      index = 79;
      found = 1;
    }
    if (found) {
      found = type;
      if ((u16)(type - 128) <= 1)
        found = 1;
      if ((u16)(type - 130) <= 2)
        found = 5;
      if (type == 132)
        found = 26;
      if (type == 133)
        found = 32;
      EventRuntime_Start();
      busy = &((u8 *)object)[91];
      *busy = 1;
      if (silent == 0)
        UiText_ShowPositionedMessageAndWaitFar(
            found + (s32)&MsgCheckedContainer, 1);
      if (gAdventure.scene == (s32)&SceneId_DailaInteriors1 && noHint == 0 &&
          !GameFlag_Test(flag)) {
        UiText_DrawQuantity(contents, 2);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgContainerHint, 1);
      }
      if (!GameFlag_IsConditionActive(flag))
        goto empty;
      if (type == 128) {
        if (contents == 0)
          ObjectEvent_SetMode6WithCue(index);
        else
          ObjectEvent_SetMode4WithCue(index);
      }
      if (type == 129)
        ObjectEvent_SetMode4WithCue(index);
      if (type == 130) {
        shown = ObjectEvent_GetObject(index);
        shown->update = Marker_MoveLeaderAwayFromEmitter;
        Func_080cf350(shown);
        Audio_PlayCue(83);
        UiText_DrawQuantity(contents, 5);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgContainerTrap, 3);
        BattleParty_ApplyDrain(1999, 0);
        UiWindow_CreateWithLayoutBoundsFar(1);
        Audio_PlayCue(126);
        if (Party_CountActiveOwnersFar() == 1)
          UiText_ShowPositionedMessageAndWaitFar((s32)&MsgContainerTrap + 6, 1);
        else
          UiText_ShowPositionedMessageAndWaitFar((s32)&MsgContainerTrap + 1, 1);
        PartyStatus_CloseFar();
        shown->update = NULL;
        Object_SetMode(shown, 2);
        Audio_PlayCue(246);
        EventRuntime_Wait(30);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgContainerTrapEnd, 1);
        ObjectEvent_Remove(index);
        if (flag != -1)
          GameFlag_SetBit(flag);
        goto done;
      }
      if (type == 132) {
        EventRuntime_Start();
        EventRuntime_ResolvePendingActions(0);
        Func_080ea14c(index);
        Func_08038340(contents);
        Func_080ad2f8(contents);
        ObjectEvent_Remove(index);
        if (flag != -1)
          GameFlag_SetBit(flag);
        EventRuntime_End();
        goto done;
      }
      if (type == 129) {
        work = Ram_HeapSlots->eventWork;
        ObjectEvent_Spring(index);
        if (flag != -1)
          gAdventure.pendingDjinnEvent = flag | 0x1000;
        if (contents > 7)
          *(u16 *)(work + 356) =
              Encounter_GetEnemyGroupAtSlot(105, contents - 8);
        else
          *(u16 *)(work + 356) = Encounter_GetEnemyGroupAtSlot(104, contents);
        gAdventure.battleRule24b = 2;
        BattleEffect_SelectBattleCue(104, contents);
        Audio_PlayCue(gAdventure.sceneCue);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgContainerMonster, 1);
        goto done;
      }
      if (type == 133)
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgSarcophagus, 1);
      if (Object_GetSpriteShape(index)) {
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgContainerShape, 1);
        if (Party_FindOwnerWithActionFar(136) != -1)
          *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 346) = 1;
        goto done;
      }
      if (contents & 0x8000) {
        if (type == 131) {
          shown = ObjectEvent_GetObject(index);
          shown->update = Marker_MoveLeaderAwayFromEmitter;
        } else {
          shown = BattleEffect_StartRandomParticleEmitter(index, 0);
          WaitFrames(30);
          if (type == 128)
            ObjectEvent_SetMode2(index);
        }
        Func_080cf350(shown);
        Audio_PlayCue(83);
        contents &= 0x7fff;
        UiText_DrawQuantity(contents, 5);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgFoundCoins, 3);
        Party_AdjustCoinsFar(contents);
        if (flag != -1)
          GameFlag_SetBit(flag);
        if (type == 131) {
          Object_SetPositionAndResetMotionFar(shown, 0, 0, 0);
          shown->update = NULL;
        } else {
          Object_Destroy(shown);
        }
        goto done;
      }
      if (contents == 0) {
        if (flag != -1)
          GameFlag_SetBit(flag);
        goto empty;
      }
      if (type == 131) {
        shown = ObjectEvent_GetObject(index);
        shown->update = Marker_MoveLeaderAwayFromEmitter;
      } else {
        shown = BattleEffect_StartRandomParticleEmitter(index, contents);
        WaitFrames(30);
      }
      slot = PartyInventory_AddFar(contents);
      if (slot == -1) {
        UiText_DrawQuantity(contents, 2);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgInventoryFull, 1);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgInventoryFull + 4, 1);
        if (type != 131)
          Object_DestroyIfPresent(shown);
        if (type == 128)
          ObjectEvent_SetMode5WithCue(index);
        goto done;
      }
      if (type == 128)
        ObjectEvent_SetMode2(index);
      Func_080cf350(shown);
      Audio_PlayCue(83);
      UiText_DrawQuantity(contents, 2);
      if (slot == gAdventure.currentOwner) {
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgFoundItem, 3);
      } else {
        UiText_DrawQuantity(slot, 1);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgFoundItemFor, 3);
      }
      if (flag != -1)
        GameFlag_SetBit(flag);
      if (type == 131) {
        Object_SetPositionAndResetMotionFar(shown, 0, 0, 0);
        shown->update = NULL;
      } else {
        Object_Destroy(shown);
      }
      if (type == 133)
        ObjectEvent_Remove(index);
      result = 0;
      goto done;
    empty:
      UiText_ShowPositionedMessageAndWaitFar(found + (s32)&MsgContainerEmpty,
                                             1);
    done:
      *busy = 0;
      EventRuntime_End();
      if (result != 0)
        return result;
    }
  }

  if (gAdventure.pausedObject == id) {
    paused = 1;
    talk = Func_080ccd78(7, id);
    if (talk == NULL) {
      talk = Func_080ccd78(0, id);
      busyFlags = 1;
      if (talk == NULL)
        return result;
      if (talk->script >= 0x10000) {
        EventRuntime_RunMessage(id, BattleEffect_GetFlags(id) * 2 +
                                        ((Random16() << 1) >> 16) + 0x137f);
        goto resume;
      }
      goto check;
    }
  } else {
    talk = Func_080ccd78(0, id);
  check:
    if (talk == NULL)
      goto resume;
  }
  if (talk->script == 0)
    goto resume;
  if (!paused) {
    ((u8 *)object)[91] = 1;
    ObjectDispatch_SetAnimationSpeedFar(object, 0);
    savedAngle = object->angle;
    if ((u8)placement->kind <= 1 || placement->kind == 3) {
      leader = Object_GetById(gAdventure.currentOwner);
      Object_SetPositionAndResetMotionFar(leader, leader->x, leader->y,
                                          leader->z);
      Object_LinkPair(id, gAdventure.currentOwner, 0);
    }
  }
  if (talk->script < 0x10000) {
    UiWork_SetBusyFlagsFar(busyFlags);
    EventRuntime_RunMessage(id, talk->script);
  } else {
    EventRuntime_PrepareCurrentObject();
    ((void (*)(s32))talk->script)(id);
  }
  if (!paused) {
    if (object->script[object->step] == 17) {
      if (placement->kind == 3) {
        object->linkedObject = Object_GetById(gAdventure.currentOwner);
        object->actionFlags |= 1;
        Object_SetActionCallback(object, (s32)gObject_LinkedMotionScript);
      } else if (placement->kind == 1) {
        object->action = savedAngle;
        ObjectDispatch_InitializeFar(object, gObjectMotion_StepAngleScript);
      }
    }
    ((u8 *)object)[91] = 0;
    ObjectDispatch_SetAnimationSpeedFar(object, 16);
  }
  result = 0;
resume:
  if (paused) {
    BattleEffect_ResumeObject(gAdventure.pausedObject);
    gAdventure.pausedObject = -1;
  }
  return result;
}
