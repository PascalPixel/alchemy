/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/MACHI.C; edition ES; function FieldScene_RunScene3a8SequenceA.
 * Removing FIELD_EVENT.H Random_Next changes mov	r2, r0 to ldr	r3, .L0
 * (82/81 assembly lines).
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


struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};



extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[];

void Scheduler_ResetTaskTable(void);
void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count);
void Scheduler_SortTasks(void);
s32 Scheduler_FindCallback(u32 callback);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order);
void Scheduler_Idle(void);
void Scheduler_EmptyCallback(void);
s32 Scheduler_RemoveCallback(u32 callback);
s32 Scheduler_EnableCallbacks(u32 callback);
s32 Scheduler_EnableUnmaskedOverlayCallbacks(void);
s32 Scheduler_SetCallbackMask(u32 callback, u32 mask);
s32 Scheduler_DisableCallbacks(u32 callback);
s32 Scheduler_DisableOverlayCallbacks(void);
s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);


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

static inline struct FieldActor *Actor_Get(s32 actor);

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z);

static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration);

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

static inline void Audio_PlayCue(s32 cue);

static inline s32 Task_RemoveCallback(void (*callback)(void));

static inline s32 Math_Sin(s32 angle);

static inline s32 Math_Cos(s32 angle);

static inline s32 Math_Divide(s32 dividend, s32 divisor);

static inline void *Heap_Allocate(s32 slot, s32 size);

static inline void Heap_Release(s32 slot);

static inline s32 Vram_Load(s32 block, s32 size, const void *data);

static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z);


static inline void Object_SetAnimation(struct FieldActor *object, s32 animation)
{
    Object_SetMode(object, animation);
}
static inline void Object_SetScript(struct FieldActor *object, const s32 *script);



enum ObjectBlendMode {
    OBJECT_BLEND_NORMAL = 0,
    OBJECT_BLEND_TRANSLUCENT = 1
};
static inline void Object_SetBlendMode(struct FieldActor *object, s32 mode);


static inline void Object_SetPalette(struct FieldActor *object, s32 palette)
{
    ObjectGroup_SetChildValue(object, palette);
}
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


struct Resource3a8Effect {
    u8 unknown_00[0x06];
    u16 unknown_06;
    u8 unknown_08[0x5e];
    s16 action_timer;
};



typedef char Resource3a8Effect_unknown_06_offset[
    ((u32)&(((struct Resource3a8Effect *)0)->unknown_06)) == 0x06 ? 1 : -1
];
typedef char Resource3a8Effect_action_timer_offset[
    ((u32)&(((struct Resource3a8Effect *)0)->action_timer)) == 0x66 ? 1 : -1
];
typedef char Resource3a8Effect_size[
    sizeof(struct Resource3a8Effect) == 0x68 ? 1 : -1
];
static __inline__ void Call1(void (*f)(), s32 a0);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2);

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10);


static __inline__ s32 Value0(s32 (*f)())
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */


    return f();
}
static __inline__ s32 Value1(s32 (*f)(), s32 a0);

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1);

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2);

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3);

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static __inline__ s32 Value7(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);








enum {

    ITEM_NUT = 181
};

enum StagedGroupMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_CAME_KALAY_BECAUSE_DIDNT_LIKE = 0x1a7c,
    MSG_WHY_WE_STOPPING_AT_PLACE = 0x1a92,
    MSG_THATS_WE_CANT_WAIT_ANY = 0x1ab2,
    MSG_LORD_HAMMET_SELLS_HIS_BEST = 0x1acf,
    MSG_DID_FIND_NEEDED_IN_WEAPON = 0x1ad1,
    MSG_LADY_LAYANA_SHARED_IN_LORD = 0x1ad5,
    MSG_LORD_HAMMETS_PALACE_LORD_AWAY = 0x1b05,
    MSG_WEVE_ARRIVED_HAMMET = 0x256f,
    MSG_DO_WANT_GO_CAVE_UP = 0x2584,
    MSG_LAYANA_WAS_VERY_HARD_ON = 0x25b3,
    MSG_VERY_CLEAN_MAINTAINED = 0x29df
};

struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj_02000040 {
    u8 filler00[100];
    u16 f64;
    u16 f66;
};

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 cnt;
};

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
    u8 unknown_1cc[12];
    u16 step;
};

struct Obj_020036f8 {
    u8 filler00[8];
    s32 f08;
    s32 f0c;
    u8 filler10[8];
    s32 f18;
    s32 f1c;
    s32 f20;
    s32 f24;
    s32 f28;
    u8 filler2c[56];
    s16 f64;
};

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect_0200390c {
    unsigned char pad00[8];
    s32 x;
    s32 y;
    unsigned char pad10[0x20];
    s32 angle;
    unsigned char pad34[4];
    s32 base_x;
    s32 base_y;
    unsigned char pad40[0x10];
    RenderData *render;
} Effect_0200390c;

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

