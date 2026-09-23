#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"

extern u8 LinkedValue_Zero;
extern u8 LinkedScene_VinasuChojo;
extern const u8 Data_0200e074[];
extern const u8 Data_0200e088[];
extern const u8 Data_0200e0ac[];
extern const u8 Data_0200e22c[];

void Scene_CallPairWith10(s32 actor, s32 angle);
void State_ApplyArgMode0AndSet10(s32 speaker);
void Scene_RunSetupSequence35c4(void);
void Actor_ParkRecord(struct FieldActor *object);
void Effect_MoveWithDrag(union FieldObject *object);
void SceneTask_020036d0(void);
void SceneTask_020037c4(void);

void Engine_ActorLaunch(s32 actor, s32 speed, s32 frames);
void Engine_ActorSetPositionAndReset(s32 actor, s32 x, s32 z);
void Engine_ActorSetPositionAndCommit(s32 actor, s32 x, s32 z);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void Engine_GameStateSetReturn(s32 scene, s32 entrance);
void Engine_GameStateSetWarp(s32 scene, s32 entrance);
void Engine_Import0808a250(s32 first, s32 second);
void Engine_Import08077268(void);

static inline void Actor_Launch(s32 actor, s32 speed, s32 frames)
{
    Engine_ActorLaunch(actor, speed, frames);
}

static inline void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetPositionAndReset(actor, x, z);
}

static inline void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetPositionAndCommit(actor, x, z);
}

static inline void Object_SetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y,
                                      s32 fixed_z)
{
    Engine_ObjectSetPosition(object, fixed_x, fixed_y, fixed_z);
}

static inline void Object_CommitPosition(struct FieldActor *object)
{
    Engine_ObjectCommitPosition(object);
}

