/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/HEYA2.C; edition ES; function FieldScene_RunFourActorEncounter.
 * Removing FIELD_EVENT.H Event_OpenMessage changes mov	r0, #3 to mov	r7, r8
 * (628/635 assembly lines).
 * Unrelated function bodies are declarations; original admission comments
 * and approved Dma/Iwram header ownership are preserved where used.
 * Production retains the measured adapter; no compiler options changed.
 */
#define ALCHEMY_TYPES_H







typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;



typedef int bool;


struct GameState {
    u8 unknown_000[0x10];

    s32 coins;
    u8 unknown_014[0x118];

    s8 won_prizes[0x10];
    u8 unknown_13c[0x84];
    s16 scene;
    s16 entrance;
    s16 saved_scene;
    s16 saved_entrance;
    u8 unknown_1c8[0x0e];

    s16 special;
    u8 unknown_1d8[4];

    s32 x;
    s32 y;
    s32 z;

    u32 heading;
    u16 turn;

    s16 scene_cue;
    u8 unknown_1f0[2];

    u8 movement_mode;
    u8 unknown_1f3;

    s32 selected_actor;

    u8 active_owners[8];
    u8 unknown_200[0x2c];
    u16 unknown_22c;
    u16 unknown_22e;
    u16 unknown_230;
    s16 unknown_232;
    u8 unknown_234[0x0a];
    s16 unknown_23e;

    s16 retreat_scene;
    s16 retreat_entrance;
    u8 unknown_244[8];

    s16 cloaked;
    u8 unknown_24e[0x56];


    u16 link_tallies[8];
};

typedef char GameState_Coins[(u32)&(((struct GameState *)0)->coins) == (0x10) ? 1 : -1];
typedef char GameState_WonPrizes[(u32)&(((struct GameState *)0)->won_prizes) == (0x12c) ? 1 : -1];
typedef char GameState_Entrance[(u32)&(((struct GameState *)0)->entrance) == (0x1c2) ? 1 : -1];
typedef char GameState_Special[(u32)&(((struct GameState *)0)->special) == (0x1d6) ? 1 : -1];
typedef char GameState_X[(u32)&(((struct GameState *)0)->x) == (0x1dc) ? 1 : -1];
typedef char GameState_MovementMode[(u32)&(((struct GameState *)0)->movement_mode) == (0x1f2) ? 1 : -1];
typedef char GameState_SelectedActor[(u32)&(((struct GameState *)0)->selected_actor) == (0x1f4) ? 1 : -1];
typedef char GameState_ActiveOwners[(u32)&(((struct GameState *)0)->active_owners) == (0x1f8) ? 1 : -1];
typedef char GameState_RetreatEntrance[(u32)&(((struct GameState *)0)->retreat_entrance) == (0x242) ? 1 : -1];
typedef char GameState_Cloaked[(u32)&(((struct GameState *)0)->cloaked) == (0x24c) ? 1 : -1];
typedef char GameState_LinkTallies[(u32)&(((struct GameState *)0)->link_tallies) == (0x2a4) ? 1 : -1];

extern struct GameState gGameState;




extern u8 gSceneState[];


extern u8 *gKorosseoWork;


extern u32 gFrameCount;


struct EventWork {
    u8 unknown_000[0x34];

    struct FieldActor *placed_actors[58];
    u8 unknown_11c[0x50];

    s16 touched_trigger;
    u8 unknown_16e[4];
    u16 unknown_172;
    u8 unknown_174[0xa];

    s16 psynergy_request;
    u8 unknown_180[2];

    s16 raised_trigger;
    u8 unknown_184[0x3c];

    s32 start_transition;
    u8 unknown_1c4[4];

    s32 transition_frames;
    u8 unknown_1cc[0x0c];

    u16 message;
    u8 unknown_1da[6];

    struct FieldActor *view_center;
};

typedef char EventWork_PlacedActors[(u32)&(((struct EventWork *)0)->placed_actors) == (0x34) ? 1 : -1];
typedef char EventWork_TouchedTrigger[(u32)&(((struct EventWork *)0)->touched_trigger) == (0x16c) ? 1 : -1];
typedef char EventWork_PsynergyRequest[(u32)&(((struct EventWork *)0)->psynergy_request) == (0x17e) ? 1 : -1];
typedef char EventWork_RaisedTrigger[(u32)&(((struct EventWork *)0)->raised_trigger) == (0x182) ? 1 : -1];
typedef char EventWork_StartTransition[(u32)&(((struct EventWork *)0)->start_transition) == (0x1c0) ? 1 : -1];
typedef char EventWork_TransitionFrames[(u32)&(((struct EventWork *)0)->transition_frames) == (0x1c8) ? 1 : -1];
typedef char EventWork_Message[(u32)&(((struct EventWork *)0)->message) == (0x1d8) ? 1 : -1];
typedef char EventWork_ViewCenter[(u32)&(((struct EventWork *)0)->view_center) == (0x1e0) ? 1 : -1];

extern struct EventWork *gEventWork;


enum SceneTransitionStyle {
    TRANSITION_FADE = 0,

    TRANSITION_BACKDROP_FADE = 1,
    TRANSITION_WINDOW = 2
};




union FieldCoordinate {
    s32 fixed;
    struct {
        u16 fraction;
        s16 pixel;
    } part;
};





struct FieldSprite {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 flip_x : 1;
    u16 flip_y : 1;
    u16 tile : 10;

    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x0a];

    u16 second_tile : 10;
    u16 second_priority : 2;
    u16 second_palette : 4;
    u8 unknown_16[2];

    s32 scale;

    u8 vram_block;
    u8 unknown_1d;
    u16 rotation;
    u8 unknown_20[6];
    u8 flags;
    u8 part_count;
};

