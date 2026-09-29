#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "RESOURCE_3A8_EFFECT.H"
extern u8 MsgKareiOkListeningLets[];
extern u8 MsgKareiDecideEnteringColosso[];
extern u8 MsgKareiShortOnePerson[];
extern u8 MsgKareiThatsWeCantWaitAny[];
extern u8 MsgKareiWhyWeStoppingAtPlace[];

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
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
void Scheduler_RemoveCallback();
void Audio_PlayCueFromEventWork();

void FieldScene_RunStagedGroupSequence(void)
{

    s32 messageId;
    const u32 *actionDescriptor;
    s32 idleState;
    s32 actionEnabled;
    u8 *actor20PairedWait;
    u8 *actor21PairedWait;
    u8 *closingWaitRecord;
    u8 *actor14FinalFacing;
    u8 *actor20FinalFacing;
    u8 *actor21FinalFacing;
    u8 *playerFinalFacing;
    u8 *actor21OpeningWait;
    u8 *actor20BeforeFirstMove;
    u8 *actor20AfterFirstMove;
    u8 *actor20BeforeSecondMove;
    u8 *actor20AfterSecondMove;
    u8 *sceneWorkspace;

    sceneWorkspace = *(u8 **)&gEventWork;
    Event_Begin();
    if (GameFlag_IsSet(2320) == 0) {
        goto skip_scene;
    }
    if (GameFlag_IsSet(2321) != 0) {
        goto skip_scene;
    }
    Event_CallWithLastActiveObjectId((s32)KareiMachi_StagedGroupScript);
    Actor_SetPosition(20, 16515072, 17825792);
    Actor_SetPosition(27, 18612224, 17301504);
    Actor_SetPosition(28, 18612224, 18350080);
    Actor_SetPosition(29, 19660800, 17301504);
    Actor_SetPosition(30, 19660800, 18350080);
    Actor_SetPosition(32, 20709376, 17301504);
    Actor_SetPosition(31, 20709376, 18350080);
    Actor_SetPosition(33, 21757952, 17301504);
    Actor_SetPosition(34, 21757952, 18350080);
    Actor_SetPosition(21, 23855104, 17825792);
    Audio_PlayCue(17);
    Ui_SetRenderResultFromObject(20);
    UiText_ShowCenteredMessage((s32)MsgKareiOkListeningLets, 1, 0);
    Audio_PlayCue(9);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Camera_SetSpeed(157286, 19660);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 57344, 20);
    } else {
        Camera_SetSpeed(78643, 9830);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
    }
    Actor_SetSpeed(20, 72089, 36044);
    Actor_SetSpeed(27, 65536, 32768);
    Actor_SetSpeed(28, 65536, 32768);
    Actor_SetSpeed(29, 58982, 29491);
    Actor_SetSpeed(30, 58982, 29491);
    Actor_SetSpeed(32, 52428, 26214);
    Actor_SetSpeed(31, 52428, 26214);
    Actor_SetSpeed(33, 45875, 22937);
    Actor_SetSpeed(34, 45875, 22937);
    Actor_SetSpeed(21, 39321, 19660);
    actionDescriptor = SceneAction_GroupMotion;
    Engine_ActorEnableActionCallback(20, actionDescriptor);
    Engine_ActorEnableActionCallback(27, actionDescriptor);
    Engine_ActorEnableActionCallback(28, actionDescriptor);
    Engine_ActorEnableActionCallback(29, actionDescriptor);
    Engine_ActorEnableActionCallback(30, actionDescriptor);
    Engine_ActorEnableActionCallback(32, actionDescriptor);
    Engine_ActorEnableActionCallback(31, actionDescriptor);
    Engine_ActorEnableActionCallback(33, actionDescriptor);
    Engine_ActorEnableActionCallback(34, actionDescriptor);
    actor21OpeningWait = Actor_Get(21);
    {
        s32 value = 0;
        *(u16 *)(actor21OpeningWait + 100) = value;
    }
    Engine_ActorEnableActionCallback(21, actionDescriptor);
    Camera_MoveTo(12189696, -1, 17825792, 1);
    Object_RefreshSelectorById(20);
    Actor_FaceDirection(20, 0, 0);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)actor21OpeningWait + 100) == 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(27, 2);
    Actor_FaceDirection(27, 20480, 20);
    Event_SetMessage((s32)MsgKareiWhyWeStoppingAtPlace);
    Event_ShowMessageAndWait(27, 0, 10);
    Actor_RunRepeatedMotion(28, 2);
    Actor_FaceDirection(28, 45056, 10);
    Actor_SetAnimation(28, 3);
    Event_ShowMessageAndWait(28, 0, 10);
    Actor_SetAttachedEffect(32, 258);
    Event_Wait(40);
    Event_ShowMessageAndWait(32, 0, 10);
    Actor_ShowEmote(31, 256, 40);
    Actor_FaceDirection(31, 45056, 10);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, 32768, 10);
    Actor_RunRepeatedMotion(31, 2);
    Actor_SetAnimation(31, 4);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, 45056, 0);
    Actor_FaceDirection(32, 20480, 20);
    Actor_SetAnimation(31, 3);
    Actor_SetAnimationAndWait(32, 3);
    Actor_RunRepeatedMotion(20, 2);
    Actor_SetAttachedEffect(20, 258);
    Event_Wait(40);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_RunRepeatedMotion(20, 2);
    actor20BeforeFirstMove = Actor_Get(20);
    {
        s32 flags = actor20BeforeFirstMove[90] & 0xfe;
        idleState = 0;
        actor20BeforeFirstMove[90] = flags;
    }
    Actor_WalkToAndWait(20, 172, 264);
    Event_Wait(1);
    actor20AfterFirstMove = Actor_Get(20);
    actionEnabled = 1;
    {
        s32 flags = actor20AfterFirstMove[90];
        flags |= actionEnabled;
        actor20AfterFirstMove[90] = flags;
    }
    Actor_FaceDirection(27, 32768, 0);
    Actor_FaceDirection(28, 32768, 0);
    Actor_FaceDirection(32, 32768, 0);
    Actor_FaceDirection(31, 32768, 20);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    actor20BeforeSecondMove = Actor_Get(20);
    *(u8 *)((u8 *)(actor20BeforeSecondMove) + 90) &= 0xfe;
    Actor_WalkToAndWait(20, 172, 272);
    Event_Wait(1);
    actor20AfterSecondMove = Actor_Get(20);
    actionEnabled |= actor20AfterSecondMove[90];
    actor20AfterSecondMove[90] = actionEnabled;
    Actor_WalkToAndWait(20, 180, 272);
    Actor_FaceDirection(20, 0, 0);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_ShowEmote(34, 261, 0);
    Actor_RunRepeatedMotion(34, 1);
    Actor_SetAnimationAndWait(34, 3);
    Event_ShowMessageAndWait(34, 0, 10);
    Actor_RunRepeatedMotion(33, 1);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_SetAnimation(33, 4);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_RunRepeatedMotion(21, 2);
    Actor_ShowEmote(21, 258, 0);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_Jump(20, 2, 20);
    Actor_Jump(20, 4, 40);
    Actor_RunRepeatedMotion(20, 2);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_SetSpeed(21, 104857, 52428);
    Actor_WalkToAndWait(21, 265, 282);
    Actor_WalkToAndWait(21, 251, 284);
    Actor_WalkToAndWait(21, 246, 296);
    Actor_FaceDirection(21, 49152, 0);
    SceneState_SetValues27Through34();
    Event_Wait(40);
    Actor_SetSpeed(21, 104857, 52428);
    Actor_WalkToAndWait(21, 228, 296);
    Actor_FaceDirection(21, 49152, 40);
    Actor_WalkToAndWait(21, 212, 296);
    Actor_FaceDirection(21, 49152, 40);
    Actor_WalkToAndWait(21, 192, 296);
    Actor_FaceDirection(21, 49152, 40);
    Actor_StartRepeatedMotion(21, 2);
    Actor_ShowEmote(21, 256, 60);
    Actor_FaceDirection(20, 12288, 0);
    Actor_WalkToAndWait(21, 184, 286);
    Actor_FaceDirection(21, 45056, 10);
    Actor_SetAttachedEffect(21, 258);
    Event_Wait(40);
    Actor_SetAnimationAndWait(21, 4);
    Actor_ShowEmote(20, 257, 40);
    Actor_SetAnimationAndWait(20, 3);
    Actor_RunRepeatedMotion(20, 2);
    Actor_FaceDirection(20, 0, 0);
    Actor_FaceDirection(21, 0, 60);
    Actor_FaceDirection(20, 12288, 0);
    Actor_FaceDirection(21, 45056, 10);
    Actor_SetAnimationAndWait(20, 3);
    Actor_SetAnimationAndWait(21, 3);
    Actor_FaceDirection(21, 0, 0);
    Actor_SetSpeed(20, 104857, 52428);
    Object_SetActionCallbackAndRefreshById(20, (s32)KareiMachi_DanceScriptB);
    Actor_WalkToAndWait(20, 228, 296);
    Actor_FaceDirection(20, 49152, 40);
    Actor_WalkToAndWait(20, 212, 296);
    Actor_FaceDirection(20, 49152, 40);
    Actor_WalkToAndWait(20, 192, 296);
    Actor_FaceDirection(20, 49152, 40);
    Actor_FaceDirection(20, 45056, 0);
    Actor_FaceDirection(21, 12288, 10);
    Actor_SetAttachedEffect(20, 258);
    Event_Wait(60);
    Actor_SetAnimationAndWait(20, 4);
    messageId = (s32)MsgKareiShortOnePerson;
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(20, 0, 40);
    KareiMachi_SpeedUpActors();
    Actor_WalkToAndWait(20, 178, 272);
    Actor_FaceDirection(20, 0, 0);
    Event_Wait(240);
    Actor_Stop(27);
    Task_Wait(1);
    Actor_FaceDirection(27, 32768, 10);
    Actor_ShowEmote(27, 257, 60);
    Event_ShowMessageAndWait(27, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(27);
    Event_Wait(80);
    Actor_Stop(28);
    Task_Wait(1);
    Actor_FaceDirection(28, 53248, 20);
    Actor_RunRepeatedMotion(28, 2);
    Event_ShowMessageAndWait(28, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(28);
    Event_Wait(160);
    Actor_Stop(32);
    Task_Wait(1);
    Actor_FaceDirection(32, 20480, 10);
    Actor_ShowEmote(32, 257, 60);
    Event_ShowMessageAndWait(32, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(32);
    Event_Wait(80);
    Actor_Stop(30);
    Task_Wait(1);
    Actor_FaceDirection(30, 45056, 10);
    Actor_RunRepeatedMotion(30, 1);
    Event_SetMessage(messageId + 6);
    Event_ShowMessageAndWait(30, 0, 10);
    Scheduler_RemoveCallback((s32)KareiMachi_AlternateDance);
    Actor_Stop(20);
    Actor_Stop(21);
    Task_Wait(1);
    actor20PairedWait = Actor_Get(20);
    *(u16 *)(actor20PairedWait + 100) = idleState;
    actor21PairedWait = Actor_Get(21);
    *(u16 *)(actor21PairedWait + 100) = idleState;
    Actor_SetSpeed(20, 52428, 26214);
    Actor_SetSpeed(21, 52428, 26214);
    Engine_ActorEnableActionCallback(20, KareiMachi_DanceScriptC);
    Engine_ActorEnableActionCallback(21, KareiMachi_DanceScriptD);
    Actor_Stop(29);
    Task_Wait(1);
    Actor_FaceDirection(29, 20480, 10);
    messageId += 5;
    Actor_RunRepeatedMotion(29, 2);
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(29, 0, 20);
    SceneEffect_SetSlotVariantAndDescriptor(29);
    SceneEffect_SetSlotVariantAndDescriptor(30);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)Actor_Get(20) + 100) == 0 ||
             *(s16 *)((u8 *)Actor_Get(21) + 100) != 1);
    Engine_ActorEnableActionCallback(20, KareiMachi_DanceScriptE);
    Engine_ActorEnableActionCallback(21, KareiMachi_DanceScriptF);
    Actor_Stop(31);
    Task_Wait(1);
    Actor_FaceDirection(31, 0x5000, 10);
    Actor_RunRepeatedMotion(31, 1);
    Actor_SetAnimationAndWait(31, 4);
    messageId = (s32)MsgKareiDecideEnteringColosso;
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(31, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(31);
    Actor_Stop(34);
    Actor_Stop(33);
    Task_Wait(1);
    Actor_ShowEmote(34, 261, 40);
    Actor_ShowEmote(33, 261, 60);
    Actor_FaceDirection(34, 45056, 10);
    Actor_FaceDirection(33, 20480, 10);
    messageId += 3;
    Actor_SetAnimationAndWait(34, 4);
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(34, 0, 10);
    Actor_RunRepeatedMotion(33, 1);
    Actor_SetAnimation(33, 4);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_ShowEmote(34, 258, 60);
    Actor_ShowEmote(20, 259, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_SetMessage((s32)MsgKareiThatsWeCantWaitAny);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_Stop(27);
    Actor_Stop(28);
    Actor_Stop(29);
    Actor_Stop(30);
    Actor_Stop(32);
    Actor_Stop(31);
    Actor_Stop(33);
    Actor_Stop(34);
    Actor_Stop(20);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_Jump(27, 2, 0);
    Actor_Jump(28, 2, 0);
    Actor_Jump(29, 2, 0);
    Actor_Jump(30, 2, 0);
    Actor_Jump(32, 2, 0);
    Actor_Jump(31, 2, 0);
    Actor_Jump(33, 2, 0);
    Actor_Jump(34, 2, 0);
    Actor_Jump(21, 2, 40);
    Actor_FaceDirection(27, 32768, 0);
    Actor_FaceDirection(28, 32768, 0);
    Actor_FaceDirection(29, 32768, 0);
    Actor_FaceDirection(30, 32768, 0);
    Actor_FaceDirection(32, 32768, 0);
    Actor_FaceDirection(31, 32768, 0);
    Actor_FaceDirection(33, 32768, 0);
    Actor_FaceDirection(34, 32768, 40);
    Actor_Jump(21, 4, 40);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_RunRepeatedMotion(20, 1);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_ShowEmote(27, 258, 40);
    Actor_StartRepeatedMotion(27, 1);
    Event_ShowMessageAndWait(27, 0, 10);
    Actor_ShowEmote(28, 258, 40);
    Event_ShowMessageAndWait(28, 0, 10);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(40);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_FaceDirection(27, 20480, 0);
    Actor_FaceDirection(28, 45056, 4);
    Actor_FaceDirection(29, 20480, 0);
    Actor_FaceDirection(30, 45056, 4);
    Actor_FaceDirection(32, 20480, 0);
    Actor_FaceDirection(31, 45056, 4);
    Actor_FaceDirection(33, 20480, 0);
    Actor_FaceDirection(34, 45056, 4);
    Actor_SetAnimation(27, 3);
    Actor_SetAnimationAndWait(28, 3);
    Actor_SetAnimation(29, 3);
    Actor_SetAnimationAndWait(30, 3);
    Actor_SetAnimation(32, 3);
    Actor_SetAnimationAndWait(31, 3);
    Actor_SetAnimation(33, 3);
    Actor_SetAnimationAndWait(34, 3);
    Actor_Jump(20, 2, 40);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_FaceDirection(27, 32768, 0);
    Actor_FaceDirection(28, 32768, 4);
    Actor_FaceDirection(29, 32768, 0);
    Actor_FaceDirection(30, 32768, 4);
    Actor_FaceDirection(32, 32768, 0);
    Actor_FaceDirection(31, 32768, 4);
    Actor_FaceDirection(33, 32768, 0);
    Actor_FaceDirection(34, 32768, 4);
    Actor_SetSpeed(20, 72089, 36044);
    Actor_SetSpeed(27, 68812, 34406);
    Actor_SetSpeed(28, 68812, 34406);
    Actor_SetSpeed(29, 65536, 32768);
    Actor_SetSpeed(30, 65536, 32768);
    Actor_SetSpeed(32, 0xf333, 0x7999);
    Actor_SetSpeed(31, 0xf333, 0x7999);
    Actor_SetSpeed(33, 58982, 29491);
    Actor_SetSpeed(34, 58982, 29491);
    Actor_SetSpeed(21, 55705, 27852);
    Actor_SetSpritePriority(27, 1);
    Actor_SetSpritePriority(28, 1);
    Actor_SetSpritePriority(29, 1);
    Actor_SetSpritePriority(30, 1);
    Actor_SetSpritePriority(32, 1);
    Actor_SetSpritePriority(31, 1);
    Actor_SetSpritePriority(33, 1);
    Actor_SetSpritePriority(34, 1);
    Actor_SetSpritePriority(20, 1);
    Actor_SetSpritePriority(21, 1);
    Actor_Stop(27);
    Actor_Stop(28);
    Actor_Stop(29);
    Actor_Stop(30);
    Actor_Stop(32);
    Actor_Stop(31);
    Actor_Stop(33);
    Actor_Stop(34);
    Actor_Stop(20);
    Actor_Stop(21);
    Task_Wait(1);
    actionDescriptor = SceneAction_GroupOffsetMotion;
    Engine_ActorEnableActionCallback(20, actionDescriptor);
    Engine_ActorEnableActionCallback(27, actionDescriptor);
    Engine_ActorEnableActionCallback(28, actionDescriptor);
    Engine_ActorEnableActionCallback(29, actionDescriptor);
    Engine_ActorEnableActionCallback(30, actionDescriptor);
    Engine_ActorEnableActionCallback(32, actionDescriptor);
    Engine_ActorEnableActionCallback(31, actionDescriptor);
    Engine_ActorEnableActionCallback(33, actionDescriptor);
    Engine_ActorEnableActionCallback(34, actionDescriptor);
    closingWaitRecord = Actor_Get(21);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Engine_ActorEnableActionCallback(21, actionDescriptor);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)Actor_Get(21) + 100) != 1);
    Event_Wait(80);
    Actor_SetPosition(14, 22282240, 17956864);
    Task_Wait(1);
    Actor_SetSpeed(14, 65536, 32768);
    Actor_WalkToAndWait(14, 224, 274);
    Actor_FaceDirection(14, 0, 40);
    Actor_FaceDirection(14, 32768, 40);
    Actor_FaceDirection(14, 49152, 40);
    Actor_FaceDirection(14, 20480, 40);
    Actor_ShowEmote(14, 257, 60);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(14, 0, 40);
    Actor_FaceDirection(14, 49152, 40);
    Actor_FaceDirection(14, 32768, 40);
    Actor_SetAttachedEffect(14, 258);
    Actor_Jump(14, 4, 40);
    Event_ShowMessageAndWait(14, 0, 20);
    Actor_RunRepeatedMotion(14, 2);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_Jump(14, 4, 40);
    Actor_SetSpeed(14, 78643, 39321);
    closingWaitRecord = Actor_Get(14);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Engine_ActorEnableActionCallback(14, SceneAction_GroupOffsetMotion);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)Actor_Get(14) + 100) != 1);
    Actor_SetPosition(14, 23527424, 20578304);
    actor14FinalFacing = Actor_Get(14);
    {
        s32 value = 53248;
        *(u16 *)(actor14FinalFacing + 6) = value;
    }
    Actor_SetPosition(20, 29818880, 28442624);
    actor20FinalFacing = Actor_Get(20);
    {
        s32 value = 53248;
        *(u16 *)(actor20FinalFacing + 6) = value;
    }
    Actor_SetPosition(21, 30408704, 27262976);
    actor21FinalFacing = Actor_Get(21);
    {
        s32 value = 20480;
        *(u16 *)(actor21FinalFacing + 6) = value;
    }
    Actor_Destroy(27);
    Actor_Destroy(28);
    Actor_Destroy(29);
    Actor_Destroy(30);
    Actor_Destroy(31);
    Actor_Destroy(32);
    Actor_Destroy(33);
    Actor_Destroy(34);
    Audio_PlayCue(17);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 224, 458);
        playerFinalFacing = Actor_Get(ACTOR_PARTY_LEADER);
        {
            s32 value = 49152;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    } else {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 248);
        playerFinalFacing = Actor_Get(ACTOR_PARTY_LEADER);
        {
            s32 value = 16384;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    }
    Audio_PlayCueFromEventWork();
    GameFlag_Set(0x911);
    goto finish;
skip_scene:
    Audio_PlayCue(123);
    Event_RequestExit(*(s16 *)(sceneWorkspace + 364));
    Event_CloseScreen();
    Event_WaitForScreen();
finish:
    Event_End();
}
