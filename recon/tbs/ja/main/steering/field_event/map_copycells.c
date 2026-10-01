/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/DOU.C; edition JA; function Gate_DrawPropped.
 * Removing FIELD_EVENT.H Map_CopyCells changes mov	r2, #62 to str	r3, [sp]
 * (100/100 assembly lines).
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
static inline s32 GameFlag_IsSet(s32 flag);

static inline s32 GameFlag_Set(s32 flag);

static inline void GameFlag_Clear(s32 flag);

static inline void Audio_PlayCue(s32 cue);

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


enum GameFlagId {





    FLAG_KEEP_PARTY_POSITION = 0x109,

    FLAG_SHOW_LOCATION_NAME = 0x12f,

    FLAG_VAULT_THIEVES_JAILED = 0x855,

    FLAG_LUNPA_HEARD_OF_PRISONER = 0x940,

    FLAG_LUNPA_TRADE_REOPENED = 0x941,

    FLAG_LUNPA_CAVE_REUNION_SEEN = 0x94d,

    FLAG_PARTY_STAYED_IN_LUNPA = 0x94f
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

enum CaveEntrance {
    CAVE_ENTRANCE_FROM_WORLD_MAP = 1,
    CAVE_ENTRANCE_FROM_LUNPA = 2,
    CAVE_ENTRANCE_THIRD = 3
};

enum CaveExit {
    CAVE_EXIT_TO_WORLD_MAP = 1,
    CAVE_EXIT_TO_LUNPA = 2,
    CAVE_EXIT_BY_WAGON = 3
};


enum {
    WORLD_MAP_ENTRANCE_FROM_CAVE = 49,
    LUNPA_ENTRANCE_FROM_CAVE = 3
};

enum CaveTrigger {
    TRIGGER_WORLD_MAP_PASSAGE = 1,
    TRIGGER_LUNPA_PASSAGE = 2,
    TRIGGER_LOWERING_SWITCH = 3,
    TRIGGER_RAISING_SWITCH = 4,
    TRIGGER_BUNZA_HIDING_PLACE = 10
};


enum CaveFlag {
    FLAG_CAVE_GATE_PROPPED = 0x200,
    FLAG_CAVE_GATE_LOWERED = 0x201,
    FLAG_CAVE_GATE_RAISED = 0x202,
    FLAG_CAVE_NORTH_PILLAR = 0x203,
    FLAG_CAVE_SOUTH_PILLAR = 0x204
};


enum CaveActor {
    ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
    ACTOR_SOUTH_PUDDLE,
    ACTOR_GATE_PUDDLE,
    ACTOR_NORTH_PUDDLE,
    ACTOR_BUNZA,
    ACTOR_HAMMET
};

enum CaveSprite {
    SPRITE_HAMMET = 0x31,
    SPRITE_BUNZA = 0x44,
    SPRITE_PUDDLE = 0xe3
};


enum {
    PUDDLE_ANIM_FROZEN = 5
};


enum {
    GATE_PILLAR_PRIORITY = 3
};




enum SightingLine {
    SIGHTING_GERALD_SAW_SOMEONE,
    SIGHTING_MIA_SAW_SOMETHING,
    SIGHTING_IVAN_ASKS_IF_FOUND,
    SIGHTING_GERALD_WILL_FIGHT,
    SIGHTING_GERALD_ASKS_WHAT_ELSE
};


enum RecognitionLine {
    RECOGNITION_HAMMET_CALLS_OUT,
    RECOGNITION_HAMMET_NAMES_BUNZA,
    RECOGNITION_BUNZA_KNOWS_VOICE,
    RECOGNITION_BUNZA_NAMES_HAMMET
};

enum ReunionLine {
    REUNION_BUNZA_ASKS_ABOUT_RELEASE,
    REUNION_HAMMET_CREDITS_IVAN,
    REUNION_IVAN_CREDITS_LEADER,
    REUNION_GERALD_QUESTIONS_IVAN,
    REUNION_IVAN_CREDITS_EVERYONE,
    REUNION_HAMMET_THANKS_PARTY,
    REUNION_HAMMET_THANKS_IVAN,
    REUNION_MIA_PRAISES_BUNZA,
    REUNION_HAMMET_PRAISES_BUNZA,
    REUNION_BUNZA_CALLS_IT_CHANCE,
    REUNION_BUNZA_AVOIDED_LUNPA,
    REUNION_BUNZA_KNEW_OF_PRISON,
    REUNION_GERALD_ASKS_WHY_BUNZA_CAME,
    REUNION_BUNZA_RECALLS_ADVICE,
    REUNION_MIA_ASKS_ABOUT_TRADE,
    REUNION_BUNZA_MEANS_WISDOM,
    REUNION_HAMMET_ON_APPEARANCES,
    REUNION_BUNZA_ON_UNPLEASANT_PLACES,
    REUNION_HAMMET_ON_SELLING,
    REUNION_IVAN_ASKS_ABOUT_SERVING,
    REUNION_IVAN_ON_FATE,
    REUNION_GERALD_ASKS_ABOUT_ENTRY,
    REUNION_BUNZA_WAS_REFUSED,
    REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME,
    REUNION_BUNZA_MENTIONS_COMMOTION,
    REUNION_IVAN_EXPLAINS_COMMOTION,
    REUNION_BUNZA_LINKS_COMMOTION,
    REUNION_BUNZA_HAD_TO_KNOW,
    REUNION_GERALD_ASKS_ABOUT_CAVE,
    REUNION_MIA_ON_GATE,
    REUNION_BUNZA_HID,
    REUNION_HAMMET_STARTLED_BUNZA,
    REUNION_BUNZA_IS_GLAD,
    REUNION_IVAN_ASKS_WHY,
    REUNION_BUNZA_WARNS_OF_SEARCH,
    REUNION_BUNZA_FEARS_CAPTURE,
    REUNION_MIA_URGES_ESCAPE,
    REUNION_IVAN_WANTS_STEALTH,
    REUNION_GERALD_ASKS_TO_FIGHT,
    REUNION_BUNZA_DISCOURAGES_FIGHT,
    REUNION_BUNZA_AGREES_NOT_TO_FIGHT
};

enum PlanLine {
    PLAN_HAMMET_ASKS_PLAN,
    PLAN_BUNZA_HAS_WAGON,
    PLAN_MIA_ASKS_ABOUT_WAGON,
    PLAN_BUNZA_OFFERS_RIDE,
    PLAN_IVAN_DOUBTS_WAGON,
    PLAN_BUNZA_REASSURES,
    PLAN_GERALD_AGREES,
    PLAN_BUNZA_IS_UNSUSPECTED
};

enum WagonLine {
    WAGON_BUNZA_LEADS_THE_WAY,
    WAGON_GERALD_ASKS_TO_RIDE
};

enum ConfusionLine {
    CONFUSION_IVAN_IS_CONFUSED,
    CONFUSION_GERALD_ASKS_AGAIN
};

enum WarningLine {
    WARNING_IVAN_ASKS_IF_STAYING,
    WARNING_BUNZA_CANNOT_WAIT,
    WARNING_MIA_ASKS_ABOUT_BUSINESS
};

enum FarewellLine {
    FAREWELL_GERALD_STAYS,
    FAREWELL_MIA_STAYS,
    FAREWELL_IVAN_STAYS,
    FAREWELL_HAMMET_LETS_IVAN_GO,
    FAREWELL_BUNZA_SAYS_GOODBYE,
    FAREWELL_GERALD_SIGHS,
    FAREWELL_MIA_HOPES_FOR_SAFETY,
    FAREWELL_IVAN_REASSURES,
    FAREWELL_GERALD_MOVES_ON
};

enum DepartureLine {
    DEPARTURE_GERALD_HEADS_FOR_KALAY,
    DEPARTURE_IVAN_THINKS_OF_LAYANA,
    DEPARTURE_HAMMET_LONGS_FOR_LAYANA,
    DEPARTURE_BUNZA_SETS_OFF
};


extern const struct SceneEntrance gCaveEntrances[];
extern const u32 gCaveExits[];
extern const struct ScenePlacement gCaveNoPlacements[];
extern const struct ScenePlacement gCavePlacements[];
extern const struct SceneEvent gCaveEvents[];

void HiddenPuddle_Freeze(void);
void SouthPuddle_Freeze(void);
void GatePuddle_Freeze(void);
void NorthPuddle_Freeze(void);
void LoweringSwitch_Flip(void);
void RaisingSwitch_Flip(void);
void Reunion_Begin(void);
void Gate_DrawPropped(void);
void Gate_Lower(void);
void Gate_Raise(void);
void Reunion_Converse(void);
void WagonChoice_Run(void);
u8 Leader_AnswersYes(void);
u8 Gerald_AsksIfNotRiding(void);
u8 Gerald_AsksAboutUnfinishedBusiness(void);
u8 Party_ConfirmsStaying(void);
u8 Bunza_AsksAboutUnfinishedBusiness(void);
u8 Gerald_ChecksNothingLeftToDo(void);
u8 Mia_AsksIfRidingAfterAll(void);
u8 Bunza_CannotWait(void);
u8 Gerald_AsksAboutThingsToDo(void);
void Party_StaysBehind(void);
void Party_RidesWagon(void);


extern u8 MsgFieldFlippedSwitch[];

extern u8 MsgRunpaBunza[];
extern u8 MsgRunpaDodonpaNeverIntention[];
extern u8 MsgRunpaMoment[];
extern u8 MsgRunpaThinkSawSomeone[];

extern u8 MsgRunpaBunzaAsksAboutUnfinishedBusiness[];
extern u8 MsgRunpaGeraldAsksAboutUnfinishedBusiness[];
extern u8 MsgRunpaGeraldAsksIfNotRiding[];
extern u8 MsgRunpaGeraldChecksNothingLeft[];
extern u8 MsgRunpaLetsTakeWagon[];
extern u8 MsgRunpaMiaAsksIfRidingAfter[];
extern u8 MsgRunpaTotallyConfusedChanged[];

extern u8 MsgRunpaMeaningWontRide[];

extern u8 MsgRunpaGeraldAsksAboutThingsTo[];

extern u8 MsgRunpaDontUnfinishedBusiness[];
extern u8 MsgRunpaInsistStick[];
const struct SceneEntrance *Scene_GetEntrances(void);

const struct SceneRegion *Scene_GetRegions(void);

const u32 *Scene_GetExits(void);

const struct ScenePlacement *Scene_GetPlacements(void);

const struct SceneEvent *Scene_GetEvents(void);

void HiddenPuddle_Freeze(void);

void SouthPuddle_Freeze(void);



void Gate_DrawPropped(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_GATE_PUDDLE);
    Engine_ActorSetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIM_FROZEN);
    if (pillar != ((void *)0)) {
        Engine_ActorSetSpriteFlags(pillar, 0);
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    Engine_MapCopyCells(41, 87, 2, 5, 21, 59);
    Engine_TaskWait(4);
    Engine_MapCopyCells(3, 93, 1, 1, 24, 62);
    Engine_MapCopyCells(1, 94, 1, 1, 21, 55);
    Engine_MapCopyCells(43, 87, 2, 5, 21, 58);
    Engine_TaskWait(4);
    Engine_MapCopyCells(41, 87, 2, 5, 21, 58);
    Engine_TaskWait(4);
    Engine_TaskWait(4);
    Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
    Map_CopyCellAttributes(21, 11, 1, 1, 22, 15);
    Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}
void GatePuddle_Freeze(void);

void NorthPuddle_Freeze(void);

void Gate_Lower(void);

void LoweringSwitch_Flip(void);

void Gate_Raise(void);

void RaisingSwitch_Flip(void);

void Reunion_Begin(void);

void Reunion_Converse(void);

void WagonChoice_Run(void);

u8 Leader_AnswersYes(void);

u8 Gerald_AsksIfNotRiding(void);

u8 Gerald_AsksAboutUnfinishedBusiness(void);

u8 Party_ConfirmsStaying(void);

u8 Bunza_AsksAboutUnfinishedBusiness(void);

u8 Gerald_ChecksNothingLeftToDo(void);

u8 Mia_AsksIfRidingAfterAll(void);

u8 Bunza_CannotWait(void);

u8 Gerald_AsksAboutThingsToDo(void);

void Party_StaysBehind(void);

void Party_RidesWagon(void);

s32 Scene_Initialize(void);

