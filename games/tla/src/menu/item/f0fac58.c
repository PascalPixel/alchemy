#include "types.h"
#include "ram_buffer.h"

struct ItemMenuIcons {
  u8 unk000[0xf8];
  void *ownerIcons[8];
  u8 unk118[0xfd];
  u8 partyCount;
};

void Animation_ApplyChildArgumentFar(void *object, s32 argument);
void Task_Remove(void (*func)(void));
void Func_080fabe0(void);

void Func_080fac58(void) {
  struct ItemMenuIcons *menu = Ram_HeapSlots->menuRuntime;
  s8 i;

  for (i = 0; i < menu->partyCount; i++)
    Animation_ApplyChildArgumentFar(menu->ownerIcons[i], 1);
  Task_Remove(Func_080fabe0);
}
