#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void MessageArgument_ClearAll(void);
void Func_020019e4(s32 value, s32 digits);
void Func_02001a14(void);
void Func_02001a64(s32 message);
void Func_02001a74(s32 actor, s32 flags);
void Func_02001ab4(s32 skip);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);

// Tells the leader's win count, total or in a row by the way the lead faces.
s32 Func_02000da0(s32 actor) {
  u32 facing = Object_GetById(4)->angle;
  u16 *count;
  s32 base;

  Func_02001a14();
  Func_02001ab4(0);
  ObjectMotion_SetAngleToward(actor, gAdventure.currentOwner, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    base = (s32)MsgItLooksLikeThisIs;
    count = &gAdventure.linkTallies[4];
    if (*count != 0) {
      MessageArgument_ClearAll();
      Func_020019e4(*count, 5);
      Func_02001a64(base + 1);
      Func_02001a74(actor, 0);
    } else {
      Func_02001a64((s32)MsgIWillTell);
      Func_02001a74(actor, 0);
    }
  } else {
    base = (s32)MsgItLooksLikeThisIsYour;
    count = &gAdventure.linkTallies[7];
    if (*count != 0) {
      MessageArgument_ClearAll();
      Func_020019e4(*count, 5);
      Func_02001a64(base + 1);
      Func_02001a74(actor, 0);
    } else {
      Func_02001a64((s32)MsgIWillTellYou);
      Func_02001a74(actor, 0);
    }
  }
}
