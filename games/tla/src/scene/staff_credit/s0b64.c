#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "event_work.h"

extern char ResourceId_FarCall00A;
extern char SceneId_Ov64c;
extern char SceneId_Ov6b4;

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitTicks(s32 ticks);
void Func_0200020c(s32 stage);
void Func_0200101c(void);
void Func_02001f8c(s32 resource);
void Func_02002044(s32 a, s32 b);
void Func_02002054(s32 scene, s32 entrance);
void Func_020020ac(s32 value);

/// Chooses where the credits go next by the entrance they were entered from:
/// entrances 0 to 5 run resource FarCall00A with 2 to 7 and continue, 2 to 6
/// after Func_0200020c's stage 0 to 4; entrance 6 leaves for SceneId_Ov6b4.
s32 Func_02000b64(void) {
  struct EventWork *work;

  GameFlag_SetBit(0x144);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0;
  work->delay = 16;
  WaitTicks(1);
  Func_02002044(0x80000, 0x10000);
  GameFlag_ClearBit(0x12f);
  Func_0200101c();
  switch (gPartyState.entrance) {
  case 0:
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(2);
    Func_02002054((s32)&SceneId_Ov6b4, 2);
    break;
  case 1:
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(3);
    Func_02002054((s32)&SceneId_Ov64c, 2);
    break;
  case 2:
    Func_0200020c(0);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(4);
    Func_02002054((s32)&SceneId_Ov64c, 3);
    break;
  case 3:
    Func_0200020c(1);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(5);
    Func_02002054((s32)&SceneId_Ov64c, 4);
    break;
  case 4:
    Func_0200020c(2);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(6);
    Func_02002054((s32)&SceneId_Ov64c, 5);
    break;
  case 5:
    Func_0200020c(3);
    Func_02001f8c((s32)&ResourceId_FarCall00A);
    Func_020020ac(7);
    Func_02002054((s32)&SceneId_Ov64c, 6);
    break;
  case 6:
    Func_0200020c(4);
    Func_02002054((s32)&SceneId_Ov6b4, 3);
    break;
  }
  return 0;
}
