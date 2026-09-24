/* NONMATCHING: 104 bytes, candidate 108, 17 differing halfwords (2026-09-24).
 * Remaining: the reference stores the fade flag before the work offsets and
 * derives 0x53c from the loaded 0x53e (one fewer literal). */
#include "TYPES.H"

void Engine_ColorBufferApplySource();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();
void Engine_TaskWait();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void ImiruFuchin_Func02001fac(void)
{
    u8 *fade;
    u8 *work;

    if (Data_02000240_t[225][0] <= 6) {
        fade = *(u8 **)0x03001f30;
        work = *(u8 **)0x03001ecc;
        fade[52] = 1;
        work[0x53e] = 0;
        work[0x53c] = 1;
        work[0x53d] = 1;
        Engine_ColorBufferApplySource(0, 1);
        Call2(Engine_ColorBufferApplyTarget, 0x203108, 1);
        Engine_ColorBufferInterpolate(16);
        Engine_TaskWait(16);
    }
}
