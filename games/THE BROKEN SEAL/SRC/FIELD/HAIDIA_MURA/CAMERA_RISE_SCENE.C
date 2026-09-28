#include "TYPES.H"
extern struct MapRenderWork *gMapWork;


s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Battle_WaitMode0();
void ObjectMotion_SetSpeedParameters();
void Engine_MessageShowCentered();
void Engine_MapRedraw();
void Battle_WaitMode0();
s32 Object_GetById();
void Battle_WaitMode0();
void Battle_WaitMode0();
void Battle_WaitMode0();
void Object_SetModeById();
void Battle_WaitMode0();
void Engine_EventEnd();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_EventShowMessageAndWait();
void Engine_ActorWalkToAndWait();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Until flag 0x808 is set, raise the camera 40 steps above an actor while
 * message 0xf4d plays, show message 0xf4f, lower it again and return the
 * camera to its target. */
void HaidiaMura_RunCameraRiseScene(void)
{
    s32 **cam;
    s32 *saved;
    s32 *rec;
    s32 n;
    s32 pos[3];

    if (Value1(Engine_GameFlagIsSet, 0x808) == 0) {
        cam = *(s32 ***)&gMapWork;
        Engine_EventBegin();
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x10000, 0x8000);
        Object_SetModeById(0, 1);
        Battle_WaitMode0(2);
        Call1(Engine_EventSetMessage, 0xf4d);
        Engine_EventShowMessageAndWait(15, 0, 2);
        Engine_EventShowMessageAndWait(16, 0, 2);
        rec = (s32 *)Value1(Object_GetById, 0);
        pos[0] = rec[2];
        pos[1] = rec[3];
        pos[2] = rec[4];
        saved = *cam;
        *cam = pos;
        n = 0;
        do {
            pos[2] += 0x20000;
            Battle_WaitMode0(1);
            n++;
            Engine_MapRedraw();
        } while (n != 40);
        Battle_WaitMode0(60);
        Call2(Engine_MessageShowCentered, 0xf4f, 1);
        n = 0;
        Battle_WaitMode0(6);
        do {
            pos[2] -= 0x20000;
            Battle_WaitMode0(1);
            n++;
            Engine_MapRedraw();
        } while (n != 40);
        *cam = saved;
        Battle_WaitMode0(60);
        Call3(Engine_ActorWalkToAndWait, 0, 70, 0x2e5);
        Engine_EventEnd();
    }
}
