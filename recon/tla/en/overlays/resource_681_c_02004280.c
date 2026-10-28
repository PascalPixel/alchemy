// Draft, differ 49 of 229: Gaia Rock's entry setup. In the first room (scene
// 0xb1) from entrances up to 4 it hides the leader's sprite layer, sets the
// sky cell window, places the statues by the stage and, once that stage's flag
// is set, starts person 8's climbing; from entrances past 8 it resets layer
// 2's scroll; in the next rooms it picks the music for the entrance. The sum
// is a signed indexed loop with its total set before Unit_Get (so it lives in
// r5), the angle is unsigned, and the second room's cases run 102 before 103
// (the 100 and 101 blocks cross-jump into the third room's). Left, ordering:
// the BG0CNT address is begun before the sky byte's store, *stage is loaded
// into r0 rather than r3, the scroll reset's 0x3f80 is built early, and
// Data_0202de00 stays in r5 where the native loads it from the pool twice.
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "iwram_call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern char SceneId_Ov681Room1;
extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room5;
extern u8 gData_020023c4[];
extern const u8 gData_020053e4[];
extern u8 Data_0202de00[];

u8 *Unit_Get(s32 owner);
void Func_020044f8(s32 level);
s32 Func_02004fe4(void);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020048a0(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02004ebc(s32 id, s32 x, s32 z);
s32 Func_02004904(void);
void Func_02004d9c(struct ObjectRuntime *object, s32 mode);
void Func_02004dc4(void);
void WaitFrames(s32 frames);
void Func_02004de4(void *data);
void Func_02004274(s32 song);
void Func_02000aac(void);
void Func_02000a60(void);

s32 Func_02004280(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov681Room1) {
    u8 *map = (u8 *)Ram_HeapSlots->mapWork;

    if (gAdventure.entrance <= 4) {
      u8 *unit;
      s32 total;
      s32 *stage;
      s32 i;

      total = 0;
      unit = Unit_Get(gAdventure.currentOwner);

      for (i = 0; i <= 13; i++)
        total += unit[i];
      Func_020044f8(total);
      Object_GetById(Func_02004fe4())->priorityFlags |= 32;
      map[261] = 1;
      {
        volatile u16 control = *(volatile u16 *)0x0400000c & ~3;

        *(volatile u16 *)0x0400000c = control | 1;
      }
      Func_020048a0();
      stage = (s32 *)(gData_020023c4 + 0x288);
      if (*stage == 3 && !GameFlag_Test(0x3f6))
        Func_02004ebc(64, 0x680000, 0x580000);
      if (*stage == 9)
        Func_02004ebc(65, 0x680000, 0x480000);
      if (GameFlag_Test(*stage + 0xa50)) {
        struct ObjectRuntime *climber = Object_GetById(8);
        u32 angle = Func_02004904();

        climber->collisionFlags = 0;
        Func_02004d9c(climber, ((angle & 0xc000) >> 14) + 2);
        climber->update = (angle & 0x4000) != 0 ? (void (*)(void))Func_02000aac
                                                : (void (*)(void))Func_02000a60;
        climber->collisionFlags = 1;
        GameFlag_SetBit(0x200);
      }
    } else if (gAdventure.entrance > 8) {
      u8 *layer;

      *(s32 *)(gData_020023c4 + 0x288) = 1;
      layer = map + 376;
      FIELD_AT(layer, u16, 40) = 127;
      FIELD_AT(layer, s32, 24) = 0;
      FIELD_AT(layer, s32, 28) = 0;
      FIELD_AT(layer, s32, 32) = 0;
      FIELD_AT(layer, s32, 36) = 0;
      FIELD_AT(layer, s32, 16) = 0x10000;
      FIELD_AT(layer, s32, 20) = 0x10000;
      FIELD_AT(layer, u16, 42) = 0x3f80;
      FIELD_AT(layer, s32, 0) =
          Iwram_MulQ16(FIELD_AT(map, s32, 228), FIELD_AT(layer, s32, 16)) +
          FIELD_AT(layer, s32, 8);
      FIELD_AT(layer, s32, 4) =
          Iwram_MulQ16(FIELD_AT(map, s32, 232), FIELD_AT(layer, s32, 20)) +
          FIELD_AT(layer, s32, 12);
      Func_02004dc4();
      WaitFrames(1);
      Iwram_CopyWords(Data_0202de00, gData_020053e4, 24);
      Func_02004de4(Data_0202de00);
    }
  } else if (gAdventure.scene == (s32)&SceneId_Ov681Room2) {
    switch (gAdventure.entrance) {
    case 1:
    case 2:
    case 5:
    case 8:
      Func_02004274(100);
      break;
    case 9:
    case 10:
      Func_02004274(102);
      break;
    case 3:
    case 4:
      Func_02004274(103);
      break;
    case 6:
    case 7:
      Func_02004274(101);
      break;
    }
  } else if (gAdventure.scene == (s32)&SceneId_Ov681Room5) {
    switch (gAdventure.entrance) {
    case 1:
    case 3:
    case 5:
      Func_02004274(100);
      break;
    case 2:
    case 4:
    case 7:
      Func_02004274(101);
      break;
    case 6:
      Func_02004274(102);
      break;
    }
  }
  return 0;
}
