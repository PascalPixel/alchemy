/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/TOREBI.C; edition ES; function FieldScene_RunScene3aeSequenceB.
 * Removing FIELD_EVENT.H Actor_WalkBy changes mov	r5, r8 to mov	r5, r9
 * (680/684 assembly lines).
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
static inline s32 Event_OpenMessage(s32 speaker, s32 flags);

static inline s32 Event_AskYesNo(s32 speaker, s32 flags);

static inline struct FieldActor *Actor_Get(s32 actor);

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z);



static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    Engine_ActorSetSpeed(actor, speed, acceleration);
}
static inline void Actor_SetDestination(s32 actor, s32 x, s32 z);

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz);

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z);

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
static inline void Actor_SetSpritePriority(s32 actor, s32 priority);

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags);

static inline void Camera_FollowActor(s32 actor, s32 keep_position);

static inline void Camera_SetSpeed(s32 speed, s32 acceleration);


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
                                 s32 dest_y);

static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                          s32 dest_x, s32 dest_y);

static inline void Map_Redraw(void);

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third);






enum {
    FLAG_ARRIVAL_EVENT_PENDING = 0x12f
};
static inline s32 GameFlag_IsSet(s32 flag);


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


static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz)
{
    Engine_ActorWalkByAndWait(actor, dx, dz);
}
static inline s32 Leader_CheckAhead(void);

static inline void Psynergy_Begin(s32 ability, s32 flags);

static inline void Psynergy_SetTarget(s32 caster, s32 target);

static inline void Psynergy_RaiseHands(void);

static inline void Psynergy_PlayEffect(s32 effect);

static inline void Psynergy_LowerHands(void);

static inline void Psynergy_Cancel(void);

static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags, s32 frames);

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

static inline void ColorBuffer_ApplySource(s32 value, s32 mode);

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode);

static inline void ColorBuffer_Interpolate(s32 frames);

static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z);


static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z)
{
    Engine_ActorWalkToAndWait(actor, x, z);
}


static inline void Actor_Jump(s32 actor, s32 height, s32 frames)
{
    Engine_ActorJump(actor, height, frames);
}
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




extern u8 *Data_03001ebc;

void BattleFx_RunPageEffectForSlot(s32, s32, s32);




extern u8 PrimaryRuntimeSelector;
extern u8 SecondaryRuntimeSelector;
extern u8 TertiaryRuntimeSelector;
extern u8 PrimaryOverlayData[];
extern u8 SecondaryOverlayData[];
extern u8 TertiaryOverlayData[];
extern u8 DefaultOverlayData[];


extern s16 Data_02000240[];




s32 UiWindow_Create();
void UiWork_PushValueSlot();
void UiText_DrawCharacterAtOffset();
void UiText_DrawNumberInWindow();
void UiWork_Finalize();
void UiWork_Finalize();
void UiWork_Finalize();
void Party_AdjustSixDigitCounterA();
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void Battle_ResetEffectCounter();
static __inline__ void bump_step(s32 amount);







s32 PartyInventory_FindOwner();
s32 Inventory_Find();
void Inventory_Discard();
void Engine_EventShowMessage();







void Engine_EventBegin(void);






void SceneState_ApplyValues14And0And5(void);
s32 SceneData_ReturnZero(void);


extern u8 KareiTorebi_SceneTable[];
u8 *SceneData_GetTable98a0(void);
void FieldScene_RunScene3ae_02000144(void);
void KareiTorebi_AskGoToTolbi(void);
void FieldScene_RunScene3ae_02000260(void);
void FieldScene_RunScene3ae_020002dc(void);
void SceneDialogue_RunActor12Event(void);
void FieldScene_RunActorThirteenFlagDialogue(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void KareiTorebi_TalkScaryTrip(void);
void KareiTorebi_TalkSeasickTourists(void);
void FieldScene_RunScene3ae_020006c8(void);
s32 FieldScene_RunScene3ae_0200086c(void);
void FieldScene_RunScene3ae_020008cc(void);
void SceneState_SetRuntimeWord448To521AndSend303(void);
void FieldScene_PlaceSlots14And15(void);
void SceneActor_SetMode3AndFlagBit1(s32 no);
void FieldScene_RunScene3aeSequenceA(void);
void FieldScene_RunScene3aeSequenceB(void);
static __inline__ void Call1(void (*f)(), s32 a0);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2);

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10);

static __inline__ s32 Value0(s32 (*f)());


