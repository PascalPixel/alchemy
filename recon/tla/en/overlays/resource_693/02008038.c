// Matches: differ 0 (tla-en, 2026-11-01)
// All five functions: Func_02000064, Func_020000e0, Func_02000c14, Func_02000d08
// and Func_02000d3c; measure each with --symbol. Scene ids 0xea and 0xeb are
// Ov693Room1 and Ov693Room2 in gScenes. Func_02000c14 has the shape of Atteka
// Inlet's matched Func_02000c14, and its goto loops are that twin's.
#include "types.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "scene_ids.h"

extern u8 gData_02001194[];
extern u8 gData_020011ac[];
extern u8 gData_020012cc[];
extern u8 gData_0200147c[];
extern u8 gData_0200159c[];
extern u8 gData_0200165c[];
extern u8 gData_0200171c[];
extern u8 gData_02001728[];
extern u8 gData_020018f0[];

void Func_02001110(s32 a, s32 b, s32 c);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_0200097c(void);
void Func_02000dc4(void);
void Func_02000ed0(void);
struct FieldActor *Actor_GetById(s32 id);
void Actor_SetSpritePriority(u32, s32);
void Func_02000ff0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

u8 *Func_02000064(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1) {
    if (GameFlag_Test(0x9b2))
      return gData_0200147c;
    if (GameFlag_Test(0x9d0))
      return gData_020012cc;
    return gData_020011ac;
  }
  if (scene == (s32)&SceneId_Ov693Room2) {
    if (GameFlag_Test(0x9d0))
      return gData_0200159c;
    return gData_0200165c;
  }
  return gData_02001194;
}

u8 *Func_020000e0(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1)
    return gData_02001728;
  if (scene == (s32)&SceneId_Ov693Room2)
    return gData_020018f0;
  return gData_0200171c;
}

void Func_02000c14(void) {
  if ((*Ram_FrameCount & 7) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x05000172;
    index = 9;
  first:
    palette = (u16 *)0x05000160;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 4)
      goto first;
    *(volatile u16 *)0x05000168 = saved;
  }
  if ((*Ram_FrameCount & 7) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x0500017e;
    index = 15;
  second:
    palette = (u16 *)0x05000160;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 12)
      goto second;
    *(volatile u16 *)0x05000178 = saved;
  }
}

s32 Func_02000d08(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1)
    Func_02000dc4();
  else if (scene == (s32)&SceneId_Ov693Room2)
    Func_02000ed0();
  return 0;
}

s32 Func_02000d3c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov693Room1 && GameFlag_Test(0x9d0)) {
    Func_02000ff0(54, 47, 4, 15, 10, 4);
    Func_02000ff0(54, 38, 73, 7, 10, 4);
    Func_02000ff0(54, 43, 21, 55, 10, 3);
    Func_02000ff0(65, 43, 21, 20, 10, 3);
    Func_02000ff0(4, 47, 4, 50, 10, 3);
  }
  return 0;
}
