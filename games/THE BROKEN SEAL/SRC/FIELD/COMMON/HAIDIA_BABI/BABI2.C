#include "RESOURCE.H"
/* Haidia village: the boulder scene. The actors are placed and the
   screen fades in with a blend, the boulder warning is shown, and the
   blend alpha ramps up and back down before the actors react. */
#include "FIELD_EVENT.H"
#include "IO_REG.H"
#include "CALL.H"
#include "HAIDIA_BABI.H"
/* The sickbed visit: actor 8 coughs, asks who told the party about the
 * illness and leaves the bed; the scene ends by setting flags 0x81e and 0x203. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FXBLEND.H"

extern u8 MsgHaidiaDoraHurryBoulder[];
void DisplayBlend_EnableRunScript();
void Object_RefreshSelectorById(s32 actor);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *script);

extern const s32 gHaidiaBabiRampActor8Action[];
extern const s32 gHaidiaBabiRampLeaderAction[];
extern const s32 gHaidiaBabiRampActor8ActionB[];
extern const s32 gHaidiaBabiRampLeaderActionB[];
extern const s32 gHaidiaBabiRampFinalAction[];
extern const s32 gHaidiaBabiRampActor10Action[];
extern const s32 gHaidiaBabiRampActor10ActionB[];

static __inline__ void SetBlendTarget(u32 value)
{
    REG_BLDCNT = value;
}

static __inline__ void SetBlendAlpha(u32 value)
{
    REG_BLDALPHA = value;
}

extern u8 MsgHaidiaCameBack2[];
extern u8 MsgHaidiaDoraWasStruckWithIllness[];
extern u8 MsgHaidiaDoraWouldntLetHimStay[];
extern u8 MsgHaidiaHomeJustToStay[];
extern u8 MsgHaidiaTheThreeTravelersSeemOdd[];
extern u8 MsgHaidiaTheVisitorsCausedTheEruption[];
extern u8 MsgHaidiaYouveGrownSoMuch[];

extern u8 MsgHaidiaCoughCoughWho[];
extern u8 MsgHaidiaWhoToldIll[];
extern struct EventWork *gEventWork;
void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_ActorStartRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorSetDestination();
void Map_SetLayerEntryFlag();
void Map_ClearLayerEntryFlag();
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait();
s32 Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
s32 Engine_GameFlagIsSet();
void Engine_EventShowMessage();
void Engine_TaskWait();
void Engine_ActorJump();
void Engine_EventEnd();

extern u8 MsgHaidiaDontWorryBest[];
extern u8 MsgHaidiaUnnOhhKyle[];
void Engine_MessageShowCentered();
void Object_SetTargetAndCallback();

/* Actor 8's departure, in the overlay's read-only data. */
extern s32 gHaidiaBabiActor8Departure[];

/* A sparkle that follows its anchor while it rises. The anchor pointer is
 * read before the frame counter is stored: the reference hoists
 * `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Sparkle {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Sparkle *anchor;         /* 0x68 */
};

/* The bag's two motion scripts, in the overlay's read-only data. */
extern s32 gHaidiaBabiBagLiftScript[];
extern s32 gHaidiaBabiBagShowScript[];
void ObjectDispatch_WaitForValue16();
void HaidiaBabi_SpawnEffectPair();

/* The overlay's veneer into the resident unsigned remainder. */
u32 __umodsi3();

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

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);
LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);
extern struct PairWork *gEffectWork;

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

extern struct WorldMapVramBlock gVramBlockCache[];
s32 Object_InitializeMode(struct FieldSprite *sprite, s32 animation);

/* The OAM view with attribute 1 ending in the two-bit size field. */
struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

void BattleEffect_CleanupSceneObjects(void);
void OverlayObject_UpdateOnFrameParity();
void OverlayObject_ApplyRandomSlotOnOddFrames();

void SceneEffect_UpdateAnchoredRiseFrame(struct Sparkle *self);
void OverlayObject_UpdateArcFromAnchor(struct Sparkle *obj);

