/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/MURA.C; edition DE; function Door_Enter.
 * Removing FIELD_EVENT.H Map_AnimateCells changes sub	r5, r5, #4 to ldr	r2, .L0+4
 * (76/76 assembly lines).
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


enum AbilityId {
    ABILITY_FROST = 0x18,
    ABILITY_MIND_READ = 0x8d,
    ABILITY_REVEAL = 0x90,
    ABILITY_CLOAK = 0x92,
    ABILITY_CATCH = 0x94
};


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
static inline void Event_ShowMessage(s32 speaker, s32 flags);

static inline s32 Event_OpenMessage(s32 speaker, s32 flags);

static inline s32 Event_AskYesNo(s32 speaker, s32 flags);



















static inline struct FieldActor *Actor_Get(s32 actor)
{
    return Object_GetById(actor);
}
static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z);



static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    Engine_ActorSetSpeed(actor, speed, acceleration);
}
static inline void Actor_SetDestination(s32 actor, s32 x, s32 z);

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz);

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z);

static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz);

static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames);

static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames);

static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames);

static inline void Actor_SetAttachedEffect(s32 actor, s32 effect);

static inline void Camera_SetSpeed(s32 speed, s32 acceleration);

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);

static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y);

static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                          s32 dest_x, s32 dest_y);

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third);






enum {
    FLAG_ARRIVAL_EVENT_PENDING = 0x12f
};
static inline s32 GameFlag_IsSet(s32 flag);

static inline s32 GameFlag_Set(s32 flag);

static inline void GameFlag_Clear(s32 flag);



static inline void Audio_PlayCue(s32 cue)
{
    Engine_AudioPlayCue(cue);
}
static inline s32 Task_RemoveCallback(void (*callback)(void));

static inline s32 Random_Next(void);

static inline s32 Math_Divide(s32 dividend, s32 divisor);

static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z);

static inline void Object_SetScript(struct FieldActor *object, const s32 *script);



enum ObjectBlendMode {
    OBJECT_BLEND_NORMAL = 0,
    OBJECT_BLEND_TRANSLUCENT = 1
};
static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z);

static inline void Item_LoadIcon(s32 item);

static inline void Item_ShowFound(s32 item, s32 height);

static inline s32 Party_GiveItem(s32 item, s32 flags);

static inline void Actor_Stop(s32 actor);


static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz)
{
    Engine_ActorCenterAndWalk(actor, priority, dz);
}
static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz);

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

static inline void Actor_EnableActionCallback(s32 actor, const u8 *table);

static inline void Actor_SetActionCallback(struct FieldActor *actor, s32 value);

static inline void Actors_Refresh(void);

static inline void Map_CopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width,
                                   s32 height);

static inline void MapRender_SetValues(s32 value0, s32 value1, s32 value2);

static inline void ColorBuffer_ApplySource(s32 value, s32 mode);

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode);

static inline void ColorBuffer_Interpolate(s32 frames);

static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z);

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z);

static inline void Actor_Jump(s32 actor, s32 height, s32 frames);

static inline void Event_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
                                                s32 other_speaker, s32 other_x, s32 other_y,
                                                s32 other_arg, s32 other_extra, s32 flags);

static inline void Party_AddMembers(s32 first, s32 second);

static inline void MapRender_WaitForValues(void);


struct FieldEffect {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[3];
    u8 priority_flags;
    u8 unknown_24[0x0c];
    s32 scale_rate_x;
    s32 scale_rate_y;
    u8 unknown_38[0x0c];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a[0x0a];
    u16 spin;
    u8 unknown_66[2];
    s32 countdown;
    void (*update)(union FieldObject *object);
};

typedef char FieldEffect_ScaleRateX[(u32)&(((struct FieldEffect *)0)->scale_rate_x) == (0x30) ? 1 : -1];
typedef char FieldEffect_VelocityX[(u32)&(((struct FieldEffect *)0)->velocity_x) == (0x44) ? 1 : -1];
typedef char FieldEffect_Sprite[(u32)&(((struct FieldEffect *)0)->sprite) == (0x50) ? 1 : -1];
typedef char FieldEffect_Spin[(u32)&(((struct FieldEffect *)0)->spin) == (0x64) ? 1 : -1];
typedef char FieldEffect_Countdown[(u32)&(((struct FieldEffect *)0)->countdown) == (0x68) ? 1 : -1];
typedef char FieldEffect_Update[(u32)&(((struct FieldEffect *)0)->update) == (0x6c) ? 1 : -1];


