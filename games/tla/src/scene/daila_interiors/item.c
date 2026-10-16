#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "daila.h"

struct DailaActor {
  u8 unk00[0xc];
  s32 y;
  u8 unk10[8];
  s32 speed18;
  s32 speed1c;
  u8 unk20[8];
  s32 unk28;
  u8 unk2c[0x1c];
  s32 unk48;
  u8 unk4c[9];
  u8 kind55;
  u8 unk56[3];
  u8 flags59;
  u8 unk5a[0x12];
  s32 unk6c;
};

struct DailaActor *Object_GetById(s32 id);
s32 PartyInventory_Add(s32 item);
s32 Func_02000d14(s32 item);
void Func_02000da4(struct DailaActor *object, s32 mode);
void Func_02000dfc(s32 cue);
void Func_02000ccc(s32 value, s32 slot);
void Func_02000c9c(struct DailaActor *object);
s32 Func_02000878(s32 item, s32 mode);

void Func_02000730(void) {
  s32 saved48;
  s32 saved28;
  s32 savedY;
  s32 item;

  saved48 = Object_GetById(9)->unk48;
  item = 458;
  saved28 = Object_GetById(9)->unk28;
  savedY = Object_GetById(9)->y;
  Engine_EventBegin();
  Engine_EventResolvePendingActions(0);
  if (Func_02000d14(item) >= 0) {
    Func_02000da4(Object_GetById(9), 3);
    Func_02000dfc(83);
    Func_02000ccc(item, 2);
    UiText_ShowPositionedMessageAndWait((s32)MsgItemReceived, 1);
    PartyInventory_Add(item);
    Func_02000d4c(9, 0, 0);
    Func_02000d4c(13, 232 << 16, 194 << 16);
    GameFlag_SetBit(0xa20);
  } else {
    Func_02000da4(Object_GetById(9), 3);
    Func_02000dfc(83);
    if (Func_02000878(item, 0) != -1) {
      Func_02000d4c(9, 0, 0);
      Func_02000d4c(13, 232 << 16, 196 << 16);
      GameFlag_SetBit(0xa20);
    } else {
      Func_02000d4c(9, 0, 0);
      FieldEvent_WaitFrames(1);
      Object_GetById(9)->unk28 = saved28;
      Object_GetById(9)->y = savedY;
      Object_GetById(9)->unk48 = saved48;
      Object_GetById(9)->unk6c = 0;
      Func_02000c9c(Object_GetById(9));
      Engine_ActorSetSpritePriority(9, 1);
      Object_GetById(9)->kind55 = 2;
      Object_GetById(9)->speed1c = 0x13333;
      Object_GetById(9)->speed18 = 0x13333;
      Object_GetById(9)->flags59 |= 4;
      Daila_ObjPlace(9, 232 << 16, 196 << 16);
    }
  }
  Engine_EventEnd();
}
