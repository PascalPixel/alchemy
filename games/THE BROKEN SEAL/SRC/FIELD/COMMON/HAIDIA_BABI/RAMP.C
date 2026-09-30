/* Haidia village: the boulder scene. The actors are placed and the
   screen fades in with a blend, the boulder warning is shown, and the
   blend alpha ramps up and back down before the actors react. */
#include "FIELD_EVENT.H"
#include "IO_REG.H"
#include "CALL.H"
extern u8 MsgHaidiaDoraHurryBoulder[];
void DisplayBlend_EnableRunScript();
void Object_RefreshSelectorById(s32 actor);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *script);

struct RampWork {
    u8 unknown_00[0x1c0];
    s32 blend_config;
    u8 unknown_1c4[4];
    s32 blend_frames;
};

/* The event work pointer and the IWRAM work pointers that follow it. */
struct RampRoots {
    struct RampWork *scene;
    u8 unknown_04[8];
    struct RampStatus *work;
};

struct RampStatus {
    u8 unknown_00[0x1f84];
    u16 enabled;
};

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

void FieldScene_RunPaletteRampSequence(void)
{
    s32 base;
    struct FieldActor *p1;
    struct FieldSprite *sprite;
    u32 i1;
    volatile u16 *alpha;

    p1 = Engine_ActorGet(10);
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
    Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
    p1->priority_flags &= 0xfe;
    p1->motion_flags = 0;
    sprite->priority = 1;
    Engine_ActorEnableActionCallback(10, (s32)gHaidiaBabiRampActor10Action);
    {
        struct RampWork *scene = ((struct RampRoots *)&gEventWork)->scene;

        scene->blend_config = 513;
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
    ((struct RampRoots *)&gEventWork)->work->enabled = 1;
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
        struct RampWork *scene = ((struct RampRoots *)&gEventWork)->scene;

        scene->blend_config = 256;
        scene->blend_frames = 32;
    }
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(21);
}
