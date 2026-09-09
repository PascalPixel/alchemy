#include "types.h"

/* Rect setup for resource_3bb.  Each call site is spelled with its own import
 * name even though several of them reach the same import. */
void Func_020044ca();
s32 *Func_02004598();
void Func_020044e4();
s32 *Func_020045b2();
void Func_020044fe();
s32 *Func_020045cc();
void Func_0200451a();

/*
 * One fixed line, then three whose fifth or sixth argument is a field of the
 * record fetched for participants 15, 16 and 17.  Records 15 and 16 contribute
 * their word at +8, record 17 its word at +16, which moves to the sixth
 * argument slot while a literal 18 takes the fifth.  The shift is arithmetic,
 * so the fields are signed fixed-point with 20 fractional bits.  Only those two
 * fields are asserted; what the six arguments mean is not established here.
 */
void SceneState_ApplyRectsForActors15To17(void)
{
    s32 field;

    Func_020044ca(100, 11, 12, 4, 14, 11);

    field = Func_02004598(15)[2] >> 20;
    Func_020044e4(13, 28, 1, 4, field, 11);

    field = Func_020045b2(16)[2] >> 20;
    Func_020044fe(13, 28, 1, 4, field, 11);

    field = Func_020045cc(17)[4] >> 20;
    Func_0200451a(13, 28, 4, 1, 18, field);
}
