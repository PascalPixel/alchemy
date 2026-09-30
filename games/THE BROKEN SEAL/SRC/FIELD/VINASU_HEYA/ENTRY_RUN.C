/* The Vinasu rooms' entry setup: per room and entrance, which actors,
 * cells and effects are placed before the scene starts. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_REG.H"
#include "SCENE_IDS.H"

void BattleFx_StartFadeOverlay(s32 mode);
void BattleFx_SetQueuedSoundAndPlay(s32 cue);
void DisplayBlend_EnableRunScript(void);
void SceneActor_SetFlagBitByRelativeDepth(union FieldObject *object);
void SceneEffect_SpawnRandomizedParticleEveryFourFrames(union FieldObject *object);
void SceneState_CallWith432And32(void);
void FieldScene_CallWith560And44(void);
void SceneState_ApplyStepToSlots15To18(void);

void SceneActor_ApplySlotsMatchingKind212(void);
void SceneActor_ClearActorModeAndSetState5(s32 actor);
void SceneActor_ApplyPositionsOfActors11And12(void);
void SceneState_MarkActorAndApplyRectAtTile();
void *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 mode);
void OverlayObject_WaitUntilIdle();
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_PlaceAndPinSlots8To10(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void VinasuHeya_RunCellPushScene(void);
void Scene_RunParticleWaveSequence(void);
void FieldScene_RunLeaderDropSequence(void);
void Scene_RunScene3c8SequenceA(void);
/* FAKEMATCH: the CallN inline wrappers order each call's argument setup as the reference does. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call5(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    f(a0, a1, a2, a3, a4);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Scene_RunPairedParticleWaveSequence(void);
void Scene_RunEastParticleWaveSequence(void);
void VinasuHeya_SettlePushedBlocks(void);
void VinasuHeya_LowerFloatingBlocks(s32 mode);



s32 Scene_RunEntrySetup(void)
{
    struct FieldActor *actor;
    s32 area;
    u32 i;
    s32 x;
    s32 z;

    Engine_EventWait(1);
    if (Engine_GameFlagIsSet(0x109))
        SceneActor_ApplySlotsMatchingKind212();
    Engine_GameFlagSet(0x110);
    gEventWork->start_transition = 0x204;
    area = gGameState.scene;
    if (area == (s32)&SceneId_VinasuHeya1) {
        gEventWork->start_transition = 0x100;
        if (!Engine_GameFlagIsSet(0x981))
            SceneActor_ClearActorModeAndSetState5(8);
        else
            Call6((void (*)())Engine_MapCopyCellAttributes, 7, 17, 2, 1, 7, 16);
        SceneActor_ClearActorModeAndSetState5(9);
        SceneActor_ClearActorModeAndSetState5(10);
        SceneActor_ClearActorModeAndSetState5(11);
        Call2((void (*)())Engine_ActorSetSpritePriority, 11, 2);
        SceneActor_ClearActorModeAndSetState5(12);
        Call2((void (*)())Engine_ActorSetSpritePriority, 12, 2);
        SceneActor_ClearActorModeAndSetState5(13);
        SceneActor_ClearActorModeAndSetState5(14);
        goto end;
    }
    if (area == (s32)&SceneId_VinasuHeya2) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
            SceneActor_ClearActorModeAndSetState5(8);
            break;
        case 5:
            Engine_GameFlagClear(0x120);
        case 3:
        case 4:
        case 6:
            SceneActor_ClearActorModeAndSetState5(9);
            break;
        case 20:
        case 21:
            if (Engine_GameFlagIsSet(0x982)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 103, 27, 89, 27, 7, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 27, 92, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 29, 93, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 27, 94, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 27, 96, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 29, 97, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 25, 91, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 92, 25, 93, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 25, 95, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 25, 97, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 27, 96, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 29, 97, 3, 2);
            } else {
                if (!Engine_GameFlagIsSet(0x983))
                    break;
                Call6((void (*)())Engine_MapCopyCellsTo, 111, 27, 89, 27, 7, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 25, 91, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 25, 93, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 25, 95, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 25, 97, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 27, 96, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 90, 29, 97, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 94, 27, 92, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 29, 93, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 94, 27, 94, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 27, 96, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 96, 29, 97, 3, 2);
            }
            break;
        case 26:
            Engine_GameFlagClear(0x121);
        case 22:
        case 23:
            Engine_GameFlagClear(0x12f);
            if (Engine_GameFlagIsSet(0x200))
                Call6((void (*)())Engine_MapCopyCellsTo, 44, 117, 41, 117, 3, 5);
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x987)) {
                Call3((void (*)())Engine_ActorSetPosition, 12, 218 << 18, 176 << 15);
                actor = Engine_ActorGet(12);
                actor->y.fixed = -0x180000;
                actor->target_y = ACTOR_NO_TARGET;
            }
            break;
        }
    } else if (area == (s32)&SceneId_VinasuHeya3) {
        switch (gGameState.entrance) {
        case 16:
            Engine_GameFlagClear(0x12f);
            break;
        case 21:
            Scene_RunPairedParticleWaveSequence();
            break;
        case 20:
            BattleFx_SetQueuedSoundAndPlay(170);
            if (!Engine_GameFlagIsSet(0x109))
                FieldScene_RunSupplementalSequenceOne();
            break;
        case 9:
        case 10:
            actor = Engine_ActorGet(11);
            SceneActor_ApplyPositionsOfActors11And12();
            if (actor->x.fixed >> 20 == 8)
                SceneState_MarkActorAndApplyRectAtTile(actor);
            actor = Engine_ActorGet(12);
            if (actor->x.fixed >> 20 == 7)
                SceneState_MarkActorAndApplyRectAtTile(actor);
            OverlayObject_SpawnWithMode14(206 << 16, 0, 0x1c10000, 223);
            OverlayObject_SpawnWithMode14(210 << 16, 0, 0x1c10000, 223);
            break;
        case 7:
        case 8:
            BattleFx_StartFadeOverlay(0);
            Engine_TaskWait(2);
            actor = Engine_ActorGet(8);
            actor->motion_flags = 0;
            actor->update = SceneActor_SetFlagBitByRelativeDepth;
            actor = Engine_ActorGet(9);
            actor->motion_flags = 0;
            actor->update = SceneActor_SetFlagBitByRelativeDepth;
            actor = Engine_ActorGet(10);
            actor->motion_flags = 0;
            actor->update = SceneActor_SetFlagBitByRelativeDepth;
            FieldScene_PlaceAndPinSlots8To10();
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            BattleFx_SetQueuedSoundAndPlay(170);
            BattleFx_StartFadeOverlay(0);
            Engine_TaskWait(2);
            if (Engine_GameFlagIsSet(0x300)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 111, 5, 117, 5, 5, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 111, 10, 117, 10, 5, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 111, 7, 111, 5, 5, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 111, 7, 111, 10, 5, 2);
                Call6((void (*)())Engine_MapCopyCells, 48, 3, 3, 10, 54, 3);
                Call6((void (*)())Engine_MapCopyCells, 55, 26, 3, 10, 48, 3);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 19:
            goto cue;
        }
    } else if (area == (s32)&SceneId_VinasuHeya4) {
        switch (gGameState.entrance) {
        case 2:
            FieldScene_RunOpeningAuxiliarySequence();
            goto cue;
        case 4:
        case 6:
            BattleFx_StartFadeOverlay(0);
            break;
        case 9:
        case 10:
            actor = Engine_ActorGet(8);
            actor->motion_flags = 0;
            actor->y.fixed = 0;
            actor = Engine_ActorGet(9);
            actor->motion_flags = 0;
            actor->collision_flags = 0;
            if (Engine_GameFlagIsSet(0x301)) {
                Engine_TaskWait(1);
                Call6((void (*)())Engine_MapCopyCellsTo, 124, 41, 110, 41, 1, 2);
                Call6((void (*)())Engine_MapCopyCellAttributes, 46, 41, 1, 1, 46, 42);
                Call3((void (*)())Engine_ActorSetPosition, 9, 186 << 18, 182 << 18);
                actor->motion_flags = 0;
                actor->y.fixed = -0x100000;
                Call2((void (*)())Engine_ActorSetSpritePriority, 9, 3);
                actor->priority_flags = 2;
                Call6((void (*)())Engine_MapCopyCellAttributes, 45, 45, 1, 1, 46, 45);
                Call2((void (*)())Engine_ActorSetAnimation, 10, 7);
                Call2((void (*)())Engine_ActorSetSpritePriority, 10, 1);
                actor = Engine_ActorGet(10);
                actor->collision_flags = 0;
                actor->priority_flags = 2;
                Call3((void (*)())Engine_ActorSetPosition, 10, 0x2e70000, 174 << 18);
                actor->update = SceneEffect_SpawnRandomizedParticleEveryFourFrames;
            }
            VinasuHeya_RunCellPushScene();
            break;
        case 11:
            gEventWork->start_transition = 0x202;
            Engine_ActorGet(0)->y.fixed = -0x20000;
        case 7:
        case 8:
            BattleFx_SetQueuedSoundAndPlay(170);
            DisplayBlend_EnableRunScript();
            {
                register s32 zero asm("r3"); /* FAKEMATCH: pins the zero to r3 */
                asm("mov %0, #0" : "=l"(zero)); /* FAKEMATCH: zero before the address load */
                REG_BLDCNT = zero;
                asm volatile(""); /* FAKEMATCH: keeps the store before the next call setup */
            }
            if (Engine_GameFlagIsSet(0x300)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 15, 96, 9, 96, 3, 3);
                Call6((void (*)())Engine_MapCopyCellsTo, 12, 96, 15, 96, 3, 3);
                Call6((void (*)())Engine_MapCopyCellsTo, 5, 50, 15, 32, 3, 4);
                Call6((void (*)())Engine_MapCopyCellsTo, 25, 45, 9, 32, 3, 4);
                Call6((void (*)())Engine_MapCopyCellAttributes, 15, 32, 3, 1, 9, 32);
                Call6((void (*)())Engine_MapCopyCellAttributes, 12, 32, 3, 1, 15, 32);
            }
            if (gGameState.entrance != 11)
                break;
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            gEventWork->start_transition = 0x204;
            break;
        case 1:
            goto cue;
        }
    } else if (area == (s32)&SceneId_VinasuHeya5) {
        switch (gGameState.entrance) {
        case 19:
            Scene_RunParticleWaveSequence();
            break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 20:
            BattleFx_SetQueuedSoundAndPlay(170);
            if (Engine_GameFlagIsSet(0x306)) {
                Call6((void (*)())Engine_MapCopyCells, 53, 12, 3, 13, 26, 12);
                Call6((void (*)())Engine_MapCopyCellsTo, 81, 41, 89, 14, 9, 2);
                Engine_TaskWait(1);
                Call2((void (*)())Engine_TaskAddCallback, SceneState_CallWith432And32, 0xc80);
            }
            if (Engine_GameFlagIsSet(0x307)) {
                Call6((void (*)())Engine_MapCopyCells, 58, 12, 3, 13, 34, 12);
                Call6((void (*)())Engine_MapCopyCellsTo, 81, 41, 97, 14, 5, 2);
                Engine_TaskWait(1);
                Call2((void (*)())Engine_TaskAddCallback, FieldScene_CallWith560And44, 0xc80);
            }
            if (gGameState.entrance == 11)
                FieldScene_RunOpeningAuxiliarySequence();
            else if (gGameState.entrance == 20)
                Scene_RunEastParticleWaveSequence();
            break;
        case 4:
        case 5:
            /* FAKEMATCH: value-returning cast prevents cross-jumping these blocks. */
            ((s32 (*)(void))FieldScene_RunOpeningAuxiliarySequence)();
            BattleFx_SetQueuedSoundAndPlay(170);
            break;
        case 15:
        case 16:
            actor = Engine_ActorGet(8);
            actor->motion_flags = 0;
            actor->y.fixed = 0;
            Engine_ActorGet(9)->motion_flags = 0;
            Engine_ActorGet(10)->motion_flags = 0;
            Engine_ActorGet(11)->motion_flags = 0;
            if (Engine_GameFlagIsSet(0x304)) {
                Engine_TaskWait(1);
                Call6((void (*)())Engine_MapCopyCellsTo, 111, 59, 109, 37, 1, 2);
                Call6((void (*)())Engine_MapCopyCellAttributes, 45, 37, 1, 1, 45, 38);
                if (Engine_GameFlagIsSet(0x302)) {
                    Call3((void (*)())Engine_ActorSetPosition, 9, 194 << 18, 166 << 18);
                    Call3((void (*)())Engine_ActorSetPosition, 10, 210 << 18, 166 << 18);
                    Call3((void (*)())Engine_ActorSetPosition, 11, 194 << 18, 174 << 18);
                } else {
                    Call3((void (*)())Engine_ActorSetPosition, 9, 210 << 18, 166 << 18);
                    Call3((void (*)())Engine_ActorSetPosition, 10, 194 << 18, 174 << 18);
                    Call3((void (*)())Engine_ActorSetPosition, 11, 210 << 18, 174 << 18);
                }
                Call2((void (*)())Engine_ActorSetSpritePriority, 9, 3);
                actor = Engine_ActorGet(9);
                actor->y.fixed = -0x100000;
                actor->priority_flags = 2;
                Call2((void (*)())Engine_ActorSetSpritePriority, 10, 3);
                actor = Engine_ActorGet(10);
                actor->y.fixed = -0x100000;
                actor->priority_flags = 2;
                Call2((void (*)())Engine_ActorSetSpritePriority, 11, 3);
                actor = Engine_ActorGet(11);
                actor->y.fixed = -0x100000;
                actor->priority_flags = 2;
                Call2((void (*)())Engine_ActorSetAnimation, 12, 7);
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(12), 0);
                Call2((void (*)())Engine_ActorSetSpritePriority, 12, 1);
                actor = Engine_ActorGet(12);
                actor->collision_flags = 0;
                actor->priority_flags = 2;
                Call3((void (*)())Engine_ActorSetPosition, 12, 0x2d70000, 158 << 18);
                actor->update = SceneEffect_SpawnRandomizedParticleEveryFourFrames;
            }
            VinasuHeya_SettlePushedBlocks();
            break;
        case 6:
            goto cue;
        }
    } else if (area == (s32)&SceneId_VinasuHeya6) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
            if (Engine_GameFlagIsSet(0x109)) {
                VinasuHeya_LowerFloatingBlocks(0);
                OverlayObject_WaitUntilIdle(Engine_ActorGet(0));
                for (i = 0; i <= 3; i++) {
                    actor = Engine_ActorGet(i + 10);
                    x = actor->x.fixed >> 20;
                    if (x == 13) {
                        z = actor->z.fixed >> 20;
                        if (z == 7 && Engine_GameFlagIsSet(i + 0x200))
                            goto found;
                    }
                }
            } else {
                actor = Engine_ActorGet(8);
                actor->motion_flags = 0;
                actor->y.fixed = -0x300000;
                actor->priority_flags |= 2;
                actor->collision_flags &= 0xfe;
                actor->unknown_64 = 3;
                Call2((void (*)())Engine_ActorSetSpritePriority, 8, 1);
                actor = Engine_ActorGet(9);
                actor->motion_flags = 0;
                actor->y.fixed = -0x300000;
                actor->priority_flags |= 2;
                actor->collision_flags &= 0xfe;
                actor->unknown_64 = 3;
                Call2((void (*)())Engine_ActorSetSpritePriority, 9, 1);
                actor = Engine_ActorGet(10);
                actor->motion_flags = 0;
                actor->unknown_64 = 0;
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(10), 0);
                actor = Engine_ActorGet(11);
                actor->motion_flags = 0;
                actor->unknown_64 = 0;
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(11), 0);
                actor = Engine_ActorGet(12);
                actor->motion_flags = 0;
                actor->unknown_64 = 0;
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(12), 0);
                actor = Engine_ActorGet(13);
                actor->motion_flags = 0;
                actor->unknown_64 = 0;
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(13), 0);
            }
            break;
        case 13:
        case 14:
        cue:
            BattleFx_SetQueuedSoundAndPlay(170);
            break;
        case 18:
        case 19:
            gEventWork->start_transition = 0x202;
            Engine_ActorGet(0)->y.fixed = -0x20000;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 20:
            Engine_ActorGet(20)->motion_flags = 4;
            Engine_ActorGet(20)->priority_flags |= 2;
            Engine_ActorGet(20)->y.fixed = -0x108000;
            DisplayBlend_EnableRunScript();
            do { *(s16 *)&REG_BLDCNT = 0; } while (0); /* FAKEMATCH: the do/while keeps the zero ahead of the register address */
            if (Engine_GameFlagIsSet(0x306)) {
                BattleFx_SetQueuedSoundAndPlay(170);
                Call6((void (*)())Engine_MapCopyCellsTo, 36, 81, 32, 81, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 36, 83, 36, 81, 3, 2);
                Call6((void (*)())Engine_MapCopyCellAttributes, 36, 17, 3, 1, 32, 17);
                Call6((void (*)())Engine_MapCopyCellAttributes, 36, 18, 3, 1, 36, 17);
                Call6((void (*)())Engine_MapCopyCellsTo, 63, 29, 33, 20, 1, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 20, 56, 36, 17, 3, 4);
            }
            if (Engine_GameFlagIsSet(0x307)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 44, 81, 48, 81, 3, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 44, 83, 44, 81, 3, 2);
                Call6((void (*)())Engine_MapCopyCellAttributes, 44, 17, 3, 1, 48, 17);
                Call6((void (*)())Engine_MapCopyCellAttributes, 44, 18, 3, 1, 44, 17);
                Call6((void (*)())Engine_MapCopyCellsTo, 63, 29, 49, 20, 1, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 41, 56, 44, 17, 3, 4);
            }
            if ((u16)(gGameState.entrance - 18) <= 1) {
                Engine_EventOpenScreen();
                Engine_EventWaitForScreen();
                gEventWork->start_transition = 0x204;
            }
            if (gGameState.entrance == 20)
                FieldScene_RunLeaderDropSequence();
            break;
        found:
            actor->priority_flags |= 2;
            actor->collision_flags = 0;
            actor->motion_flags = 0;
            Call6((void (*)())Engine_MapCopyCellAttributes, 4, 19, 1, 1, x, z);
            break;
        case 15:
        case 16:
            Engine_TaskWait(1);
            Call2((void (*)())Engine_TaskAddCallback, SceneState_ApplyStepToSlots15To18, 0xc80);
            actor = Engine_ActorGet(14);
            actor->motion_flags = 0;
            actor->y.fixed = 0;
            Engine_ActorGet(15)->motion_flags = 0;
            Engine_ActorGet(16)->motion_flags = 0;
            Engine_ActorGet(17)->motion_flags = 0;
            Engine_ActorGet(18)->motion_flags = 0;
            if (Engine_GameFlagIsSet(0x308)) {
                Engine_TaskWait(1);
                Call6((void (*)())Engine_MapCopyCellsTo, 95, 56, 77, 35, 1, 2);
                Call6((void (*)())Engine_MapCopyCellAttributes, 13, 35, 1, 1, 13, 36);
                Call3((void (*)())Engine_ActorSetPosition, 15, 132 << 17, 186 << 18);
                actor = Engine_ActorGet(15);
                actor->y.fixed = -0x100000;
                actor->priority_flags = 2;
                Call2((void (*)())Engine_ActorSetSpritePriority, 15, 3);
                Call3((void (*)())Engine_ActorSetPosition, 16, 184 << 16, 158 << 18);
                actor = Engine_ActorGet(16);
                actor->y.fixed = -0x100000;
                actor->priority_flags = 2;
                Call2((void (*)())Engine_ActorSetSpritePriority, 16, 3);
                Call3((void (*)())Engine_ActorSetPosition, 17, 232 << 16, 174 << 18);
                actor = Engine_ActorGet(17);
                actor->y.fixed = -0x100000;
                actor->priority_flags = 2;
                Call2((void (*)())Engine_ActorSetSpritePriority, 17, 3);
                Call3((void (*)())Engine_ActorSetPosition, 18, 184 << 16, 166 << 18);
                actor = Engine_ActorGet(18);
                actor->y.fixed = -0x100000;
                actor->priority_flags = 2;
                Call2((void (*)())Engine_ActorSetSpritePriority, 18, 3);
                Call2((void (*)())Engine_ActorSetAnimation, 19, 7);
                Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(19), 0);
                Call2((void (*)())Engine_ActorSetSpritePriority, 19, 1);
                actor = Engine_ActorGet(19);
                actor->collision_flags = 0;
                actor->priority_flags = 2;
                Call3((void (*)())Engine_ActorSetPosition, 19, 215 << 16, 150 << 18);
                actor->update = SceneEffect_SpawnRandomizedParticleEveryFourFrames;
            }
            Scene_RunScene3c8SequenceA();
            break;
        }
    }
end:
    return 0;
}