typedef char FieldSprite_Scale[(u32)&(((struct FieldSprite *)0)->scale) == (0x18) ? 1 : -1];
typedef char FieldSprite_VramBlock[(u32)&(((struct FieldSprite *)0)->vram_block) == (0x1c) ? 1 : -1];
typedef char FieldSprite_Rotation[(u32)&(((struct FieldSprite *)0)->rotation) == (0x1e) ? 1 : -1];
typedef char FieldSprite_PartCount[(u32)&(((struct FieldSprite *)0)->part_count) == (0x27) ? 1 : -1];





union FieldObject;


struct FieldActor {
    u8 unknown_00[6];
    u16 facing;
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unknown_14[4];

    s32 scale_x;
    s32 scale_y;

    u16 radius;
    u8 unknown_22;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;

    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x0c];
    struct FieldSprite *sprite;

    u8 active;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c;
    u8 unknown_5d[5];

    u8 rise_counter;
    u8 rise_enabled;
    u16 unknown_64;
    u16 unknown_66;
    u8 unknown_68[4];

    void (*update)(union FieldObject *object);
};

typedef char FieldActor_Facing[(u32)&(((struct FieldActor *)0)->facing) == (0x06) ? 1 : -1];
typedef char FieldActor_X[(u32)&(((struct FieldActor *)0)->x) == (0x08) ? 1 : -1];
typedef char FieldActor_Z[(u32)&(((struct FieldActor *)0)->z) == (0x10) ? 1 : -1];
typedef char FieldActor_ScaleY[(u32)&(((struct FieldActor *)0)->scale_y) == (0x1c) ? 1 : -1];
typedef char FieldActor_PriorityFlags[(u32)&(((struct FieldActor *)0)->priority_flags) == (0x23) ? 1 : -1];
typedef char FieldActor_MotionFlags[(u32)&(((struct FieldActor *)0)->motion_flags) == (0x55) ? 1 : -1];
typedef char FieldActor_CollisionFlags[(u32)&(((struct FieldActor *)0)->collision_flags) == (0x59) ? 1 : -1];
typedef char FieldActor_RiseEnabled[(u32)&(((struct FieldActor *)0)->rise_enabled) == (0x63) ? 1 : -1];
typedef char FieldActor_VelocityY[(u32)&(((struct FieldActor *)0)->velocity_y) == (0x28) ? 1 : -1];
typedef char FieldActor_Speed[(u32)&(((struct FieldActor *)0)->speed) == (0x30) ? 1 : -1];
typedef char FieldActor_TargetX[(u32)&(((struct FieldActor *)0)->target_x) == (0x38) ? 1 : -1];
typedef char FieldActor_Sprite[(u32)&(((struct FieldActor *)0)->sprite) == (0x50) ? 1 : -1];
typedef char FieldActor_Update[(u32)&(((struct FieldActor *)0)->update) == (0x6c) ? 1 : -1];





enum ActorPriorityFlag {

    ACTOR_PRIORITY_AUTOMATIC = 0x01,




    ACTOR_PRIORITY_UNDERFOOT = 0x02
};


enum ActorMotionFlag {

    ACTOR_FOLLOWS_TERRAIN = 0x01,

    ACTOR_FALLS = 0x02
};


enum {
    TASK_PRIORITY_SCENE = 3200
};





enum ActorAnimation {
    ANIM_STAND = 1,
    ANIM_WALK = 2,
    ANIM_NOD = 3,
    ANIM_SHAKE_HEAD = 4
};


void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
void Engine_TaskWait(s32 frames);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
s32 Engine_EventAskYesNo(s32 speaker, s32 flags);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
void Engine_MessageShowCentered(s32 message, s32 flags);
void Engine_EventRequestExit(s32 exit);
void Engine_EventOpenScreen(void);
void Engine_EventCloseScreen(void);
void Engine_EventWaitForScreen(void);
void Engine_BlendSetDarkenTarget16(s32 target);
u8 *Engine_ResourceGetTableEntry(s32 resource);
void Engine_ResourceDecodeType01(const u8 *source, void *destination);
struct FieldActor *Object_GetById(s32 actor);
void Engine_ActorSetPosition(s32 actor, s32 fixed_x, s32 fixed_z);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorWalkTo(s32 actor, s32 x, s32 z);
void Engine_ActorWalkBy(s32 actor, s32 dx, s32 dz);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorFaceDirection(s32 actor, s32 facing, s32 frames);
void Engine_ActorTurnToAngle(s32 actor, s32 angle, s32 frames);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_ActorFaceEachOther(s32 actor, s32 other, s32 frames);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
void Engine_ActorSetAnimationAndWait(s32 actor, s32 animation);
void Engine_ActorStartRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorRunRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorShowEmote(s32 actor, s32 emote, s32 frames);
void Engine_ActorSetAttachedEffect(s32 actor, s32 effect);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void Engine_ActorSetSpriteFlags(struct FieldActor *actor, s32 flags);
void Engine_CameraFollowActor(s32 actor, s32 keep_position);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
void Engine_CameraMoveToActor(s32 actor, s32 pan);
void Engine_CameraWaitForMove(void);
void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                  s32 dest_y);
void Engine_MapRedraw(void);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
s32 Engine_GameFlagIsSet(s32 flag);
s32 Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
void Engine_AudioPlayCue(s32 cue);
void Engine_ShopOpen(s32 shop, s32 keeper);
void Engine_InnOpen(s32 inn, s32 keeper);
void Engine_SanctumOpen(s32 priest);
s32 Engine_TaskAddCallback(void (*callback)(void), s32 priority);
s32 Engine_TaskRemoveCallback(void (*callback)(void));
s32 Engine_RandomNext(void);
s32 Engine_MathSin(s32 angle);
s32 Engine_MathCos(s32 angle);
s32 __divsi3(s32 dividend, s32 divisor);
void *Engine_HeapAllocate(s32 slot, s32 size);
void Engine_HeapRelease(s32 slot);
s32 Engine_VramLoad(s32 block, s32 size, const void *data);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void Object_SetMode(struct FieldActor *object, s32 animation);
void Engine_ObjectSetScript(struct FieldActor *object, const s32 *script);
void Engine_ObjectSetBlendMode(struct FieldActor *object, s32 mode);
void ObjectGroup_SetChildValue(struct FieldActor *object, s32 palette);
void Engine_ObjectSetPartPalettes(struct FieldActor *object, s32 palette);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void Engine_MapAnimateCells(const u16 *steps, s32 dest_x, s32 dest_y);
void Engine_MapObjectSetPosition(s32 object, s32 fixed_x, s32 fixed_z);
void Engine_ItemLoadIcon(s32 item);
void Engine_ItemShowFound(s32 item, s32 height);
s32 Engine_PartyGiveItem(s32 item, s32 flags);
void Engine_ActorStop(s32 actor);
void Engine_ActorCenterAndWalk(s32 actor, s32 priority, s32 dz);
void Engine_ActorWalkByAndWait(s32 actor, s32 dx, s32 dz);
s32 Engine_LeaderCheckAhead(void);
void Engine_PsynergyBegin(s32 ability, s32 flags);
void Engine_PsynergySetTarget(s32 caster, s32 target);
void Engine_PsynergyRaiseHands(void);
void Engine_PsynergyPlayEffect(s32 effect);
void Engine_PsynergyLowerHands(void);
void Engine_PsynergyCancel(void);
void Engine_EventShowMessageAndWait(s32 speaker, s32 flags, s32 frames);
struct FieldActor *Engine_EventGetViewCenter(void);
struct FieldActor *Engine_ActorLookup(s32 actor);
void Engine_ActorDestroy(s32 actor);
void Engine_ActorSetChildValue(s32 actor, s32 value);
void Engine_ActorEnableActionCallback(s32 actor, const u8 *table);
void Engine_ActorSetActionCallback(struct FieldActor *actor, s32 value);
void Engine_ActorsRefresh(void);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Engine_MapRenderSetValues(s32 value0, s32 value1, s32 value2);
void Engine_ColorBufferApplySource(s32 value, s32 mode);
void Engine_ColorBufferApplyTarget(s32 value, s32 mode);
void Engine_ColorBufferInterpolate(s32 frames);
void Engine_ActorMoveToAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
void Engine_EventShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
                                        s32 other_speaker, s32 other_x, s32 other_y,
                                        s32 other_arg, s32 other_extra, s32 flags);
