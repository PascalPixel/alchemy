/// @file
/// Scene code of overlay 0x671, in ROM order, from s4d48's offset up to the next object the overlay links.

#include "types.h"
#include "event_inline.h"

struct Unk02004d48 {
  u8 unk00[8];
  s32 x;
  u8 unk0c[76 - 12];
  u32 unk4c;
  u8 unk50[98 - 80];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u16 unk66;
};

void Func_02005654();
void Func_02005624();
void Func_02004938();

s32 Func_02004d48(s32 first, s32 check) {
  struct Unk02004d48 *actor;
  s32 second = first + 1;
  s32 frame, tries, id;
  s32 done;
  s32 result;

  actor = ((struct Unk02004d48 *)Actor_Get(first));
  actor->unk64 = 0;
  actor->unk66 = 7;
  actor->unk62 = 2;
  actor->unk63 = 1;
  actor->unk4c = 0;
  /* FAKEMATCH(signature): Func_02005654, Actor_ResetMotion's veneer, called
   * without the actor it reads from r0; as Func_02005654(actor), 73 of 230
   * instructions differ in tla-en. */
  Func_02005654();
  Func_02005624(actor, 2);
  id = second;
  actor = ((struct Unk02004d48 *)Actor_Get(id++));
  actor->unk64 = 2;
  actor->unk66 = 7;
  actor->unk62 = 0;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = ((struct Unk02004d48 *)Actor_Get(id++));
  actor->unk64 = 8;
  actor->unk66 = 7;
  actor->unk62 = 2;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = ((struct Unk02004d48 *)Actor_Get(id));
  actor->unk64 = 10;
  actor->unk66 = 7;
  actor->unk62 = 0;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  for (tries = 0; tries <= 127; tries++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
      for (id = first; id < first + 4; id++) {
        actor = ((struct Unk02004d48 *)Actor_Get(id));
        if (actor->unk63 != 0 && actor->unk63 != 7)
          done = 0;
        Func_02004938(actor, frame, id);
      }
      System_Wait(1);
    }
    if (done)
      break;
  }
  System_Wait(20);
  result = 3;
  if (check) {
    u32 a = 0;
    u32 b = 0;
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first));
      if (r->x != 0)
        a = r->unk4c;
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(second));
      if (r->x != 0) {
        u32 value = r->unk4c;
        if (a == 0 || a > value)
          a = value;
      }
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first + 2));
      if (r->x != 0)
        b = r->unk4c;
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first + 3));
      if (r->x != 0) {
        u32 value = r->unk4c;
        if (b == 0 || b > value)
          b = value;
      }
    }
    if (a != 0) {
      result = 1;
      if (b != 0) {
        result = 0;
        if (a != b) {
          if (a < b) {
            Event_SetFlag(0x9eb);
            result = 1;
          } else {
            Event_SetFlag(0x9eb);
            result = 2;
          }
        }
      }
    } else {
      if (b != 0)
        return 2;
      return 3;
    }
  }
  return result;
}

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
};

extern struct SceneState gSceneState;

void Func_02004f40(u32 *x_out, u32 *z_out) {
  u32 z;
  u32 x;

  for (z = 0; z <= 7; z++) {
    for (x = 0; x <= 10; x++) {
      if (gSceneState.cells[z * 11 + x].kind == 10 && !(x == 5 && z == 3) &&
          !(z == 7 && (x == 1 || x == 9))) {
        *x_out = x;
        *z_out = z;
        return;
      }
    }
  }
}

#include "iwram_call.h"

struct Sprite {
  u8 b0, b1, b2, b3, b4;
  u8 mode : 2;
  u8 b5rest : 6;
  u8 b6;
  u8 b7a : 1;
  u8 affine : 5;
  u8 b7b : 2;
  u32 w8;
};

struct ClockWork {
  u8 *tiles;  ///< the digit sheet
  u8 *digits; ///< the five digit pictures, as loaded
  u8 old[8];
  u8 now[8];
  u8 digit[8];
  struct Sprite oam[8];
  s32 tileEntry;
  s32 timer;
  u8 ringing;
  u8 mode;
};

