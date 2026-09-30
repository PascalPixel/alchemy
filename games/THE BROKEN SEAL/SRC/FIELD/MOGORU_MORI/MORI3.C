#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "MORI.H"

extern const struct SceneEvent gMogoruMoriEvents1[];
extern const struct SceneEvent gMogoruMoriEvents2[];
extern const struct SceneEvent gMogoruMoriEvents3[];
extern const struct SceneEvent gMogoruMoriEventsOther[];

extern struct GameState gGameState;
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
void Engine_ActorSetSpriteFlags();
void Engine_EventEnd();
void InitializeOrbitingEffect();
void Object_SetModeById();
void Map_CopyCellAttributeRect();
s32 StagedActor_FillGridAttributeRectangle(u32, s32, s32, u32, u32, s32);
u8 *OverlayObject_SpawnConfiguredWithMode15(s32, s32, s32, s32);
void FieldScene_RunSupplementalSequenceOne();
void Engine_ActorEnableActionCallback();
extern u8 MogoruMori_ActorScript[];

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriEvents1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriEvents2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriEvents3;
    }
    return gMogoruMoriEventsOther;
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
        mode = *(s16 *)((u8 *)&gGameState + off);
    }
    if (mode == (s32)&SceneId_MogoruMori1) {
        step = *(s16 *)((u8 *)&gGameState + 450);
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
                Engine_ActorWalkTo(0, 136, 64);
                Engine_EventOpenScreen();
                Engine_EventWaitForScreen();
                ObjectMotion_CommitCurrentPositionAndActivate(0);
                Battle_WaitMode0(30);
                Call3(Engine_ActorShowEmote, 10, 256, 0);
                Engine_ActorRunRepeatedMotion(10, 2);
                Battle_WaitMode0(30);
                FieldScene_RunScene39f_02000d90(10, 136, 116, 0x70000);
                MogoruMori_SpawnPuffRing(10);
                Engine_ActorSetChildValue(10, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(10), 0);
                Engine_GameFlagSet(0x89c);
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
            rec = (u8 *)Object_GetById(0);
            if (rec != 0) {
                Engine_ActorSetPosition(16, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
            }
            rec = (u8 *)Object_GetById(16);
            *(s32 *)(rec + 108) = 0;
            if (Engine_GameFlagIsSet(0x109) != 0) {
                rec = (u8 *)Object_GetById(16);
                *(s32 *)(rec + 12) = 0x200000;
            }
            WaitFrames(1);
            Engine_ActorSetPosition(16, 0x2780000, 0x1b80000);
            if (Engine_GameFlagIsSet(0xfd4) == 0) {
                InitializeOrbitingEffect(16);
            }
            Engine_ActorSetChildValue(11, 15);
            Engine_ActorSetChildValue(12, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(11), 0);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(12), 0);
            FieldScene_RedrawActorFootprint(8);
            if (Engine_GameFlagIsSet(784) == 0) {
                FieldScene_RedrawActorFootprint(9);
                break;
            }
            WaitFrames(1);
            Call3(Engine_ActorSetPosition, 9, 0x2100000, 0x1980000);
            Object_SetModeById(9, 4);
            Call6(Map_CopyCellAttributeRect, 38, 27, 4, 2, 31, 25);
            *(u8 *)((u8 *)Object_GetById(9) + 35) = 2;
            break;
        }
    } else if (mode == (s32)&SceneId_MogoruMori2) {
        step = *(s16 *)((u8 *)&gGameState + 450);
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (Engine_GameFlagIsSet(0x303) == 0) {
                Engine_ActorSetChildValue(12, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(12), 0);
            }
            if (Engine_GameFlagIsSet(772) != 0) {
                break;
            }
            Engine_ActorSetChildValue(13, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(13), 0);
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
                Object_SetModeById(10, 4);
                *(u8 *)((u8 *)Object_GetById(10) + 35) = 2;
                Call6(Map_CopyCellAttributeRect, 44, 30, 2, 4, 34, 30);
                StagedActor_FillGridAttributeRectangle(0, 35, 29, 1, 4, attr);
            }
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            pos = *(s32 *)((u8 *)Object_GetById(11) + 8);
            tmp = *(s32 *)((u8 *)Object_GetById(11) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            WaitFrames(1);
            Engine_ActorSetChildValue(11, 6);
            {
                u8 *obj = (u8 *)Object_GetById(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Engine_GameFlagIsSet(0x306) != 0) {
                break;
            }
            Engine_ActorSetChildValue(14, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(14), 0);
            if (Engine_GameFlagIsSet(0x305) == 0) {
                break;
            }
            Call3(Engine_ActorSetPosition, 14, 0x1a80000, 0x1e00000);
            Call3(Engine_ActorSetPosition, 17, 0x1a80000, 0x1e00000);
            break;
        }
    } else if (mode == (s32)&SceneId_MogoruMori3) {
        step = *(s16 *)((u8 *)&gGameState + 450);
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            WaitFrames(1);
            if (Engine_GameFlagIsSet(0x307) == 0) {
                Engine_ActorSetChildValue(15, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(15), 0);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(19), 0);
            }
            if (Engine_GameFlagIsSet(776) == 0) {
                Engine_ActorSetChildValue(16, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(16), 0);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(20), 0);
            }
            if (Engine_GameFlagIsSet(0x309) != 0) {
                break;
            }
            Engine_ActorSetChildValue(17, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(17), 0);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(21), 0);
            break;

        case 7:
            pos = *(s32 *)((u8 *)Object_GetById(13) + 8);
            tmp = *(s32 *)((u8 *)Object_GetById(13) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            Engine_ActorSetChildValue(13, 6);
            WaitFrames(1);
            {
                u8 *obj = (u8 *)Object_GetById(8);
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
                Engine_ActorSetPosition(9, 0x2ba0000, 0x18e0000);
                {
                    u8 *obj = (u8 *)Object_GetById(9);
                    u32 mask = 2;
                    mask = mask | obj[35];
                    obj[35] = mask;
                }
                Call6(Map_CopyCellAttributeRect, 26, 20, 2, 4, 42, 23);
                Engine_GameFlagSet(532);
                Call3(Engine_ActorSetPosition, 14, 0x2780000, 0x1b80000);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(14), 0);
            }
            if (Engine_GameFlagIsSet(0x313) == 0) {
                FieldScene_RedrawActorFootprint(11);
            } else {
                WaitFrames(1);
                Object_SetModeById(11, 4);
                Engine_ActorSetPosition(11, 0x29a0000, 0x2260000);
                *(u8 *)((u8 *)Object_GetById(11) + 35) = 2;
                Call6(Map_CopyCellAttributeRect, 26, 20, 2, 4, 40, 32);
            }
            pos = *(s32 *)((u8 *)Object_GetById(14) + 8);
            tmp = *(s32 *)((u8 *)Object_GetById(14) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            Engine_ActorSetChildValue(14, 6);
            WaitFrames(1);
            {
                u8 *obj = (u8 *)Object_GetById(9);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Engine_GameFlagIsSet(0x30b) == 0) {
                Engine_ActorSetChildValue(18, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(18), 0);
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

s32 UpdateOrbitingSceneObject(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Math_Sin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Math_Cos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Math_Cos(p[12] + 0x8000) / 8;
    a = Random_Next();
    b = Random_Next();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeOrbitingEffect(s32 id)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = (OrbitingSceneObject *)Object_GetById(id);
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)UpdateOrbitingSceneObject;
    actor->state = zero;
}
