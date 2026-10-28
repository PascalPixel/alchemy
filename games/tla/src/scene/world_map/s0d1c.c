#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02004c20(void);
void Func_02004c28(void);
s32 Func_02004c38(s32 a, s32 b, s32 c);
void Func_02004c90(s32 id, s32 x, s32 z, s32 a);
void Func_02004db8(s32 value);
void Func_02004e18(s32 id, s32 a);

void Func_02000d1c(void) {
  Func_02004c20();
  Func_02004db8(0);
  if (Value3((s32 (*)())Func_02004c38, 19, 0x198, -1) == 0)
    GameFlag_ClearBit(0xa77);
  Func_02004c28();
}

void Func_02000d4c(void) {
  struct Actor *object;
  u8 *animation;
  s32 a, id;

  Func_02004e18(19, 0x198);
  object = Actor_GetById(19);
  animation = object->animation;
  animation[17] |= 1;
  Actor_SetSingleChildFlags(Actor_GetById(19), 0);
  a = 0x4000;
  id = 19;
  /* FAKEMATCH: the native builds the last argument, then the id, before it
   * loads the two positions; a direct call and a forwarding inline load the
   * positions first. */
  __asm__ volatile("" : "+r"(a), "+r"(id));
  Func_02004c90(id, 0x27b80000, 0x25b00000, a);
  GameFlag_SetBit(0xa77);
}
