#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "mem.h"

struct SparkWork {
  u8 unk0000[0x2102];
  u16 tileBase;
  u8 unk2104[0x80];
  s16 resource;
};

void Task_RemoveFar(void (*callback)(void));

void VenusLighthouseEntrance_StopSparks(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_0200258c);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
