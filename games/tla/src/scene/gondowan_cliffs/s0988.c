#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "call.h"

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

extern char SceneId_Ov66bRoom1, SceneId_Ov66bRoom2;
extern u8 gData_02001ae0[];
void Func_02000768(void);
void Func_020007d8(void);
s32 GameFlag_Test(s32 flag);
s32 Func_02001314(void);
struct Actor *Actor_GetById(u32 id);
void Func_020012f4(void);
void Func_0200130c(s32 a, s32 b, s32 c, s32 d);
void Func_020012fc(void);
void Func_02001304(s32 a, s32 b, s32 c);
void Func_0200124c();
void Func_020011d4();
void Func_020011dc();
void Func_02000fcc(s32 a, s32 b);
u16 *Func_02001068(s32 index, s32 actor, s32 start, s32 low, s32 high);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Actor_SetModeById(u32 id, s32 mode);
void Task_Add(void (*callback)(void), s32 priority);
void Func_02000738(void);

/// Gondowan Cliffs' setup: Ov66bRoom2 runs Func_02000768 and returns; with
/// flag 0x8f3 set and 0x8ef clear it runs Func_020007d8; otherwise in
/// Ov66bRoom1 it starts the room's lists, places or raises actor 14 by flag
/// 0x885, copies cells by flags 0x201 and 0x202, records the swinging actor 17
/// (Func_02001068) and places it by flag 0x886, sets actor 16's mode, places
/// actor 15 by flag 0x884, saves four palette colours, schedules Func_02000738
/// and sets BG layer priority. Returns 0.
s32 Func_02000988(void) {
  struct Actor *actor;
  s32 height;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
  if (gAdventure.scene == (s32)&SceneId_Ov66bRoom2) {
    Func_02000768();
    return 0;
  }
  if (GameFlag_Test(0x8f3) && !GameFlag_Test(0x8ef)) {
    Func_020007d8();
    return 0;
  }
  Actor_GetById(Func_02001314())->priorityFlags |= 32;
  /* FAKEMATCH(one-pass): the room test through the placing of actor 17 in a
   * once-only block, which loads gAdventure before the scene offset; as
   * plain statements, 2 of 227 instructions differ in tla-en. */
  do {
    if (gAdventure.scene != (s32)&SceneId_Ov66bRoom1)
      goto done;
    Func_020012f4();
    Func_0200130c(0, 0x200, 10, 11);
    if (GameFlag_Test(0x8ab)) {
      Func_020012fc();
      Func_02001304(0, 8, 9);
    }
    if (GameFlag_Test(0x885))
      Func_0200124c(14, 0, 0);
    else
      Actor_GetById(14)->scaleY = 0x14000;
    if (GameFlag_Test(0x201))
      Call6(Func_020011d4, 79, 28, 1, 1, 81, 28);
    if (GameFlag_Test(0x202))
      Call6(Func_020011d4, 115, 35, 1, 1, 117, 35);
    Func_02000fcc(0, 240);
    Func_02001068(0, 17, 0, 0, 64);
    if (GameFlag_Test(0x886)) {
      actor = Actor_GetById(17);
      Call6(Func_020011dc, 124, 1, 1, 1, 119, 43);
      Call3(Func_0200124c, 17, 0x7780000, 0x2d80000);
      height = Map_GetTerrainHeight(0, actor->x, actor->z);
      actor->y = height;
      actor->terrainHeight = height;
    }
  } while (0);
  Actor_SetModeById(16, 4);
  if (GameFlag_Test(0x884)) {
    Call6(Func_020011d4, 74, 31, 1, 1, 74, 33);
    Func_0200124c(15, 0x4a90000, 0x2120000);
    Actor_GetById(15)->priorityFlags |= 2;
    Actor_GetById(15)->collisionFlags = 0;
    Actor_SetModeById(15, 2);
  } else {
    Actor_GetById(15)->collisionFlags |= 16;
  }
  Iwram_CopyWords(gData_02001ae0, (void *)0x05000018, 8);
  Task_Add(Func_02000738, 0x480);
  ((struct MapLayerBits *)Ram_HeapSlots->mapWork)->priority = 1;
done:
  return 0;
}
