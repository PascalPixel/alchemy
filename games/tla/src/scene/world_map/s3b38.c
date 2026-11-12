/// @file
/// Scene code of overlay 0x64b, in ROM order, from s3b38's offset up to the next object the overlay links.

#include "types.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "main_routine_work.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020039f4(struct Actor *object);

void Func_02003b38(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_020039f4(object);
  }
}

extern s32 gData_02006be4;
extern const s32 gData_02004fc8[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02003aac(struct Actor *object);

void Func_02003b70(struct Actor *object) {
  if (gFrameCount & 1) {
    Animation_ApplyChildValues(object, gData_02004fc8[gData_02006be4]);
    gData_02006be4 = (gData_02006be4 + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    Func_02003aac(object);
  }
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02003b38(struct Actor *object);
void Func_02003b70(struct Actor *object);

void Func_02003bb8(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValues(object, 0);
    Event_PlaySound(390);
  } else if (mode == 1) {
    object->update = (void (*)(void))Func_02003b38;
  } else {
    object->update = (void (*)(void))Func_02003b70;
    Event_PlaySound(291);
  }
}

void Func_02003bb8(struct Actor *object, s32 mode);
s32 Func_02004b90(s32 *position);
s32 Func_02004e20(s32 a, s32 b);

void Func_02003bf0(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(212);
  Func_02003bb8(object, 1);
  for (i = 0; i < 20; i++) {
    s32 hit = Func_02004b90(&object->x);

    if (hit != 0 && Func_02004e20(2, hit) != 0)
      break;
    System_Wait(1);
  }
  Func_02003bb8(object, 2);
}

extern u8 gData_02006c08[];
extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
u32 Random16Far(void);
void Func_02004c20(void);
void Func_02004c28(void);
s32 Func_02004c38(s32 a, s32 b, s32 c);
void Func_02003654(s32 a, s32 b, s32 c);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020036a8(void);
void Func_020048f4(void);
void Func_02003bb8(struct Actor *object, s32 value);
void Func_02004aa8(const u8 *a, u8 *b);
void Resource_ResetEntry(s32 entry);
u32 __umodsi3(u32 a, u32 b);

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

void Func_02003c3c(void) { Func_02003bb8(Actor_GetById(Party_GetLeaderActor()), 0); }

extern u32 gFrameCount;
extern u16 gData_02006e08;

static inline void QueueBlend(u32 address, u32 value) {
  /* FAKEMATCH(wrapper): forwarding through the inline sets the arguments up in
   * their own order: the native finishes the value's orr before the register
   * address's add; as direct calls, 4 of 31 instructions of Func_02003c50
   * differ in tla-en. */
  QueueIoWriteDelay2(address, value);
}

/// Every other two frames the world map's blend alternates between two
/// strengths.
void Func_02003c50(void) {

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2)
    QueueBlend((u32)&REG_BLDALPHA, gData_02006e08 | 12);
  else
    QueueBlend((u32)&REG_BLDALPHA, gData_02006e08 | 16);
}

extern u32 gFrameCount;

u32 Random16Far(void);

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): as in the scene layer of The Broken Seal, forwarding
   * through the inline sets the arguments up in their own order: the native
   * shifts z before it starts the -1, where called directly the -1 starts
   * first; as direct calls, 6 of 66 instructions of Func_02003ca8 differ in
   * tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
void Func_02003ca8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16Far() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      Camera_MoveWithinMapBounds((x << 16) + 0x10000, -1, (z << 16) - 0x10000,
                                 1);
      break;
    }
    case 2:
      CameraMove((x << 16) + 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 3:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) - 0x10000, 1);
      break;
    }
  }
}