void Engine_PartyAddMembers(s32 first, s32 second);
void Engine_MapRenderWaitForValues(void);


s32 Engine_UiWorkWaitThenFinalizeCapacity(s32 first, s32 second);
void Engine_ObjectMotionArmCallback(s32 actor, s32 angle, s32 frames);
void Engine_ObjectMotionSetPositionAndCommit(s32 actor, s32 x, s32 z);
void Engine_ObjectMotionSetPositionAndReset(s32 actor, s32 x, s32 z);

void Engine_ObjectMotionLaunch(s32 actor, s32 speed, s32 frames);
void Engine_ObjectDispatchRelease(struct FieldActor *object);
void Engine_MapWaitWorkValuesBelow256(void);
void Engine_RunRisingObjectSequence(struct FieldActor *object, s32 mode);










static inline void Event_ShowMessage(s32 speaker, s32 flags)
{
    Engine_EventShowMessage(speaker, flags);
}
static inline s32 Event_AskYesNo(s32 speaker, s32 flags);


static inline s32 Event_ChooseYesNo(s32 actor, s32 flags)
{
    return Engine_EventChooseYesNo(actor, flags);
}
static inline void Message_ShowCentered(s32 message, s32 flags);

static inline void Event_RequestExit(s32 exit);


static inline void Event_OpenScreen(void)
{
    Engine_EventOpenScreen();
}
static inline void Event_CloseScreen(void);


static inline void Event_WaitForScreen(void)
{
    Engine_EventWaitForScreen();
}





static inline struct FieldActor *Actor_Get(s32 actor)
{
    return Object_GetById(actor);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}


static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    Engine_ActorSetSpeed(actor, speed, acceleration);
}
static inline void Actor_SetDestination(s32 actor, s32 x, s32 z);

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz);

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z);

static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz);

static inline void Actor_WaitForMove(s32 actor);








static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
    Engine_ActorFaceDirection(actor, facing, frames);
}
static inline void Actor_TurnToAngle(s32 actor, s32 angle, s32 frames);

static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames);

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames);



static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
    Engine_ActorSetAnimation(actor, animation);
}







static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorStartRepeatedMotion(actor, repeats);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorRunRepeatedMotion(actor, repeats);
}


static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
    Engine_ActorShowEmote(actor, emote, frames);
}


static inline void Actor_SetAttachedEffect(s32 actor, s32 effect)
{
    Engine_ActorSetAttachedEffect(actor, effect);
}

static inline void Actor_SetSpritePriority(s32 actor, s32 priority)
{
    Engine_ActorSetSpritePriority(actor, priority);
}
static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags);

static inline void Camera_FollowActor(s32 actor, s32 keep_position);


static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    Engine_CameraSetSpeed(speed, acceleration);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
    Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}
static inline void Camera_MoveToActor(s32 actor, s32 pan);


static inline void Camera_WaitForMove(void)
{
    Engine_CameraWaitForMove();
}





static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y)
{
    Engine_MapCopyCells(src_x, src_y, width, height, dest_x, dest_y);
}


static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                          s32 dest_x, s32 dest_y)
{
    Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}
static inline void Map_Redraw(void);

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third);






enum {
    FLAG_ARRIVAL_EVENT_PENDING = 0x12f
};

static inline s32 GameFlag_IsSet(s32 flag)
{
    return Engine_GameFlagIsSet(flag);
}

static inline s32 GameFlag_Set(s32 flag)
{
    return Engine_GameFlagSet(flag);
}
static inline void GameFlag_Clear(s32 flag);



static inline void Audio_PlayCue(s32 cue)
{
    Engine_AudioPlayCue(cue);
}
static inline void Shop_Open(s32 shop, s32 keeper);

static inline void Inn_Open(s32 inn, s32 keeper);

static inline void Sanctum_Open(s32 priest);

