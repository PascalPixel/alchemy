#include "mem.h"
#include "types.h"
#include "owner_state.h"
#include "serial_runtime.h"

void Message_DecodeRawUnboundedFar(s32, u16 *);
void MemFree(void *);
void WaitFrames(s32);
void *Resource_FarCall005(s32);

extern char MsgEnemyLabel;

s32 LinkBattle_ReceiveParty(void) {
  u16 name_text[16];
  void *buffer;
  u8 *name_entry;
  s32 named_count;
  s32 index;
  s32 len;
  s32 i;

  buffer = Runtime_BumpAllocateAlternatePool(340);
  named_count = 0;
  index = 0;
  while (index <= 2) {
    name_entry = Unit_Get(index + 128);
    if (Serial_StartReceive(name_entry) == -1) {
      break;
    }
    Serial_WaitForReceive();
    if (name_entry[298] != 0) {
      named_count += 1;
    }
    WaitFrames(2);
    Message_DecodeRawUnboundedFar((s32)&MsgEnemyLabel, name_text);
    i = 0;
    if (name_text[i] != 0) {
      do {
        i += 1;
        if (i > 4) {
          break;
        }
      } while (name_text[i] != 0);
    }
    len = i;
    for (i = 14; i >= len; i--) {
      name_entry[i] = name_entry[i - len];
    }
    for (i = 0; i < len; i++) {
      name_entry[i] = (u8)name_text[i];
    }
    name_entry[14] = 0;
    index += 1;
  }
  MemFree(buffer);
  buffer = Runtime_BumpAllocateAlternatePool(340);
  if (Serial_StartReceive(Resource_FarCall005(1)) != -1) {
    Serial_WaitForReceive();
    WaitFrames(2);
  }
  MemFree(buffer);
  return named_count;
}
