#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"

s32 GameFlag_Test(s32 flag);
void Func_080ca4a8(void);

struct SceneMusicEntry {
  s16 scene;
  s16 entrance; // -1 any
  s16 flag;     // -1 none; the entry applies while the flag is clear
  u8 first;
  u8 second;
};

extern const struct SceneMusicEntry Data_080ee6d4[];

struct EventWorkView {
  u8 unk000[0x18c];
  u8 settings[8];
  s32 unk194;
  s32 unk198;
};

void Func_080c9e48(s32 search) {
  struct EventWorkView *work = Ram_HeapSlots->eventWork;
  s32 scene = gAdventure.scene;
  s32 entrance = gAdventure.entrance;
  u8 *out = work->settings;
  u32 first = 0;
  u32 second = 0;
  const struct SceneMusicEntry *entry = Data_080ee6d4;
  u32 i;

  if (search != 0) {
    for (; entry->scene != -1; entry++) {
      if (entry->scene != scene)
        continue;
      if (entry->entrance != -1 && entry->entrance != entrance)
        continue;
      if (entry->flag != -1 && GameFlag_Test(entry->flag))
        continue;
      first = entry->first;
      second = entry->second;
      break;
    }
  }
  *out++ = first;
  *out++ = second;
  for (i = 0; i < 6; i++)
    *out++ = 0;
  work->unk194 = 0;
  work->unk198 = 0x100000;
  Func_080ca4a8();
}