static inline s32 Task_AddCallback(void (*callback)(void), s32 priority);

static inline s32 Task_RemoveCallback(void (*callback)(void));

static inline s32 Random_Next(void);

static inline s32 Math_Sin(s32 angle);

static inline s32 Math_Cos(s32 angle);

static inline s32 Math_Divide(s32 dividend, s32 divisor);

static inline void *Heap_Allocate(s32 slot, s32 size);

static inline void Heap_Release(s32 slot);

static inline s32 Vram_Load(s32 block, s32 size, const void *data);

static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z);

static inline void Object_SetAnimation(struct FieldActor *object, s32 animation);

static inline void Object_SetScript(struct FieldActor *object, const s32 *script);



enum ObjectBlendMode {
    OBJECT_BLEND_NORMAL = 0,
    OBJECT_BLEND_TRANSLUCENT = 1
};
static inline void Object_SetBlendMode(struct FieldActor *object, s32 mode);

static inline void Object_SetPalette(struct FieldActor *object, s32 palette);

static inline void Object_SetPartPalettes(struct FieldActor *object, s32 palette);

static inline void Map_AnimateCells(const u16 *steps, s32 dest_x, s32 dest_y);

static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z);

static inline void Item_LoadIcon(s32 item);

static inline void Item_ShowFound(s32 item, s32 height);

static inline s32 Party_GiveItem(s32 item, s32 flags);

static inline void Actor_Stop(s32 actor);

static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz);

static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz);

static inline s32 Leader_CheckAhead(void);

static inline void Psynergy_Begin(s32 ability, s32 flags);

static inline void Psynergy_SetTarget(s32 caster, s32 target);

static inline void Psynergy_RaiseHands(void);

static inline void Psynergy_PlayEffect(s32 effect);

static inline void Psynergy_LowerHands(void);

static inline void Psynergy_Cancel(void);



static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags, s32 frames)
{
    Engine_EventShowMessageAndWait(speaker, flags, frames);
}
static inline struct FieldActor *Event_GetViewCenter(void);

static inline struct FieldActor *Actor_Lookup(s32 actor);

static inline void Actor_Destroy(s32 actor);

static inline void Actor_SetChildValue(s32 actor, s32 value);








static inline void Actor_EnableActionCallback(s32 actor, const u8 *table)
{
    Engine_ActorEnableActionCallback(actor, table);
}
static inline void Actor_SetActionCallback(struct FieldActor *actor, s32 value);

static inline void Actors_Refresh(void);

static inline void Map_CopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width,
                                   s32 height);

static inline void MapRender_SetValues(s32 value0, s32 value1, s32 value2);



static inline void ColorBuffer_ApplySource(s32 value, s32 mode)
{
    Engine_ColorBufferApplySource(value, mode);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode)
{
    Engine_ColorBufferApplyTarget(value, mode);
}

static inline void ColorBuffer_Interpolate(s32 frames)
{
    Engine_ColorBufferInterpolate(frames);
}
static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z);

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z);

static inline void Actor_Jump(s32 actor, s32 height, s32 frames);

static inline void Event_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
                                                s32 other_speaker, s32 other_x, s32 other_y,
                                                s32 other_arg, s32 other_extra, s32 flags);

static inline void Party_AddMembers(s32 first, s32 second);

static inline void MapRender_WaitForValues(void);



extern char SceneId_Clear;
extern char SceneId_Title;


extern char SceneId_WorldMap;
extern char SceneId_HaidiaMura;
extern char SceneId_HaidiaIe;
extern char SceneId_SoruIriguchi1;
extern char SceneId_SoruIriguchi2;
extern char SceneId_KuupuappuHeya;
extern char SceneId_GomaSuiro1;
extern char SceneId_GomaSuiro2;
extern char SceneId_BiribinoMura1;
extern char SceneId_BiribinoMura2;
extern char SceneId_BiribinoKyuden;
extern char SceneId_BiribinoNiwa;
extern char SceneId_BiribinoMura3;
extern char SceneId_KorimaMura1;
extern char SceneId_KorimaMura2;
extern char SceneId_KorimaMura3;
extern char SceneId_KorimaHashi;
extern char SceneId_ToretoHeya;
extern char SceneId_BiribinoDou1;
extern char SceneId_BiribinoDou2;
extern char SceneId_BiribinoDou3;
extern char SceneId_ImiruMura1;
extern char SceneId_ImiruMura2;
extern char SceneId_ImiruFuchin1;
extern char SceneId_MakyuriIriguchi;
extern char SceneId_MakyuriHeya1;
extern char SceneId_MakyuriHeya2;
extern char SceneId_MakyuriHeya3;
extern char SceneId_MakyuriHeya4;
extern char SceneId_MakyuriChojo1;
extern char SceneId_ShianJiin1;
extern char SceneId_ShianJiin2;
extern char SceneId_ImiruFuchin2;
extern char SceneId_ImiruFuchin3;
extern char SceneId_ImiruFuchin4;
extern char SceneId_ImiruFuchin5;
extern char SceneId_ImiruFuchin6;
extern char SceneId_ImiruFuchin7;
extern char SceneId_MogoruMori1;
extern char SceneId_MogoruMori2;
extern char SceneId_MogoruMori3;
extern char SceneId_YamaRama1;
extern char SceneId_ArutinMura1;
extern char SceneId_ArutinMura2;
extern char SceneId_ArutinYama1;
extern char SceneId_ArutinYama2;
extern char SceneId_ArutinYama3;
extern char SceneId_ArutinYama4;
extern char SceneId_ArutinYama5;
extern char SceneId_ArutinYama6;
extern char SceneId_ArutinYama7;
extern char SceneId_ArutinYama8;
extern char SceneId_ArutinYama9;
extern char SceneId_ArutinYama10;
extern char SceneId_ArutinYama11;
extern char SceneId_YamaRama2;
extern char SceneId_RamakanSabaku1;
extern char SceneId_RamakanSabaku2;
extern char SceneId_RamakanSabaku3;
extern char SceneId_RamakanSabaku4;
extern char SceneId_HaidiaDou1;
extern char SceneId_HaidiaDou2;
extern char SceneId_HaidiaDou3;
extern char SceneId_KuupuappuDou1;
extern char SceneId_KuupuappuDou2;
extern char SceneId_KuupuappuDou3;
extern char SceneId_KareiMachi1;
extern char SceneId_KareiHeya1;
extern char SceneId_KareiHeya2;
extern char SceneId_KareiMachi2;
extern char SceneId_KareiKyuden;
extern char SceneId_RunpaMura1;
extern char SceneId_RunpaSuhara;
extern char SceneId_RunpaDou;
extern char SceneId_KareiTorebi1;
extern char SceneId_KareiTorebi2;
extern char SceneId_FuneKanpan;
extern char SceneId_FuneHeya;
extern char SceneId_KareiTorebi3;
extern char SceneId_TakaraHashira1;
extern char SceneId_TakaraHashira2;
extern char SceneId_TakaraHashira3;
extern char SceneId_TakaraHashira4;
extern char SceneId_TakaraHashira5;
extern char SceneId_KorosseoKawa;
extern char SceneId_KorosseoKabe;
extern char SceneId_TakaraAshiba1;
extern char SceneId_TakaraAshiba2;
extern char SceneId_TakaraAshiba3;
extern char SceneId_TorebiKyuden1;
extern char SceneId_TorebiKyuden2;
extern char SceneId_KorosseoMaruta;

