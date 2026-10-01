/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/CHOJO2.C; edition DE; function VinasuChojo_RunTransitionStep.
 * Removing FIELD_EVENT.H Object_Create changes mov	r6, r0 to mov	sl, r0
 * (348/372 assembly lines).
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



















static inline struct FieldActor *Actor_Get(s32 actor)
{
    return Object_GetById(actor);
}
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



static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
    Engine_WorkSetValuesIfNonNegative(first, second, third);
}





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
static inline s32 Task_RemoveCallback(void (*callback)(void));


static inline s32 Random_Next(void)
{
    return Engine_RandomNext();
}
static inline s32 Math_Divide(s32 dividend, s32 divisor);

static inline void Object_SetAnimation(struct FieldActor *object, s32 animation);


static inline void Object_SetScript(struct FieldActor *object, const s32 *script)
{
    Engine_ObjectSetScript(object, script);
}


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


static inline void Actor_Stop(s32 actor)
{
    Engine_ActorStop(actor);
}
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



enum {
    ACTOR_FIRST_OF_PAIR = 20,
    ACTOR_SECOND_OF_PAIR = 19,
    FLAG_PAIR_SLIDING = 0x234,
    FLAG_PAIR_GROWING = 0x235
};

void VinasuChojo_ShowMessage(s32 speaker);
void VinasuChojo_FaceActor(s32 actor, s32 facing);
void VinasuChojo_FlashScreen(void);
void VinasuChojo_UpdateBeamActors(void);



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





void *CreateOverlayObject(s32, s32, s32, s32);
void SetOverlayObjectMode(void *, s32);
void SetOverlayObjectSlot(void *, s32);


typedef struct {
    u8 filler0[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} Sub;

typedef struct {
    u8 filler0[0x50];
    Sub *sub;
} Obj;

typedef struct {
    u8 filler0[0x18];
    s32 unk18;
    s32 unk1c;
    u8 filler20[0x44];
    u16 unk64;
} Spr;

typedef struct {
    u8 filler0[0x1e];
    u16 unk1e;
} Obj_02000400;

typedef struct {
    u8 filler0[0x50];
    Obj_02000400 *obj;
} Spr_02000400;

typedef struct {
    u8 filler0[6];
    u16 unk06;
} Spr_02000424;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Obj_020004bc;

typedef struct {
    u8 filler0[0x50];
    Obj_020004bc *obj;
} Spr_020004bc;

struct SceneActor {
    u8 pad00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[20];
    s32 motion28;
    u8 pad2c[4];
    s32 motion30;
    s32 motion34;
    u8 pad38[29];
    u8 flags55;
};

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[0x3d];
    u8 unk55;
} Spr_020005ec;

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[8];
    s32 unk18;
    u8 filler1c[7];
    u8 unk23;
    u8 filler24[0x31];
    u8 unk55;
} Spr_0200071c;

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

extern unsigned char Data_0200e3d4[];
extern unsigned char Data_0200e464[];
extern unsigned char Data_0200e478[];
extern unsigned char Data_0200e6ec[];
extern Spr *Data_0200e6e8;

s32 Math_RemainderUnsigned(s32, s32);
void OverlayObject_SetValue1(void *, s32);
void SceneActor_ParkRecord(u8 *record);
s32 Object_CheckMovementCollision();
void Vector_AddPolarOffset();
void SceneActor_SetByte55ForActorZeroAnd12To17();
s32 SceneActor_FindNearestSlotOfKindF2(void);
void SceneState_ForwardByRuntimeWordBits(s32 a);
void Scene_RunScriptedActorPresentation(void);
void SceneEffect_SpawnParticlesBesideActor();
void Scene_RunPairedActorEffectSequence(void);
void Inventory_AddItem(s32, s32);
void FieldScene_RunScene3c9_02003924(void);
void FieldScene_RunBracketedSceneWithFlag282(void);
void VinasuChojo_RunActorTransition();
void FieldScene_RunMultiActorPresentation();
void BattleEffect_CleanupSceneObjects(s32);
void VinasuChojo_FaceActor();
void VinasuChojo_ShowMessage();

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position);

