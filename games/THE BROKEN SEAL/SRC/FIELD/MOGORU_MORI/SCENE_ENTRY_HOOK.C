#include "TYPES.H"
#include "SCENE_IDS.H"

extern u8 Data_02000240[];

extern struct EventWork *gEventWork;

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void WaitFrames();
void Engine_ActorSetChildValue();
void Engine_ActorSetPosition();
void Engine_ActorFaceDirection();
void ObjectMotion_SetSpeedParameters();
void Engine_ActorWalkTo();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Battle_WaitMode0();
void Engine_ActorShowEmote();
void Engine_ActorRunRepeatedMotion();
void FieldScene_RunScene39f_02000d90();
void MogoruMori_SpawnPuffRing();
u8 *Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Engine_GameFlagSet();
void Engine_EventEnd();
void InitializeOrbitingEffect();
void FieldScene_RedrawActorFootprint();
void Object_SetModeById();
void Map_CopyCellAttributeRect();
s32 StagedActor_FillGridAttributeRectangle(u32, s32, s32, u32, u32, s32);
u8 *OverlayObject_SpawnConfiguredWithMode15(s32, s32, s32, s32);
void FieldScene_RunSupplementalSequenceOne();
void Engine_ActorEnableActionCallback();
extern u8 MogoruMori_ActorScript[];

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

/* Stores 0x204 in the scene work word at 448, then by area of Mogoru Forest
 * (SceneId_MogoruMori1 to 3) and entry number applies the flag-dependent
 * actor placements and states for that entry. Returns 0. */