extern char SceneId_KorashiamuIriguchi1;
extern char SceneId_KorashiamuIriguchi2;
extern char SceneId_KorashiamuIriguchi3;

extern char SceneId_TakaraShima1;
extern char SceneId_TakaraShima2;
extern char SceneId_TakaraShima3;
extern char SceneId_TakaraShima4;
extern char SceneId_TakaraShima5;
extern char SceneId_TakaraShima6;
extern char SceneId_TakaraShima14;
extern char SceneId_TorebiHeya;
extern char SceneId_TorebiIzumi1;

extern char SceneId_ArutamiraDou1;
extern char SceneId_ArutamiraDou2;
extern char SceneId_ArutamiraDou3;
extern char SceneId_ArutamiraDou4;
extern char SceneId_ArutamiraDou5;
extern char SceneId_ArutamiraDou6;
extern char SceneId_KaragoruDou1;
extern char SceneId_KareiMachi3;
extern char SceneId_KareiMachi4;
extern char SceneId_KareiMachi5;
extern char SceneId_KareiMachi6;
extern char SceneId_RunpaMura2;
extern char SceneId_RunpaJo1;
extern char SceneId_RunpaJo2;
extern char SceneId_RunpaJo3;
extern char SceneId_RunpaJo4;
extern char SceneId_SuharaGate1;
extern char SceneId_SuharaGate2;
extern char SceneId_SuharaGate3;
extern char SceneId_SuharaSabaku1;
extern char SceneId_SuharaSabaku2;
extern char SceneId_SuharaSabaku3;

extern char SceneId_KaragoruDou2;
extern char SceneId_KaragoruDou3;

extern char SceneId_BabiChika1;
extern char SceneId_BabiChika2;
extern char SceneId_BabiIriguchi1;
extern char SceneId_BabiIriguchi2;
extern char SceneId_BabiIriguchi3;
extern char SceneId_RariberoHeya1;
extern char SceneId_RariberoHeya2;
extern char SceneId_VinasuHeya1;
extern char SceneId_VinasuHeya2;
extern char SceneId_VinasuHeya3;
extern char SceneId_VinasuHeya4;
extern char SceneId_VinasuHeya5;
extern char SceneId_VinasuHeya6;
extern char SceneId_TorebiIzumi2;
extern char SceneId_LinkLobby;
extern char SceneId_VinasuChojo;


enum SceneId {
    SCENE_WORLD_MAP = 2,
    SCENE_KUUPUAPPU_MURA = 20,
    SCENE_KUUPUAPPU_RUNPA = 22,
    SCENE_KUUPUAPPU_MURA_SAI = 23,
    SCENE_RUNPA_MURA = 104,
    SCENE_RUNPA_SUHARA = 105,
    SCENE_RUNPA_DOU = 106,
    SCENE_RUNPA_JO_GATE = 159,
    SCENE_RUNPA_JO = 160,
    SCENE_SUHARA_GATE = 169
};


enum Facing {
    FACING_EAST = 0x0000,
    FACING_SOUTHEAST = 0x2000,
    FACING_SOUTH = 0x4000,
    FACING_SOUTHWEST = 0x6000,
    FACING_WEST = 0x8000,
    FACING_NORTHWEST = 0xa000,
    FACING_NORTH = 0xc000,
    FACING_NORTHEAST = 0xe000
};


enum {
    FACING_STEP = 0x1000
};





enum {

    SCENE_TABLE_END = -1,

    CONDITION_ALWAYS = -1,

    CONDITION_FLAG_SET = 0x1000
};








enum {
    FLAG_PARTY_LEFT_VALE = 0x815
};


struct SceneEntrance {
    s16 entrance;
    s16 required_flag;
    s16 x;
    s16 y;
    s16 z;
    u16 facing;
    s16 unused1;

    s16 camera_left;
    s16 camera_top;
    s16 camera_right;
    s16 camera_bottom;
    s16 unused2;
};

typedef char SceneEntrance_Size[sizeof(struct SceneEntrance) == (24) ? 1 : -1];
typedef char SceneEntrance_CameraLeft[(u32)&(((struct SceneEntrance *)0)->camera_left) == (14) ? 1 : -1];






struct ScenePlacement {
    s16 sprite;
    s16 condition;
    s32 behavior;
    s32 x;
    s32 y;
    s32 z;
    u16 facing;
    u8 talk_facing;
    u8 flags;
};

typedef char ScenePlacement_Size[sizeof(struct ScenePlacement) == (24) ? 1 : -1];
typedef char ScenePlacement_TalkFacing[(u32)&(((struct ScenePlacement *)0)->talk_facing) == (22) ? 1 : -1];

