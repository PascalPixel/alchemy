#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"

struct EventObjects {
  u8 unk00[20];
  struct Actor *objects[64];
};

struct Actor *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02003b84(s32 a, s32 b);

void Func_02000594(void) { Func_02003b84(8, 56); }

s32 Func_020005a0(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}