void FieldScene_RunPaletteRampSequence(void)
{
    s32 base;
    struct FieldActor *p1;
    struct FieldSprite *sprite;
    u32 i1;
    volatile u16 *alpha;

    p1 = Object_GetById(10);
    sprite = p1->sprite;
    Engine_EventBegin();
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    Engine_ActorSetPosition(14, 0, 0);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_ActorSetPosition(16, 0, 0);
    Call3(Engine_ActorSetPosition, 8, 28246016, 25624576);
    Engine_ActorSetPosition(10, 30343168, 26476544);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    p1->priority_flags &= 0xfe;
    p1->motion_flags = 0;
    sprite->priority = 1;
    Engine_ActorEnableActionCallback(10, (s32)gHaidiaBabiRampActor10Action);
    {
        struct EventWork *scene = gEventWork;

        scene->start_transition = 513;
    }
    Engine_MapCopyCellsTo(83, 15, 83, 19, 5, 4);
    Engine_MapCopyCellsTo(90, 16, 90, 20, 5, 4);
    Engine_MapCopyCellsTo(77, 23, 82, 23, 5, 7);
    Engine_MapCopyCellsTo(83, 33, 85, 33, 2, 2);
    Engine_MapCopyCellsTo(91, 28, 90, 28, 1, 1);
    Engine_MapCopyCellsTo(91, 28, 88, 30, 1, 1);
    Engine_MapCopyCellsTo(94, 27, 94, 23, 6, 4);
    Engine_MapCopyCellsTo(92, 28, 87, 23, 4, 4);
    Engine_MapCopyCellsTo(65, 53, 88, 24, 2, 2);
    DisplayBlend_EnableRunScript();
    SetBlendTarget(0x3f42);
    /* FAKEMATCH: keep the initial value/port publication boundary. */
    do {
        s32 value = 0x100c;

        SetBlendAlpha(value);
    } while (0);
    BattleFx_StartTwelveFrameBlend();
    (*(struct FieldBlendWork **)((u8 *)&gEventWork + 12))->loud = 1;
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(30);
    Engine_CameraFollowActor(8, 1);
    Call3(Engine_ActorSetSpeed, 8, 98304, 49152);
    Call3(Engine_ActorSetSpeed, 0, 98304, 49152);
    Call3(Engine_ActorSetSpeed, 9, 98304, 49152);
    Engine_ActorEnableActionCallback(0, (s32)gHaidiaBabiRampLeaderAction);
    Engine_ActorEnableActionCallback(8, (s32)gHaidiaBabiRampActor8Action);
    Engine_EventOpenScreen();
    Object_RefreshSelectorById(8);
    Engine_AudioPlayCue(158);
    Call3(Engine_ActorShowEmote, 8, 256, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Call3(Engine_ActorFaceDirection, 8, 16384, 10);
    Call2(Engine_CameraSetSpeed, 262144, 32768);
    Call4(Engine_CameraMoveTo, 27131904, -1, 34734080, 1);
    Call3(Engine_ActorSetPosition, 9, 27131904, 34734080);
    Engine_ActorWalkToAndWait(9, 427, 483);
    Engine_CameraWaitForMove();
    Engine_EventSetMessage((s32)MsgHaidiaDoraHurryBoulder);
    Engine_EventShowMessageAndWait(32777, 0, 10);
    Engine_CameraSetSpeed(98304, 12288);
    Engine_CameraMoveTo(31457280, -1, 29097984, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 8, 32768, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_ActorWalkTo(9, 415, 589);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(8, (s32)gHaidiaBabiRampActor8ActionB);
    Engine_ActorEnableActionCallback(0, (s32)gHaidiaBabiRampLeaderActionB);
    Engine_AudioPlayCue(234);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(10, (s32)gHaidiaBabiRampActor10ActionB);
    alpha = &REG_BLDALPHA;
    i1 = 0;
ramp:
    {
        register s32 start asm("r2") = 0x100e; /* FAKEMATCH: pins the ramp start to r2 */

        asm("" : "+l"(start)); /* FAKEMATCH: reloads the ramp start every step */
        {
            register s32 level asm("r3"); /* FAKEMATCH: pins the level to r3 */

            asm("add %0, %1, %2" : "=l"(level) : "l"(i1), "l"(start)); /* FAKEMATCH: adds the step into a fresh register */
            *alpha = level;
        }
    }
    Engine_TaskWait(1);
    if (++i1 <= 3)
        goto ramp;
    Engine_AudioPlayCue(202);
    Engine_TaskWait(10);
    base = 0x100f;
    {
        u32 cnt;
        register volatile u16 *port asm("r5") = &REG_BLDALPHA; /* FAKEMATCH: pins the port to r5 */

        for (cnt = 0; cnt <= 15; cnt++) {
            *port = base - cnt;
            Engine_TaskWait(1);
        }
    }
    Object_RefreshSelectorById(0);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 8, 49152, 0);
    Call3(Engine_ActorFaceDirection, 0, 49152, 20);
    Call2(Engine_ActorSetAttachedEffect, 8, 258);
    Engine_ActorSetAttachedEffect(0, 258);
    Engine_EventWait(80);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorFaceEachOther(8, 0, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(52428, 6553);
    Engine_CameraFollowActor(8, 1);
    Engine_ActorEnableActionCallback(8, (s32)gHaidiaBabiRampFinalAction);
    Object_SetActionCallbackAndRefreshById(0, (s32)gHaidiaBabiRampFinalAction);
    {
        struct EventWork *scene = gEventWork;

        scene->start_transition = 256;
        scene->transition_frames = 32;
    }
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(21);
}

/* The innkeeper's and the villagers' talk about the house. */
void HaidiaBabi_RunInnkeeperTalk(void)
{
    u32 i;
    s32 record;

    record = (s32)Object_GetById(0);
    if ((u32)(*(u16 *)(record + 6) + -0x2000) > 0x9000) {
        Engine_InnOpen(0, 13);
    } else {
        Engine_EventBegin();
        if (GameFlag_IsSet(0x87a) != 0) {
            Engine_ActorRunRepeatedMotion(13, 2);
            Actor_FaceActor(13, ACTOR_PARTY_LEADER, 10);
            if (GameFlag_IsSet(0x300) == 0) {
                Engine_EventSetMessage((s32)MsgHaidiaCameBack2);
                Event_ShowMessage(13, 0);
                GameFlag_Set(0x300);
            }
            Engine_EventSetMessage((s32)MsgHaidiaHomeJustToStay);
            Event_AskYesNo(13, 0);
            Actor_FaceDirection(13, 0x9000, 10);
        } else {
            if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Engine_EventSetMessage((s32)MsgHaidiaTheVisitorsCausedTheEruption);
            } else {
                Engine_EventSetMessage((s32)MsgHaidiaTheThreeTravelersSeemOdd);
            }
            Event_ShowMessage(13, 0);
        }
        Engine_EventEnd();
    }
}

void SceneDialogue_ShowLine1C13WithActor16Steps(void)
{
    Engine_EventBegin();
    Actor_FaceActor(0x10, ACTOR_PARTY_LEADER, 0xA);
    Engine_EventSetMessage((s32)MsgHaidiaYouveGrownSoMuch);
    Event_ShowMessage(0x10, 0);
    Actor_FaceDirection(0x10, 0xB000, 0xA);
    GameFlag_Set(0x301);
    Engine_EventEnd();
}

void SceneDialogue_RunActorThirteenDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDoraWouldntLetHimStay);
    Event_ShowMessage(0xD, 0);
    GameFlag_Set(0x81C);
    Engine_EventEnd();
}