void Scene_RunPairedActorEffectSequence(void)
{
    struct EffectOptions options_a;
    s32 velocity[3];
    struct EffectOptions options_b;
    struct FieldActor *actor;
    struct FieldActor *origin;
    struct FieldActor *object;
    struct FieldActor *bird;
    struct FieldSprite *sprite;
    u8 *buffer;
    s32 yes;
    u32 i;

    Event_Begin();
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(1)->facing = 0x8000;
    Actor_SetPosition(1, 0x1480000, 0xa80000);
    Actor_Get(2)->facing = 0x8000;
    Actor_SetPosition(2, 0x1540000, 0xc40000);
    Actor_Get(3)->facing = 0x8000;
    Actor_SetPosition(3, 0x1460000, 0xcc0000);
    Actor_Get(6)->facing = 0x3000;
    Actor_SetPosition(6, 0x10c0000, 0x9a0000);
    Actor_Get(21)->facing = 0x3000;
    Actor_SetPosition(21, 0x10c0000, 0xa40000);
    actor = Actor_Get(20);
    actor->unknown_64 = 10;
    actor->facing = 0xd000;
    Actor_SetPosition(20, 0x1260000, 0xd40000);
    Actor_SetAnimation(20, 9);
    Actor_EnableActionCallback(20, Data_0200e074);
    Actor_SetSpriteFlags(Actor_Get(20), 0);
    actor = Actor_Get(19);
    actor->unknown_64 = 10;
    actor->facing = 0;
    Actor_SetPosition(19, 0x11e0000, 0xc00000);
    Actor_SetAnimation(19, 7);
    Actor_EnableActionCallback(19, Data_0200e074);
    Actor_SetSpriteFlags(Actor_Get(19), 0);
    Event_GetViewCenter()->motion_flags = 0;
    Camera_MoveTo(0x1300000, 0x200000, 0xb40000, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(80);

    Actor_Launch(1, 2, 20);
    Scene_CallPairWith10(1, 0x2000);
    Event_SetMessage(0x27cf);
    State_ApplyArgMode0AndSet10(0x1001);
    Scene_CallPairWith10(3, 0xa000);
    State_ApplyArgMode0AndSet10(3);
    Actor_TurnToAngle(1, 0x8000, 0);
    Scene_CallPairWith10(2, 0xa000);
    Actor_RunRepeatedMotion(21, 2);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_Get(21)->unknown_5a &= ~1;
    Actor_SetPositionAndReset(21, 0x118, 164);
    Event_Wait(1);
    Actor_Get(21)->unknown_5a |= 1;
    Event_Wait(20);
    Actor_ShowEmote(2, 0x102, 40);
    State_ApplyArgMode0AndSet10(2);
    Actor_SetAnimationAndWait(21, 4);
    State_ApplyArgMode0AndSet10(21);
    Actor_ShowEmote(1, 0x103, 20);
    Actor_RunRepeatedMotion(1, 1);
    State_ApplyArgMode0AndSet10(1);
    Actor_RunRepeatedMotion(21, 1);
    Scene_CallPairWith10(21, 0);
    State_ApplyArgMode0AndSet10(21);
    Actor_StartRepeatedMotion(3, 2);
    State_ApplyArgMode0AndSet10(3);
    Actor_SetAnimation(3, 4);
    State_ApplyArgMode0AndSet10(3);
    Actor_ShowEmote(21, 0x105, 40);
    Scene_CallPairWith10(21, 0x3000);
    State_ApplyArgMode0AndSet10(21);
    Actor_Launch(2, 2, 20);
    State_ApplyArgMode0AndSet10(2);
    Actor_SetAnimationAndWait(21, 3);
    State_ApplyArgMode0AndSet10(21);
    Actor_RunRepeatedMotion(1, 1);
    Scene_CallPairWith10(1, 0x2000);
    Event_OpenMessage(1, 0);
    Actor_TurnToAngle(2, 0xe000, 0);
    Actor_TurnToAngle(3, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        yes = TRUE;
    } else {
        gEventWork->message++;
        yes = FALSE;
    }
    Task_Wait(20);
    Actor_TurnToAngle(1, 0x8000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Scene_CallPairWith10(3, 0xa000);
    Scene_CallPairWith10(21, 0);
    Event_ShowMessage(21, 0);
    Audio_PlayCue(17);
    Event_Wait(40);
    if (yes) {
        gEventWork->message++;
    }

    Actor_Stop(20);
    Actor_Stop(19);
    Task_Wait(1);
    actor = Actor_Get(20);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    actor = Actor_Get(19);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    Task_Wait(1);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(21, 0x3000, 0);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(1, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 40);
    Actor_SetAnimation(20, 1);
    Actor_SetSpriteFlags(Actor_Get(20), 1);
    Event_Wait(20);
    Actor_SetSpeed(20, 0x3333, 0x1999);
    Actor_SetPositionAndReset(20, 0x12c, 206);
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_Get(20)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(20, 8);
    Event_Wait(20);
    Actor_SetAnimation(19, 1);
    Actor_SetSpriteFlags(Actor_Get(19), 1);
    Actor_Launch(19, 4, 40);
    Actor_SetSpeed(19, 0x3333, 0x1999);
    Actor_Get(19)->unknown_5a &= ~1;
    Actor_SetPositionAndReset(19, 0x128, 186);
    Actor_SetSpeed(19, 0x1999, 0xccc);
    Actor_SetPositionAndReset(19, 0x124, 186);
    Actor_Get(19)->unknown_5a |= 1;
    Actor_Get(19)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(19, 5);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 1);
    Event_Wait(80);
    Actor_SetAnimationAndWait(3, 4);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(40);
    State_ApplyArgMode0AndSet10(19);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0x2000, 0);
    Actor_TurnToAngle(2, 0x6000, 0);
    Actor_TurnToAngle(3, 0xe000, 40);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(1, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 20);

    actor = Actor_Get(20);
    actor->facing = 0x3000;
    actor->scale_x = 0x10000;
    Actor_SetAnimation(20, 1);
    Event_Wait(10);
    Scene_CallPairWith10(20, 0xd000);
    Actor_Launch(20, 6, 0);
    Event_Wait(10);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0xa000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Actor_TurnToAngle(3, 0xa000, 0);
    Actor_TurnToAngle(21, 0xd000, 0);
    Actor_TurnToAngle(6, 0, 0);
    object = Object_Create(22, actor->x.fixed, actor->y.fixed + 0x80000, actor->z.fixed);
    if (object != NULL) {
        sprite = object->sprite;
        sprite->part_count = 0;
        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        object->priority_flags &= ~1;
        object->motion_flags = 0;
        object->unknown_5c = 1;
        object->speed = 0x19999;
        object->acceleration = 0xcccc;
        buffer = Heap_Allocate(17, 0x608);
        Item_LoadIcon(220);
        Vram_Load(sprite->vram_block, 128, buffer + 0x400);
        Heap_Release(17);
    }
    Actor_SetSpritePriority(22, 1);
    bird = Actor_Get(22);
    bird->x.fixed = actor->x.fixed;
    bird->y.fixed = 0x200000;
    bird->z.fixed = actor->z.fixed;
    bird->motion_flags = 3;
    bird->speed = 0x19999;
    bird->acceleration = 0xcccc;
    bird->scale_x = 0xc000;
    bird->scale_y = 0xc000;
    if (object != NULL) {
        object->motion_flags = 3;
        ((union FieldObject *)object)->effect.velocity_y = 0x9999;
        ((union FieldObject *)object)->effect.velocity_x = 0xcccc;
        object->velocity_y = 0x80000;
        Object_SetPosition(object, 0x1340000, 0x200000, 0xa40000);
    }
    Actor_SetPositionAndCommit(22, 0x134, 164);
    Actor_SetPosition(22, 0, 0);
    Actor_SetSpritePriority(21, 0);
    if (object != NULL) {
        Audio_PlayCue(0x135);
        Actor_SetSpriteFlags(object, 0);
        object->velocity_y = 0x40000;
        Object_SetPosition(object, 0x13a0000, 0x200000, 0x890000);
        Object_CommitPosition(object);
        Audio_PlayCue(0x135);
        object->sprite->priority = 1;
        object->velocity_y = 0x60000;
        Object_SetPosition(object, 0x11d0000, 0x200000, 0x920000);
        Object_CommitPosition(object);
        Audio_PlayCue(0x135);
        object->velocity_y = 0x50000;
        Object_SetPosition(object, 0x12c0000, 0x200000, 0x9a0000);
        Object_CommitPosition(object);
        Task_Wait(6);
        object->x.fixed = 0;
        object->y.fixed = 0;
        object->z.fixed = 0;
        Actor_ParkRecord(object);
    }
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 30);
    Actor_SetSpritePriority(21, 1);
    Actor_Get(21)->priority_flags |= 1;
    Actor_StartRepeatedMotion(2, 2);
    State_ApplyArgMode0AndSet10(2);
    Actor_RunRepeatedMotion(3, 1);
    Event_ShowMessageAndWait(3, 0, 40);
    Actor_RunRepeatedMotion(20, 1);
    Event_Wait(20);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0x5000, 0);
    Actor_TurnToAngle(1, 0x5000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 0);
    Actor_TurnToAngle(6, 0x3000, 0);
    Scene_CallPairWith10(21, 0x3000);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 60);
    Actor_ShowEmote(20, 0x108, 40);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0x2000, 40);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(1, ANIM_NOD);
        yes = TRUE;
    } else {
        gEventWork->message++;
        Actor_SetAnimation(1, ANIM_SHAKE_HEAD);
        yes = FALSE;
    }
    State_ApplyArgMode0AndSet10(1);
    if (yes) {
        gEventWork->message++;
    }
    Event_Wait(20);

    Actor_SetSpriteFlags(Actor_Get(24), 0);
    Actor_SetChildValue(24, 7);
    actor = Actor_Get(24);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = 0x400000;
    actor->z.fixed = 0x9e0000;
    actor->x.fixed = 0x1300000;
    Actor_SetSpriteFlags(Actor_Get(25), 0);
    Actor_SetChildValue(25, 7);
    actor = Actor_Get(25);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = 0x600000;
    actor->x.fixed = 0x1300000;
    actor->z.fixed = 0x9e0000;
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Actor_TurnToAngle(1, 0xa000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Actor_TurnToAngle(3, 0xa000, 0);
    Actor_TurnToAngle(21, 0xd000, 0);
    Actor_TurnToAngle(6, 0, 0);
    Actor_EnableActionCallback(24, Data_0200e088);
    Actor_EnableActionCallback(25, Data_0200e088);
    Audio_PlayCue(145);
    MapRender_SetValues(0x60000, 0x60000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(16);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(24);
    Task_Wait(60);
    Task_AddCallback(SceneTask_020036d0, TASK_PRIORITY_SCENE);
    Audio_PlayCue(141);
    MapRender_SetValues(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(120);
    Actor_EnableActionCallback(24, Data_0200e0ac);
    Actor_EnableActionCallback(25, Data_0200e0ac);
    Task_Wait(120);
    MapRender_SetValues(0x30000, 0x30000, 0x10000);
    ColorBuffer_ApplyTarget(0x203210, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    Audio_PlayCue(63);
    MapRender_SetValues(0x20000, 0x20000, 0x10000);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    MapRender_SetValues(0x10000, 0x10000, 0x10000);

    Actor_SetAnimation(19, 1);
    Actor_TurnToAngle(19, 0, 0);
    Actor_Get(19)->scale_x = 0x10000;
    Actor_Launch(19, 4, 40);
    Actor_RunRepeatedMotion(19, 1);
    Event_ShowMessageAndWait(19, 0, 20);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(1, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 0);
    Actor_TurnToAngle(21, 0x3000, 0);
    Actor_TurnToAngle(6, 0x3000, 20);
    Actor_RunRepeatedMotion(20, 1);
    State_ApplyArgMode0AndSet10(20);
    Actor_RunRepeatedMotion(3, 1);
    Scene_CallPairWith10(3, 0x8000);
    State_ApplyArgMode0AndSet10(3);
    Scene_CallPairWith10(20, 0);
    Actor_SetAnimationAndWait(20, 4);
    State_ApplyArgMode0AndSet10(20);
    Actor_ShowEmote(19, 0x103, 20);
    Actor_StartRepeatedMotion(19, 2);
    State_ApplyArgMode0AndSet10(19);
    Actor_ShowEmote(1, 0x100, 20);
    Scene_CallPairWith10(1, 0x6000);
    State_ApplyArgMode0AndSet10(1);
    Scene_CallPairWith10(20, 0xd000);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 20);
    Actor_RunRepeatedMotion(1, 1);
    State_ApplyArgMode0AndSet10(1);
    Actor_TurnToAngle(19, 0xb000, 20);
    Actor_RunRepeatedMotion(19, 1);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(2, 1);
    Actor_RunRepeatedMotion(3, 1);
    Actor_TurnToAngle(0, 0x8000, 0);
    Actor_TurnToAngle(1, 0x8000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Scene_CallPairWith10(3, 0xa000);
    Actor_ShowEmote(21, 0x101, 80);
    State_ApplyArgMode0AndSet10(21);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0x2000, 0);
    Actor_TurnToAngle(2, 0x6000, 0);
    Actor_TurnToAngle(3, 0xe000, 40);
    Actor_RunRepeatedMotion(20, 1);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0x8000, 0);
    Actor_TurnToAngle(1, 0x8000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 20);
    Scene_CallPairWith10(20, 0xb000);
    State_ApplyArgMode0AndSet10(0x2014);
    Actor_SetAnimationAndWait(21, 3);
    Actor_TurnToAngle(21, 0xb000, 60);
    Actor_TurnToAngle(21, 0x3000, 40);
    Actor_RunRepeatedMotion(19, 1);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_TurnToAngle(21, 0, 40);
    State_ApplyArgMode0AndSet10(21);
    Actor_TurnToAngle(20, 0xd000, 40);
    Scene_CallPairWith10(20, 0xb000);
    State_ApplyArgMode0AndSet10(0x2014);
    Actor_TurnToAngle(21, 0x3000, 40);
    Actor_TurnToAngle(21, 0x3000, 20);
    Actor_SetAnimationAndWait(21, 3);
    Actor_TurnToAngle(19, 0, 40);
    Actor_TurnToAngle(19, 0xb000, 20);
    Actor_ShowEmote(19, 0x101, 40);
    State_ApplyArgMode0AndSet10(0x2013);
    Scene_CallPairWith10(21, 0x3000);
    Actor_ShowEmote(21, 0x101, 20);
    State_ApplyArgMode0AndSet10(21);
    Actor_TurnToAngle(20, 0xb000, 20);
    Actor_SetAnimation(19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_ShowEmote(21, 0x103, 80);
    State_ApplyArgMode0AndSet10(21);
    Actor_SetAnimationAndWait(19, 3);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    Actor_StartRepeatedMotion(21, 2);
    State_ApplyArgMode0AndSet10(21);
    Actor_SetAnimation(19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_ShowEmote(21, 0x102, 60);
    Actor_RunRepeatedMotion(21, 1);
    State_ApplyArgMode0AndSet10(21);
    Actor_ShowEmote(20, 0x108, 40);
    Actor_SetAnimation(20, 4);
    Event_ShowMessageAndWait(0x2014, 0, 40);
    Actor_RunRepeatedMotion(21, 1);
    Event_Wait(20);
    State_ApplyArgMode0AndSet10(21);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Actor_TurnToAngle(3, 0xa000, 20);
    Actor_TurnToAngle(19, 0x3000, 20);
    Actor_SetAnimation(19, 3);
    Actor_SetAnimationAndWait(20, 3);
    Actor_TurnToAngle(21, 0xb000, 20);
    State_ApplyArgMode0AndSet10(0xa015);
    Actor_SetSpeed(6, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    Actor_EnableActionCallback(21, Data_0200e22c);
    Event_Wait(20);
    Actor_EnableActionCallback(6, Data_0200e22c);
    Scene_CallPairWith10(1, 0x6000);
    Actor_StartRepeatedMotion(1, 2);
    State_ApplyArgMode0AndSet10(1);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x40250d, 1);
    ColorBuffer_Interpolate(40);
    Scene_CallPairWith10(20, 0xd000);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 0);
    Scene_RunSetupSequence35c4();
    Actor_SetChildValue(20, 7);
    Actor_SetChildValue(19, 7);
    Event_Wait(20);
    Actor_SetChildValue(20, 0x100);
    Actor_SetChildValue(19, 0x100);
    Event_Wait(20);
    Scene_RunSetupSequence35c4();
    Actor_Launch(3, 2, 20);
    State_ApplyArgMode0AndSet10(3);
    Scene_CallPairWith10(19, 0);
    State_ApplyArgMode0AndSet10(19);
    Scene_RunSetupSequence35c4();

    origin = Actor_Get(19);
    options_a.palette = 7;
    options_a.update = Effect_MoveWithDrag;
    options_a.start_scale_x = 0x10000;
    options_a.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        velocity[0] = Math_Cos(i << 12);
        velocity[1] = 0;
        velocity[2] = Math_Sin(i << 12) * 2;
        velocity[0] *= 3;
        Effect_Spawn(origin->x.fixed, origin->y.fixed, origin->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &options_a);
    }
    Audio_PlayCue(212);
    Task_Wait(6);
    Scene_RunSetupSequence35c4();
    origin = Actor_Get(20);
    options_b.palette = 7;
    options_b.update = Effect_MoveWithDrag;
    options_b.start_scale_x = 0x10000;
    options_b.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        velocity[0] = Math_Cos(i << 12);
        velocity[1] = 0;
        velocity[2] = Math_Sin(i << 12) * 2;
        velocity[0] *= 3;
        Effect_Spawn(origin->x.fixed, origin->y.fixed, origin->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &options_b);
    }
    Audio_PlayCue(212);
    Actor_Launch(2, 6, 20);
    Audio_PlayCue(54);
    State_ApplyArgMode0AndSet10(2);
    Actor_SetAnimation(20, 4);
    State_ApplyArgMode0AndSet10(20);
    Scene_RunSetupSequence35c4();
    Actor_SetSpeed(20, 0x3333, 0x1999);
    Actor_SetSpeed(19, 0x3333, 0x1999);
    Actor_Get(20)->unknown_5a &= ~1;
    Actor_Get(19)->unknown_5a &= ~1;
    Actor_SetDestination(20, 0x126, 196);
    Actor_SetDestination(19, 0x126, 196);
    Task_AddCallback(SceneTask_020037c4, TASK_PRIORITY_SCENE);
    Actor_StartRepeatedMotion(1, 2);
    Event_ShowMessage(1, 0);
    GameFlag_Set(0x234);
    Event_ShowMessage(2, 0);
    GameFlag_Set(0x235);
    Scene_RunSetupSequence35c4();
    Event_Wait(20);
    Scene_RunSetupSequence35c4();
    Event_Wait(20);
    ((u8 *)&gGameState)[0x22b] = 3;
    Engine_GameStateSetReturn((s32)&LinkedScene_VinasuChojo, 3);
    Engine_GameStateSetWarp((s32)&LinkedScene_VinasuChojo, 9);
    Engine_Import0808a250(98, 0);
    Engine_Import08077268();
    GameFlag_Set(0x351);
}