enum ActorBehavior {
    ACTOR_STAND = 1,
    ACTOR_WANDER = 2
};


enum ActorTalkFacing {
    TALK_FACE_PARTY = 0,
    TALK_FACE_PARTY_AND_BACK = 1,
    TALK_KEEP_FACING = 2
};


enum {
    ACTOR_PARTY_LEADER = 0,
    ACTOR_GERALD = 1,
    ACTOR_IVAN = 2,
    ACTOR_MIA = 3,
    ACTOR_JASMINE = 5,
    ACTOR_FIRST_PLACED = 8
};




struct SceneEvent {
    u32 control;
    s16 trigger;
    s16 condition;
    u32 value;
};

typedef char SceneEvent_Size[sizeof(struct SceneEvent) == (12) ? 1 : -1];


enum SceneEventKind {

    EVENT_TALK = 0,

    EVENT_EXIT = 1,

    EVENT_TOUCH = 2,

    EVENT_SEARCH = 3,

    EVENT_PSYNERGY = 5,

    EVENT_RAISED = 6
};





enum SearchTarget {

    SEARCH_UNNAMED = 0,
    SEARCH_CHEST = 1,
    SEARCH_JAR,
    SEARCH_BARREL,
    SEARCH_WALL,
    SEARCH_GROUND,
    SEARCH_ROCK,
    SEARCH_HOLE,
    SEARCH_GRAVE,
    SEARCH_TREE,
    SEARCH_UNDERBRUSH,
    SEARCH_DOOR,
    SEARCH_CHIMNEY,
    SEARCH_WOODEN_BOX,
    SEARCH_BED,
    SEARCH_BOOKCASE,
    SEARCH_STONE_COFFIN,
    SEARCH_FIREPLACE,
    SEARCH_WATER,
    SEARCH_STONE_PILLAR,
    SEARCH_STALACTITE,
    SEARCH_BOARDS,
    SEARCH_FOUNTAIN,
    SEARCH_OVEN,
    SEARCH_TABLE,
    SEARCH_STONE_STATUE,
    SEARCH_STONE_TABLET,
    SEARCH_SHELF,
    SEARCH_WARDROBE,
    SEARCH_FIREWOOD,
    SEARCH_BOOKS,
    SEARCH_WELL
};


struct SceneRegion;


s32 Scene_Initialize(void);
const struct SceneEntrance *Scene_GetEntrances(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
const struct SceneRegion *Scene_GetRegions(void);






void *CreateOverlayObject(s32, s32, s32, s32);
void SetOverlayObjectMode(void *, s32);
void SetOverlayObjectSlot(void *, s32);


struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct ConfiguredEffectOptions {
    s32 mode_bits;
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};



struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};







extern s16 *ToretoHeya_PaletteBuffer;
extern u8 ToretoHeya_ActionTable2[];
extern u8 ToretoHeya_ActionTable1[];
extern u8 ToretoHeya_SparkOrigin[];
extern u8 ToretoHeya_SparkCounter[];

void ToretoHeya_HandleFloorSwitch(int, int, int, int);
void ToretoHeya_PlayGesture();
void PartyInventory_FindOwner();
void Object_SetActionCallbackAndRefreshById();
void ToretoHeya_SpawnSwirlSparks();
void ToretoPalette_ApplyTint(void);
void ToretoHeya_AdvanceEffectMotion();
static __inline__ void Call1(void (*f)(), s32 a0);


static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */


    f(a0, a1);
}
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2);

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10);

static __inline__ s32 Value0(s32 (*f)());

static __inline__ s32 Value1(s32 (*f)(), s32 a0);

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1);

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2);

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3);

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static __inline__ s32 Value7(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

static __inline__ s32 Iwram_Call2(s32 left, s32 right, void *routine);




extern u8 IwramMulQ16ReturnIp[];

extern u8 IwramIrqMain[];
static __inline__ void Dma_Set(const void *source, void *destination, u32 control, volatile u32 *channel);






extern const struct SceneEntrance gToretoHeyaEntrances[];
extern const struct SceneRegion gToretoHeyaRegions[];
extern const u32 gToretoHeyaExits[];
extern const struct ScenePlacement gToretoHeyaPlacements[];

void Event_SetPairWork1c0(s32 scene, s32 entrance);

extern const struct SceneEvent gToretoHeyaEvents[];

extern u8 MsgToretoHmHrooom[];


extern const u8 ToretoHeya_TableActions0[];
extern const u8 ToretoHeya_TableActions1[];
extern const u8 ToretoHeya_TableActions2[];
extern const u8 ToretoHeya_TableActions3[];
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 mode);

extern u8 MsgToretoDoingNowsNot[];
extern u8 MsgToretoMmmmm[];
extern u8 MsgToretoTurnedPeopleKolima[];

extern struct MapRenderWork *gMapWork;
void ToretoPalette_CaptureBank(void);
void ToretoHeya_ApplyFlaggedMapPatches(void);
void SceneState_ApplyRectsByFlag844(s32 flag);
void ToretoHeya_RunLandingDustScene(void);

extern struct BattleEffectBuffers *Data_03001ed0;




extern u8 *gWork[];


extern s32 ToretoHeya_TintIndex;
extern s32 ToretoHeya_TintSteps[];

void Vector_AddPolarOffset();
void Resource_ResetEntry();
void Engine_ObjectDispatchRelease();

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};



struct Swirl {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 scaleX;
    s32 scaleY;
    u8 pad20[24];
    s32 ox;
    s32 oy;
    s32 oz;
    u8 pad44[12];
    u8 *sprite;
    u8 pad54[16];
    s16 timer;
    s16 angle;
};

s32 Object_ReplaceResourceEntry(struct FieldSprite *sprite, s32 previous);

struct Spark {
    u8 unknown_00[0x64];
    u16 phase;
    u16 angle;
};

