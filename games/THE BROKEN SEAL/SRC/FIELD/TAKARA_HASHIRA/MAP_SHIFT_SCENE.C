#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
void Engine_EventBegin();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_MapCopyCellsTo();
s32 Engine_TaskAddCallback();
s32 Engine_ActorGet();
void Engine_ActorSetAnimation();
void Main_0808a118();
void Engine_MapCopyCellAttributes();
void Engine_GameFlagClear();
void Engine_ActorSetChildValue();
void Engine_ActorEnableActionCallback();
void Engine_TaskRemoveCallback();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Crossbone Isle: the first time (flag 0x203 clear) set flag 0x202, pan the
 * camera and shift the cells at column 73 while a task runs; with flag
 * 0x201, actor 12 plays its part and the cells at (17, 13) are copied. */
void TakaraHashira_RunMapShiftScene(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_GameFlagIsSet, 0x203);
    if (rec7 == 0) {
        Call1(Engine_GameFlagSet, 0x202);
        Engine_EventBegin();
        Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
        Call4(Engine_CameraMoveTo, 0x1380000, -1, 0xb80000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(20);
        Call6(Engine_MapCopyCellsTo, 73, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_TaskAddCallback(0x200a231, 0xc80);
        Engine_EventWait(40);
        if (Value1(Engine_GameFlagIsSet, 0x201) != 0) {
            record = Engine_ActorGet(12);
            *(s32 *)(record + 108) = 0x200a2d9;
            Engine_ActorSetAnimation(12, 6);
            Main_0808a118(12);
            record = Engine_ActorGet(12);
            *(s32 *)(record + 108) = rec7;
            Call6(Engine_MapCopyCellAttributes, 17, 13, 1, 1, 18, 13);
            Call1(Engine_GameFlagClear, 0x201);
            Engine_ActorSetChildValue(12, 0);
            Engine_ActorEnableActionCallback(12, 1);
        } else {
            Engine_EventWait(60);
        }
        Call1(Engine_TaskRemoveCallback, 0x200a231);
        Engine_EventWait(20);
        Call6(Engine_MapCopyCellsTo, 72, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_EventEnd();
    }
}