extern u8 gData_020023c4[];
s32 Math_Cosine(s32 angle);

/// Converts the remaining frames into minutes, seconds and tenths
/// (`MM:SS:T`).
void Func_02004f84(s32 time, u8 *text) {
  if (time < 0)
    time = 0;
  *text = time / 36000 + '0';
  time %= 36000;
  *++text = time / 3600 + '0';
  time %= 3600;
  *++text = ':';
  *++text = time / 600 + '0';
  time %= 600;
  *++text = time / 60 + '0';
  time %= 60;
  *++text = ':';
  *++text = time / 6 + '0';
  text[1] = 0;
}

s32 Func_02005020(s32 time) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  s32 second = time % 60;
  s32 volume = 512;
  u8 *ringing = &clock->ringing;

  if (second != 0) {
    *ringing = 1;
  } else if (*ringing) {
    if (clock->timer != 0)
      Event_PlaySound(236);
    *ringing = second;
  }
  if (second <= 19) {
    s32 cosine = Math_Cosine((second << 15) / 10);
    s32 (*mul)(s32 a, s32 b);
    s32 scale = 128;

    mul = Iwram_MulQ16;
    volume = mul(scale << 2, cosine);
  }
  if (volume <= 16)
    volume = 16;
  return volume;
}

#include "affine.h"
#include "graphics/vram_tab.h"

void Func_020055ec(struct Sprite *oam, s32 depth);

/// Draws the clock's five-digit count: copies the pictures of the digits of
/// timer + 50 out of the digit sheet and loads them, builds a plain and a
/// scaled affine matrix, and fills five OAM entries, giving the scaled matrix
/// to the middle digit and to each digit where timer + 60 and timer differ.
void Func_02005090(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  u8 *digit = clock->digit;
  u8 *old = clock->tiles;
  u8 *now = clock->digits;
  u32 tile = gResourceTableEntries[clock->tileEntry].offset >> 5;
  struct Sprite *oam = clock->oam;
  s32 *timer = &clock->timer;
  struct AffineTransform effect;
  s32 plain, scaled, i;
  s32 scale;
  u32 *p;

  Func_02004f84(*timer + 50, digit);
  Func_02004f84(*timer + 60, clock->old);
  Func_02004f84(*timer, clock->now);
  // old and now walk the sheet and the pictures here, the digit lists below.
  for (i = 0; i < 5; i++) {
    Iwram_CopyWords(now, old + (*digit << 6) - 0xc00, 64);
    now += 64;
    digit++;
  }
  Vram_Load(clock->tileEntry, 320, clock->digits);
  effect.angle = 0;
  effect.scaleX = 0x200;
  effect.scaleY = 0x200;
  plain = AffineMatrix_BuildForEffect(&effect);
  scale = Func_02005020(*timer);
  effect.angle = 0;
  effect.scaleX = scale;
  effect.scaleY = scale;
  scaled = AffineMatrix_BuildForEffect(&effect);
  old = clock->old;
  now = clock->now;
  for (i = 0; i <= 4; i++, tile += 2, oam++) {
    p = (u32 *)oam;
    *p++ = 0;
    *p++ = (i << 20) | 0x8000;
    *p = tile | 0x400;
    oam->mode = 3;
    if (*old++ != *now++ || i == 2)
      oam->affine = scaled;
    else
      oam->affine = plain;
    Func_020055ec(oam, 236);
  }
}

#include "ram_buffer.h"

void Func_02005210(void);
void Func_0200525c(void);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

void Func_02005210(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (*(s16 *)(event + 358) == 0 && *(s16 *)(event + 346) == 0 &&
      *(s16 *)(event + 350) == 0 && clock->timer > 0)
    clock->timer--;
  Func_02005090();
}

void Func_0200525c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (*(s16 *)(event + 358) == 0 && *(s16 *)(event + 346) == 0 &&
      *(s16 *)(event + 350) == 0 && clock->timer > 0)
    clock->timer--;
}

#include "task.h"
#include "field.h"
#include "map.h"

void *Heap_AllocEwram(s32 size);
void *Resource_GetTableEntry(s32 index);
void Func_020055cc(const u8 *data, void *buffer);
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
