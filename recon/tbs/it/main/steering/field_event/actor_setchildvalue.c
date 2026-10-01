/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Source: games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/FUNKA.C; edition IT; function FieldScene_RunScene381_02000e30.
 * Removing FIELD_EVENT.H Actor_SetChildValue changes mov	r0, sl to lsl	r1, r1, #1
 * (114/114 assembly lines).
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
static inline void Event_ShowMessage(s32 speaker, s32 flags);

static inline s32 Event_OpenMessage(s32 speaker, s32 flags);

static inline s32 Event_AskYesNo(s32 speaker, s32 flags);

static inline struct FieldActor *Actor_Get(s32 actor);


static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration);

static inline void Actor_SetDestination(s32 actor, s32 x, s32 z);

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz);

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z);

static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz);












static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
    Engine_ActorFaceDirection(actor, facing, frames);
}
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
static inline void Map_AnimateCells(const u16 *steps, s32 dest_x, s32 dest_y);

static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z);

static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz);

static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz);

static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags, s32 frames);

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

enum CollapseActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    ACTOR_GARCIA = ACTOR_FIRST_PLACED + 5,
    ACTOR_ALEX
};


struct FacingObject {
    u8 unknown_00[6];
    u16 facing;
    s32 position_x;
    u8 unknown_0c[4];
    s32 position_z;
    u8 unknown_14[0x46];
    u8 facing_flags;
    u8 unknown_5b[9];

    s16 unknown_64;
    struct FacingObject *facing_target;
};


typedef char FacingObject_size[sizeof(struct FacingObject) == 0x6c ? 1 : -1];
typedef char FacingObject_facing_offset[
    ((u32)&(((struct FacingObject *)0)->facing)) == 0x06 ? 1 : -1
];
typedef char FacingObject_position_x_offset[
    ((u32)&(((struct FacingObject *)0)->position_x)) == 0x08 ? 1 : -1
];
typedef char FacingObject_position_z_offset[
    ((u32)&(((struct FacingObject *)0)->position_z)) == 0x10 ? 1 : -1
];
typedef char FacingObject_facing_flags_offset[
    ((u32)&(((struct FacingObject *)0)->facing_flags)) == 0x5a ? 1 : -1
];
typedef char FacingObject_unknown_64_offset[
    ((u32)&(((struct FacingObject *)0)->unknown_64)) == 0x64 ? 1 : -1
];
typedef char FacingObject_facing_target_offset[
    ((u32)&(((struct FacingObject *)0)->facing_target)) == 0x68 ? 1 : -1
];

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[72];
    u8 *unk50;
} Ent;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} Ent_02002820;

typedef struct {
    u8 unk[9];
    u8 unk0 : 2;
    u8 mode : 2;
    u8 unk4 : 4;
} SpriteMode;

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
    u8 filler20[28];
    s32 unk3C;
} Ent_02002ba0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[16];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
} Ent_02002c1c;



struct Actor_02002e0c {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 amplitude_x;
    s32 amplitude_y;
    u8 filler20[0x44];
    u16 frame;
    u8 filler66[2];
    struct Actor_02002e0c *anchor;
};



struct Actor_02002e5c {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 amplitude_x;
    s32 amplitude_y;
    u8 filler20[0x44];
    u16 frame;
    u8 filler66[2];
    struct Actor_02002e5c *anchor;
};

extern u8 *gParticleWork[];

void SoruFunka_ThrowEruptionRing(void);
void Party_RemoveOwnerRestored();
void SceneState_InitStateWordsAndSlots();
s32 Engine_TaskAddCallback(void (*)(), s32);
void FieldScene_RunVariantStep();
void FieldEffect_InitSparkles(void);
void SceneState_ForwardByRuntimeWordBits(s32 object);
void SceneActor_MoveTo232_125AndFace4000(s32 no);
void SceneState_ApplyRectsByCondition(s32 a);
void SceneState_ApplyRectPairByFlag(s32 a);
void FieldScene_RunRandomHalfBranch(void);
void FieldScene_RunLateRandomHalfBranch(void);
void FieldScene_RunFourWayEffectSequence(u32 mode);
void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b);
void SceneState_ConfigureEightCornerRegions(void);
void FieldScene_RunStepByRuntimeBits(s32 a);
void SceneState_SetValue140Mode0(void);
void FieldScene_CallHelper6620(void);
void FieldScene_RunActor15TwoStep(void);
void BattleEffect_CleanupSceneObjects(void);
void SceneEffect_AdvanceTenEntryTimers(void);


extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern u8 Placement_Actors[];
extern u8 Placement_Effects[];
void SoruFunka_StepEmbers();
void SceneState_UpdateRandomTimerLevel();





extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;
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

static __inline__ s32 Iwram_Call2(s32 left, s32 right, void *routine);




extern u8 IwramMulQ16ReturnIp[];

extern u8 IwramIrqMain[];

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


extern u8 MsgSoruIKnowItsARock[];
extern u8 MsgSoruJasmineWhatHappened[];
extern u8 MsgSoruSomeoneIsLiftingIt[];
extern u8 MsgSoruSukuretaCouldThatBeThe[];

s32 Math_DivideUnsigned(s32 num, s32 den);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
s32 Engine_RandomNext(void);
s32 Math_RemainderUnsigned(s32 value, s32 modulus);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_AudioPlayCue(s32 cue);