static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */


    return f(a0);
}
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1);

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2);

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3);

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static __inline__ s32 Value7(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);



extern const struct SceneEntrance gKareiTorebiEntrances1[];
extern const struct SceneEntrance gKareiTorebiEntrances2[];
extern const struct SceneEntrance gKareiTorebiEntrances3[];
extern const struct SceneEntrance gKareiTorebiEntrancesOther[];

extern const struct ScenePlacement gKareiTorebiPlacements1[];
extern const struct ScenePlacement gKareiTorebiPlacements1Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacements3[];
extern const struct ScenePlacement gKareiTorebiPlacements3Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacementsOther[];

extern const struct SceneEvent gKareiTorebiEvents1[];
extern const struct SceneEvent gKareiTorebiEvents1Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2[];
extern const struct SceneEvent gKareiTorebiEvents2Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2Flag950[];
extern const struct SceneEvent gKareiTorebiEvents3[];
extern const struct SceneEvent gKareiTorebiEvents3Flag950[];
extern const struct SceneEvent gKareiTorebiEventsOther[];

extern u8 MsgKareiFinishedOutHereBoardShip[];
extern u8 MsgKareiGoingTakeShip[];
extern u8 MsgKareiPassMessageDaughter[];
extern u8 MsgKareiWantGoToTolbi[];

extern u8 MsgKareiWantGoAshore[];
extern u8 MsgKareiTicketsPlease[];

extern u8 MsgKareiAfterHandingOverTicket[];
extern u8 MsgKareiBoardWaitSetSail[];
extern u8 MsgKareiHandInTicketPlank[];
extern u8 MsgKareiHaveTicketsGiveHim[];
extern u8 MsgKareiHearScaryTrip[];
extern u8 MsgKareiLookingForTickets[];
extern u8 MsgKareiSilkRoadBlocked[];
extern u8 MsgKareiThanksMessageParents[];
extern u8 MsgKareiTouristsLookUpset[];

extern const u16 KareiTorebi_LeaveCells1[];
extern const u16 KareiTorebi_LeaveCells3[];

void FieldScene_ConfigureFlaggedActors(void);

void FieldScene_PlaceSlots14And15(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};


struct SpriteBytes {
    u8 unknown_00[9];
    u8 priority;
    u8 unknown_0a[11];
    u8 second_priority;
    u8 unknown_16[8];
    u16 rotation;
    u8 unknown_20[6];
    u8 flags;
};

extern u8 KareiTorebi_ActorSixteenScript[];
extern u8 MsgKareiIncredibleOcean[];
extern u8 MsgKareiReturnedTicketCost[];
void SceneState_ApplyValues14And0And5(void);

const struct SceneEntrance *Scene_GetEntrances(void);

s32 SceneData_ReturnZero(void);

u8 *SceneData_GetTable98a0(void);

const struct ScenePlacement *Scene_GetPlacements(void);

void FieldScene_RunScene3ae_02000144(void);

const struct SceneEvent *Scene_GetEvents(void);

void KareiTorebi_AskGoToTolbi(void);

void FieldScene_RunScene3ae_02000260(void);

void FieldScene_RunScene3ae_020002dc(void);

void SceneDialogue_RunActor12Event(void);

void FieldScene_RunActorThirteenFlagDialogue(void);

void FieldScene_RunOpeningAuxiliarySequence(void);

void KareiTorebi_TalkScaryTrip(void);

void KareiTorebi_TalkSeasickTourists(void);

void FieldScene_RunScene3ae_020006c8(void);

void KareiTorebi_LeaveScene(void);

s32 Scene_Initialize(void);

void FieldScene_RunScene3ae_020008cc(void);

void FieldScene_ConfigureFlaggedActors(void);

void SceneState_SetRuntimeWord448To521AndSend303(void);

void FieldScene_PlaceSlots14And15(void);

void SceneActor_SetMode3AndFlagBit1(s32 no);

void FieldScene_RunScene3aeSequenceA(void);


