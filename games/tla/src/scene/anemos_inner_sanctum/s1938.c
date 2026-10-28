#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_020017b4(void);
void Func_02003458(s32 a, s32 b);
void Func_02003468(s32 a);
void Func_020034a8(void);

void Func_02001938(void) {
  struct Actor *object;

  if (gAdventure.entrance == 1)
    object = Actor_GetById(8);
  else if (gAdventure.entrance == 3)
    object = Actor_GetById(9);
  else
    object = Actor_GetById(10);
  if (*(s16 *)((u8 *)Actor_GetById(gAdventure.currentOwner) + 18) <= 247) {
    object->motionFlags = 0;
    object->update = Func_020017b4;
    object->unk4c = Actor_GetById(gAdventure.currentOwner)->x;
    object->acceleration = Actor_GetById(gAdventure.currentOwner)->z;
    GameFlag_SetBit(0x200);
  }
}