union FieldObject {
    struct FieldActor actor;
    struct FieldEffect effect;
};













enum EffectSpawnFlags {

    EFFECT_SCRIPT_MASK = 0x0f,
    EFFECT_USE_PALETTE = 0x10000,
    EFFECT_USE_PRIORITY = 0x20000,
    EFFECT_SCALE_TO_TARGET = 0x40000,
    EFFECT_USE_START_SCALE = 0x80000,
    EFFECT_USE_TYPE = 0x100000,
    EFFECT_USE_SCRIPT = 0x200000,
    EFFECT_USE_ROTATION = 0x400000,
    EFFECT_USE_SPIN = 0x800000,
    EFFECT_USE_UPDATE = 0x1000000
};

struct EffectOptions {
    s32 priority;
    s32 palette;
    s32 start_scale_x;
    s32 start_scale_y;
    s32 target_scale_x;
    s32 target_scale_y;
    s16 type;
    const s32 *script;
    u16 rotation;
    u16 spin;
    void (*update)(union FieldObject *object);
};

typedef char EffectOptions_Type[(u32)&(((struct EffectOptions *)0)->type) == (0x18) ? 1 : -1];
typedef char EffectOptions_Update[(u32)&(((struct EffectOptions *)0)->update) == (0x24) ? 1 : -1];

void Effect_SetPriority(struct FieldEffect *effect, s32 priority);
struct FieldEffect *Effect_CreateTranslucent(s32 x, s32 y, s32 z, s32 type);
struct FieldEffect *Effect_Create(s32 x, s32 y, s32 z, s32 type);
void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
                  const struct EffectOptions *options);
void Effect_SpawnResident(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
                          const struct EffectOptions *options);



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


enum GameFlagId {





    FLAG_KEEP_PARTY_POSITION = 0x109,

    FLAG_SHOW_LOCATION_NAME = 0x12f,

    FLAG_VAULT_THIEVES_JAILED = 0x855,

    FLAG_LUNPA_HEARD_OF_PRISONER = 0x940,

    FLAG_LUNPA_TRADE_REOPENED = 0x941,

    FLAG_LUNPA_CAVE_REUNION_SEEN = 0x94d,

    FLAG_PARTY_STAYED_IN_LUNPA = 0x94f
};


enum ItemId {
    ITEM_NUT = 0xb5,
    ITEM_VIAL = 0xb6,
    ITEM_ANTIDOTE = 0xbb,
    ITEM_ELIXIR = 0xbc,
    ITEM_WATER_OF_LIFE = 0xbd,
    ITEM_SMOKE_BOMB = 0xe2,
    ITEM_SLEEP_BOMB = 0xe3,
    ITEM_LUCKY_MEDAL = 0xe5,
    ITEM_BONE = 0xe7,
    ITEM_CELL_KEY = 0xea,
    ITEM_BLACK_ORB = 0xf2
};


enum SoundId {
    SOUND_PUZZLE_SOLVED = 80,
    SOUND_TREASURE_FOUND = 83,
    SOUND_SHOP_PURCHASE = 101,
    SOUND_TRIPLE_TONE_LOW = 108,
    SOUND_TRIPLE_TONE_HIGH = 109,
    SOUND_MENU_CURSOR_MOVE = 111,
    SOUND_MENU_CONFIRM = 112,
    SOUND_MENU_CANCEL = 113,
    SOUND_MENU_ERROR = 114,
    SOUND_SCUFFLE = 121,
    SOUND_MAP_EXIT = 123,
    SOUND_RECOVERY = 126,
    SOUND_LANDING_THUD = 127,
    SOUND_HEAVY_IMPACT = 134,
    SOUND_ITEM_BREAK = 138,
    SOUND_GATE_MOVE = 157,
    SOUND_DOOR_OPEN = 158,
    SOUND_SCENE_TRANSITION = 167,
    SOUND_HIDDEN_PASSAGE_OPEN = 210,
};

