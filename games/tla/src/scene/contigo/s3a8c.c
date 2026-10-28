#include "contigo.h"
#include "call.h"
#include "edition.h"

// The Spanish edition's form differs; it is still on the listing.
#if (LANGUAGE != LANGUAGE_SPANISH)

struct ContigoPrizes {
  u8 unk000[0x14c];
  s8 list[8];
};

extern u8 gSceneState[];

void Func_02001c44(s32 id);
s32 Func_02001a60(s32 topic);
void Func_0200403c(s32 value, s32 digits);
void Func_020041fc(s32 item, s32 count);
s32 PartyInventory_GiveItem(s32 item, s32 mode);
void Sound_WaitForFanfare(void);

void Func_02003a8c(void) {
  s32 diff;
  s8 *list;
  s8 *prize;
  s32 item;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Func_02001c44(13);
  Func_02001c44(16);
  Func_02001c44(17);
  Func_02001c44(19);
  Func_02001c44(20);
  Object_SetPartAttribute(Object_GetById(14), 1);
  Object_SetPartAttribute(Object_GetById(15), 2);
  Object_SetPartAttribute(Object_GetById(8), 0);
  Object_SetPartAttribute(Object_GetById(9), 3);
  if (!Value1((s32 (*)())GameFlag_Test, 0x9e5))
    return;
  GameFlag_ClearBit(0x9e5);
  if (gAdventure.entrance == 18) {
    diff = gAdventure.coins - *(s32 *)gSceneState;
    Func_0200406c();
    Func_02004204(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff > 19999)
        Func_020042b4(93);
      else if (diff > 4999)
        Func_020042b4(92);
      else
        Func_020042b4(91);
      FieldEvent_WaitFrames(20);
      Func_0200412c((s32)MsgContigo04);
      Func_0200403c(diff, 5);
      Func_02004144(14, 0);
      Sound_WaitForFanfare();
    } else if (diff < 0) {
      Func_0200412c((s32)MsgContigo05);
      Func_0200403c(-diff, 5);
      Func_02004144(14, 0);
    }
    Func_02004074();
  } else if (gAdventure.entrance == 19) {
    diff = gAdventure.coins - *(s32 *)gSceneState;
    Func_0200406c();
    Func_02004204(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff > 19999)
        Func_020042b4(93);
      else if (diff > 4999)
        Func_020042b4(92);
      else
        Func_020042b4(91);
      FieldEvent_WaitFrames(20);
      Func_0200412c((s32)MsgContigo04);
      Func_0200403c(diff, 5);
      Func_02004144(14, 0);
      Sound_WaitForFanfare();
    } else if (diff < 0) {
      Func_0200412c((s32)MsgContigo05);
      Func_0200403c(-diff, 5);
      Func_02004144(14, 0);
    }
    Func_02004074();
  } else if (gAdventure.entrance == 20) {
    list = ((struct ContigoPrizes *)&gAdventure)->list;
    Func_0200406c();
    Func_02004204(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_WaitFrames(20);
    if (list[0] == -1) {
      Func_02001938(1);
    } else if (list[0] == -2) {
      Func_0200412c((s32)MsgContigo11);
      Func_02004144(18, 0);
    } else {
      Func_0200412c((s32)MsgContigo14);
      Func_02004144(18, 0);
      if (list[0] != -1) {
        prize = list;
      next:
        if (prize == list)
          Func_0200412c((s32)MsgContigo15);
        else
          Func_0200412c((s32)MsgContigo16);
        item = Func_02001a60(*prize);
        Func_0200403c(item, 2);
        Func_02004144(18, 0);
        Func_020041fc(item, 3);
        PartyInventory_GiveItem(item, 0);
        Call2((void (*)())Func_0200415c, 0, 0xc000);
        prize++;
        if (*prize != -1)
          goto next;
      }
      ((struct ContigoPrizes *)&gAdventure)->list[0] = -2;
      Func_0200412c((s32)MsgContigo17);
      Func_02004144(18, 0);
    }
    Func_02004074();
  }
}
#endif
