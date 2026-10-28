#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

extern char MsgLoho17;
extern char MsgLoho19;
extern char MsgLoho51;
extern char MsgLoho53;

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001380(s32 message);
void Func_02001390(s32 id, s32 flags);
void Func_02001430(s32 a, s32 b);
void Func_02001438(s32 id);

void Func_02000d4c(void) {
  Object_GetById(gAdventure.currentOwner);
  if (GameFlag_Test(800)) {
    Func_02001430(30, 8);
  } else {
    if (GameFlag_Test(0xa5f))
      Func_02001380((s32)&MsgLoho51);
    else
      Func_02001380((s32)&MsgLoho17);
    Func_02001390(8, 0);
  }
}

void Func_02000da4(s32 actor) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);

  /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as
   * plain statements the constant is built first (differ 2) */
  do {
    u16 facing = (leader->angle + 0x2000) & ~0x3fff;

    if (facing == 0xc000) {
      Func_02001438(actor);
    } else {
      if (GameFlag_Test(0xa5f))
        Func_02001380((s32)&MsgLoho53);
      else
        Func_02001380((s32)&MsgLoho19);
      Func_02001390(actor, 0);
    }
  } while (0);
}