struct Vec3 {
    s32 x;
    s32 y;
    s32 z;
};


extern s16 ToretoHeya_MapPatches[];

void ToretoHeya_AdvanceEffectMotion();
void Engine_EventBegin(void);
void Engine_CameraMoveTo();
void Engine_MapRedraw(void);
void Engine_TaskWait(s32 frames);
void Engine_EventOpenScreen(void);
void Engine_EventWaitForScreen(void);
void Engine_AudioPlayCue(s32 cue);
void Engine_EventWait(s32 frames);
s32 Engine_MathCos(s32 angle);
s32 Engine_MathSin(s32 angle);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetAnimation(s32 actor, s32 anim);
void Engine_WorkSetValuesIfNonNegative();
void Engine_MapWaitWorkValuesBelow256(void);
void Engine_EventEnd(void);

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[20];
    s32 script;
};

void ToretoHeya_UpdateSwirlObject(struct Swirl *obj);
const struct SceneEntrance *Scene_GetEntrances(void);

const struct SceneRegion *Scene_GetRegions(void);

const u32 *Scene_GetExits(void);

const struct ScenePlacement *Scene_GetPlacements(void);

void ToretoHeya_HandleFloorSwitch(s32 flag, s32 src_x, s32 src_y, s32 entrance);

void FieldScene_RunStep200(void);

void FieldScene_RunStep201(void);

void FieldScene_RunStep202(void);

void FieldScene_RunStep203(void);

void FieldScene_RunStep204(void);

void FieldScene_RunStep205(void);

void FieldScene_RunStep206(void);

void FieldScene_RunStep207(void);

void FieldScene_RunStep208(void);

void FieldScene_RunStep209(void);

void FieldScene_RunStep20a(void);

void FieldScene_RunStep20b(void);

void FieldScene_RunStep20c(void);

void FieldScene_RunStep20d(void);

void FieldScene_RunStep20e(void);

void FieldScene_RunStep20f(void);

void FieldScene_RunStep210(void);

void FieldScene_RunStep211(void);

void FieldScene_RunStep212(void);

void FieldScene_RunStep213(void);

void FieldScene_RunStep214(void);

void SceneState_ClearStoryVariantWhenIdle(void);

const struct SceneEvent *Scene_GetEvents(void);

void ToretoHeya_RunTableScene(void);


void FieldScene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    s32 record;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = GameFlag_IsSet(3);
    *((u8 *)Object_GetById(3) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    *((u8 *)Object_GetById(0) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    PartyInventory_FindOwner(184);
    Audio_PlayCue(17);
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa60000, 0x500000);
    v6 = 192;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_GERALD, 0x940000, 0x5a0000);
    record = Actor_Get(ACTOR_GERALD);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_IVAN, 0xb60000, 0x5a0000);
    record = Object_GetById(ACTOR_IVAN);
    *(u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Actor_SetPosition(ACTOR_MIA, 0xa60000, 0x680000);
        record = Object_GetById(ACTOR_MIA);
        *(u16 *)(record + 6) = (v6 << 8);
    }
    ToretoHeya_PlayGesture(0);
    Engine_TaskWait(10);
    gEventWork->start_transition = ((TRANSITION_BACKDROP_FADE) << 8 | (0));
    gEventWork->transition_frames = 48;
    Event_OpenScreen();
    Event_WaitForScreen();
    Engine_EventWait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0xa80000, -1, 0x980000, 1);
    Camera_WaitForMove();
    Engine_EventWait(10);
    v5 = 10;
    Audio_PlayCue(123);
    Map_CopyCellAttributes(26, 3, 1, 2, v5, 8);
    Map_CopyCells(26, 38, 1, 1, v5, 43);
    Engine_TaskWait(4);
    Map_CopyCells(26, 37, 1, 2, v5, 42);
    Engine_TaskWait(4);
    Map_CopyCells(26, 36, 1, 3, v5, 41);
    Engine_TaskWait(4);
    Map_CopyCells(26, 35, 1, 4, v5, 40);
    Engine_TaskWait(80);
    Engine_EventSetMessage((s32)MsgToretoMmmmm);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Camera_MoveTo(0xa80000, -1, 0x5a0000, 1);
    Camera_WaitForMove();
    Engine_EventWait(40);
    ToretoHeya_PlayGesture(1);
    Engine_EventWait(60);
    Audio_PlayCue(21);
    ToretoHeya_PlayGesture(4);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 80);
    Event_ShowMessage(0x8009, 0);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    ToretoHeya_PlayGesture(0);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Engine_EventWait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Event_ShowMessageAndWait(0x8002, 0, 20);
    ToretoHeya_PlayGesture(0);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, (v6 << 8), 0);
    Engine_ActorFaceDirection(1, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 40);
    ToretoHeya_PlayGesture(4);
    Engine_EventOpenMessage(0x8009, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
        Actor_SetAnimation(ACTOR_GERALD, 4);
        Engine_EventSetMessage((s32)MsgToretoDoingNowsNot);
        Event_ShowMessage(0x8001, 0);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 10);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Event_ShowMessage(0x8002, 0);
    }
    Engine_EventWait(20);
    ToretoHeya_PlayGesture(4);
    Engine_EventSetMessage((s32)MsgToretoTurnedPeopleKolima);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Engine_EventWait(20);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(20);
    Engine_TaskWait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_GERALD, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 20);
    Engine_EventWait(20);
    *(s32 *)ToretoHeya_SparkCounter = 0;
    {
        s32 *bank = (s32 *)ToretoHeya_SparkOrigin;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)ToretoHeya_SpawnSwirlSparks;
        bank[2] = 0x340000;
    }
    Call2(Engine_TaskAddCallback, base5_200962d, 0xc80);
    Engine_EventWait(220);
    Engine_TaskRemoveCallback(base5_200962d);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(20);
    Engine_TaskWait(40);
    ToretoHeya_PlayGesture(4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Event_ShowMessage(0x8009, 0);
    Object_SetActionCallbackAndRefreshById(8, (s32)ToretoHeya_ActionTable1);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Event_ShowMessage(0x8001, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 10);
    Event_ShowMessage(0x8002, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    if (rec != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 1);
        Engine_EventShowMessageAndWait(0x8003, 0, 10);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    base5_2009ec8 = (s32)ToretoHeya_ActionTable2;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_2009ec8);
    if (rec != 0) {
        Actor_EnableActionCallback(ACTOR_MIA, base5_2009ec8);
    }
    Object_SetActionCallbackAndRefreshById(2, base5_2009ec8);
    Engine_EventWait(20);
    *((u8 *)Object_GetById(0) + 35) |= 1;
    GameFlag_Set(0x844);
    Engine_TaskAddCallback((s32)ToretoPalette_ApplyTint, 0xc80);
    Engine_EventEnd();
}
void SceneState_ApplyRectsByFlag844(s32 flag);

