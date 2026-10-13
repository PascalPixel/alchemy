/* Draft, differ 2 as written, differ 0 with The Lost Age's identity helper: the native builds the
 * identity's 1.0 before its zeros (mov r1, #128; lsl r1, r1, #9; mov r2, #0; mov r3, #0;
 * mov r4, #0; then the three stmia), where transform.h's proven Transform_SetIdentity is in
 * The Broken Seal's order (the lsl after the zeros). With a copy of that helper in the native
 * order this function is differ 0, and all twelve editions compared OK with it linked from
 * render.c (2026-10-12, then under games/common, now games/tla/src/graphics/render). It is
 * not adopted because that reordered statement has no reviewed row in
 * tools/alchemy/evidence.tsv, so check no-asm refuses it: once the helper is reviewed, define
 * it for GAME_TLA in transform.h and move this function into render.c beside its neighbours
 * (xfsetpos.c and xfrot.c then fold back in). */
#include "system.h"
#include "types.h"
#include "transform.h"
#include "fixed_math.h"
#include "iwram_call.h"

#define COS(a) Trig_Sin((a) + 0x4000)

void SceneTransform_SetYaw(s32 ang)
{
	s32 *m = gTransform;
	s32 sin, cos;

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(m);
	cos = COS(ang);
	m[0] = cos;
	m[8] = cos;
	sin = Trig_Sin(ang);
	m[2] = -sin;
	m[6] = sin;
}