struct EmberSprite {
    u8 unknown_00[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

struct Ember {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[16];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[12];
    struct EmberSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
};


extern const s32 Funka_EmberScript[];
extern s32 gEmberTimer;
extern s32 gEmberState[16];
extern s32 gEmberMask;
extern u8 MsgSoruCannotResist[];
extern u8 MsgSoruFriendsGone[];
extern u8 MsgSoruOverHere[];
extern u8 MsgSoruQuitActingTough[];
extern u8 MsgSoruReturnStarToBag[];
extern u8 MsgSoruThanksALot[];
extern u8 MsgSoruTheyllBeSafe[];
extern u8 MsgSoruThisIsTerrible[];
extern u8 MsgSoruWellTurnedBadly[];

enum {
    ACTOR_WISE_ONE = 15,

    ACTOR_FIRST_ROCK = 16,
    ROCK_COUNT = 16
};

enum {
    OBJECT_ELEMENTAL_STAR = 22,

    ITEM_MARS_STAR = 222
};

enum {

    FLAG_SOL_SANCTUM_ERUPTED = 0x814,
    FLAG_STAR_ROOM_COLLAPSED = 0x83f
};

struct QuakeWork {
    u8 unknown_000[0x40c];
    s32 unknown_40c;
};


extern s32 Funka_ArcOrigins[][2];
void SoruFunka_SpawnEffectPair();
void SceneEffect_UpdateArcOverAnchor();
void SceneEffect_UpdateAnchoredRiseArc();

struct PairDetail {
    u8 unknown_00[22];
    u8 field_16;
};

struct PairSprite {
    struct FieldSprite sprite;
    struct PairDetail *detail;
};

union PairObject {
    union FieldObject object;
    s32 words[28];
    struct {
        u8 unknown_00[0x68];
        union PairObject *parent;
    } link;
};

struct PairWork {
    u8 unknown_00[70];
    u16 vram_block;
};

typedef char PairSprite_Detail[(u32)&(((struct PairSprite *)0)->detail) == (0x28) ? 1 : -1];
typedef char PairObject_Parent[(u32)&(((union PairObject *)0)->link.parent) == (0x68) ? 1 : -1];
extern struct PairWork *gEffectWork;

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

extern struct WorldMapVramBlock gVramBlockCache[];
void Resource_ResetEntry(s32 block);
s32 AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);


struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};
u8 *SceneData_GetScriptTable(void);

s32 SceneData_ReturnZero(void);

u8 *SceneData_GetMessageTable(void);

u8 *SceneData_GetActorTable(void);

u8 *SceneData_GetEffectTable(void);

void Scene_SaturosTakesHostages(void);


void FieldScene_RunScene381_02000e30(s32 a0)
{
    u8 *rec7;
    s32 recA;
    s32 rec2;
    u8 i;

    recA = Object_GetById(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Engine_ActorWalkToAndWait(a0, 0x1d7, 0x122);
    Actor_FaceDirection(a0, 0xc000, 0);
    Engine_EventWait(10);
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    rec7 = Object_GetById(a0);
    rec2 = Object_GetById(a0);
    Engine_ActorSetSpriteFlags(rec2, 0);
    Engine_ActorSetChildValue(a0, 0x100);
    rec7[85] = 0;
    Audio_PlayCue(201);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x8000;
        Engine_EventWait(1);
        i++;
    } while (i != 60);
    Audio_PlayCue(190);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x1999;
        *(s32 *)(rec7 + 24) -= 0x28f;
        *(s32 *)(rec7 + 28) -= 0x28f;
        *(s32 *)(recA + 24) -= 0x28f;
        *(s32 *)(recA + 28) -= 0x28f;
        Engine_EventWait(1);
        i++;
    } while (i != 90);
    Actor_SetPosition(a0, 0, 0);
    Actor_SetPosition(8, 0, 0);
}
void Resource381_NoOpCallbackA(void);

void Resource381_NoOpCallbackB(void);

s32 FieldScene_RunWhenWord225Is10(void);

s32 OverlayObject_SetRecordAngleFromHeading(Ent *p);

void SoruFunka_ThrowEruptionRing(void);

void SceneState_InitStateWordsAndSlots(void);

void SoruFunka_StepEmbers(void);

void SceneState_UpdateRandomTimerLevel(void);

void Scene_RunExtendedEffectPresentation(void);

void SceneActor_MoveTo232_125AndFace4000(s32 no);

void SceneState_ApplyRectsByCondition(s32 a);

void SceneState_ApplyRectPairByFlag(s32 a);

void FieldScene_RunRandomHalfBranch(void);

void FieldScene_RunLateRandomHalfBranch(void);

void FieldScene_RunFourWayEffectSequence(u32 mode);

void SceneEffect_AdvanceTenEntryTimers(void);

void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b);

void SceneState_ConfigureEightCornerRegions(void);

void FieldScene_RunVariantStep(s32 a, s32 b, s32 c);

void FieldScene_RunStepByRuntimeBits(s32 a);

void SceneState_ForwardByRuntimeWordBits(s32 a);

void SceneEffect_UpdateArcOverAnchor(struct Actor_02002e0c *self);

void SceneEffect_UpdateAnchoredRiseArc(struct Actor_02002e5c *obj);

void SoruFunka_SpawnEffectPair(union PairObject *parent);

void SceneState_SetValue140Mode0(void);

void FieldScene_CallHelper6620(void);

void FieldScene_RunActor15TwoStep(void);

