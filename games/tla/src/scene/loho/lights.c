#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

extern const u8 gData_020017d8[];
extern const u8 gData_020017a4[];
extern const u8 gData_0200171c[];
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
void Actor_SetMovementTypeById(s32 id, const u8 *callback);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 value);
void Func_02001400(s32 a, s32 b, s32 c);
void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_020013f8(s32 value);
void Func_02000f38(void);

void Func_02000c54(s32 id) {
  s32 high = 192;
  s32 kind = 64;

  /* FAKEMATCH: ⚓️ sets the kind between the halves of the value */
  asm("" : "+l"(high), "+l"(kind));
  Func_020013f0(id, kind, high << 2);
}

void Func_02000c64(s32 id) {
  s32 value = 768;
  s32 kind = 65;

  /* FAKEMATCH: ⚓️ sets the kind between the halves of the value */
  asm("" : "+l"(value), "+l"(kind));
  Func_020013f0(id, kind, value + 1);
}

void Func_02000c74(s32 id) {
  s32 value = 768;
  s32 kind = 66;

  /* FAKEMATCH: ⚓️ sets the kind between the halves of the value */
  asm("" : "+l"(value), "+l"(kind));
  Func_020013f0(id, kind, value + 2);
}
