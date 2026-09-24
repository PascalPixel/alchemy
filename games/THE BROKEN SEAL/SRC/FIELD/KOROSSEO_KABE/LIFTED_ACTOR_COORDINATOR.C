#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum LiftedActorCoordinatorMessage {
    MSG_FANS_JUST_CALL_WALL = 0x20aa
};


void Func_020034de();
s32 Func_020036f4();
void Func_02003918();
void Func_02003992();
s32 Func_02004310();
void Func_020045d4();
void Func_02004902();
void Func_02005628();
void Func_02005654();
void Func_0200565e();
void Func_02005702();
void *Func_0200571c();
void Func_0200571e();
void Func_0200572a();
void Func_0200573a();
void Func_02005744();
void Func_02005750();
void Func_02005752();
void Func_0200575e();
void Func_0200576a();
void Func_0200576c();
void Func_0200576e();
void Func_0200577a();
void Func_0200577e();
void Func_020057b6();
void Func_020057c2();
void Func_020057d0();
void Func_020057e0_a();
void Func_020057ea();
void Func_02005816();
void Func_02005818();
void Func_0200582a();
void Func_0200583c();
void Func_0200583e();
void Func_02005854();
void Func_02005862();
void Func_02005868();

extern s16 Data_02000240[];

#define SceneTransition_Phase Data_02000240[225]

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* In transition phase 2 only the fast path runs. Otherwise route 0 plays the
 * full presentation, passing actor 0's position on with Y raised by 0x400000,
 * route 1 plays the short revisit, and every such path ends in the common
 * coordinator tail. */
void FieldScene_RunLiftedActorCoordinator(s32 scene)
{
    void *actor;
    s32 path;

    if (SceneTransition_Phase == 2) {
        Func_020034de();
        return;
    }
    Func_02005628();
    path = Func_020036f4(scene, 4);
    if (path == 0) {
        Event_SetMessage(MSG_FANS_JUST_CALL_WALL);
        Call2(Func_0200572a, 196608, 24576);
        Call4(Func_02005744, 35127296, -1, 15728640, 1);
        Func_02005750();
        Func_0200565e(45);
        Call2(Func_02005752, 65536, 8192);
        Call4(Func_0200576c, 35127296, -1, 12582912, 1);
        Func_0200577a();
        Func_0200573a(scene, 0);
        Func_02004310(0, 632, 264);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
        Call3(Func_02005702, 0, 616, 264);
        Value3(Func_0200577e, 0, 49152, 20);
        Func_020057ea();
        Call2(Func_020057b6, 16384, 2048);
        Call4(Func_020057d0, 35127296, -1, 10485760, 1);
        Call3(Func_0200571e, 0, 32768, 16384);
        Func_0200575e(0, 10);
        actor = Func_0200571c(0);
        Func_02005654(actor, *(s32 *)((u8 *)actor + 8),
            *(s32 *)((u8 *)actor + 12) + 4194304,
            *(s32 *)((u8 *)actor + 16));
        Func_0200576a(0);
        Func_0200583e();
        Call4(Func_02005818, -1, -1, -1, 0);
        Event_ShowMessage(scene, 0);
        Call3(Func_0200576e, 0, 98304, 49152);
        Value3(Func_02004902, 0, 488, 248);
        Call3(Func_02005816, 0, 16384, 20);
        Func_020057e0_a(0, 6, 0);
        Call4(Func_02005862, 35127296, -1, 10485760, 1);
        Func_0200582a(scene, 0);
        Func_020045d4(0);
        Func_02005868(0, 0);
        Func_02003918(scene, 4);
    } else if (path == 1) {
        Call1(Func_0200583c, 8361);
        Func_02005854(scene, 0);
    }
    Value3(Func_02003992, path, scene, 4);
    Func_020057c2();
}
