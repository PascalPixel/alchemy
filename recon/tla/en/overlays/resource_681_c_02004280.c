// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it (instructions; the
// diff omits pools). Gaia Rock's entry setup. In the first room (scene 0xb1)
// from entrances up to 4 it hides the leader's sprite layer, sets the sky cell
// window, places the statues by the stage and, once that stage's flag is set,
// starts person 8's climbing; from entrances past 8 it resets layer 2's scroll
// and starts the room's display blend script; in the next rooms it picks the
// music for the entrance.
// The last 17 differences were the blend script's buffer. It is
// Ram_MapCollision + 0x1e00, the buffer Func_0802a6b8 decodes a map's blend
// script into before DisplayBlend_StartScript (Func_02004de4 is that call's
// overlay veneer), so it is written that way and Data_0202de00 is gone; the
// pool word stays 0x0202de00, which is where ewram.s places Data_0202de00.
// Called directly, the copy's address goes into a pseudo that CSE then shares
// with the blend call (r5, held across the copy). The native loads it straight
// into r0 for each call, after the source and the routine's address. That is
// what an inline does: the constant replaces the inline's parameter where r0
// is set, so nothing holds it across the call. Hence CopyWords_ (tagged
// wrapper); the blend call then needs no forwarder. Tried and worse or no
// better: a local set at the scroll reset (r8, 64), a volatile asm between the
// calls (4: r0 loaded after r2), an integer address with direct calls or
// Call1 (17), Call3 for the copy (6: loads in the wrong order), a never-true
// test on map[261] between the calls (52; combine leaves it). The two Call3
// forwarders for Func_02004ebc are still needed (direct: 2 each, 4 both).
// Earlier notes: the sum is a signed indexed loop with its total set before
// Unit_Get (so it lives in r5), the angle is unsigned, and the second room's
// cases run 102 before 103 (the 100 and 101 blocks cross-jump into the third
// room's).
#include "call.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "map.h"

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
extern u32 gData_020053e4[];

u8 *Unit_Get(s32 owner);
void Func_020044f8(s32 level);
s32 Func_02004fe4(void);
struct Actor *Actor_GetById(s32 id);
void Func_020048a0(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02004ebc(s32 id, s32 x, s32 z);
s32 Func_02004904(void);
void Func_02004d9c(struct Actor *object, s32 mode);
void Func_02004dc4(void);
void wait(s32 frames);
void Func_02004de4(u16 *script);
void Func_02004274(s32 song);
void Func_02000aac(void);
void Func_02000a60(void);

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

s32 Func_02004280(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room1) {
    u8 *map = (u8 *)Ram_HeapSlots->mapWork;

    if (gMainRoutineWork.entrance <= 4) {
      u8 *unit;
      s32 total;
      s32 *stage;
      s32 i;

      total = 0;
      unit = Unit_Get(gMainRoutineWork.currentOwner);

      for (i = 0; i <= 13; i++)
        total += unit[i];
      Func_020044f8(total);
      Actor_GetById(Func_02004fe4())->priorityFlags |= 32;
      map[261] = 1;
      {
        volatile u16 control = *(volatile u16 *)0x0400000c & ~3;
        // Once-only block: keeps the sky cell window's store after its read.
        do {
        } while (0);

        *(volatile u16 *)0x0400000c = control | 1;
      }
      Func_020048a0();
      stage = (s32 *)(gData_020023c4 + 0x288);
      /* FAKEMATCH(wrapper): the call forwarded through Call3; as a direct
       * Func_02004ebc call, 2 of 229 instructions differ in tla-en. */
      if (*stage == 3 && !GameFlag_Test(0x3f6))
        Call3((void (*)())Func_02004ebc, 64, 0x680000, 0x580000);
      /* FAKEMATCH(wrapper): the call forwarded through Call3; as a direct
       * Func_02004ebc call, 2 of 229 instructions differ in tla-en. */
      if (*stage == 9)
        Call3((void (*)())Func_02004ebc, 65, 0x680000, 0x480000);
      if (GameFlag_Test(*stage + 0xa50)) {
        struct Actor *climber = Actor_GetById(8);
        u32 angle = Func_02004904();

        climber->collisionFlags = 0;
        Func_02004d9c(climber, ((angle & 0xc000) >> 14) + 2);
        climber->update = (angle & 0x4000) != 0 ? Func_02000aac : Func_02000a60;
        climber->collisionFlags = 1;
        GameFlag_SetBit(0x200);
      }
    } else if (gMainRoutineWork.entrance > 8) {
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
      // Once-only block: keeps the scale defaults before the position sums.
      do {
      } while (0);
      ((struct MapLayer *)layer)->x =
          Iwram_MulQ16(((struct MapWork *)map)->viewX,
                       FIELD_AT(layer, s32, 16)) +
          FIELD_AT(layer, s32, 8);
      ((struct MapLayer *)layer)->y =
          Iwram_MulQ16(((struct MapWork *)map)->viewZ,
                       FIELD_AT(layer, s32, 20)) +
          FIELD_AT(layer, s32, 12);
      Func_02004dc4();
      wait(1);
      /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, so the
         blend buffer's address is loaded straight into r0 at each call; as a
         direct Iwram_CopyWords call, 17 of 229 instructions differ in
         tla-en. */
      CopyWords_(Ram_MapCollision + 0x1e00, gData_020053e4, 24);
      Func_02004de4((u16 *)(Ram_MapCollision + 0x1e00));
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2) {
    switch (gMainRoutineWork.entrance) {
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
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5) {
    switch (gMainRoutineWork.entrance) {
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
