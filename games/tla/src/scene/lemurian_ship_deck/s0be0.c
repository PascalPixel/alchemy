#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "scene_ids.h"

extern const u8 gData_020020dc[];

struct Actor *Actor_GetById(s32 id);
void Task_Add(void (*func)(void), s32 priority);
void Func_020020c4(const void *data);
void Func_02000188(void);
void Func_020001cc(void);
void Func_02000210(void);
void Func_02000b84(void);

struct MapMode {
  u8 unk00[23];
  struct MapModeBits {
    u8 low : 2;
    u8 mode : 2;
  } __attribute__((packed)) bits;
};

static __inline__ void AddTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Task_Add(func, priority);
}

s32 Func_02000be0(void) {
  struct Adventure *party;
  struct MapMode *map = (struct MapMode *)Ram_HeapSlots->mapWork;
  struct MapModeBits bits = map->bits;

  bits.mode = 1;
  /* FAKEMATCH: a one-byte copy with the loop note before its store loads the
   * adventure's address between the or and the store; as map->bits.mode = 1,
   * 2 instructions differ in tla-en. */
  do {
  } while (0);
  party = &gAdventure;
  map->bits = bits;
  if (party->scene == (s32)&SceneId_Ov657Room1) {
    Func_02000b84();
  } else {
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
    /* FAKEMATCH: the loop note ends the scheduling block after the store;
     * without it, tla-en differs. */
    do {
    } while (0);
    Actor_GetById(party->currentOwner)->priorityFlags |= 0x20;
    Actor_SetModeById(8, 1);
    Func_020020c4(gData_020020dc);
    AddTask(Func_02000188, 0x480);
    if (party->entrance == 99)
      Func_02000210();
    else
      AddTask(Func_020001cc, 0x480);
  }
  return 0;
}
