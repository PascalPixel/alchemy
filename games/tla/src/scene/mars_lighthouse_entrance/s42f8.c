#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "field_effect.h"

void Effect_AdvanceMotion(union FieldObject *object);
void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02004bf0(void);

static __inline__ void Func_02004c50_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_020042f8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_02004bf0();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  SCENE_WORK->unk1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
next:
  {
    s32 angle = i << 12;

    velocity.x = Math_Cosine(angle);
    velocity.y = 0;
    velocity.z = Math_Sin(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16Far() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16Far() * 3 << 13) >> 16) - 0x3000;
    Func_02003b30(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                  velocity.z, 0x1090001, &options);
  }
  if (++i <= 16)
    goto next;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  Func_02004c50_(0x50000, 0x50000, 0x10000);
  Func_02004c50_(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_02004cb0();
}

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "cell_target.h"

extern struct CellTarget Data_020068b4;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_020040f0(struct CellTarget *work,
                   struct CellTargetEntry *entry);
void Func_02004160(struct CellTarget *work,
                   struct CellTargetEntry *entry);

s32 Func_02004490(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &Data_020068b4;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_020040f0(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02004160(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

void Func_02004540(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(Data_020068b4.flagByte);
  s32 dz = FieldGameFlag_GetByte(Data_020068b4.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(Data_020068b4.flagByte, 0);
    GameFlag_SetByte(Data_020068b4.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02004bf0();
    Func_020042f8();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "dma.h"
#include "io_reg.h"

extern u16 *gData_0200e88c;
extern u16 *gData_02006890;
extern u16 *Data_02006894;
extern u16 *Data_02006880;

void Func_020048e8(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Func_020045f8(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_0200e88c = slots;
  gData_02006890 = colours;
  Data_02006894 = a;
  Data_02006880 = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(dead): the colour's build and store in a once-only
       * block, so both lists are stepped right after their loads and the
       * next entry's marker loads before 0xffff is built; as plain
       * statements, 22 of 80 instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)BG_PLTT)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_020048e8();
}

#include "room.h"
#include "task.h"

extern u16 gData_0200687c;
extern u16 gData_02006884;
extern u16 gData_02006898;
extern u16 gData_0200689c;
extern u16 gData_0200e888;

s32 Func_02004794(u16 *list);
void Func_020047b8(void);

void Func_020046b4(void) {
  gData_0200689c = 0;
  gData_02006898 = Func_02004794(gData_0200e88c);
  gData_0200e888 = 15;
  gData_02006884 = 16;
  gData_0200687c = 1;
  Task_AddFrame(Func_020047b8, TASK_PASS_VBLANK);
}

void Func_02004718(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(Func_020047b8);
}

void Func_02004738(void) {
  gData_0200689c = 0;
  gData_02006898 = Func_02004794(gData_0200e88c);
  gData_0200e888 = 2;
  gData_02006884 = 2;
  gData_0200687c = 1;
  Task_AddFrame(Func_020047b8, TASK_PASS_VBLANK);
}

s32 Func_02004794(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}

void Func_020047b8(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02006884 != 0) {
    if (--gData_02006884 == 0) {
      step = 1;
      gData_02006884 = gData_0200e888;
      if (gData_0200e888 > 2)
        gData_0200e888--;
    }
  }
  if (step == 0)
    return;
  src = Data_02006894;
  for (i = 0; i < gData_02006898; i++) {
    dst = &BG_PLTT[Data_02006880[gData_0200689c * gData_02006898 + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(dead): the store in a once-only block, whose end holds the
     * order around it; as the plain statement, 2 of 120 instructions differ in
     * tla-en. */
    do {
      *dst = (blue << 10) | (green << 5) | red;
    } while (0);
  }
  dst = &BG_PLTT[gData_0200e88c[gData_02006898]];
  if (gData_0200687c != 0)
    src = gData_02006890 + gData_02006898 * 3;
  else
    src = Data_02006894 + gData_02006898 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(dead): the last read in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_0200687c ^= 1;
  if (++gData_0200689c == gData_02006898)
    gData_0200689c = 0;
}

struct MarsLighthouseEntranceEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Func_02004dc8(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02004d58_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_020048e8(void) {
  struct MarsLighthouseEntranceEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  Func_02004d58_(0x10000, 2);
  ((struct MarsLighthouseEntranceEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004738();
  Func_02004dc8();
  System_Wait(40);
  Func_02004718();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "field.h"

void Func_02004a60(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02004970(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
  do {
    id = spec[0];
    object = Actor_Get(id);
    mode = spec[1];
    flag = spec[2];
    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_02004a60(id, (u16)mode, (u16)flag, object->terrainId);
    limit = 0xff;
    spec += 3;
    first = spec[0];
  } while (first != ((limit << 8) + 0xff));
}

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

// Finds the current target's table row and shrinks it.
void Func_020049dc(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  /* FAKEMATCH(dead): the entry jump in a once-only block; as the plain
   * goto, 5 of 61 instructions differ in tla-en. */
  do {
    goto test;
  } while (0);
  do {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Func_02004a60(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}