void SceneDialogue_RunActor16LineAndFlag81c(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDoraWasStruckWithIllness);
    Event_ShowMessage(0x10, 0);
    GameFlag_Set(0x81C);
    Engine_EventEnd();
}

void HaidiaBabi_RunSickbedVisit(void)
{
    u8 *record;
    s32 msg;

    Engine_EventBegin();
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 0, 0x239, 0x189);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 40);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgHaidiaCoughCoughWho);
    Engine_EventShowMessageAndWait(8, 0, 80);
    Call3(Engine_ActorShowEmote, 8, 0x101, 60);
    Engine_ActorStartRepeatedMotion(8, 1);
    Engine_EventShowMessageAndWait(8, 0, 60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(80);
    Call3(Engine_ActorSetSpeed, 8, 0xcccc, 0x6666);
    Engine_ActorSetDestination(8, 0x248, 0x196);
    Map_SetLayerEntryFlag(11);
    Map_ClearLayerEntryFlag(12);
    Engine_ActorSetAnimation(8, 12);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Call3(Engine_ActorShowEmote, 8, 0x108, 40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Call3(Engine_ActorShowEmote, 0, 0x105, 60);
    Engine_ActorSetAnimationAndWait(8, 13);
    Call3(Engine_ActorShowEmote, 8, 0x103, 0);
    Engine_ActorSetAnimation(8, 11);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 12);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x102, 60);
    Engine_ActorSetAnimation(8, 13);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    if (Value1(Engine_GameFlagIsSet, 0x81c) != 0) {
        Call3(Engine_ActorShowEmote, 8, 0x102, 60);
    }
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Call3(Engine_ActorShowEmote, 8, 0x107, 60);
    msg = (s32)MsgHaidiaWhoToldIll;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    if (Engine_GameFlagIsSet(0x81c) != 0) {
        Call3(Engine_ActorShowEmote, 8, 0x102, 60);
    }
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Engine_ActorShowEmote(8, 0x107, 60);
    Engine_EventSetMessage(msg + 3);
    Engine_EventShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 13);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    record = (u8 *)Object_GetById(0);
    {
        /* FAKEMATCH: the facing is parked in a word-sized local before its
         * halfword store, so its constant is a word; the direct store makes
         * a halfword constant whose short pool reach moves the literal pools. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Engine_TaskWait(1);
    ((u8 *)Object_GetById(0))[90] &= 254;
    Call3(Engine_ActorSetDestination, 0, 0x22e, 0x184);
    Call3(Engine_ActorSetSpeed, 8, 0x13333, 0x9999);
    Engine_ActorSetAnimation(8, 14);
    Engine_ActorMoveToAndWait(8, 0x24a, 0x190);
    Engine_EventWait(40);
    Call3(Engine_ActorWalkToAndWait, 8, 0x244, 0x17e);
    Call3(Engine_ActorFaceDirection, 8, 0x8000, 40);
    {
        u8 *record = (u8 *)Object_GetById(0);
        /* FAKEMATCH: a result temporary, not a compound or-assign: the
         * reference merges the byte into the mask's register, which the
         * two-address ORR does only when the result is its own object. */
        u8 merged = (u8)(record[90] | 1);

        record[90] = merged;
    }
    Engine_ActorFaceDirection(8, 0xc000, 8);
    Engine_ActorFaceDirection(8, 0, 8);
    Call3(Engine_ActorFaceDirection, 8, 0x4000, 8);
    Call3(Engine_ActorFaceDirection, 8, 0x8000, 10);
    Engine_ActorJump(8, 4, 20);
    Engine_ActorJump(8, 6, 40);
    Engine_ActorJump(8, 4, 20);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Call3(Engine_ActorSetSpeed, 8, 0x6666, 0x3333);
    Call3(Engine_ActorWalkToAndWait, 8, 0x23c, 0x180);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_GameFlagSet(0x81e);
    Engine_GameFlagSet(0x203);
    Engine_EventEnd();
}

