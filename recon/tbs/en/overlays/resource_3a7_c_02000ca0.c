/* NONMATCHING: 960 of 960 bytes, 3 halfword edits (2026-09-24). Hand-written
 * from the jump-table disassembly; a single-overlay unit source binding the
 * Engine_* names at their import veneers (listing offset + 0x8000). Remaining:
 * the do-while around the first store keeps the 0x02000240 base load after
 * it (without it the base load is hoisted above the store, 4 edits), but
 * moves the prologue sub sp, #8 before the store; the reference places it
 * after the first ldrsh. */
#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_MapCopyCellAttributes();
void Main_080091b8();
void Engine_TaskWait();
void Engine_MapRedraw();
void Engine_GameFlagClear();
u8 *Engine_ActorGet();
void SceneActor_InitSlots10To15AndStartTask();
void Engine_ActorSetAnimation();
void SceneState_ApplyRectAndMarkActor16();
void SceneState_ConfigureRegion26_30AndMarkActor17();
void SceneState_ConfigureRegion26_30AndClearActor18Mode();
void SceneState_ApplyRectAndSetupActor19();
void SceneActor_SetupSlotTwenty();
void SceneActor_MarkSlot21AndSetFlag205();
void Engine_TaskAddCallback();
void SceneState_ApplyThreeRects();
void Engine_ActorSetPosition();
void Engine_ActorSetSpriteFlags();
void SceneActor_SetupActors11To14AndInstallTask();
void SceneState_ApplyThreeRectsRows9And10();

extern s16 Data_02000240_t[][1];
extern u8 Data_00000060[];
extern u8 Data_00000061[];
extern u8 Data_00000062[];

struct ActorFlags {
    u8 pad[89];
    u8 flags;
};

/* Set up the actors and map cells of the current area for its story state. */
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

s32 Local_02000ca0(void)
{
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x204;
    if (Data_02000240_t[224][0] == (s32)Data_00000060 + 0) {
        switch (Data_02000240_t[225][0]) {
        case 5:
        case 6:
        case 7:
        case 8:
        case 13:
            if (Engine_GameFlagIsSet(0x9a8) == 0) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 22, 29, 1, 1, 21, 29);
            } else {
                Call6((void (*)())Main_080091b8, 108, 27, 1, 1, 92, 27);
                Engine_TaskWait(1);
                Call6((void (*)())Main_080091b8, 19, 83, 15, 8, 19, 91);
                Engine_TaskWait(1);
                Call6((void (*)())Main_080091b8, 2, 24, 1, 2, 25, 27);
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
            break;
        case 10:
            Engine_GameFlagClear(0x9a8);
            break;
        }
    }
    if (Data_02000240_t[224][0] == (s32)Data_00000061) {
        if (Engine_GameFlagIsSet(0x300) == 0) {
            *(s32 *)(Engine_ActorGet(22) + 28) = 0x18000;
        }
        switch (Data_02000240_t[225][0]) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (Engine_GameFlagIsSet(0x9a8) == 0) {
                Call6((void (*)())Main_080091b8, 5, 81, 11, 7, 5, 73);
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
            Call2((void (*)())Engine_TaskAddCallback, 0x20086e5, 0xc80);
            break;
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x9a9) != 0) {
                SceneState_ApplyThreeRects();
                Call3((void (*)())Engine_ActorSetPosition, 9, 0xf80000, 0x36c0000);
            }
            Engine_ActorGet(8)[35] = 2;
            break;
        }
        Call2((void (*)())Engine_ActorSetAnimation, 8, 2);
        Call2((void (*)())Engine_ActorSetAnimation, 9, 2);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(8), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
        Engine_ActorGet(9)[89] = 1;
    }
    if (Data_02000240_t[224][0] == (s32)Data_00000062) {
        Call2((void (*)())Engine_ActorSetAnimation, 8, 2);
        if (Engine_GameFlagIsSet(0x207) == 0) {
            Call2((void (*)())Engine_ActorSetAnimation, 10, 2);
        }
        Engine_ActorSetSpriteFlags(Engine_ActorGet(8), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
        Engine_ActorGet(10)[89] |= 0x80;
        ((struct ActorFlags *)Engine_ActorGet(9))->flags |= 0x80;
        switch (Data_02000240_t[225][0]) {
        case 5:
        case 6:
            SceneActor_SetupActors11To14AndInstallTask();
            Engine_ActorGet(11)[89] = 2;
            Engine_ActorGet(12)[89] = 2;
            Engine_ActorGet(13)[89] = 2;
            Engine_ActorGet(14)[89] = 2;
            Engine_ActorGet(8)[89] = 1;
            Engine_ActorGet(10)[89] = 1;
            Engine_ActorGet(9)[89] = 1;
            if (Engine_GameFlagIsSet(0x9aa) != 0) {
                SceneState_ApplyThreeRectsRows9And10();
                Call3((void (*)())Engine_ActorSetPosition, 10, 0x1080000, 0xcc0000);
            }
            break;
        }
    }
    Data_02000240_t[289][0] = 10;
    Data_02000240_t[288][0] = (s32)Data_00000060;
    return 0;
}