enum LunpaEntrance {
    LUNPA_ENTRANCE_FROM_WORLD_MAP = 1,
    LUNPA_ENTRANCE_FROM_GATE,
    LUNPA_ENTRANCE_FROM_CAVE,
    LUNPA_ENTRANCE_FROM_FIRST_HOME,
    LUNPA_ENTRANCE_FROM_SECOND_HOME,
    LUNPA_ENTRANCE_FROM_THIRD_HOME,
    LUNPA_ENTRANCE_FROM_ARMS_SHOP,
    LUNPA_ENTRANCE_FROM_ITEM_SHOP,
    LUNPA_ENTRANCE_FROM_INN,
    LUNPA_ENTRANCE_FROM_TEMPLE,
    LUNPA_ENTRANCE_FROM_EIGHTH_ROOM,
    LUNPA_ENTRANCE_FROM_JAIL
};


enum LunpaExit {
    LUNPA_EXIT_TO_WORLD_MAP = 1,
    LUNPA_EXIT_TO_GATE,
    LUNPA_EXIT_TO_CAVE,
    LUNPA_EXIT_TO_FIRST_HOME,
    LUNPA_EXIT_TO_SECOND_HOME,
    LUNPA_EXIT_TO_THIRD_HOME,
    LUNPA_EXIT_TO_ARMS_SHOP,
    LUNPA_EXIT_TO_ITEM_SHOP,
    LUNPA_EXIT_TO_INN,
    LUNPA_EXIT_TO_TEMPLE,
    LUNPA_EXIT_TO_EIGHTH_ROOM,
    LUNPA_EXIT_TO_JAIL
};

enum GateEntrance {
    GATE_ENTRANCE_FROM_FORTRESS = 1,
    GATE_ENTRANCE_FROM_LUNPA,
    GATE_ENTRANCE_SNEAKING_OUT,
    GATE_ENTRANCE_THROWN_OUT
};

enum GateExit {
    GATE_EXIT_TO_FORTRESS = 1,
    GATE_EXIT_TO_LUNPA,
    GATE_EXIT_THIRD
};


enum {
    WORLD_MAP_ENTRANCE_FROM_LUNPA = 34,
    CAVE_ENTRANCE_FROM_LUNPA = 2,
    ROOM_ENTRANCE_FIRST = 1,
    JAIL_ENTRANCE_FROM_LUNPA = 2,
    FORTRESS_ENTRANCE_FROM_GATE = 1
};




enum LunpaTrigger {
    TRIGGER_WORLD_MAP_ROAD = 1,
    TRIGGER_GATE_ROAD,
    TRIGGER_CAVE_ROAD,
    TRIGGER_FIRST_HOME_DOOR,
    TRIGGER_SECOND_HOME_DOOR,
    TRIGGER_THIRD_HOME_DOOR,
    TRIGGER_ARMS_SHOP_DOOR,
    TRIGGER_ITEM_SHOP_DOOR,
    TRIGGER_INN_DOOR,
    TRIGGER_TEMPLE_DOOR,
    TRIGGER_EIGHTH_ROOM_DOOR,
    TRIGGER_HIDDEN_PASSAGE,
    TRIGGER_HIDDEN_SWITCH = 20,
    TRIGGER_WELL = 90,
    TRIGGER_GRAVE_SLEEP_BOMB = 100,
    TRIGGER_BARREL_NUT,
    TRIGGER_PSYNERGY_STONE
};

enum GateTrigger {
    TRIGGER_FORTRESS_DOOR = 1,
    TRIGGER_LUNPA_ROAD,
    TRIGGER_THIRD_ROAD,
    TRIGGER_GUARDED_GROUND = 10,
    TRIGGER_FORTRESS_APPROACH = 20,
    TRIGGER_PARTY_SPOTTED = 91
};


enum {
    DOOR_COUNT = 8,
    DOOR_TEMPLE = TRIGGER_TEMPLE_DOOR - TRIGGER_FIRST_HOME_DOOR
};