/* Actor 8's two lines, depending on whether he has already left. */

/* When flag 0x203 is set, actor 8 walks off and MsgHaidiaDontWorryBest plays;
 * otherwise it turns, shows MsgHaidiaUnnOhhKyle and then 0x1c7a. */
void HaidiaBabi_RunActorEightMessageScene(void)
{
    s32 record;
    s32 dream;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x203) != 0) {
        Object_SetTargetAndCallback(8, 0x10000, (s32)gHaidiaBabiActor8Departure);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaDontWorryBest);
        Engine_EventShowMessage(8, 0);
    } else {
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(40);
        dream = (s32)MsgHaidiaUnnOhhKyle;
        Engine_EventSetMessage(dream);
        Engine_EventShowMessageAndWait(8, 0, 40);
        Engine_MessageShowCentered((dream + 1), 1);
    }
    Engine_EventEnd();
}

/* The Mythril Bag scene and the sparkles that rise from the bag. */
void ActorPresentation_SetTwoSceneCells(void)
{
    {
        s32 extent = 2;

        Map_CopyCellsTo(22, 85, 25, 85, extent, extent);
    }
    {
        s32 extent = 25;

        Map_CopyCellAttributes(25, 15, 2, 2, extent, extent);
    }
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 rec7;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(18, 0x1e00000, 0xca0000);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(18, 1);
    rec7 = 0;
    actor = (struct FieldActor *)Engine_ObjectCreate(22, 0x1480000, 0x20000, 0xc30000);
    actor->motion_flags = rec7;
    sprite = actor->sprite;
    actor->y.fixed = 0x50000;
    sprite->part_count = rec7;
    sprite->full_color = 0;
    sprite->palette = 0;
    rec7 = Value2(Engine_HeapAllocate, 17, 0x608);
    Engine_ItemLoadIcon(ITEM_MYTHRIL_BAG);
    Engine_VramLoad(sprite->vram_block, 128, rec7 + 0x400);
    Engine_HeapRelease(17);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Actor_SetSpeed(18, 0x10000, 0x8000);
    Actor_WalkToAndWait(18, 0x1e0, 176);
    Actor_WalkToAndWait(18, 0x1a4, 164);
    Actor_WalkToAndWait(18, 0x146, 185);
    Actor_FaceDirection(18, 0x4000, 10);
    Engine_ObjectSetScript(actor, gHaidiaBabiBagLiftScript);
    ObjectDispatch_WaitForValue16(actor);
    Object_SetScript(actor, gHaidiaBabiBagShowScript);
    ObjectDispatch_WaitForValue16(actor);
    Engine_EventWait(20);
    Engine_ObjectDispatchRelease(actor);
    Engine_ActorJump(18, 2, 20);
    Actor_FaceDirection(18, 0, 40);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(22);
}

