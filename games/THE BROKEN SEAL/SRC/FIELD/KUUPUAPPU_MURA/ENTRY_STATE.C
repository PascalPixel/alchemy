#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_EventRequestExit();
void Engine_MapCopyCellAttributes();
void OverlayObject_CreateConfiguredObject();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
s32 Engine_GameFlagClear();
void Engine_EventBegin();
void Engine_ActorFaceEachOther();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Main_0808a048();
s32 Engine_EventEnd();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 KuupuappuMura_RestoreEntryState(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v7;
    s32 zero;

    if (Value1(Engine_GameFlagIsSet, 0x87a) != 0) {
        Engine_EventRequestExit(14);
    }
    if (Value1(Engine_GameFlagIsSet, 0x200) != 0) {
        Call6(Engine_MapCopyCellAttributes, 55, 26, 4, 2, 23, 26);
    }
    OverlayObject_CreateConfiguredObject(0x800000, 0, 0x1a40000, 223);
    v7 = 1;
    zero = 0;
    record = Engine_ActorGet(14);
    *(u16 *)(record + 100) = v7;
    *(s32 *)(record + 108) = 0x2008315;
    record = Value1(Engine_ActorGet, 15);
    *(u16 *)(record + 100) = zero;
    /* FAKEMATCH: the empty loop keeps the halfword store ahead of the
     * script-pointer store, as in the reference schedule. */
    do { } while (0);
    *(s32 *)(record + 108) = 0x2008315;
    if (Value1(Engine_GameFlagIsSet, 0x858) != 0) {
        Call3(Engine_ActorSetPosition, 19, 0xd80000, 0x1880000);
    }
    rec7 = Value1(Engine_GameFlagIsSet, 0x853);
    if (Value1(Engine_GameFlagIsSet, 0x855) == 0) {
        if ((v7 & rec7) != 0) {
            record = Engine_ActorGet(21);
            *(s32 *)(record + 108) = 0x20083ad;
        }
    }
    if (Data_02000240_t[225][0] <= 2) {
        if (Value1(Engine_GameFlagIsSet, 0x109) == 0) {
            Call1(Engine_GameFlagClear, 0x867);
            if (Value1(Engine_GameFlagIsSet, 0x855) == 0) {
                if (Value1(Engine_GameFlagIsSet, 0x856) != 0) {
                    Engine_EventBegin();
                    record = Value1(Engine_ActorGet, 0);
                    if (record != 0) {
                        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
                    }
                    if (Data_02000240_t[225][0] == 1) {
                        Call3(Engine_ActorSetPosition, 2, 0x1900000, 0x1c00000);
                    } else {
                        Call3(Engine_ActorSetPosition, 2, 0xe00000, 0xa20000);
                    }
                    Engine_ActorFaceEachOther(2, 0, 0);
                    Engine_EventOpenScreen();
                    Engine_EventWaitForScreen();
                    Engine_EventWait(30);
                    Engine_ActorRunRepeatedMotion(2, 2);
                    Call1(Engine_EventSetMessage, 0x1328);
                    Engine_EventShowMessageAndWait(2, 0, 20);
                    Engine_ActorSetAnimationAndWait(0, 3);
                    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
                    Engine_ActorSetAnimation(2, 2);
                    record = Value1(Engine_ActorGet, 0);
                    if (record != 0) {
                        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
                    }
                    Engine_ActorWaitForMove(2);
                    Engine_ActorSetPosition(2, 0, 0);
                    Main_0808a048(2, 0);
                    ((void (*)())Engine_EventEnd)();
                }
            }
        }
    }
    if (Value1(Engine_GameFlagIsSet, 0x867) != 0) {
        Call3(Engine_ActorSetPosition, 23, 0x1980000, 0x780000);
    }
    if (Data_02000240_t[225][0] == 11) {
        if (Value1(Engine_GameFlagIsSet, 0x855) == 0) {
            if (Value1(Engine_GameFlagIsSet, 0x856) != 0) {
                if (Value1(Engine_GameFlagIsSet, 2) == 0) {
                    Engine_EventBegin();
                    Call3(Engine_ActorSetPosition, 2, 0x280000, 0x1360000);
                    Engine_ActorFaceEachOther(2, 0, 0);
                    Engine_EventOpenScreen();
                    Engine_EventWaitForScreen();
                    Engine_EventWait(30);
                    Engine_ActorRunRepeatedMotion(2, 2);
                    Call1(Engine_EventSetMessage, 0x1328);
                    Engine_EventShowMessageAndWait(2, 0, 20);
                    Engine_ActorSetAnimationAndWait(0, 3);
                    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
                    Engine_ActorSetAnimation(2, 2);
                    record = Value1(Engine_ActorGet, 0);
                    if (record != 0) {
                        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
                    }
                    Engine_ActorWaitForMove(2);
                    Engine_ActorSetPosition(2, 0, 0);
                    Main_0808a048(2, 0);
                    Engine_EventEnd();
                }
            }
        }
        Call1(Engine_GameFlagClear, 0x12f);
    } else {
        if (Data_02000240_t[225][0] == 13) {
            if (Value1(Engine_GameFlagIsSet, 0x855) != 0) {
                Engine_ActorSetPosition(20, 0, 0);
            }
        }
    }
    return 0;
}