enum LunpaFlag {
    FLAG_LUNPA_PUDDLE_FROZEN = 0x200,
    FLAG_LUNPA_SECRETS_HIDDEN = 0x201,
    FLAG_LUNPA_PASSAGE_OPEN = 0x202,
    FLAG_GATE_GUARDS_BLOCKING = 0x240,
    FLAG_GATE_CLOAK_CAST = 0x241,
    FLAG_GATE_FORTRESS_ENTERED = 0x242,
    FLAG_GATE_TURNING_BACK = 0x243,
    FLAG_GATE_PARTY_CAUGHT = 0x244,
    FLAG_GUARDS_SUSPECT_KALAY = 0x85a,
    FLAG_GATE_GUARD_SAW_HAMMET = 0x9af,
    FLAG_LUNPA_PSYNERGY_STONE = 0xf27,
    FLAG_LUNPA_GRAVE_SLEEP_BOMB = 0xf88,
    FLAG_LUNPA_BARREL_NUT = 0xf89,
    FLAG_LUNPA_NUT_CAUGHT = 0xfd1
};





enum {
    FLAG_FORTRESS_VISIT = 0x943
};

enum LunpaActor {
    ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
    ACTOR_VILLAGER_A,
    ACTOR_VILLAGER_B,
    ACTOR_VILLAGER_C,
    ACTOR_VILLAGER_D,
    ACTOR_VILLAGER_E,
    ACTOR_VILLAGER_F,
    ACTOR_VILLAGER_G,
    ACTOR_VILLAGER_H,
    ACTOR_WEST_GUARD,
    ACTOR_EAST_GUARD,
    ACTOR_SWITCH_GLINT,
    ACTOR_FLOATING_NUT
};


enum {
    ACTOR_LAST_PLACED = 65
};

enum GateActor {
    ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED,
    ACTOR_RIGHT_GUARD,
    ACTOR_GATE_GERALD
};

enum LunpaSprite {
    SPRITE_ITEM_ICON = 0x16,
    SPRITE_TOWNSPERSON_G = 0x66,
    SPRITE_TOWNSPERSON_B = 0x67,
    SPRITE_TOWNSPERSON_C = 0x6a,
    SPRITE_TOWNSPERSON_H = 0x6b,
    SPRITE_TOWNSPERSON_D = 0x6c,
    SPRITE_TOWNSPERSON_I = 0x6d,
    SPRITE_TOWNSPERSON_E = 0x6f,
    SPRITE_TOWNSPERSON_J = 0x73,
    SPRITE_GUARD = 0x96,
    SPRITE_PUDDLE = 0xe3,
    SPRITE_GLINT = 0x11d
};


enum {
    ACTOR_FLOAT = 7
};


enum {
    REVEAL_SHOWN = 5,
    REVEAL_FADED = 4,
    CLOAK_CAST = 0,
    CLOAK_FADED = 2
};


enum {
    GLINT_MOTION_FLAGS = 8,
    GLINT_SCALE_X = 0x13333,
    GLINT_SCALE_Y = 0x18000
};


enum {
    GATE_RETREAT_ENTRANCE = 10
};

enum LunpaMessage {
    MSG_WEST_GUARD_SEALED_THOUGHTS = 0x1ba4,
    MSG_VILLAGER_B_SEALED = 0x1ba9,
    MSG_VILLAGER_D_SEALED = 0x1bad,
    MSG_VILLAGER_E_SEALED = 0x1bae,
    MSG_VILLAGER_F_SEALED = 0x1baf,
    MSG_VILLAGER_H_SEALED = 0x1bb3,
    MSG_VILLAGER_A_SEALED_THOUGHTS = 0x1bb7,
    MSG_VILLAGER_B_SEALED_THOUGHTS,
    MSG_VILLAGER_C_SEALED_THOUGHTS,
    MSG_VILLAGER_D_SEALED_THOUGHTS,
    MSG_VILLAGER_E_SEALED_THOUGHTS,
    MSG_VILLAGER_F_SEALED_THOUGHTS,
    MSG_VILLAGER_G_SEALED_THOUGHTS,
    MSG_VILLAGER_H_SEALED_THOUGHTS,
    MSG_VILLAGER_A_REOPENED = 0x24d0,
    MSG_VILLAGER_C_REOPENED = 0x24d2,
    MSG_VILLAGER_E_REOPENED = 0x24d6,
    MSG_VILLAGER_F_REOPENED,
    MSG_VILLAGER_G_REOPENED,
    MSG_VILLAGER_H_REOPENED,
    MSG_VILLAGER_A_REOPENED_THOUGHTS = 0x24df,
    MSG_VILLAGER_B_REOPENED_THOUGHTS,
    MSG_VILLAGER_C_REOPENED_THOUGHTS,
    MSG_VILLAGER_D_REOPENED_THOUGHTS,
    MSG_VILLAGER_E_REOPENED_THOUGHTS,
    MSG_VILLAGER_F_REOPENED_THOUGHTS,
    MSG_VILLAGER_G_REOPENED_THOUGHTS,
    MSG_VILLAGER_H_REOPENED_THOUGHTS,
    MSG_WEST_GUARD_REOPENED = 0x2509,
    MSG_EAST_GUARD_REOPENED = 0x250a,
    MSG_WEST_GUARD_REOPENED_THOUGHTS = 0x250b
};