s32 ToretoHeya_EnterRoom(void);

void ToretoPalette_CaptureBank(void);



void ToretoPalette_ApplyTint(void)
{
    u16 *src;
    u16 *dst;
    u32 i;
    s32 r;
    s32 g;
    s32 b;
    s32 red;
    s32 green;
    s32 blue;
    u32 color;
    u8 *event;

    event = gWork[0];
    src = (u16 *)gWork[5];
    if (*(s16 *)(event + 0x17e) != 0)
        return;
    if ((gFrameCount & 31) != 0)
        return;
    src += 16;
    dst = (u16 *)0x05000020;
    i = 0;
    for (; i <= 62; i++, src++) {
        r = ToretoHeya_TintSteps[ToretoHeya_TintIndex];
        g = ToretoHeya_TintSteps[ToretoHeya_TintIndex + 1];
        b = ToretoHeya_TintSteps[ToretoHeya_TintIndex + 2];
        if (i > 47) {
            r -= r / 2 + r / 3;
            g -= g / 2 + g / 3;
            b -= b / 2 + b / 3;
        } else if (i > 31) {
            r -= r / 3 + r / 4;
            g -= g / 3 + g / 4;
            b -= b / 3 + b / 4;
        } else if (i > 15) {
            r -= r / 4 + r / 5;
            g -= g / 4 + g / 5;
            b -= b / 4 + b / 5;
        }
        color = *src;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += r;
        green += g;
        blue += b;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        *dst++ = (blue << 10) | (green << 5) | red;
    }
    ToretoHeya_TintIndex += (Engine_RandomNext() & 7) * 3;
    if (ToretoHeya_TintSteps[ToretoHeya_TintIndex] == 99)
        ToretoHeya_TintIndex = 0;
}


void ToretoHeya_PlayGesture(s32 gesture)
{
    switch (gesture) {
    case 0:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        break;
    case 1:
        Engine_ActorSetAnimation(8, 1);
        break;
    case 2:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 5);
        break;
    case 3:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 4);
        break;
    case 4:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 1);
        break;
    case 5:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 2);
        break;
    case 7:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        break;
    case 8:
        Engine_ActorSetAnimation(8, 6);
        break;
    case 9:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 9);
        break;
    case 10:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 10);
        break;
    case 11:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 6);
        break;
    case 12:
        Engine_ActorSetAnimation(8, 6);
        break;
    }
    Engine_TaskWait(12);
}

void ToretoHeya_UpdateSwirlObject(struct Swirl *obj)
{
    struct Vec pos;
    s32 t;

    t = obj->timer;
    if (t <= 79) {
        pos.x = obj->ox;
        pos.y = obj->oy;
        pos.z = obj->oz;
        {
            s32 a = obj->angle;

            Vector_AddPolarOffset(t << 16, ((t * 3) << 8) + a, &pos);
        }
        obj->x = pos.x;
        obj->y = pos.y;
        obj->z = pos.z;
        if (obj->timer <= 39) {
            obj->scaleX += -0x51e;
            obj->scaleY += -0x51e;
        }
        obj->timer++;
    } else {
        Resource_ResetEntry(obj->sprite[28]);
        Engine_ObjectDispatchRelease(obj);
    }
}

void ToretoHeya_SpawnSwirlSparks(void)
{
    /* FAKEMATCH: the counter pointer is taken again here so it is dead
         * across the spark loop, where the loop index takes its register. */

    struct FieldActor *spark;
    u32 frame;
    s32 previous;
    s32 wave;
    u32 i;
    s32 *counter;

    counter = &ToretoHeya_SparkCounter;
    frame = *counter;
    previous = 0;
    wave = __divsi3(frame, 10);
    switch (frame) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        for (i = 0; i < 6 - wave; i++) {
            spark = Engine_ObjectCreate(0x11d, (*(struct Vec3 *)ToretoHeya_SparkOrigin).x, (*(struct Vec3 *)ToretoHeya_SparkOrigin).y, (*(struct Vec3 *)ToretoHeya_SparkOrigin).z);
            if (spark != 0) {
                previous = Object_ReplaceResourceEntry(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 0;
                Engine_ActorSetSpriteFlags(spark, 0);
                Object_SetMode(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = ((360 / (u32)(6 - wave) * i) << 16) / 360;
                spark->target_x = (*(struct Vec3 *)ToretoHeya_SparkOrigin).x;
                spark->target_y = (*(struct Vec3 *)ToretoHeya_SparkOrigin).y;
                spark->target_z = (*(struct Vec3 *)ToretoHeya_SparkOrigin).z;
                spark->speed = 0x19999;
                spark->update = ToretoHeya_UpdateSwirlObject;
            }
        }
    case 44:
        Engine_AudioPlayCue(0x121);


        counter = &ToretoHeya_SparkCounter;
        break;
    }
    if (++*counter > 120) {
        *counter = 0;
    }
}
void ToretoHeya_ApplyFlaggedMapPatches(void);

void SceneEffect_RegisterPaletteFadeCallback(void);

void ToretoHeya_AdvanceEffectMotion(struct Effect *effect);

void ToretoHeya_RunLandingDustScene(void);

