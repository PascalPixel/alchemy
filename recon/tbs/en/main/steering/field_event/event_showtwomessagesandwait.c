/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/MURA.C; edition EN; function HouseScene_RunRepairMorning.
 * Removing FIELD_EVENT.H Event_ShowTwoMessagesAndWait changes mov	r3, #14 to mov	r3, #2
 * (2087/2088 assembly lines).
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

static inline s32 Event_OpenMessage(s32 speaker, s32 flags)
{
    return Engine_EventOpenMessage(speaker, flags);
}
static inline s32 Event_AskYesNo(s32 speaker, s32 flags);



















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







static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetDestination(actor, x, z);
}
static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz);








static inline void Actor_WalkTo(s32 actor, s32 x, s32 z)
{
    Engine_ActorWalkTo(actor, x, z);
}
static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz);












static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
    Engine_ActorFaceDirection(actor, facing, frames);
}
static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames);




















static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
    Engine_ActorShowEmote(actor, emote, frames);
}


static inline void Actor_SetAttachedEffect(s32 actor, s32 effect)
{
    Engine_ActorSetAttachedEffect(actor, effect);
}








static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    Engine_CameraSetSpeed(speed, acceleration);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
    Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}
static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y);



static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                          s32 dest_x, s32 dest_y)
{
    Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}
static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third);






enum {
    FLAG_ARRIVAL_EVENT_PENDING = 0x12f
};
static inline s32 GameFlag_IsSet(s32 flag);


static inline s32 GameFlag_Set(s32 flag)
{
    return Engine_GameFlagSet(flag);
}

static inline void GameFlag_Clear(s32 flag)
{
    Engine_GameFlagClear(flag);
}
static inline void Audio_PlayCue(s32 cue);

static inline s32 Task_RemoveCallback(void (*callback)(void));

static inline s32 Random_Next(void);

static inline s32 Math_Divide(s32 dividend, s32 divisor);

static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z);

static inline void Object_SetScript(struct FieldActor *object, const s32 *script);



enum ObjectBlendMode {
    OBJECT_BLEND_NORMAL = 0,
    OBJECT_BLEND_TRANSLUCENT = 1
};
static inline void Map_AnimateCells(const u16 *steps, s32 dest_x, s32 dest_y);

static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z);

static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz);

static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz);

















static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags, s32 frames)
{
    Engine_EventShowMessageAndWait(speaker, flags, frames);
}









static inline void Actor_SetChildValue(s32 actor, s32 value)
{
    Engine_ActorSetChildValue(actor, value);
}


















static inline void Map_CopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width,
                                   s32 height)
{
    Engine_MapCopyCellsTo(src_x, src_y, dest_x, dest_y, width, height);
}


static inline void MapRender_SetValues(s32 value0, s32 value1, s32 value2)
{
    Engine_MapRenderSetValues(value0, value1, value2);
}
static inline void ColorBuffer_ApplySource(s32 value, s32 mode);

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode);








static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z)
{
    Engine_ActorMoveToAndWait(actor, x, z);
}

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z)
{
    Engine_ActorWalkToAndWait(actor, x, z);
}








static inline void Party_AddMembers(s32 first, s32 second)
{
    Engine_PartyAddMembers(first, second);
}


static inline void MapRender_WaitForValues(void)
{
    Engine_MapRenderWaitForValues();
}


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

    ITEM_NUT = 181
};



enum {
    ACTOR_DORA = 21
};




struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