void FieldScene_RunScene3c9_02004b28();
static __inline__ void Call1(void (*f)(), s32 a0);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1);

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




union StagedActorCoordinate {
    s32 value;
    struct {
        s16 fraction;
        s16 cell;
    } parts;
};

struct StagedActor {
    u8 unknown_00[6];
    u16 direction_and_kind;
    union StagedActorCoordinate x;
    s32 y;
    union StagedActorCoordinate z;
    u8 unknown_14[0x0e];
    u8 transition_mode;
    u8 unknown_23;
    s32 unknown_24;
    s32 unknown_28;
    s32 unknown_2c;
    s32 move_rate_x;
    s32 move_rate_z;
    s32 unknown_38;
    s32 unknown_3c;
    s32 unknown_40;
    u8 unknown_44[0x15];
    u8 collision_flags;
    u8 unknown_5a[8];
    u8 transition_busy;
    u8 unknown_63[3];
    s16 vertical_motion_direction;
    u8 unknown_68[4];
    u32 movement_callback;
};

struct StagedActorMoveArgs {
    s32 displacement_row;
    s32 actor_slot;
    s32 target_x;
    s32 elevation;
    s32 target_z;
    void (*callback)(void);
};


typedef char StagedActor_size[sizeof(struct StagedActor) == 0x70 ? 1 : -1];
typedef char StagedActor_x_offset[
    ((u32)&(((struct StagedActor *)0)->x)) == 0x08 ? 1 : -1
];
typedef char StagedActor_transition_mode_offset[
    ((u32)&(((struct StagedActor *)0)->transition_mode)) == 0x22 ? 1 : -1
];
typedef char StagedActor_collision_flags_offset[
    ((u32)&(((struct StagedActor *)0)->collision_flags)) == 0x59 ? 1 : -1
];
typedef char StagedActor_transition_busy_offset[
    ((u32)&(((struct StagedActor *)0)->transition_busy)) == 0x62 ? 1 : -1
];
typedef char StagedActor_vertical_motion_direction_offset[
    ((u32)&(((struct StagedActor *)0)->vertical_motion_direction)) == 0x66 ? 1 : -1
];
typedef char StagedActor_movement_callback_offset[
    ((u32)&(((struct StagedActor *)0)->movement_callback)) == 0x6c ? 1 : -1
];







struct StagedActorProbe {
    s32 footprint_index;
    s32 actor_slot;
    s32 position_x;
    s32 position_y;
    s32 position_z;
    void (*callback)(void);
};

struct StagedActorProbePosition {
    s32 x;
    s32 y;
    s32 z;
};


struct StagedActorFootprint {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};



typedef char StagedActorProbe_size[
    sizeof(struct StagedActorProbe) == 0x18 ? 1 : -1
];
typedef char StagedActorProbe_position_x_offset[
    ((u32)&(((struct StagedActorProbe *)0)->position_x)) == 0x08 ? 1 : -1
];
typedef char StagedActorProbe_position_z_offset[
    ((u32)&(((struct StagedActorProbe *)0)->position_z)) == 0x10 ? 1 : -1
];


typedef char StagedActorFootprint_size[
    sizeof(struct StagedActorFootprint) == 0x10 ? 1 : -1
];


struct StagedActorProbePoint {
    s32 x;
    s32 y;
    s32 z;
};

struct StagedActorProbeDetails {
    u8 unknown_00[0x28];
    s16 *unknown_28;
};

struct StagedActorProbeActorView {
    u8 unknown_00[0x50];
    struct StagedActorProbeDetails *unknown_50;
};






typedef char StagedActorProbePoint_size[
    sizeof(struct StagedActorProbePoint) == 0x0c ? 1 : -1
];
typedef char StagedActorProbeDetails_unknown_28_offset[
    ((u32)&(((struct StagedActorProbeDetails *)0)->unknown_28)) == 0x28 ? 1 : -1
];
typedef char StagedActorProbeDetails_size[
    sizeof(struct StagedActorProbeDetails) == 0x2c ? 1 : -1
];
typedef char StagedActorProbeActorView_unknown_50_offset[
    ((u32)&(((struct StagedActorProbeActorView *)0)->unknown_50)) == 0x50 ? 1 : -1
];



