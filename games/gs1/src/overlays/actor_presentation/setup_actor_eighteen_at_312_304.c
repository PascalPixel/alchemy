#include "types.h"

#define ActorPresentation_SetupActorEighteenAt312_304 Func_02000af0

/* Complete scene-variant six actor-zero setup wrapper through its pool. */
extern const u8 Data_02009756[];

extern void Func_02001ca6(s32 value);
extern void Func_02001b88(const void *tbl, s32 value, s32 actor);
extern void Func_0200158e(s32 x, s32 y, s32 value);
void ActorPresentation_SetupActorEighteenAt312_304(void)
{
    Func_02001ca6(158);
    Func_02001b88(Data_02009756, 50, 18);
    Func_0200158e(312, 304, 6);
}
