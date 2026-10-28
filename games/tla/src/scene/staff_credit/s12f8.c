#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
s32 Func_0200209c(void);
void Func_02001274(struct Actor *object, s32 mode);

void Func_020012f8(void) { Func_02001274(Actor_GetById(Func_0200209c()), 0); }