typedef struct StagedActorRecord {
    u8 padding_00[6];
    u16 orientation;
    s32 x;
    s32 depth;
    s32 y;
    u8 padding_14[16];
    s32 horizontal_velocity;
    u8 padding_28[4];
    s32 vertical_velocity;
    s32 movement_rate;
    s32 movement_step;
    u8 padding_38[52];
    u32 callback;
} StagedActorRecord;

typedef struct StagedActorPosition {
    s32 x;
    u8 padding_04[4];
    s32 y;
} StagedActorPosition;



s32 FixedPoint_Distance(s32 *first_position, s32 *second_position);
struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *self);
void StagedActor_AdvancePair(void);
s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);
s32 StagedActor_StopBlockedMotion(struct StagedActor *actor);
u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint);
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);
void SceneActor_MoveAndRedraw(struct StagedActorProbe probe);
s32 FieldScene_RedrawActorFootprint(s32 id);


struct MapLayerScroll {
    s32 x, y;
    s32 offset_x, offset_y;
    s32 scale_x, scale_y;
    s32 speed_x, speed_y;
    s32 phase_x, phase_y;
    u16 mask_x, mask_y;
    s32 unknown_2c;
};

struct MapScrollWork {
    s32 *origin;
    s32 shake_x, shake_y, shake_decay;
    u8 unknown_010[0xe4 - 0x10];
    s32 view_x, view_y;
    s32 min_x, min_y, max_x, max_y;
    s32 unknown_0fc;
    u8 enabled[3];
    u8 unknown_103;
    struct MapLayerScroll layers[3];
};

struct BgScroll { u16 x, y; };

extern struct MapScrollWork *gCam;
extern struct BgScroll gBgScroll[];

void Map_RenderPaletteMappedBlock(u32 layer, s32 x, s32 y);
void Map_RenderMetatileRow(u32 layer, s32 x, s32 y);



extern u8 MsgVinasuPairDefeated[];
void SceneEffect_SpawnParticlesAboveActor(void);

extern u8 Data_0200e088[];
extern u8 Data_0200e130[];
void SceneEffect_SpawnParticlesAboveActor();
void VinasuChojo_RunTransitionStep();


extern s32 VinasuChojo_TransitionTimer;
extern s32 VinasuChojo_TransitionStep;
extern u8 Data_0200e0d0[];
extern u8 Data_0200e0f4[];
void Engine_EventWait();
void VinasuChojo_ShowMessage();
void Object_SetActionCallbackAndRefreshById();
void SceneActor_ParkRecord();
void Event_SetPairWork1c0();
void ObjectTable_Snapshot();




struct Half {
    u16 v;
};

extern u8 MsgVinasuNoooo[];

enum {
    PARTICLE_SOURCE_ACTOR = 23
};

void SceneEffect_SpawnParticlesBesideActor(void);

extern u8 MsgVinasuDespiteLongTiring[];
extern u8 MsgVinasuWhyHappeningProtectVenusLighthouse[];

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
};

extern struct SceneWork *Data_03001ebc;
void OverlayObject_DecayRecordField1e(u8 *);
extern u8 Data_0200e324[];
extern u8 Data_0200e360[];
extern u8 Data_0200e074[];
extern u8 Data_0200e3c0[];
extern u8 Data_0200e39c[];
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Graphics_EnableObjLayerAndCallbacks();
void VinasuChojo_FaceActor();
void Object_RefreshSelectorById();

extern const u8 VinasuChojo_RiseActionScript[];
extern const s32 VinasuChojo_RiseParticleScript[];
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor);
void SceneEffect_AdvanceGatedRiseCounter(u8 *obj);

extern struct MapScrollWork *gMapWork;
extern const s32 VinasuChojo_BesideParticleScript[];
void FieldScene_RunScene3c9_02005b90(union FieldObject *object);
void VinasuChojo_UpdateBeamActors(void);

void FieldScene_RunThreeStepsInBracket(void);

