// Draft, differ 9 of 35, skeleton gap 4 (tla-en, 2026-11-01)
// Overlay 679 holds five functions here, so measure each with --symbol. The
// first line gives the worst of each figure: differ from Func_02000ef8, gap
// from Func_02001054.
//
// Func_02000ef8, differ 9 of 35, skeleton gap 3 (removed 1, added 2): the only
// one of the five not adopted. Its one structural difference is a pool branch.
// The 64-byte reach of the HImode pool load of 0x100c forces a pool ahead of
// the shared tail store, with a b.n around it. The native loads 0x100c after
// movs r2, #128; here it comes first, because it has two dependents (the store
// and the next write of r3) against one and sched2 prefers more. That two bytes
// is what keeps the pool out of the way. A one-pass block around the
// REG_BG1HOFS store, or around the two blend stores together, gives differ 0
// (around REG_BLDALPHA alone, differ 2): a tagged one-pass device would close
// this, not tried here as a kept form. Plain forms, none better: value as u16,
// s16 or s32 or in inner blocks; a separate local for the constants or the
// value reused for them; a pointer to the blend registers; a shared REG_BLDCNT
// store after the arms; early return, goto, inverted or reversed compare; a
// VCOUNT local; casts on the constants. Overlay 690's Func_02000e30 is the twin
// and fails the same way.
//
// Func_02000f58, differ 2 of 24, gap 2: adopted in events_f.c with a one-pass
// block. The ldr of gData_02002a9c comes after the ldrsh instead of before it.
// Func_02001054, differ 4 of 35, gap 4 with the tagged devices: adopted in
// s1054.c. Both pairs are order only: ldr r0 before lsls r1 at Task_Add, and
// lsls r1 before the small constants at Func_02001e04. s1054.c's one-pass form
// measured here is differ 2, and needs its own prototypes, which conflict with
// local.h's; Task_Add_ and Func_02001e04_ change nothing.
// Func_020010d8 and Func_020017bc: differ 0, adopted in s10d8.c and s17bc.c.
#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "map.h"

void Func_02000ef8(void) {
  u32 value;

  if (REG_VCOUNT < gData_02002a98) {
    value = gData_02002a9c;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gData_02002a9e;
    /* FAKEMATCH(one-pass): the once-only block orders this statement; without it, 9 of 35 instructions
     * differ in tla-en. */
    do {
      REG_BG1HOFS = value;
    } while (0);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}

void Func_02000f58(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 *scroll = (s16 *)&map->layers[2];
  s32 hofs;

  gData_02002a98 = 0x5a8 - scroll[3];
  gData_02002a9c = hofs = scroll[1];
  gData_02002a9e = hofs - (gFrameCount >> 2);
}

void Func_02001054(void) {
  struct MainRoutineWork *party;
  s16 *entrance;

  Task_Add(Func_020003e8, 0x480);
  Func_02001dfc();
  Func_02001e04(0, 0x200, 8, 9);
  Func_02001dec(170);
  Func_02000f98();
  party = &gMainRoutineWork;
  /* FAKEMATCH: the native loads gMainRoutineWork before it builds the entrance
   * offset. */
  __asm__ volatile("" : "+l"(party));
  entrance = &party->entrance;
  if (*entrance == 3)
    Func_02001500();
  /* FAKEMATCH: the native reloads the entrance for each test; gcse shares one
   * halfword load without the barrier. */
  __asm__ volatile("" : "+l"(entrance));
  if (*entrance == 4)
    Func_02001500();
  /* FAKEMATCH: the native reloads the entrance for each test; gcse shares one
   * halfword load without the barrier. */
  __asm__ volatile("" : "+l"(entrance));
  if (*entrance == 5)
    Func_02001500();
}

void Func_020010d8(struct Actor *object) {
  struct Actor *child = Import_ActorCreate(
      14, object->x, object->y + 0x40000, object->z + 0x40000);
  struct FieldSprite *parentSprite = object->animation;

  if (child != 0) {
    struct FieldSprite *sprite = child->animation;

    child->terrainHeight = object->terrainHeight;
    Func_02001c74_(child, gData_02001ee8);
    child->motionFlags = 0;
    if (sprite != 0) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = parentSprite->priority;
    }
  }
}

void Func_020017bc(void) {
  s32 priority;

  u16 *work = gData_02002aa0;

  Iwram_ClearWords(work, 0x1004);
  *work = Resource_FindFreeEntry();
  /* FAKEMATCH: the native stores the entry before it starts the size (differ 2
   * without the barrier). */
  __asm__("" : "+m"(*(u8 *)work));
  VramCache_Load(*work, 0x200, 0);
  priority = 0x480;
  /* FAKEMATCH: the native builds the priority before loading the task function. */
  __asm__ volatile("" : "+r"(priority));
  Task_Add(Func_020015f8, priority);
}
