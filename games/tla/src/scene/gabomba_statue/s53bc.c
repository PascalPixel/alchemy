#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
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
    struct Actor *object = Actor_GetById(id);
    s16 mode = entry[0];
    s16 flag = entry[1];

    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_020054ac(id, (u16)mode, (u16)flag, object->terrainId);
    entry += 2;
  }
}
