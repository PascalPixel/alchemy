#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_0808a5e0(s32 value);
void ConfigureAndPlaceActorFourteen(void);
void FieldScene_RunScene3a2SequenceA(void);
void ActorPresentation_PrepareActorFourteenWithCallback(void);
void FieldScene_SetSlot15Byte89AndRunStep(void);
void Scene_RunActorExchange(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000058[];
extern u8 Data_0000004a[];

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

/* Mountain entry: set the entrance selector; in area 0x58 stage the actors and the entrance scene, in area 0x4a restore the exchange scene's actors and cells from its story flags. */
s32 YamaRama_ApplyEntryState(void)
{
    s32 entrance;

    gEventWork->start_transition = 0x100;
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000058) {
        Main_0808a5e0(169);
        Engine_ActorSetAnimation(11, 5);
        Engine_ActorSetAnimation(12, 5);
        Engine_ActorSetAnimation(14, 2);
        Call6(Engine_MapCopyCellAttributes, 21, 9, 1, 1, 21, 73);
        ConfigureAndPlaceActorFourteen();
        if (Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Call3(Engine_ActorSetPosition, 13, 0x880000, 0x1000000);
            Engine_ActorFaceDirection(13, 0, 0);
        }
        entrance = Data_02000240_t.halves[225][0];
        if (entrance == 2) {
            Engine_GameFlagClear(0x12f);
        } else if (entrance == 3 && !Value1(Engine_GameFlagIsSet, 0x109)) {
            FieldScene_RunScene3a2SequenceA();
        }
    } else if (Data_02000240_t.halves[224][0] == (s32)Data_0000004a) {
        Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
        Engine_ActorGet(14)->priority_flags |= 2;
        if (Engine_GameFlagIsSet(0x200)) {
            Engine_ActorSetAnimation(14, 5);
            ActorPresentation_PrepareActorFourteenWithCallback();
        }
        if (Value1(Engine_GameFlagIsSet, 0x201)) {
            Engine_ActorSetAnimation(15, 4);
            FieldScene_SetSlot15Byte89AndRunStep();
        }
        if (Data_02000240_t.halves[225][0] == 4 || Data_02000240_t.halves[225][0] == 5) {
            Engine_GameFlagClear(0x12f);
        }
        if (!Value1(Engine_GameFlagIsSet, 0x89a) && !Value1(Engine_GameFlagIsSet, 0x895) && !Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (!Value1(Engine_GameFlagIsSet, 0x8b2) && Value1(Engine_GameFlagIsSet, 0x895) && Data_02000240_t.halves[225][0] == 2) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_GameFlagSet(0x8b2);
            Engine_GameFlagSet(0x8b3);
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Call6(Engine_MapCopyCellsTo, 54, 21, 53, 21, 1, 2);
            Call6(Engine_MapCopyCellAttributes, 18, 20, 1, 3, 17, 21);
            Call6(Engine_MapCopyCellsTo, 44, 18, 43, 17, 1, 1);
            Call6(Engine_MapCopyCellAttributes, 8, 17, 1, 1, 7, 17);
        }
        if (Value1(Engine_GameFlagIsSet, 0x895) && !Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Call3(Engine_ActorSetPosition, 8, 0xc00000, 0x1080000);
            Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
            Call3(Engine_ActorSetPosition, 10, 0xb80000, 0x1300000);
            Call3(Engine_ActorFaceDirection, 8, 0x5000, 0);
            Call3(Engine_ActorFaceDirection, 10, 0xb000, 0);
            Engine_ActorEnableActionCallback(9, (const u8 *)0x2009730);
            Engine_ActorGet(9)->scale_x = -0x10000;
        }
        if (!Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
            Engine_ActorEnableActionCallback(9, (const u8 *)0x2009730);
            Engine_ActorGet(9)->scale_x = -0x10000;
        }
        if (Data_02000240_t.halves[225][0] == 5 && !Value1(Engine_GameFlagIsSet, 0x8b1) && !Value1(Engine_GameFlagIsSet, 0x109) && !Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Scene_RunActorExchange();
        }
    }
    return 0;
}
