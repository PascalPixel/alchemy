// Draft, differ 4 of 106: on entering a scene, looks up Data_080ee6d4 (8-byte
// entries: scene, entrance or -1, a flag that must be clear or -1, and two
// bytes; a scene of -1 ends it) when search is set, and writes the matching
// entry's two bytes (0 without one) and six zeros to the event work's
// settings at 0x18c, zeroes 0x194, sets 0x198 to 0x100000 and runs
// Func_080ca4a8. The two bytes live in stack slots in the native as here.
// Left: instruction order only: the second byte's load and store are
// scheduled around the pointer step the other way.
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

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
  s32 scene = gPartyState.scene;
  s32 entrance = gPartyState.entrance;
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
