#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALLBACK_SCHEDULER.H"

void SceneActor_InitSlots10To15AndStartTask(void);
void SceneState_ApplyRectAndMarkActor16(void);
void SceneState_ConfigureRegion26_30AndMarkActor17(void);
void SceneState_ConfigureRegion26_30AndClearActor18Mode(void);
void SceneState_ApplyRectAndSetupActor19(void);
void SceneActor_SetupSlotTwenty(void);
void SceneActor_MarkSlot21AndSetFlag205(void);
void SceneState_ApplyThreeRects(void);
void SceneActor_SetupActors11To14AndInstallTask(void);
void SceneState_ApplyThreeRectsRows9And10(void);
void SceneState_DispatchByActorZeroDepth(void);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/*
 * Kuupuappu Cave entry: open the screen with the window transition, then
 * for each of the cave's three areas set its actors and map cells for the
 * story so far. Retreat returns the party to the first area.
 */
s32 KuupuappuDou_ApplyEntryState(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_KuupuappuDou1) {
        switch (gGameState.entrance) {
        case 5:
        case 6:
        case 7:
        case 8:
        case 13:
            if (Engine_GameFlagIsSet(0x9a8) == 0) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 22, 29, 1, 1, 21, 29);
            } else {
                Call6((void (*)())Engine_MapCopyCells, 108, 27, 1, 1, 92, 27);
                Engine_TaskWait(1);
                Call6((void (*)())Engine_MapCopyCells, 19, 83, 15, 8, 19, 91);
                Engine_TaskWait(1);
                Call6((void (*)())Engine_MapCopyCells, 2, 24, 1, 2, 25, 27);
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
            break;
        case 10:
            Engine_GameFlagClear(0x9a8);
            break;
        }
    }
    if (gGameState.scene == (s32)&SceneId_KuupuappuDou2) {
        if (Engine_GameFlagIsSet(0x300) == 0) {
            Engine_ActorGet(22)->scale_y = 0x18000;
        }
        switch (gGameState.entrance) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (Engine_GameFlagIsSet(0x9a8) == 0) {
                Call6((void (*)())Engine_MapCopyCells, 5, 81, 11, 7, 5, 73);
            } else {
                Call6((void (*)())Engine_MapCopyCellAttributes, 5, 12, 1, 1, 6, 12);
                Call6((void (*)())Engine_MapCopyCellAttributes, 12, 10, 1, 1, 12, 11);
            }
            break;
        case 8:
        case 9:
        case 14:
            SceneActor_InitSlots10To15AndStartTask();
            if (Engine_GameFlagIsSet(0x200) != 0) {
                Call2((void (*)())Engine_ActorSetAnimation, 16, 5);
                SceneState_ApplyRectAndMarkActor16();
            }
            if (Engine_GameFlagIsSet(0x201) != 0) {
                Call2((void (*)())Engine_ActorSetAnimation, 17, 5);
                SceneState_ConfigureRegion26_30AndMarkActor17();
            }
            if (Engine_GameFlagIsSet(0x202) != 0) {
                Call2((void (*)())Engine_ActorSetAnimation, 18, 5);
                SceneState_ConfigureRegion26_30AndClearActor18Mode();
            }
            if (Engine_GameFlagIsSet(0x203) != 0) {
                Call2((void (*)())Engine_ActorSetAnimation, 19, 5);
                SceneState_ApplyRectAndSetupActor19();
            }
            if (Engine_GameFlagIsSet(0x204) != 0) {
                Call2((void (*)())Engine_ActorSetAnimation, 20, 5);
                SceneActor_SetupSlotTwenty();
            }
            if (Engine_GameFlagIsSet(0x205) != 0) {
                Call2((void (*)())Engine_ActorSetAnimation, 21, 5);
                SceneActor_MarkSlot21AndSetFlag205();
            }
            Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)SceneState_DispatchByActorZeroDepth, 0xc80);
            break;
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x9a9) != 0) {
                SceneState_ApplyThreeRects();
                Call3((void (*)())Engine_ActorSetPosition, 9, 0xf80000, 0x36c0000);
            }
            Engine_ActorGet(8)->priority_flags = 2;
            break;
        }
        Call2((void (*)())Engine_ActorSetAnimation, 8, 2);
        Call2((void (*)())Engine_ActorSetAnimation, 9, 2);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(8), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
        Engine_ActorGet(9)->collision_flags = 1;
    }
    if (gGameState.scene == (s32)&SceneId_KuupuappuDou3) {
        Call2((void (*)())Engine_ActorSetAnimation, 8, 2);
        if (Engine_GameFlagIsSet(0x207) == 0) {
            Call2((void (*)())Engine_ActorSetAnimation, 10, 2);
        }
        Engine_ActorSetSpriteFlags(Engine_ActorGet(8), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
        Engine_ActorGet(10)->collision_flags |= 0x80;
        Engine_ActorGet(9)->collision_flags |= 0x80;
        switch (gGameState.entrance) {
        case 5:
        case 6:
            SceneActor_SetupActors11To14AndInstallTask();
            Engine_ActorGet(11)->collision_flags = 2;
            Engine_ActorGet(12)->collision_flags = 2;
            Engine_ActorGet(13)->collision_flags = 2;
            Engine_ActorGet(14)->collision_flags = 2;
            Engine_ActorGet(8)->collision_flags = 1;
            Engine_ActorGet(10)->collision_flags = 1;
            Engine_ActorGet(9)->collision_flags = 1;
            if (Engine_GameFlagIsSet(0x9aa) != 0) {
                SceneState_ApplyThreeRectsRows9And10();
                Call3((void (*)())Engine_ActorSetPosition, 10, 0x1080000, 0xcc0000);
            }
            break;
        }
    }
    gGameState.retreat_entrance = 10;
    gGameState.retreat_scene = (s32)&SceneId_KuupuappuDou1;
    return 0;
}