void SceneEffect_UpdateObjectByFrameParity(u8 *obj)
{
    if ((gFrameCount & 2) != 0) {
        Engine_ObjectSetPartPalettes(obj, 7);
    } else {
        Engine_ObjectSetPartPalettes(obj, 0);
    }
    if ((gFrameCount & 15) == 0) {
        HaidiaBabi_SpawnEffectPair(obj);
    }
}

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    volatile s32 *frames = (volatile s32 *)&gFrameCount;

    if ((*frames & 1) != 0) {
        Engine_ObjectSetPartPalettes(obj, __umodsi3((s32)((u32)*frames >> 1), 6));
    }
    if ((*frames & 15) == 0) {
        HaidiaBabi_SpawnEffectPair(obj);
    }
}

void OverlayObject_ApplyRandomSlotOnOddFrames(s32 obj)
{
    volatile s32 *frames = (volatile s32 *)&gFrameCount;

    if ((*frames & 1) != 0) {
        s32 slot = ((u32)*frames >> 1) % 6;

        Engine_ObjectSetPartPalettes(obj, slot);
    }
}

void SceneEffect_UpdateAnchoredRiseFrame(struct Sparkle *self)
{
    struct Sparkle *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(self);
        return;
    }

    amplitude = Engine_MathSin(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

void OverlayObject_UpdateArcFromAnchor(struct Sparkle *obj)
{
    struct Sparkle *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(obj);
        return;
    }

    amp = Engine_MathSin(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

/* Spawns the linked pair of effect objects above the parent actor and gives the two their update routines and priorities. */
void HaidiaBabi_SpawnEffectPair(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = gEffectWork;
    s32 i;

    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct PairSprite *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                Object_InitializeMode(sprite, 0);
                sprite->flags = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: a plain byte access; the struct field store
                 * leaves a dead QImode zero that takes r3 from the +85
                 * address. */
                *(u8 *)&sprite->unknown_1d |= 1;
                sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = OverlayObject_UpdateArcFromAnchor;
        sp->priority = 2;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = 2;
        p->update = SceneEffect_UpdateAnchoredRiseFrame;
        p->priority_flags = 2;
    }
}

/* The Psynergy steps the scene's events run. */
void SceneState_SetValue140Mode0(void)
{
    Engine_PsynergyBegin(0x8C, 0);
}

void FieldScene_RunSingleStep(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunActor8TwoStep(void)
{
    OverlayObject_UpdateOnFrameParity(Object_GetById(8));
}

void FieldScene_RunStep17(void)
{
    OverlayObject_ApplyRandomSlotOnOddFrames(Object_GetById(17));
}

void FieldScene_RunSixStepSequence17e4(void)
{
    Engine_PsynergyBegin(0x94, 1);
    Engine_PsynergySetTarget(8, 0x11);
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
    BattleEffect_CleanupSceneObjects();
}