void FieldScene_RunScene3aeSequenceB(void)
{
    u32 i;
    u8 *record;
    s32 none;
    s32 v5;
    s32 v6;

    GameFlag_Set(0x8ab);
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_EventSetMessage((s32)MsgKareiReturnedTicketCost);
    record = ((u8 *)Object_GetById(11));
    none = 0;
    record[35] = none;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 21) |= 12;
    Camera_MoveTo(0xe80000, -1, 0x1300000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 0x110);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Camera_WaitForMove();
    Engine_EventWait(20);
    Actor_ShowEmote(13, 0x102, 50);
    Event_ShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, 0x107, 50);
    Actor_FaceDirection(10, 0, 0);
    Actor_Jump(10, 4, 13);
    Actor_Jump(10, 4, 30);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(13, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, 0x103, 55);
    Actor_SetSpeed(10, 0x20000, 0x10000);
    Actor_WalkByAndWait(10, 16, 0);
    Actor_Jump(10, 7, 0);
    v5 = 254;
    Actor_WalkByAndWait(10, 24, 0);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Engine_ActorWalkBy(10, -16, 0);
    Audio_PlayCue(153);
    Actor_SetSpeed(13, 0x26666, 0x13333);
    Actor_WalkByAndWait(13, 16, 0);
    Engine_EventWait(10);
    v6 = 1;
    Actor_SetAnimation(10, 1);
    *(u8 *)(((s32)Object_GetById(10)) + 90) |= v6;
    Actor_SetAttachedEffect(13, 0x102);
    Actor_StartRepeatedMotion(13, 2);
    Audio_PlayCue(155);
    Engine_EventWait(10);
    Audio_PlayCue(155);
    Engine_EventWait(10);
    Audio_PlayCue(155);
    Engine_EventWait(10);
    Engine_EventWait(20);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Actor_Jump(13, 6, 0);
    Audio_PlayCue(159);
    Actor_WalkByAndWait(13, -8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(13, 0x102, 70);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, -8, 0);
    Actor_FaceDirection(16, 0x5000, 0);
    Engine_EventWait(30);
    Actor_SetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(10, 0xe000, 0);
    Engine_EventWait(35);
    Actor_RunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(16, 0x2000, 0);
    Engine_EventWait(55);
    Actor_FaceDirection(16, 0x5000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(11, 0xe000, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(13, 2);
    Engine_EventWait(20);
    Event_ShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(13, 0xa000, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(13, 0x8000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(13, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(10, 0, 0);
    Actor_FaceDirection(11, 0, 0);
    Engine_EventWait(20);
    Actor_RunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Actor_SetSpeed(10, 0x13333, 0x9999);
    Actor_WalkByAndWait(10, 8, 0);
    Actor_SetSpeed(16, 0x20000, 0x10000);
    Actor_WalkByAndWait(16, -8, 16);
    Actor_FaceDirection(16, 0x8000, 0);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, 0x102, 50);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -8, 0);
    *(u8 *)(((s32)Object_GetById(10)) + 90) |= v6;
    Engine_EventWait(20);
    Actor_RunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -16, 0);
    *(u8 *)(((s32)Object_GetById(10)) + 90) |= v6;
    Actor_FaceDirection(10, 0, 0);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(10, 4);
    Engine_EventWait(20);
    Actor_SetSpeed(10, 0x1cccc, 0xe666);
    Actor_WalkByAndWait(10, 8, 0);
    Actor_Jump(10, 6, 0);
    Actor_WalkByAndWait(10, 24, 0);
    Audio_PlayCue(133);
    Actor_Jump(16, 6, 0);
    Actor_EnableActionCallback(16, KareiTorebi_ActorSixteenScript);
    *(u8 *)(((s32)Object_GetById(10)) + 90) &= v5;
    Actor_Jump(10, 6, 0);
    Actor_WalkByAndWait(10, -12, 4);
    record = Value1(Object_GetById, 10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(s32 *)((s32)record + 80);
        s32 shown = 0xc000;

        *(u16 *)(target + 30) = shown;
    }
    Actor_WalkByAndWait(10, -12, 4);
    Actor_FaceDirection(10, 0x4000, 0);
    {
        u8 *flags = ((u8 *)Object_GetById(10)) + 90;
        u8 value = *flags | v6;

        *flags = value;
    }
    Audio_PlayCue(159);
    Engine_EventWait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Actor_SetSpeed(11, 0x18000, 0xc000);
    Actor_WalkByAndWait(11, 24, 0);
    Actor_FaceDirection(11, 0xc000, 0);
    Engine_EventWait(10);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(20);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkByAndWait(16, -8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(16, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_FaceDirection(16, 0, 0);
    Engine_EventWait(40);
    Actor_SetAnimationAndWait(13, 3);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(13, 3);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, 24, -24);
    Actor_WalkByAndWait(16, 8, 0);
    Actor_FaceDirection(16, 0xe000, 0);
    Engine_EventWait(20);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkByAndWait(13, 0, -8);
    Engine_EventWait(10);
    Engine_EventEnd();
}
