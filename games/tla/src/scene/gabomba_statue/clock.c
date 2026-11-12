/// @file
/// Scene code of overlay 0x671, in ROM order, from clock's offset up to the next object the overlay links and the overlay's data.

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "task.h"
#include "event_inline.h"
#include "field.h"
#include "map.h"

struct ClockWork {
  u8 *tiles;
  u8 *digits;
  u8 unk08[120];
  s32 tileEntry;
  s32 timer;
  u8 ringing;
  u8 mode;
};

extern u8 gData_020023c4[];
void Func_02005090(void);
s32 Math_Cosine(s32 angle);
void Func_02005210(void);
void Func_0200525c(void);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);
void *Heap_AllocEwram(s32 size);
void *Resource_GetTableEntry(s32 index);
void Func_020055cc(const void *data, void *buffer);
s32 Resource_FindFreeEntry(void);
extern char ResourceId_ClockDigits;

void Func_020052a4(s32 mode) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  u8 *tiles;

  if (!Event_GetFlag(0x109)) {
    Iwram_ClearWords(clock, sizeof(struct ClockWork));
    clock->timer = 3600;
    clock->ringing = 1;
  }
  tiles = Heap_AllocEwram(0x400);
  Func_020055cc(Resource_GetTableEntry((s32)&ResourceId_ClockDigits), tiles);
  clock->tiles = tiles;
  clock->digits = tiles + 0x2c0;
  clock->tileEntry = Resource_FindFreeEntry();
  Vram_Load(clock->tileEntry, 320, NULL);
  clock->mode = mode;
  if (mode)
    Task_AddFrame(Func_02005210, TASK_PASS_VBLANK);
  else
    Task_AddFrame(Func_0200525c, TASK_PASS_VBLANK);
}

void Func_0200533c(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (clock->mode)
    FieldTask_Remove(Func_02005210);
  else
    FieldTask_Remove(Func_0200525c);
}

void Func_02005364(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (clock->mode)
    Task_AddFrame(Func_02005210, TASK_PASS_VBLANK);
  else
    Task_AddFrame(Func_0200525c, TASK_PASS_VBLANK);
  clock->timer = 3600;
}

s32 Func_020053a0(void) {
  return ((struct ClockWork *)(gData_020023c4 + 0x288))->timer;
}

void Func_020053ac(void) {
  ((struct ClockWork *)(gData_020023c4 + 0x288))->timer = 3600;
}

#include "actor.h"

void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer);

struct EffectWork {
  u8 unk00[26];
  s16 target;
};

// Layer switch table: three words (id, mode, flag), terminated by 0xffff.
void Func_020053bc(const u16 *entry) {
  s32 value;

  while ((value = *entry) != 0xffff) {
    u16 id = *entry++;
    struct Actor *object = Actor_Get(id);
    s16 mode = entry[0];
    s16 flag = entry[1];

    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_020054ac(id, (u16)mode, (u16)flag, object->terrainId);
    entry += 2;
  }
}

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

// Finds the current target's table row and shrinks it.
void Func_02005428(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  while (*list != 0xffff) {
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
        Func_020054ac(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  }
}

s32 Func_02005844(struct Actor *object);
void Func_02005624(struct Actor *object, s32 mode);

void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02005844(object);
  if (Event_GetFlag(flag)) {
    /* FAKEMATCH(temporary): the flags read into a local before the cell store
     * and a local holding 2, so the flags are ORed into the register that holds
     * 2; as one priorityFlags |= 2, 8 of 75 instructions differ in tla-en. */
    u8 flags = object->priorityFlags;
    s32 bits;

    cell->collisionCode = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (mode) {
    case 0:
      Func_02005624(object, 2);
      break;
    case 1:
      Func_02005624(object, 4);
      break;
    case 2:
      Func_02005624(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (mode) {
    case 0:
      Func_02005624(object, 1);
      break;
    case 1:
      Func_02005624(object, 3);
      break;
    case 2:
      Func_02005624(object, 5);
      break;
    }
  }
}

#include "scene_tables.h"
#include "objcmd.h"

// clang-format off
u32 gData_0200585c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005898[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020058d4[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005910[] = {
    0x20007, 512, 0x20003, 0x20004, 516, 515,
    0x20101, 0x10201, 0x20102, 0x10202, 0xf800, 0,
};
// clang-format on

u16 gData_02005940[] = {
    259,
    2,
};

u32 gData_02005944[] = {
    38,
};
