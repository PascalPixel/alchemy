#include "TYPES.H"
extern u8 gMenuCtrlWork[];
/* The game state, read here as bytes: the byte at 498 is the retry flag. */
extern u8 gGameState[];

u8 * Engine_ActorGet();
void Engine_ActorSetAnimation();
void Engine_ActorFaceDirection();
void Engine_EventWait();
void Engine_ActorSetSpriteFlags();
void Engine_ObjectSetAnimation();
void Engine_TaskWait();

/* The competitor's starting position and facing, kept in the overlay's
 * variables while a round runs. */
extern s32 Korosseo_CompetitorStartX;
extern s32 Korosseo_CompetitorStartZ;
extern s32 Korosseo_CompetitorStartAngle;

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Colosso: put the competitor back at its stored start position and facing
 * after a round, replaying the fall animation unless the retry flag is set.
 * The same function sits in each of the three Colosso trial overlays. */
void Korosseo_RestoreCompetitor(s32 a0)
{
    u8 *rec7;
    u8 *p7;
    s32 zero;
    /* FAKEMATCH: a one-halfword aggregate holds the zero stored at +34, so
     * it loads as a halfword pool constant whose short range places the
     * literal pool before the epilogue. */
    struct Half {
        u16 v;
    } fall;

    p7 = *(u8 **)gMenuCtrlWork;
    rec7 = Engine_ActorGet(a0);
    if (gGameState[498] == 1) {
        gGameState[498] = 0;
        Engine_ActorSetAnimation(a0, 1);
    } else {
        Call3(Engine_ActorFaceDirection, a0, 0x4000, 30);
        Engine_ActorSetAnimation(a0, 3);
        Engine_EventWait(30);
    }
    zero = 0;
    p7[7] = zero;
    p7[6] = 15;
    *(s32 *)((s32)rec7 + 8) = Korosseo_CompetitorStartX;
    *(s32 *)((s32)rec7 + 16) = Korosseo_CompetitorStartZ;
    *(u16 *)((s32)rec7 + 6) = Korosseo_CompetitorStartAngle;
    *(s32 *)((s32)rec7 + 56) = -0x80000000;
    *(s32 *)((s32)rec7 + 64) = -0x80000000;
    *(s32 *)((s32)rec7 + 36) = zero;
    *(s32 *)((s32)rec7 + 44) = zero;
    fall.v = 0;
    rec7[85] = 3;
    rec7[34] = fall.v;
    *(s32 *)((s32)rec7 + 12) = zero;
    *(s32 *)((s32)rec7 + 20) = zero;
    Engine_ActorSetSpriteFlags((s32)rec7, 1);
    Engine_ObjectSetAnimation((s32)rec7, 0);
    Engine_ObjectSetAnimation((s32)rec7, 1);
    Engine_TaskWait(1);
}
