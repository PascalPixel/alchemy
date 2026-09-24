#include "TYPES.H"

void Engine_ColorBufferApplySource();
void Engine_GameFlagSet();
s32 Engine_ActorGet();
void Engine_ActorSetSpritePriority();
void SoruStar_StartActorRing();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
void Engine_MapCopyCellsTo();
void Engine_MapCopyCellAttributes();
void Local_0200227c();
void Scene_EnterStarRoom();


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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Elemental Star Room entry: set flag 0x144, show actors 15..24 above the
 * floor and start their ring, then restore the actors and the opened cells
 * each story flag records. */
s32 SoruStar_ApplyEntryState(void)
{
    u32 i;
    s32 record;
    s32 base5_f;

    Call2(Engine_ColorBufferApplySource, 0x10000, 0);
    Call1(Engine_GameFlagSet, 0x144);
    base5_f = 15;
    do {
        *(u8 *)(Engine_ActorGet(base5_f) + 89) = 0;
        Engine_ActorSetSpritePriority(base5_f++, 1);
    } while ((u32)base5_f <= 24);
    SoruStar_StartActorRing(15, 16);
    if (Value1(Engine_GameFlagIsSet, 0x83b) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0x1c80000, 0x1680000);
        Call3(Engine_ActorSetPosition, 5, 0x1b80000, 0x15a0000);
    }
    if (Value1(Engine_GameFlagIsSet, 0x83c) != 0) {
        Call6(Engine_MapCopyCellsTo, 0, 40, 43, 66, 3, 3);
        Call6(Engine_MapCopyCellsTo, 83, 40, 96, 29, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 41, 29);
        Call6(Engine_MapCopyCellsTo, 87, 42, 41, 31, 1, 2);
        Call6(Engine_MapCopyCellsTo, 83, 40, 74, 29, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 19, 29);
        Call6(Engine_MapCopyCellsTo, 87, 42, 19, 31, 1, 2);
        Call6(Engine_MapCopyCellsTo, 83, 40, 96, 10, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 41, 10);
        Call6(Engine_MapCopyCellsTo, 87, 42, 41, 12, 1, 2);
    }
    if (Value1(Engine_GameFlagIsSet, 0x83d) != 0) {
        Call6(Engine_MapCopyCellsTo, 0, 40, 43, 46, 3, 3);
        Call6(Engine_MapCopyCellsTo, 83, 40, 84, 4, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 29, 4);
        Call6(Engine_MapCopyCellsTo, 87, 42, 29, 6, 1, 2);
        Call6(Engine_MapCopyCellsTo, 83, 40, 76, 21, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 21, 21);
        Call6(Engine_MapCopyCellsTo, 87, 42, 21, 23, 1, 2);
        Call6(Engine_MapCopyCellsTo, 83, 40, 76, 29, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 21, 29);
        Call6(Engine_MapCopyCellsTo, 87, 42, 21, 31, 1, 2);
    }
    if (Value1(Engine_GameFlagIsSet, 0x83e) != 0) {
        Call6(Engine_MapCopyCellsTo, 0, 40, 13, 66, 3, 3);
        Call6(Engine_MapCopyCellsTo, 83, 40, 65, 31, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 10, 31);
        Call6(Engine_MapCopyCellsTo, 87, 42, 10, 33, 1, 2);
        Call6(Engine_MapCopyCellsTo, 83, 40, 79, 9, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 24, 9);
        Call6(Engine_MapCopyCellsTo, 87, 42, 24, 11, 1, 2);
        Call6(Engine_MapCopyCellsTo, 83, 40, 91, 10, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 36, 10);
        Call6(Engine_MapCopyCellsTo, 87, 42, 36, 12, 1, 2);
        Local_0200227c();
    }
    if (Value1(Engine_GameFlagIsSet, 0x83b) == 0) {
        if (Data_02000240_t[225][0] == 10) {
            Scene_EnterStarRoom();
        }
    }
    return 0;
}