enum CatchLine {
    CATCH_LEFT_GUARD_CHALLENGES,
    CATCH_RIGHT_GUARD_WONDERS,
    CATCH_LEFT_GUARD_REFUSES_ENTRY,
    CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
};





enum RecognitionLine {
    RECOGNITION_SEEN_THAT_MAN,
    RECOGNITION_IMPOSSIBLE
};



enum {
    ANIM_SPRAWLED = 19
};


enum {
    PILLAR_TOP_HEIGHT = ((16) << 16),
    CELL_SIZE = ((16) << 16)
};


enum {
    MAP_OBJECT_PSYNERGY_STONE = 102
};


enum {
    HEAP_ITEM_ICON = 17,
    ITEM_ICON_BUFFER_SIZE = 0x608,
    ITEM_ICON_TILES = 0x400,
    ITEM_ICON_TILE_BYTES = 128
};


struct LunpaDoor {
    const u16 *steps;
    u16 x;
    u16 y;
};






struct FloatingNut {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[0x13];
    u8 priority_flags;
    u8 unknown_24[0x0c];
    s32 angle;
    u8 unknown_34[4];
    s32 rest_x;
    s32 rest_y;
    u8 unknown_40[0x10];
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 status;
    u8 unknown_57[5];
    u8 ready;
    u8 unknown_5d[4];

    u8 free_motion;
    u8 unknown_62[10];
    s32 (*update)(union FieldObject *object);
};

extern const struct SceneEntrance gLunpaEntrances[];
extern const struct SceneEntrance gGateEntrances[];
extern const u32 gLunpaExits[];
extern const u32 gGateExits[];
extern const struct ScenePlacement gLunpaPlacements[];
extern const struct ScenePlacement gGatePlacements[];
extern const struct SceneEvent gLunpaSealedEvents[];
extern const struct SceneEvent gLunpaReopenedEvents[];
extern const struct SceneEvent gGateEvents[];
extern const struct LunpaDoor gLunpaDoors[];

void FloatingNut_Catch(void);
void HiddenPuddle_Freeze(void);
void Reveal_ShowSecrets(void);
void Well_Search(void);
void WestGuard_Talk(void);
void EastGuard_Talk(void);
void EastGuard_MindRead(void);
void VillagerA_Talk(void);
void VillagerC_Talk(void);
void VillagerG_Talk(void);
void VillagerB_Shivers(void);
void VillagerD_Talk(void);
void LeftGuard_Talk(void);
void RightGuard_Talk(void);
void LeftGuard_MindRead(void);
void RightGuard_MindRead(void);
void Party_WatchForFortress(void);
void Gerald_RefusesToReturn(void);
void Door_Enter(void);
void HiddenPassage_Enter(void);
void FortressGate_Enter(void);
void HiddenSwitch_Pull(void);
void Guards_BlockGate(void);
void Cloak_Begin(void);
void Guards_CatchParty(void);
void Cloak_End(void);
void Reveal_HideSecrets(void);
void Gateway_Reopen(void);
void Guards_Watch(void);
void Leader_KickUpDust(void);
void Party_ThrownOut(void);
void Leader_SneaksOut(void);
s32 FloatingNut_Update(union FieldObject *object);
void FloatingNut_Initialize(s32 actor);


extern u8 MsgFieldPeeredWell[];
extern u8 MsgRunpaWellFrogs[];

