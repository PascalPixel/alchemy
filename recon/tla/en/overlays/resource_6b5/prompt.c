// Near miss, differ 4 of 63 (2026-10-12), from ☀️ LinkLobby_TalkLinkedWins: in
// each branch the listing schedules the message load and the tally offset one
// step apart from this draft (ldr r3 before movs r2 in the first, between movs
// and lsls in the second). A barrier after the message assignment moves the
// save of the message (mov r8, r3) ahead instead.
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "object_runtime.h"
#include "adventure.h"
#include "text/msg_ids.h"

void Func_020019e4(s32 value, s32 digits);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
s32 Func_02001a74(s32 actor, s32 flags);
void Func_02001ab4(s32 skip);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
s32 Window_Create();
void WindowSprite_PrepareForRedraw();
void Text_DrawNumber();
s32 DebugParty_LoadPreset();
void MessageArgument_ClearAll(void);

s32 Func_02000da0(s32 actor) {
  u32 facing = ((struct ObjectRuntime *)Object_GetById(4))->angle;
  u16 *count;
  s32 base;

  Func_02001a14();
  Func_02001ab4(0);
  ObjectMotion_SetAngleToward(actor, gAdventure.currentOwner, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    base = (s32)MsgItLooksLikeThisIs;
    count = &gAdventure.linkTallies[4];
    if (*count == 0) {
      Func_02001a64((s32)MsgIWillTell);
      return Func_02001a74(actor, 0);
    }
  } else {
    base = (s32)MsgItLooksLikeThisIsYour;
    count = &gAdventure.linkTallies[7];
    if (*count == 0)
      goto none;
  }
  MessageArgument_ClearAll();
  Func_020019e4(*count, 5);
  Func_02001a64(base + 1);
  return Func_02001a74(actor, 0);
none:
  Func_02001a64((s32)MsgIWillTellYou);
  return Func_02001a74(actor, 0);
}