struct SourceEntity {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ParticleRecord {
    u8 f00[9];
    u8 f09;
};

struct StagedParticle {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct ParticleRecord *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};


struct Resource373Emitter {
    u8 unknown_00[6];
    u16 field06;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 field18;
    s32 field1c;
    u8 unknown_20[4];
    s32 field24;
    s32 field28;
    s32 field2c;
    u8 unknown_30[8];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Resource373Particle {
    u8 unknown_00[0x28];
    s32 lifetime;
    u8 unknown_2c[4];
    s32 field30;
    s32 field34;
    u8 unknown_38[0x10];
    s32 field48;
    u8 unknown_4c[9];
    u8 field55;
};

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

extern s32 gHaidiaMuraEntrances[];
extern s32 gHaidiaMuraExits[];
extern s32 gHaidiaMuraPlacements4[];
extern s32 gHaidiaMuraPlacements3[];
extern s32 gHaidiaMuraPlacements2[];
extern s32 gHaidiaMuraPlacements[];
extern s32 gHaidiaMuraEvents3[];
extern s32 gHaidiaMuraEvents2[];
extern s32 gHaidiaMuraEvents[];
extern u8 gHaidiaMuraCellAnimA[];
extern u8 gHaidiaMuraCellAnimB[];
extern u8 gHaidiaMuraCellAnimC[];
extern u8 gHaidiaMuraCellAnimD[];
extern u8 gHaidiaMuraActor22Actions[];
extern u8 gHaidiaMuraLeaderWalkActions[];
extern u8 gHaidiaMuraActor22WalkActions[];
extern u8 gVillagerAction[];
extern u8 gLeaderHammerAction[];
extern u8 gGeraldAction[];
extern u8 gJasmineAction[];
extern s32 gHaidiaMuraPairTableA[];
extern s32 gHaidiaMuraPairTableB[];
extern s32 gHaidiaMuraPairTableC[];
extern s32 gHaidiaMuraPairTableD[];
extern s32 gDustBurstScript[];

void Object_RefreshSelectorById();
s32 Object_SetActionCallbackAndRefreshById();
void Graphics_EnableObjLayerAndCallbacks();
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Audio_PlayCueFromEventWork();
void Object_SetPosition();
void HaidiaMura_RunWalkScene032B0(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void HaidiaMura_OpenVillagerLane(void);
s32 SceneActor_RunStep18WhenTargetSet();
void HaidiaMura_RunWalkScene03380(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void FieldScene_RunStep8C();
void SceneState_SetValue0ThenCall();
void FieldScene_RunStep17();
void SceneState_SetValue1ThenCall();
void FieldScene_RunStep9();
void Effect_ConfigureSpawnedParticle(struct SourceEntity *source);
void SceneState_SetValue24ThenCall();
void SceneState_SetValue25ThenCall();
void Effect_PlayStepSound();
void Vector_AddPolarOffset(s32, s32, s32 *);
void BattleEffect_CleanupSceneObjects(void);
s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force);
void InitializeStagedActorSceneOrbitingEffect(s32 id);
u8 *Runtime_AllocateHeapBlock(s32, s32);
void WorldMap_CreateLinkedEffects(s32);
s32 SceneEffect_UpdateOrbitPosition(s32 *p);
void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z);
static __inline__ void bump_step(s32 amount);

static __inline__ void Call1(void (*f)(), s32 a0);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1);


static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */


    f(a0, a1, a2);
}
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

extern u8 MsgHaidiaDoYouNeedToGo[];
extern u8 MsgHaidiaDontGoBeyondSukuretasCottage[];
extern u8 MsgHaidiaIFeltAnotherOne[];
extern u8 MsgHaidiaIToldGeraldItWas[];
extern u8 MsgHaidiaIllGetYouForMy[];
extern u8 MsgHaidiaIsJasmineBackYet[];
extern u8 MsgHaidiaItWontRainForSome[];
extern u8 MsgHaidiaNoTravelersSinceTheEruption[];
extern u8 MsgHaidiaSukuretaHasntComeBack[];
extern u8 MsgHaidiaSukuretaIsWaitingForUs[];
extern u8 MsgHaidiaTheGroundStillShakes[];
extern u8 MsgHaidiaYouCantBeRobin[];
extern u8 MsgHaidiaYouMakeMeSoMad[];
extern u8 MsgHaidiaYourGrandpaIsTheMayor[];
extern u8 MsgHaidiaPuppiesPlayingOver[];
extern u8 MsgHaidiaRrruffRrrruff[];
extern struct MapRenderWork *gMapWork;
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Battle_WaitMode0();
void ObjectMotion_SetSpeedParameters();
void Engine_MessageShowCentered();
void Engine_MapRedraw();
void Object_SetModeById();
void Engine_EventEnd();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorWalkToAndWait();
extern u8 MsgHaidiaNotSneakingUpMtAleph[];

