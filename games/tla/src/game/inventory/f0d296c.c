#include "types.h"
#include "adventure.h"
#include "object_runtime.h"

extern char MsgFoundItem;

struct ObjectRuntime *Object_GetById(s32 id);
s32 PartyInventory_FindRoomFar(s32 item);
void BattleEffect_StartEffectObject22(s32 item, s32 mode);
void MessageArgument_PushFar(s32 value, s32 slot);
void FieldMessage_ShowFar(s32 message, s32 mode);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Event_WaitFrames(s32 frames);
void ObjectMotion_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
s32 PartyInventory_GiveItem(s32 item, s32 unused);
void GameFlag_SetBit(s32 flag);

/// Gives the party item from actor: with no room it shows the item, says it
/// was found and left behind and returns -1; otherwise it adds the item, sets
/// flag (unless -1) and returns 0.
s32 Func_080d296c(s32 actor, s32 item, s32 flag) {
  if (PartyInventory_FindRoomFar(item) < 0) {
    Object_GetById(actor)->scaleX = 0;
    BattleEffect_StartEffectObject22(item, 3);
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar((s32)&MsgFoundItem, 1);
    // Four messages on: "But <protagonist>'s party can't carry any more".
    FieldMessage_ShowFar((s32)&MsgFoundItem + 4, 1);
    Object_GetById(actor)->scaleX = 0x10000;
    ObjectMotion_SetAngleToward(gAdventure.currentOwner, actor, 0);
    Event_WaitFrames(20);
    return -1;
  }
  ObjectMotion_SetHorizontalPositionWithTerrain(actor, 0, 0);
  BattleEffect_StartEffectObject22(item, 3);
  PartyInventory_GiveItem(item, 0);
  if (flag != -1)
    GameFlag_SetBit(flag);
  return 0;
}