extern const struct SceneEntrance gKareiMachiEntrances1[];
extern const struct SceneEntrance gKareiMachiEntrances2[];
extern const struct SceneEntrance gKareiMachiEntrances3[];
extern const struct SceneEntrance gKareiMachiEntrances4[];
extern const struct SceneEntrance gKareiMachiEntrances5[];
extern const struct SceneEntrance gKareiMachiEntrances6[];
extern const struct SceneEntrance gKareiMachiEntrancesOther[];

extern u8 KareiMachi_SceneTable01[];

extern const struct ScenePlacement gKareiMachiPlacements1[];
extern const struct ScenePlacement gKareiMachiPlacements2[];
extern const struct ScenePlacement gKareiMachiPlacements3[];
extern const struct ScenePlacement gKareiMachiPlacements6[];
extern const struct ScenePlacement gKareiMachiPlacementsOther[];

extern u8 MsgFieldPeeredWell[];
extern u8 MsgKareiVeryCleanMaintained[];

extern const struct SceneEvent gKareiMachiEvents1[];
extern const struct SceneEvent gKareiMachiEvents2[];
extern const struct SceneEvent gKareiMachiEvents3[];
extern const struct SceneEvent gKareiMachiEvents4[];
extern const struct SceneEvent gKareiMachiEvents5[];
extern const struct SceneEvent gKareiMachiEvents6[];
extern const struct SceneEvent gKareiMachiEventsOther[];

extern u8 MsgKareiCameKalayBecauseDidntLike[];
extern u8 MsgKareiDidFindNeededInWeapon[];
extern u8 MsgKareiDoWantGoCaveUp[];
extern u8 MsgKareiLadyLayanaSharedInLord[];
extern u8 MsgKareiLayanaWasVeryHardOn[];
extern u8 MsgKareiLordHammetSellsHisBest[];

struct DoorCells {
    const u16 *steps;
    u16 x;
    u16 y;
};

extern struct DoorCells KareiMachi_DoorCells[];

u8 *Object_GetByIdFar();

extern u8 MsgKareiOkListeningLets[];
extern u8 MsgKareiDecideEnteringColosso[];
extern u8 MsgKareiShortOnePerson[];
extern u8 MsgKareiThatsWeCantWaitAny[];
extern u8 MsgKareiWhyWeStoppingAtPlace[];
extern const u32 SceneAction_GroupMotion[];
extern const u32 SceneAction_GroupOffsetMotion[];
extern u8 KareiMachi_StagedGroupScript[];
extern u8 KareiMachi_DanceScriptB[];
extern u8 KareiMachi_DanceScriptC[];
extern u8 KareiMachi_DanceScriptD[];
extern u8 KareiMachi_DanceScriptE[];
extern u8 KareiMachi_DanceScriptF[];
void KareiMachi_AlternateDance();
void Event_CallWithLastActiveObjectId();
void Ui_SetRenderResultFromObject();
void UiText_ShowCenteredMessage();
void Object_RefreshSelectorById();
void Object_SetActionCallbackAndRefreshById();
void KareiMachi_SpeedUpActors();
void Audio_PlayCueFromEventWork();

extern u8 *gActorEffectWork;

void KareiMachi_SetupEntryActors(void);
void SceneState_CheckFlags941And940(void);
void SceneState_ApplyFlagGatedActorEightSetup(void);
void SceneState_SetWork448AndRunFlag915Step(void);
void FieldScene_RunMiddleSequence(void);

s32 Engine_GameFlagIsSet();
s32 Engine_GameFlagSet();
void Engine_ActorSetPosition();
void Engine_ActorSetChildValue();
void Engine_ItemLoadIcon();
s32 Engine_VramLoad();
void Engine_HeapRelease();

struct SpriteBits {
    u8 pad0[5];
    u8 b5_lo : 5;
    u8 hidden : 1;
    u8 b5_hi : 2;
    u8 pad6[3];
    u8 b9_lo : 2;
    u8 mode : 2;
    u8 b9_hi : 4;
};

extern u8 KareiMachi_Script01[];

union GameStateRows {
    s16 halves[512][1];
};

void FieldScene_RunSupplementalSequenceOne();
void BattleFx_StartFadeOverlay();
void BattleFx_SetQueuedSoundAndPlay();

void Engine_ActorSetSpeed();
void KareiMachi_AlternateDance(void);
extern s32 KareiMachi_DanceStep;

extern u8 KareiMachi_DanceScriptA[];
void Engine_ActorEnableActionCallback();
static __inline__ void ClearStep(u8 *a);