extern u8 MsgHaidiaAsStubbornAsYourFather[];
extern u8 MsgHaidiaDevastatedWhenKyle[];
extern u8 MsgHaidiaGoodJob[];
extern u8 MsgHaidiaWorkingYourselvesBone[];

enum HouseActor {
    ACTOR_BOARD = 23,
    ACTOR_LAST_BOARD = 24
};

enum {
    ANIM_HAMMER = 11
};

void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);





void HaidiaMura_RunWalkScene032B0(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void HaidiaMura_RunWalkScene03380(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void PaletteGlow_Update(s32 a, s32 b);
void FieldScene_RunLargeStagingSequence(void);
void HaidiaMura_OpenVillagerLane(void);
void Scene_RepairTheHouse(void);
void SceneState_Send210AndApplyRectAt40x84(void);
void BattleFx_SetQueuedSoundAndPlay(s32 value);
void SceneActor_SetFlagByteBySlotZeroPosition(void);
void FieldScene_RunScene373SequenceB(void);
s32 SceneActor_RunStep18WhenTargetSet();
extern u8 gHaidiaMuraActor22Actions[];

extern u8 MsgHaidiaAh[];
extern u8 MsgHaidiaEverProtectFamily[];
extern u8 MsgHaidiaHowHaveYouBeen[];
extern u8 MsgHaidiaIUsedToPlayHere[];
void Engine_ActorSetSpeed();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
void Engine_ActorMoveToAndWait();
void Engine_EventWait();

struct Flags38 {
    u8 pad[38];
    u8 flags;
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

extern u8 MsgHaidiaRepairCaption[];

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
u16 ArcTan2(s32 z, s32 x);

extern u8 MsgHaidiaDoorWontOpen[];
s32 MapStagedScene_SelectPrimaryData(void);

s32 MapStagedScene_GetEmptyData(void);

s32 MapStagedScene_SelectSecondaryData(void);

s32 MapStagedScene_SelectTertiaryData(void);

void SceneDialogue_RunActor181Scene(void);

void FieldScene_RunActor181Scene(void);

s32 MapStagedScene_SelectQuaternaryData(void);

void SceneDialogue_RunActorTenFlaggedDialogue(void);

void SceneDialogue_RunActorFourteenTalk(void);

void FieldScene_RunFlag807BranchSequence(void);

void SceneDialogue_RunActor21FlaggedLine(void);

void SceneDialogue_RunActor10LineAndFlag81f(void);

void FieldScene_RunScene373_02000cd0(void);

void SceneDialogue_RunActorNineteenDialogue(void);

void SceneState_Send210AndApplyRectAt40x84(void);

void SceneState_Send210AndApplyRect(void);

void FieldScene_RunScene373_02000dc0(void);

void SceneState_ApplyFlag801Branch(void);

void SceneState_SetValue123Mode3(void);

void SceneState_SetValue123Mode4(void);

void SceneState_ApplyValues123And2(void);

void FieldScene_RunScene373_02000e54(void);

void FieldScene_RunScene373_02000e84(void);

void SceneDialogue_RunFlag815GatedStep(void);

void FieldScene_RunPrimarySequence(void);

void FieldScene_RunScene373SequenceA(void);

void FieldScene_RunPrimarySequenceSecond(void);

void FieldScene_RunScene373SequenceC(void);

void HaidiaMura_RunCameraRiseScene(void);

void FieldScene_RunPuppyBarks(void);

void SceneState_RunFlag204Step(void);

void SceneState_SetFlag204AndConfigureRegion49_46(void);

void FieldScene_RunScene373SequenceE(void);

void SceneState_RunTablePairWhenActor22State1(void);

void FieldScene_RunScene373_02001490(s32 a0, s32 a1);

void SceneState_RunTablePairWhenActor22State2(void);

void SceneState_RunTablePairByActor22State(void);








void HouseScene_RunRepairMorning(void)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    u8 leader_motion_flags;
    u8 *leader_motion;
    s32 i;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    actor = Engine_EventGetViewCenter();
    actor->motion_flags = 0;
    Camera_MoveTo(((383) << 16), ((160) << 16), ((877) << 16), 0);
    Engine_EventWait(1);
    Engine_MapRedraw();
    Map_CopyCellAttributes(49, 41, 7, 3, 20, 50);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader_motion = &leader->motion_flags;
    leader_motion_flags = *leader_motion;
    *leader_motion = 0;
    Actor_SetPosition(ACTOR_PARTY_LEADER, ((407) << 16), ((690) << 16));
    Actor_SetPosition(ACTOR_DORA, ((392) << 16), ((896) << 16));
    Actor_SetPosition(ACTOR_GERALD, ((298) << 16), ((736) << 16));
    Actor_SetPosition(ACTOR_JASMINE, ((298) << 16), ((760) << 16));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    SceneActor_ResetActorRun(ACTOR_BOARD, 2, 1);
    gEventWork->start_transition = 0;
    gEventWork->transition_frames = 32;
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Engine_ActorEnableActionCallback(ACTOR_JASMINE, gJasmineAction);
    Engine_ActorEnableActionCallback(ACTOR_GERALD, gGeraldAction);


    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ((const u8 *)1));
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
    Actor_SetChildValue(ACTOR_BOARD, 2);
    Actor_SetSpeed(ACTOR_BOARD, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_MoveToAndWait(ACTOR_BOARD, 402, 828);
    Engine_EventWait(80);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(ACTOR_BOARD, 0);
    Actor_SetPosition(ACTOR_BOARD, ((390) << 16), ((842) << 16));
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    Engine_EventWait(200);
    Map_CopyCellsTo(7, 102, 84, 41, 2, 1);


    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ((const u8 *)1));
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
    Actor_SetChildValue(ACTOR_BOARD, 2);
    Actor_SetSpeed(ACTOR_BOARD, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_MoveToAndWait(ACTOR_BOARD, 377, 828);
    Engine_EventWait(80);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(ACTOR_BOARD, 0);
    Actor_SetPosition(ACTOR_BOARD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    Engine_EventWait(200);
    Map_CopyCellsTo(6, 102, 83, 41, 1, 1);


    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ((const u8 *)1));
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
    Actor_SetChildValue(ACTOR_LAST_BOARD, 2);
    Actor_SetSpeed(ACTOR_LAST_BOARD, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR_LAST_BOARD, 390, 832);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_MoveToAndWait(ACTOR_LAST_BOARD, 360, 842);
    Engine_EventWait(80);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(ACTOR_LAST_BOARD, 0);
    Actor_SetPosition(ACTOR_LAST_BOARD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
    Engine_EventWait(200);
    Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ((const u8 *)1));
    leader->scale_x = 0x10000;
    leader->scale_y = 0x10000;


    Engine_EventSetMessage((s32)MsgHaidiaGoodJob);
    Engine_ActorJump(ACTOR_DORA, 2, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
    HaidiaMura_RunWalkScene032B0(ACTOR_DORA, 5, 6, 0);
    Actor_SetSpeed(ACTOR_DORA, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
    Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        gEventWork->message++;
    }
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_EventSetMessage((s32)MsgHaidiaWorkingYourselvesBone);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
        gEventWork->message++;
    }
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgHaidiaDevastatedWhenKyle);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100 | 2, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);


    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(((377) << 16), ((160) << 16), ((860) << 16), 1);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 369, 904);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 904);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_STAND);
    HaidiaMura_RunWalkScene032B0(ACTOR_JASMINE, 10, 11, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_NORTHWEST, 0);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
    Engine_ActorJump(ACTOR_JASMINE, 4, 0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 843);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    HaidiaMura_RunWalkScene032B0(ACTOR_GERALD, 10, 11, 0);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_GERALD, 0x4ccc, 0x2666);
    Actor_WalkTo(ACTOR_GERALD, 392, 843);
    Actor_Get(ACTOR_JASMINE)->unknown_5a &= 0xfe;
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 843);
    Engine_EventWait(1);
    Actor_Get(ACTOR_JASMINE)->unknown_5a |= 1;
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 0);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_STAND);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 30);
    Engine_ActorJump(ACTOR_DORA, 4, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100 | 2);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_DORA, 0x100 | 1, 80);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100 | 2, 80);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(100);
    Engine_ActorFaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_DORA, 0x100 | 5, 60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(((373) << 16), ((160) << 16), ((837) << 16), 1);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
    Engine_ActorFaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 30);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100 | 5, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(10);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
        gEventWork->message++;
    }
    Engine_EventWait(40);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);


    Engine_EventSetMessage((s32)MsgHaidiaAsStubbornAsYourFather);
    Actor_ShowEmote(ACTOR_DORA, 0x100 | 3, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorJump(ACTOR_DORA, 4, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(ACTOR_DORA, 7);
    Engine_EventWait(5);
    Engine_EventShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, ACTOR_GERALD, 10, 14, 4, 14, 0);
    actor = Actor_Get(ACTOR_DORA);
    actor->sprite->flags = 0;
    actor->unknown_5a &= 0xfe;
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
    Engine_EventWait(4);
    for (i = 0; i != 4; i++) {
        actor->z.fixed += 0x18000;
        actor->scale_y -= 0x1999;
        Engine_EventWait(1);
    }
    Actor_SetPosition(ACTOR_DORA, 0, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x30000, 0x18000);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 374, 827);
    Event_ShowMessage(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100 | 0, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_ShowEmote(ACTOR_GERALD, 0x100 | 0, 10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 13);
    Engine_ActorJump(ACTOR_GERALD, 2, 5);
    MapRender_SetValues(0, 0x40000, 0x10000);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 3);
    MapRender_SetValues(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, 0x100 | 2, 80);
    Engine_ActorSetAnimation(ACTOR_DORA, 8);
    actor->scale_y = 0x8000;
    Actor_SetPosition(ACTOR_DORA, ((364) << 16), ((811) << 16));
    for (i = 0; i != 5; i++) {
        actor->scale_y += 0x1999;
        Engine_EventWait(1);
    }
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(((372) << 16), ((160) << 16), ((859) << 16), 1);
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Engine_ActorJump(ACTOR_DORA, 6, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(30);
    actor = Actor_Get(ACTOR_DORA);
    actor->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
    Actor_ShowEmote(ACTOR_DORA, 0x100 | 1, 80);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_SetAttachedEffect(ACTOR_DORA, 0x100 | 2);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 30);
    Actor_ShowEmote(ACTOR_GERALD, 0x100 | 2, 80);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 1);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_STAND);
    Actor_SetSpeed(ACTOR_GERALD, 0x40000, 0x20000);
    actor = Actor_Get(ACTOR_GERALD);
    actor->unknown_5a &= 0xfe;
    Actor_SetDestination(ACTOR_GERALD, 403, 827);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x100 | 2);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_NORTH, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 1);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x100 | 0, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 13);
    Engine_ActorJump(ACTOR_GERALD, 2, 5);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    MapRender_SetValues(0, 0x40000, 0x10000);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    MapRender_SetValues(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, 0x100 | 2, 30);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 855);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_ShowEmote(ACTOR_DORA, 0x100 | 5, 60);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 3);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_SHAKE_HEAD);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
    Actor_ShowEmote(ACTOR_DORA, 0x100 | 5, 80);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100 | 1, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100 | 1, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100 | 1, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 70);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_WEST, 60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
    Actor_ShowEmote(ACTOR_DORA, 0x100 | 0, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Engine_ActorJump(ACTOR_GERALD, 4, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 398, 828);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    Engine_EventWait(60);


    actor = Actor_Get(ACTOR_GERALD);
    actor->unknown_5a |= 1;
    actor = Actor_Get(ACTOR_JASMINE);
    actor->unknown_5a |= 1;
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_WalkTo(ACTOR_JASMINE, actor->x.part.pixel + 16, actor->z.part.pixel);
    Actor_WalkToAndWait(ACTOR_GERALD, actor->x.part.pixel + 16, actor->z.part.pixel - 16);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 30);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_JASMINE, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(40);
    Actor_WalkToAndWait(ACTOR_JASMINE, actor->x.part.pixel, actor->z.part.pixel);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Party_AddMembers(ACTOR_GERALD, ACTOR_JASMINE);
    Camera_MoveTo(((377) << 16), ((160) << 16), ((887) << 16), 1);
    HaidiaMura_RunWalkScene03380(ACTOR_PARTY_LEADER, 13, 10, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_Get(ACTOR_DORA)->unknown_5a |= 1;
    HaidiaMura_RunWalkScene03380(ACTOR_DORA, 6, 5, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
    Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
    Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(100);
    GameFlag_Set(0x202);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    *leader_motion = leader_motion_flags;
    Engine_EventEnd();
}
s32 HaidiaMura_ApplyEntryState(void);

void FieldScene_RunSecondaryActorSequence(void);

void FieldScene_RunPrimaryActorSequence(void);

void FieldScene_RunCompanionActorSequence(void);


void HaidiaMura_RunWalkScene032B0(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p8;
    s32 p9;
    s32 p9b;
    u8 *rec8;
    s32 record;
    u8 *p5;

    p9 = a3;
    p8 = a1;
    p10 = a2;
    rec8 = ((u8 * (*)())Object_GetById)();
    p5 = *(s32 *)((s32)rec8 + 80);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, a0, 0x188, 0x376);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 10);
    ((struct Flags85 *)rec8)->flags = 0;
    ((struct Flags38 *)p5)->flags = 0;
    Engine_ActorSetAnimation(a0, p8);
    Call3(Engine_ActorSetSpeed, a0, 0x4ccc, 0x2666);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x36b);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(a0, p10);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x35b);
    p5[38] = 1;
    if (p9 != 0) {
        rec8[85] = 3;
    }
    Engine_EventWait(10);
    Engine_ActorSetAnimation(a0, 1);
    p9b = (s32)p5 + 38;
    p10b = (s32)rec8 + 85;
}

