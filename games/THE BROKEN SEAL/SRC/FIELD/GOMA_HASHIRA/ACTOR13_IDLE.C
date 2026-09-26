#include "TYPES.H"

void Engine_ActorFaceDirection();
void Engine_ActorShowEmote();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorJump();
void Engine_ActorSetAnimation();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][2];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Drive actor 13's idle routine from a frame counter. */
void GomaHashira_DriveActor13Idle(void)
{
    u8 *work;
    s32 t;

    work = *(u8 **)0x03001ebc;
    t = ++*(s32 *)0x020098ec;
    switch (t) {
    case 60:
        Call3(Engine_ActorFaceDirection, 13, 0x2000, 0);
        Engine_ActorShowEmote(13, 2, 0);
        break;
    case 180:
        Engine_ActorStartRepeatedMotion(13, 3);
        break;
    case 240:
    case 270:
        Engine_ActorJump(13, 4, 0);
        break;
    case 480:
        Engine_ActorSetAnimation(13, 4);
        break;
    }
    /* FAKEMATCH: the 99 goes through the counter variable so it is built with
     * movs instead of a halfword pool constant. */
    if (Data_02000240_t[141][0] == 0)
        *(u16 *)(work + 0x182) = t = 99;
}