extern u8 MsgKareiLordHammetsPalaceLordAway[];
extern u8 MsgKareiWeveArrivedHammet[];
extern const s32 KareiMachi_ActionScript01[];
extern const s32 KareiMachi_Script02[];
extern s32 KareiMachi_Data01[];
extern const u8 KareiMachi_Data02[];
extern const u8 KareiMachi_Data03[];
void FieldScene_RunScene3a8SequenceB();
s32 Object_CheckMovementCollision();
void Object_SetPosition();
void Object_CommitPosition();
void BattleFx_PlayQueuedSound();
void FieldScene_RunScene3a8SequenceA();
s32 Object_CreateFar();
u8 *Battle_GetWorkObject1e0();
void Object_Destroy(void);
static __inline__ void bump_step_0200164c(s32 amount);


void SceneState_LinkRecordZeroWhenFlag200Clear(void);

void SceneEffect_UpdateLobeOrbitEffect26(void);
void SceneState_ApplyValues12And2And3(void);

s32 SceneEffect_UpdateRandomAction(struct Resource3a8Effect *effect);

s32 OverlayObject_AdvanceXWhenCounterExpires(struct Object *obj);

const struct SceneEntrance *Scene_GetEntrances(void);

s32 SceneData_ReturnZero(void);

u8 *SceneData_GetTablec6b8(void);

const struct ScenePlacement *Scene_GetPlacements(void);

void FieldScene_RunStepWithValue29df(void);

void FieldScene_RunStepWithValueFd6(void);

const struct SceneEvent *Scene_GetEvents(void);

void SceneDialogue_RunActorNineteenDialogue(void);

void FieldScene_RunSlotZeroFacingSequence(void);

void SceneDialogue_RunActorTenDialogue(void);

void SceneState_BranchOnSlotZeroFacing(void);

void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void);

void FieldScene_RunActorTwentyFourAngleDialogue(void);

void KareiMachi_RunExitDoor(void);

void RunSceneArrivalSetup(void);

void FieldScene_RunStagedGroupSequence(void);

void SceneState_ClearWord24WhenFlag200(void);

void SceneState_LinkRecordZeroWhenFlag200Clear(void);

void SceneActor_SetActorZeroByte35Bit0(void);

void SceneState_ClearRecordZeroBit0At35(void);

void SceneDialogue_RunActor181Line916(void);

s32 Scene_Initialize(void);

void KareiMachi_SetupEntryActors(void);

void SceneState_CheckFlags941And940(void);

void SceneState_ApplyFlagGatedActorEightSetup(void);

void SceneState_SetWork448AndRunFlag915Step(void);

void FieldScene_RunMiddleSequence(void);

void SceneState_SetValues27Through34(void);

void SceneEffect_SetSlotVariantAndDescriptor(s32 a);

void KareiMachi_SpeedUpActors(s32 a0, s32 a1);

void KareiMachi_AlternateDance(void);

void FieldScene_RunTwoActorCutsceneSequence(void);

void FieldScene_RunSecondaryGroupSequence(void);

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z);

void FieldScene_RunLateSequence(void);

void FieldScene_RunScene3a8SequenceB(void);

void SceneEffect_UpdateMotionWithDamping(struct Obj_020036f8 *p);

void FieldScene_RunSupplementalSequenceOne(s32 a0);


void FieldScene_RunScene3a8SequenceA(s32 a0, s32 a1, s32 a2)
{
    s32 p8;
    u8 *rec7;
    s32 value;
    s32 mask;
    u8 *link;

    p8 = a2;
    value = Engine_RandomNext();
    rec7 = (u8 *)Object_CreateFar(222, (a0 + -0x80000), (((((u32)(value << 3) >> 16) << 16) + a1) + 0x100000), p8);
    if ((s32)rec7 != 0) {
        rec7[85] = (mask = 0);
        link = (u8 *)*(s32 *)((s32)rec7 + 80);
        mask -= 13;
        link[9] = ((link[9] & mask) | 8);
        Object_SetPalette((s32)rec7, 9);
        Engine_ActorSetSpriteFlags((s32)rec7, 0);
        value = Engine_RandomNext();
        *(s32 *)((s32)rec7 + 36) = ((((u32)(value << 1) >> 16) - 1) << 16);
        value = Value0(Engine_RandomNext);
        *(s32 *)((s32)rec7 + 40) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
        {
            u16 *target = (u16 *)((s32)rec7 + 100);
            s32 shown = 20;

            *target = shown;
            *((u8 *)target - 3) = 1;
        }
        Object_SetAnimation((s32)rec7, 1);
        Engine_ObjectSetScript((s32)rec7, (s32)KareiMachi_Script02);
    }
}
void SceneEffect_UpdateLobeOrbitEffect26(void);

void FieldScene_DrawTilesAndRaiseActor11(void);

s32 SceneEffect_UpdateOrbitingEffect(Effect_0200390c *effect);

void InitializeOrbitingRenderEffect(void);

