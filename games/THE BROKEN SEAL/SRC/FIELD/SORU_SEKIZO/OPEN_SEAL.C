#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 SceneEventRuntime_SelectInitialSceneByFlags(void);
void Local_02000c8c(void);

/* A map-cell copy as MapCopyCellsTo takes it. */
struct CellCopy {
    s32 src_x;
    s32 src_y;
    s32 dest_x;
    s32 dest_y;
    s32 width;
    s32 height;
};

struct SealScene {
    struct CellCopy open;
    struct CellCopy pulse_a;
    struct CellCopy pulse_b;
    struct CellCopy opened;
    s32 flag;
    s32 facing;
    s32 dest_x;
    s32 dest_z;
    struct CellCopy stairs[3];
    s32 actor;
    s32 actor_x;
    s32 actor_z;
};

extern struct SealScene Data_0200a980;

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define COPY_CELLS(c) Call6(Engine_MapCopyCellsTo, (c).src_x, (c).src_y, (c).dest_x, (c).dest_y, (c).width, (c).height)

/* Sol Sanctum seal: unless flag 0x80f is set, pan to the seal and pulse its cells twenty times, open it, then either walk the party onto the revealed stairs or, once the three statue flags are set, run the follow-up scene. */
void SoruSekizo_OpenSeal(void)
{
    struct SealScene *scene;
    s32 result;
    s32 i;

    result = 0;
    Engine_EventBegin();
    if (!Value1(Engine_GameFlagIsSet, 0x80f)) {
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Engine_CameraMoveTo(0x2400000, -1, 0xac0000, 1);
        Engine_CameraWaitForMove();
        Engine_AudioPlayCue(186);
        COPY_CELLS(Data_0200a980.open);
        for (i = 0; i != 20; i++) {
            Engine_AudioPlayCue(246);
            COPY_CELLS(Data_0200a980.pulse_a);
            Engine_EventWait(4);
            Engine_AudioPlayCue(246);
            COPY_CELLS(Data_0200a980.pulse_b);
            Engine_EventWait(4);
        }
        scene = &Data_0200a980;
        COPY_CELLS(scene->opened);
        Engine_GameFlagSet(scene->flag);
        result = SceneEventRuntime_SelectInitialSceneByFlags();
        if (result == -1) {
            if (!Engine_GameFlagIsSet(0x818)) {
                Engine_CameraFollowActor(0, 1);
                Engine_ActorGet(0)->facing = scene->facing;
                Call3(Engine_ActorSetSpeed, 0, 0x20000, 0x20000);
                Engine_ActorGet(0)->unknown_5a &= ~1;
                Engine_ActorJump(0, 4, 0);
                Engine_ActorSetDestination(0, scene->dest_x, scene->dest_z);
                COPY_CELLS(scene->stairs[0]);
                COPY_CELLS(scene->stairs[1]);
                COPY_CELLS(scene->stairs[2]);
                Engine_ActorGet(scene->actor)->unknown_5a &= ~1;
                Engine_ObjectMotionSetPositionAndCommit(scene->actor, scene->actor_x, scene->actor_z);
                Engine_ActorGet(0)->unknown_5a |= 1;
                Engine_GameFlagClear(scene->flag);
            }
        } else if (result == 0 && Engine_GameFlagIsSet(0x818)) {
            if (Engine_GameFlagIsSet(0x80b) && Engine_GameFlagIsSet(0x80d) && Engine_GameFlagIsSet(0x80e)) {
                if (!Value1(Engine_GameFlagIsSet, 0x80f)) {
                    Call1(Engine_GameFlagSet, 0x80f);
                    Local_02000c8c();
                }
            } else if (Engine_GameFlagIsSet(0x812)) {
                Engine_EventRequestExit(5);
                result = 1;
            }
        }
    }
    if (result == 1) {
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
    }
    Engine_EventEnd();
}