extern u8 MsgRunpaEastGuardAsksIfFrom[];
extern u8 MsgRunpaEastGuardDemandsAuthorization[];
extern u8 MsgRunpaEastGuardFearsBlame[];
extern u8 MsgRunpaEastGuardThinksMerchantHarmless[];
extern u8 MsgRunpaEastGuardTrustsCaveGate[];
extern u8 MsgRunpaVillagerAAsksHowLong[];
extern u8 MsgRunpaVillagerBShivers[];
extern u8 MsgRunpaVillagerCAsksAboutKidnapping[];
extern u8 MsgRunpaVillagerDAsksAboutCommotion[];
extern u8 MsgRunpaVillagerGAsksAboutDonpa[];
extern u8 MsgRunpaWestGuardAsksAboutEntering[];
extern u8 MsgRunpaLeftGuardHearsSomeone[];
extern u8 MsgRunpaLeftGuardRecognizesHammet[];
extern u8 MsgRunpaLeftGuardResentsDodonpa[];

extern u8 MsgRunpaLeftGuardGlimpsedMerchant[];
extern u8 MsgRunpaLeftGuardDismissesThought[];
extern u8 MsgRunpaLeftGuardThoughts[];
extern u8 MsgRunpaRightGuardBoasts[];
extern u8 MsgRunpaRightGuardFeelsCreepy[];
extern u8 MsgRunpaRightGuardWondersHow[];
extern u8 MsgRunpaYoudShadowSneak[];
extern u8 MsgRunpaRightGuardReopenedThoughts[];

extern u8 MsgRunpaGeraldRefusesToReturn[];
extern u8 MsgRunpaGuardsWarnPartyAway[];
extern u8 MsgRunpaGuardsCatchParty[];

extern u8 MsgRunpaGuardForbidsReturn[];
const struct SceneEntrance *Scene_GetEntrances(void);

const struct SceneRegion *Scene_GetRegions(void);

const u32 *Scene_GetExits(void);

const struct ScenePlacement *Scene_GetPlacements(void);

void FloatingNut_Catch(void);

void HiddenPuddle_Freeze(void);

void IcePillar_UpdateDrawOrder(void);

void Reveal_ShowSecrets(void);

void Reveal_HideSecrets(void);

void Well_Search(void);

const struct SceneEvent *Scene_GetEvents(void);

void WestGuard_Talk(void);

void EastGuard_Talk(void);

void EastGuard_MindRead(void);

void VillagerA_Talk(void);

void VillagerC_Talk(void);

void VillagerG_Talk(void);

void VillagerB_Shivers(void);

void VillagerD_Talk(void);

void LeftGuard_Talk(void);

void RightGuard_Talk(void);

void LeftGuard_MindRead(void);

void RightGuard_MindRead(void);

void Party_WatchForFortress(void);

void Gerald_RefusesToReturn(void);



void Door_Enter(void)
{
    struct EventWork *work;
    struct FieldActor *actor;
    u32 id;
    s32 index;

    work = gEventWork;
    Engine_EventBegin();
    for (id = ACTOR_FIRST_PLACED; id <= ACTOR_LAST_PLACED; id++) {
        actor = Actor_Get(id);
        if (actor != ((void *)0)) {
            actor->motion_flags = 0;
        }
    }
    Audio_PlayCue(SOUND_DOOR_OPEN);
    index = work->touched_trigger - TRIGGER_FIRST_HOME_DOOR;
    Engine_MapAnimateCells(gLunpaDoors[index].steps, gLunpaDoors[index].x, gLunpaDoors[index].y);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    if (index != DOOR_TEMPLE) {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
        Engine_EventWait(10);
    }
    Engine_EventRequestExit(work->touched_trigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}
void HiddenPassage_Enter(void);

void FortressGate_Enter(void);

void Party_CheckAhead(void);

void Reveal_PlayTreasureCue(void);

void HiddenSwitch_Pull(void);

void Guards_BlockGate(void);

void Scene_DoNothing(void);

void Cloak_Begin(void);

void Guards_CatchParty(void);

void Gateway_Reopen(void);

void Cloak_End(void);

void Guards_Watch(void);

void Leader_KickUpDust(void);

void Party_ThrownOut(void);

void Leader_SneaksOut(void);

s32 Scene_Initialize(void);

s32 FloatingNut_Update(union FieldObject *object);

void FloatingNut_Initialize(s32 actor);