void HaidiaMura_RunWalkScene03380(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    u8 *p11;
    s32 p11b;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 rec8;
    s32 record;
    u8 *p5;

    p9 = a3;
    p8 = a1;
    p10 = a2;
    rec8 = ((s32 (*)())Object_GetById)();
    p5 = *(s32 *)(rec8 + 80);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, a0, 0x188, 0x35b);
    Call3(Engine_ActorFaceDirection, a0, 0xc000, 10);
    ((struct Flags85 *)rec8)->flags = 0;
    ((struct Flags38 *)p5)->flags = 0;
    Engine_ActorSetAnimation(a0, p8);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x36b);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, a0, 0x4ccc, 0x2666);
    Engine_ActorSetAnimation(a0, p10);
    Call3(Engine_ActorMoveToAndWait, a0, 0x188, 0x37a);
    *(s32 *)(rec8 + 40) = 0x20000;
    p5[38] = 1;
    if (p9 != 0) {
        ((struct Flags85 *)rec8)->flags = 3;
    }
    Engine_ActorSetAnimation(a0, 1);
    p9b = (s32)p5 + 38;
    p11b = a0;
}

void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Actor_Get(selector);

            actor->flag55 = 0;
            Engine_ActorSetSpriteFlags(actor, 0);
            actor->field08 = 0x01860000;
            actor->field0c = 0x00a00000;
            actor->field10 = 0x034a0000;
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Actor_SetPosition(selector, 0, 0);
        selector++;
    }
}
void FieldScene_RunLargeStagingSequence(void);

void Scene_RepairTheHouse(void);

void FieldScene_RunStep8C(void);

void FieldScene_RunSingleStep(void);

void SceneState_SetValue1ThenCall(void);

void SceneState_SetValue0ThenCall(void);

void FieldScene_RunStep9(void);

void FieldScene_RunStep17(void);

void SceneState_SetValue24ThenCall(void);

void SceneState_SetValue25ThenCall(void);

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);

s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force);

s32 SceneActor_RunStep18WhenTargetSet(s32 *p);

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source);

void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter);

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z);

void SceneState_ApplyRectAndRunTwo(void);

void HaidiaMura_OpenVillagerLane(void);

void Effect_PlayStepSound(void);

void FieldScene_RunScriptedStepEE4(void);

void FieldScene_RunScene373SequenceB(void);

void SceneActor_SetFlagByteBySlotZeroPosition(void);

s32 SceneEffect_UpdateOrbitPosition(s32 *p);

void InitializeStagedActorSceneOrbitingEffect(s32 id);

void OverlayObject_UpdateOnFrameBit1(s32 p);

void SceneEffect_UpdateObjectOnOddFrames(s32 p);

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p);

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect);

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect);