s32 FieldScene_RunSceneEntryHook(void)
{
    u8 *rec;
    s32 mode;
    s32 step;
    s32 pos;
    s32 tmp;

    /* FAKEMATCH: one scoped offset of 448 serves both the event work's start
       transition and the game state's scene, so the constant is loaded once
       into a register the two accesses share. */
    {
        u8 *work = (u8 *)gEventWork;
        s32 off = 448;

        *(s32 *)(work + off) = 0x204;
        mode = *(s16 *)(Data_02000240 + off);
    }
    if (mode == (s32)&SceneId_MogoruMori1) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (Value1(Engine_GameFlagIsSet, 0x89c) == 0) {
                Engine_EventBegin();
                WaitFrames(1);
                Engine_ActorSetChildValue(10, 1);
                Call3(Engine_ActorSetPosition, 10, 0x5c0000, 0x780000);
                Call3(Engine_ActorFaceDirection, 10, 0xd000, 0);
                Call3(ObjectMotion_SetSpeedParameters, 0, 0x6666, 0x3333);
                Call3(Engine_ActorWalkTo, 0, 136, 64);
                Engine_EventOpenScreen();
                Engine_EventWaitForScreen();
                ObjectMotion_CommitCurrentPositionAndActivate(0);
                Battle_WaitMode0(30);
                Call3(Engine_ActorShowEmote, 10, 256, 0);
                Engine_ActorRunRepeatedMotion(10, 2);
                Battle_WaitMode0(30);
                FieldScene_RunScene39f_02000d90(10, 136, 116, 0x70000);
                MogoruMori_SpawnPuffRing(10);
                Call2(Engine_ActorSetChildValue, 10, 15);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
                Call1(Engine_GameFlagSet, 0x89c);
                Battle_WaitMode0(60);
                Engine_EventEnd();
            }
            if (Engine_GameFlagIsSet(0x109) == 0) {
                break;
            }
            if (Engine_GameFlagIsSet(768) != 0) {
                break;
            }
            Engine_ActorSetChildValue(10, 15);
            Call3(Engine_ActorSetPosition, 10, 0x880000, 0x740000);
            break;

        case 7:
        case 8:
        case 9:
            rec = Engine_ActorGet(0);
            if (rec != 0) {
                Engine_ActorSetPosition(16, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
            }
            rec = Engine_ActorGet(16);
            *(s32 *)(rec + 108) = 0;
            if (Engine_GameFlagIsSet(0x109) != 0) {
                rec = Engine_ActorGet(16);
                *(s32 *)(rec + 12) = 0x200000;
            }
            WaitFrames(1);
            Call3(Engine_ActorSetPosition, 16, 0x2780000, 0x1b80000);
            if (Engine_GameFlagIsSet(0xfd4) == 0) {
                InitializeOrbitingEffect(16);
            }
            Engine_ActorSetChildValue(11, 15);
            Call2(Engine_ActorSetChildValue, 12, 15);
            Engine_ActorSetSpriteFlags(Engine_ActorGet(11), 0);
            Engine_ActorSetSpriteFlags(Engine_ActorGet(12), 0);
            FieldScene_RedrawActorFootprint(8);
            if (Engine_GameFlagIsSet(784) == 0) {
                FieldScene_RedrawActorFootprint(9);
                break;
            }
            WaitFrames(1);
            Call3(Engine_ActorSetPosition, 9, 0x2100000, 0x1980000);
            Object_SetModeById(9, 4);
            Call6(Map_CopyCellAttributeRect, 38, 27, 4, 2, 31, 25);
            *(u8 *)(Engine_ActorGet(9) + 35) = 2;
            break;
        }
    } else if (mode == (s32)&SceneId_MogoruMori2) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (Engine_GameFlagIsSet(0x303) == 0) {
                Call2(Engine_ActorSetChildValue, 12, 15);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(12), 0);
            }
            if (Engine_GameFlagIsSet(772) != 0) {
                break;
            }
            Call2(Engine_ActorSetChildValue, 13, 15);
            Engine_ActorSetSpriteFlags(Engine_ActorGet(13), 0);
            break;

        case 10:
        case 11:
        case 12:
            if (Engine_GameFlagIsSet(0x311) == 0) {
                FieldScene_RedrawActorFootprint(10);
            } else {
                s32 attr = 0;

                WaitFrames(1);
                Call3(Engine_ActorSetPosition, 10, 0x2280000, 0x1fe0000);
                Call2(Object_SetModeById, 10, 4);
                *(u8 *)(Engine_ActorGet(10) + 35) = 2;
                Call6(Map_CopyCellAttributeRect, 44, 30, 2, 4, 34, 30);
                StagedActor_FillGridAttributeRectangle(0, 35, 29, 1, 4, attr);
            }
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            pos = *(s32 *)(Engine_ActorGet(11) + 8);
            tmp = *(s32 *)(Engine_ActorGet(11) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            WaitFrames(1);
            Engine_ActorSetChildValue(11, 6);
            {
                u8 *obj = Engine_ActorGet(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Engine_GameFlagIsSet(0x306) != 0) {
                break;
            }
            Call2(Engine_ActorSetChildValue, 14, 15);
            Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
            if (Engine_GameFlagIsSet(0x305) == 0) {
                break;
            }
            Call3(Engine_ActorSetPosition, 14, 0x1a80000, 0x1e00000);
            Call3(Engine_ActorSetPosition, 17, 0x1a80000, 0x1e00000);
            break;
        }
    } else if (mode == (s32)&SceneId_MogoruMori3) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            WaitFrames(1);
            if (Engine_GameFlagIsSet(0x307) == 0) {
                Call2(Engine_ActorSetChildValue, 15, 15);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(15), 0);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(19), 0);
            }
            if (Engine_GameFlagIsSet(776) == 0) {
                Call2(Engine_ActorSetChildValue, 16, 15);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(16), 0);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(20), 0);
            }
            if (Engine_GameFlagIsSet(0x309) != 0) {
                break;
            }
            Call2(Engine_ActorSetChildValue, 17, 15);
            Engine_ActorSetSpriteFlags(Engine_ActorGet(17), 0);
            Engine_ActorSetSpriteFlags(Engine_ActorGet(21), 0);
            break;

        case 7:
            pos = *(s32 *)(Engine_ActorGet(13) + 8);
            tmp = *(s32 *)(Engine_ActorGet(13) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            Call2(Engine_ActorSetChildValue, 13, 6);
            WaitFrames(1);
            {
                u8 *obj = Engine_ActorGet(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            FieldScene_RedrawActorFootprint(8);
            break;

        case 8:
        case 9:
        case 10:
        case 11:
            OverlayObject_SpawnConfiguredWithMode15(0x2de0000, 0, 0x1720000, 223);
            OverlayObject_SpawnConfiguredWithMode15(0x2f20000, 0, 0x1720000, 223);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(12);
            if (Engine_GameFlagIsSet(0x312) == 0) {
                FieldScene_RedrawActorFootprint(9);
            } else {
                WaitFrames(1);
                Object_SetModeById(9, 4);
                Call3(Engine_ActorSetPosition, 9, 0x2ba0000, 0x18e0000);
                {
                    u8 *obj = Engine_ActorGet(9);
                    u32 mask = 2;
                    mask = mask | obj[35];
                    obj[35] = mask;
                }
                Call6(Map_CopyCellAttributeRect, 26, 20, 2, 4, 42, 23);
                Engine_GameFlagSet(532);
                Call3(Engine_ActorSetPosition, 14, 0x2780000, 0x1b80000);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
            }
            if (Engine_GameFlagIsSet(0x313) == 0) {
                FieldScene_RedrawActorFootprint(11);
            } else {
                WaitFrames(1);
                Object_SetModeById(11, 4);
                Call3(Engine_ActorSetPosition, 11, 0x29a0000, 0x2260000);
                *(u8 *)(Engine_ActorGet(11) + 35) = 2;
                Call6(Map_CopyCellAttributeRect, 26, 20, 2, 4, 40, 32);
            }
            pos = *(s32 *)(Engine_ActorGet(14) + 8);
            tmp = *(s32 *)(Engine_ActorGet(14) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            Call2(Engine_ActorSetChildValue, 14, 6);
            WaitFrames(1);
            {
                u8 *obj = Engine_ActorGet(9);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Engine_GameFlagIsSet(0x30b) == 0) {
                Call2(Engine_ActorSetChildValue, 18, 15);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(18), 0);
                if (Engine_GameFlagIsSet(0x30a) != 0) {
                    Call3(Engine_ActorSetPosition, 22, 0x2e80000, 0x1f80000);
                    Call3(Engine_ActorSetPosition, 18, 0x2e80000, 0x1f80000);
                }
            }
            FieldScene_RunSupplementalSequenceOne();
            break;

        case 12:
        case 13:
            Engine_ActorEnableActionCallback(18, MogoruMori_ActorScript);
            if (Engine_GameFlagIsSet(0x893) == 0) {
                break;
            }
            if (Engine_GameFlagIsSet(0x89e) == 0) {
                break;
            }
            Engine_GameFlagSet(0x88f);
            break;

        case 15:
            Engine_GameFlagSet(0x89e);
            break;
        }
    }
    return 0;
}
