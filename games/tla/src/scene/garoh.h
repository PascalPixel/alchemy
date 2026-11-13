#ifndef TLA_GAROH_H
#define TLA_GAROH_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "flag_call.h"

#ifdef MODERN
// The rope routine is called with no prototype in scope, as its bytes need:
// called as returning int, the two post ids load before the first argument.
// A current GCC gets it here.
void Rope_Lash(s32 a, s32 b, s32 c);
#endif
void Actor_PlaceBlocks(const void *data);
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);

void Func_02000e78(s32 id, struct Actor *object);
void Func_02000eb0(void);
void Func_02000eec(void);
void Func_02000f28(void);
void Func_02000f3c(void);
void Func_020012a8(s32 id);

#endif