void FieldScene_RunBracketedSceneWithFlag282(void);

void FieldScene_RunScene3c9_02003924(void);

void FieldScene_RunPairDefeat(void);

void FieldScene_RunMultiActorPresentation(void);

void VinasuChojo_RunActorTransition(void);

void FieldScene_RestageParty(void);

void FieldScene_RunScene3c9_02004b28(void);

void Scene_RunExtendedActorTransition(void);

void SceneActor_ParkRecord(u8 *record)
{

    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}







void VinasuChojo_RunTransitionStep(void)
{
    struct FieldActor *center;
    struct FieldActor *effect;
    struct FieldSprite *sprite;
    s32 spawn;
    u32 rise;
    s32 angle;

    center = Actor_Get(23);
    spawn = 0;
    switch (VinasuChojo_TransitionStep) {
    case 0:
        Audio_PlayCue(220);
        Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
        ColorBuffer_ApplyTarget(0x2063ff, 1);
        ColorBuffer_Interpolate(8);
        break;
    case 8:
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        break;
    case 16:
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        break;
    case 24:
        center->x.fixed = 152 << 17;
        center->y.fixed = -0x1680000;
        center->z.fixed = 164 << 16;
        center->scale_x = 0x10000;
        center->scale_y = 0x10000;
        SceneActor_ParkRecord((u8 *)center);
        Actor_EnableActionCallback(23, VinasuChojo_RiseActionScript);
        break;
    case 25:
        VinasuChojo_TransitionStep--;
        if (center->y.fixed > 0) {
            ColorBuffer_ApplyTarget(0x203210, 0);
            ColorBuffer_Interpolate(16);
            VinasuChojo_TransitionStep++;
            Actor_Get(0)->rise_counter = 1;
            Actor_Get(1)->rise_counter = 1;
            Actor_Get(2)->rise_counter = 1;
            Actor_Get(3)->rise_counter = 1;
            Actor_Get(21)->rise_counter = 1;
            Actor_Get(6)->rise_counter = 1;
        } else {
            if ((gFrameCount & 7) == 0)
                Audio_PlayCue(246);
            spawn = 1;
            center->y.fixed += 0x24000;
        }
        break;
    case 26:
        VinasuChojo_TransitionStep--;
        if (center->y.fixed > (160 << 14)) {
            ColorBuffer_ApplyTarget(0x10000, 0);
            ColorBuffer_Interpolate(40);
            VinasuChojo_TransitionStep++;
        } else {
            if ((gFrameCount & 7) == 0)
                Audio_PlayCue(246);
            spawn = 1;
            center->y.fixed += 0x24000;
        }
        break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
        spawn = 1;
        break;
    case 36:
        Audio_PlayCue(187);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        ColorBuffer_Interpolate(12);
        break;
    case 48:
        Actor_Stop(23);
        GameFlag_Set(0x237);
        break;
    }
    if (spawn) {
        rise = ((u32)(Random_Next() * 80) >> 16) << 16;
        effect = Engine_ObjectCreate(284, center->x.fixed, center->y.fixed - rise + (s32)0xfff80000, center->z.fixed);
        if (effect != 0) {
            sprite = effect->sprite;
            Object_SetScript(effect, VinasuChojo_RiseParticleScript);
            Object_SetPalette(effect, 1);
            effect->motion_flags = 0;
            angle = Random_Next() & 0xffff000;
            effect->unknown_64 = angle;
            effect->unknown_66 = 0;
            effect->rise_counter = (u32)Random_Next() >> 13;
            effect->update = (void (*)(union FieldObject *))SceneEffect_UpdateCounterDrivenOrbit;
            effect->speed = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24;
            effect->speed = center->speed >> 16;
            sprite->flags = 0;
            sprite->priority = 1;
        }
    }
    VinasuChojo_TransitionStep++;
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(0));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(1));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(2));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(3));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(21));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(6));
}


void SceneEffect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        SceneActor_ParkRecord(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}
void SceneEffect_SpawnParticlesBesideActor(void);

void FieldScene_RunScene3c9_02005b90(union FieldObject *object);

