#include "types.h"

#define FieldScene_RunOpeningSequenceHead Func_020019a4

void Func_02006736();
void Func_0200676c();
void Func_02006846();
void Func_020068f8();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006736, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200676c, a0, a1, a2)
#define Audio_PlayCue_1(a0) Value1(Func_020068f8, a0)
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Word at +456 of the shared scene work record. */
#define SCENE_WORD_1C8 (*(u32 *)(*(u8 **)0x03001ebc + 456))

/* Sets up the opening sequence: two calls with fixed argument pairs, a
 * write to the scene work record, and two more calls with fixed args. */
void FieldScene_RunOpeningSequenceHead(void)
{
    ObjectMotion_SetSpeedParameters_1(0, 32768, 16384); /* object_id 0, speed_limit 32768, acceleration 16384 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 728, 408); /* object_id 0, x 728, z 408 */
    SCENE_WORD_1C8 = 16;
    Audio_PlayCue_1(123);
    Call1(Func_02006846, 15); /* main:0808a248 */
}
