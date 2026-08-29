#include "types.h"

/*
 * Continuation of the member-orbit-style battle effect whose head is
 * Region_080d4ce8 (games/gs1/asm/080d4ce8.s): this owner begins with no
 * prologue of its own and ends by restoring r8-r11 and the 64-byte frame
 * that Region_080d4ce8 allocates in its own prologue, so the two owners are
 * one physical function split only by the tooling's literal-pool boundary.
 * See games/gs1/src/battle/effects/member_orbit/run.c (Func_080ce85c) for
 * the same template at a different call site: prepare two rectangle-blit
 * routines, run a per-frame loop redrawing a scanline table and orbiting
 * sprites around listed party members, then release the routines.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

s32 Func_08004458(void);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 b);
void Func_080072f4(void *dest, s32 x, s32 y, s32 w, s32 h, void *src);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
void Func_08004278(void *callback);
s32 Func_080cdbc0(void);

void Func_080d4e4c(void);
void Func_080d4ece(void);
void Func_080d4fe0(void);

void Func_080d5094(void)
{
}
