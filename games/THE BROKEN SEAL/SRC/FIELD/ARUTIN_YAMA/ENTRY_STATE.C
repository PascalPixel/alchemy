#include "TYPES.H"
extern u8 ArutinYama_ActorScript[];

s32 Engine_GameFlagIsSet();
void FieldScene_RunOpeningAuxiliarySequence();
void Local_02003a44();
u8 * Engine_ActorSetPosition();
u8 * Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetChildValue();
void Engine_ActorSetAnimation();
void Engine_MapCopyCellAttributes();
void SceneState_StoreParamsAndInstallTask();
void Engine_ObjectSetTargetAndCallback();
void SceneActor_ClearCollisionFlagAndPlaceMarker();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

/* Altin Peak entry: run the opening at entrance 99 before flag 0x109, then
 * place the scene actors and restore the cells each story flag records. */
void ArutinYama_ApplyEntryState(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    u8 *record;
    u8 *p5;

    if (Value1(Engine_GameFlagIsSet, 0x109) == 0) {
        if (Data_02000240_t[225][0] == 99) {
            FieldScene_RunOpeningAuxiliarySequence();
        }
    }
    Local_02003a44();
    if (Value1(Engine_GameFlagIsSet, 0x8fd) == 0) {
        Engine_ActorSetPosition(8, 0, 0);
    } else {
        record = Value1(Engine_ActorGet, 8);
        if ((s32)record != 0) {
            record[35] = 2;
            Engine_ActorSetSpriteFlags((s32)record, 0);
        }
    }
    record = Value1(Engine_ActorGet, 9);
    if ((s32)record != 0) {
        record[35] = 2;
        Engine_ActorSetSpriteFlags((s32)record, 0);
    }
    rec7 = Value1(Engine_GameFlagIsSet, 0x8fd);
    if (rec7 == 0) {
        Engine_ActorSetChildValue(10, 2);
        rec8 = Value1(Engine_GameFlagIsSet, 0x905);
        if (rec8 != 0) {
            Engine_ActorSetAnimation(9, 0);
            record = Engine_ActorGet(9);
            *(s32 *)((s32)record + 108) = 0x200ace1;
            *(u8 *)(Engine_ActorGet(9) + 85) = rec7;
            record = Engine_ActorGet(9);
            *(s32 *)((s32)record + 12) = 0x200000;
            Call6(Engine_MapCopyCellAttributes, 2, 0, 1, 1, 18, 13);
            Call3((void (*)())Engine_ActorSetPosition, 10, 0x780000, 0xd70000);
            record = Engine_ActorGet(10);
            *(u16 *)((s32)record + 6) = rec7;
            Engine_ActorSetAnimation(10, 3);
            SceneState_StoreParamsAndInstallTask(0x820000, 0, 0xa80000, 0);
            goto L_020022de;
        }
        if (Value1(Engine_GameFlagIsSet, 0x904) == 0) {
            goto L_020022de;
        }
        Engine_ActorSetAnimation(9, 0);
        record = Engine_ActorGet(9);
        *(s32 *)((s32)record + 108) = 0x200ace1;
        *(u8 *)(Engine_ActorGet(9) + 85) = rec8;
        record = Engine_ActorGet(9);
        *(s32 *)((s32)record + 12) = 0x200000;
        Call6(Engine_MapCopyCellAttributes, 2, 0, 1, 1, 18, 13);
        Call3(Engine_ActorSetPosition, 10, 0x1040000, 0xd70000);
        {
            u8 *script = ArutinYama_ActorScript;

            Value3(Engine_ObjectSetTargetAndCallback, 10, 0x10000, (s32)script);
        }
    } else {
        p5 = *(s32 *)0x03001e70;
        Call3((void (*)())Engine_ActorSetPosition, 10, 0, 0);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 2, 3, 14);
        *(u16 *)((s32)p5 + 20) &= 0xfdff;
        SceneActor_ClearCollisionFlagAndPlaceMarker(8);
        if (Value1(Engine_GameFlagIsSet, 0x200) != 0) {
            Engine_ActorSetAnimation(8, 5);
            Call6(Engine_MapCopyCellAttributes, 7, 13, 1, 1, 9, 13);
            record = Engine_ActorGet(8);
            *(s32 *)((s32)record + 12) = 0;
            {
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 2);
            }
        }
        SceneActor_ClearCollisionFlagAndPlaceMarker(9);
        if (Value1(Engine_GameFlagIsSet, 0x201) != 0) {
            Engine_ActorSetAnimation(9, 5);
            Call6(Engine_MapCopyCellAttributes, 29, 1, 3, 1, 17, 13);
            record = Engine_ActorGet(9);
            *(s32 *)((s32)record + 12) = 0x200000;
            {
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 2);
            }
        }
    }
    L_020022de:;
}
