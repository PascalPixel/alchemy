/* NONMATCHING: resource_39a at 0x02009fac (104 bytes with its pool),
 * ImiruFuchin_StartFadeIn, between FIELD/COMMON/IMIRU_FUCHIN/CALLBACKS.C and
 * HEADING.C, stays listing. It was FIELD/COMMON/IMIRU_FUCHIN/FADE_IN.C.
 *
 * Remaining difference: the reference loads the effect work pointer's
 * address (0x03001f30) from its pool and reaches the light-flag work pointer
 * 100 bytes below it with one subtract, as two members of one resident
 * structure do when both lie beyond a load's reach from its start. The main
 * image names them as separate variables (gEffectWork and Data_03001ecc),
 * which load separately; spelled as gWork[29] and gWork[4] they share gWork's
 * base register instead (54 differing bytes). The structure that holds them
 * is not yet recovered.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 *gWork[];

void Engine_ColorBufferApplySource();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

struct FadeWork {
    u8 unknown_00[52];
    u8 active;
};

/* While the stage is early enough, start the fade-in: raise the fade flag,
 * set the three light flags and interpolate the palette over 16 frames. */
void ImiruFuchin_StartFadeIn(void)
{
    struct FadeWork *fade;
    u8 *work;

    if (gGameState.entrance <= 6) {
        fade = (struct FadeWork *)gWork[29];
        work = gWork[4];
        fade->active = 1;
        work[0x53e] = 0;
        work[0x53c] = 1;
        work[0x53d] = 1;
        Engine_ColorBufferApplySource(0, 1);
        Call2(Engine_ColorBufferApplyTarget, 0x203108, 1);
        Engine_ColorBufferInterpolate(16);
        Engine_TaskWait(16);
    }
}
