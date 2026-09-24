#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08015360(s32 a, s32 b);
void FieldScene_RunLargeStagingSequence(void);
void InitializeStagedActorSceneOrbitingEffect(s32 actor);
void Local_02005950(void);
void Scene_RepairTheHouse(void);
void SceneState_Send210AndApplyRectAt40x84(void);
void Main_0808a5e0(s32 value);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Vale entry: from entrance 16 run the staging scene; otherwise place the story actors and cells for the flags, start the village tasks and redraw. */
s32 HaidiaMura_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 set;

    if (Data_02000240_t.halves[225][0] == 16) {
        Main_08015360(Data_02000240_t.bytes[258][1], Data_02000240_t.bytes[259][0]);
        FieldScene_RunLargeStagingSequence();
    } else {
        if (!Engine_GameFlagIsSet(0xfd0)) {
            if (!Engine_GameFlagIsSet(0x87a)) {
                InitializeStagedActorSceneOrbitingEffect(26);
            } else {
                InitializeStagedActorSceneOrbitingEffect(20);
            }
        }
        Call6(Engine_MapCopyCellsTo, 2, 102, 84, 41, 2, 1);
        Call6(Engine_MapCopyCellsTo, 1, 102, 83, 41, 1, 1);
        actor = Engine_ActorGet((Engine_GameFlagIsSet(0x87a) != 0) + 20);
        Engine_ActorSetSpriteFlags(actor, 0);
        if (Engine_GameFlagIsSet(0x314)) {
            actor->x.fixed = 181 << 17;
        } else if (Engine_GameFlagIsSet(0x316)) {
            actor->x.fixed = 197 << 17;
        } else {
            actor->x.fixed = 189 << 17;
        }
        actor->z.fixed = 0x2480000;
        actor->y.fixed = 0xc00000;
        Local_02005950();
        actor->unknown_22 = 3;
        actor->motion_flags = 0;
        Engine_TaskAddCallback((void (*)(void))0x200da95, 0xc80);
        if (!Engine_GameFlagIsSet(0x87a)) {
            if (Value1(Engine_GameFlagIsSet, 0x815)) {
                actor = Engine_ActorGet(21);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(21), 0);
                actor->scale_x = 0x28f;
                actor->scale_y = 0x28f;
            }
            if (Engine_GameFlagIsSet(0x808)) {
                Engine_ActorSetPosition(15, 0, 0);
                Engine_ActorSetPosition(16, 0, 0);
                Engine_ActorSetPosition(17, 0, 0);
            }
            set = Value1(Engine_GameFlagIsSet, 0x815);
            if (set == 0) {
                if (!Engine_GameFlagIsSet(0x109)) {
                    if (Engine_GameFlagIsSet(0x823)) {
                        Call3(Engine_ActorSetPosition, 22, 0x1000000, 0x1c80000);
                        Engine_ActorGet(22)->update = (void *)0x200d72d;
                        Engine_ActorEnableActionCallback(22, (const u8 *)0x200e248);
                    }
                } else {
                    Engine_ActorGet(22)->unknown_5b = set;
                    Engine_GameFlagClear(0x241);
                }
                if (Data_02000240_t.halves[225][0] != 16 && !Engine_GameFlagIsSet(0x87a)) {
                    Engine_TaskAddCallback((void (*)(void))0x200da41, 0xc80);
                }
            }
            if (!Value1(Engine_GameFlagIsSet, 0x308) && Data_02000240_t.halves[225][0] == 17) {
                Scene_RepairTheHouse();
                Call1(Engine_GameFlagSet, 0x308);
            }
        }
        if (Engine_GameFlagIsSet(0x109)) {
            if (Engine_GameFlagIsSet(0x204)) {
                Call6(Engine_MapCopyCellAttributes, 49, 53, 8, 4, 20, 50);
            }
            if (Engine_GameFlagIsSet(0x210)) {
                SceneState_Send210AndApplyRectAt40x84();
            }
        }
        Main_0808a5e0(170);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    }
    return 0;
}
