#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

void ToretoPalette_CaptureBank(void);
void ToretoHeya_PlayGesture(s32 gesture);
void Local_020017ec(void);
void SceneState_ApplyRectsByFlag844(s32 flag);
void ToretoHeya_RunLandingDustScene(void);
void Main_0808a238(s32 map, s32 entrance);

/* The game state as halfwords: [225] is the entrance, [250] the leader. */
extern s16 Data_02000240_t[][1];
extern s16 *Data_0200add0;
extern u8 Data_02001000[];
extern u8 Data_0000002d[];

/* Spelled through these wrappers, the constants go straight into the
 * argument registers instead of a shared pseudo. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Enter the room: hide the lamps, drift the camera, set the room flags, and
 * finish the fall from the floor above when entered through entrance 20 to
 * 50 (the leader lands on the entrance ten below it). */
s32 ToretoHeya_EnterRoom(void)
{
    struct FieldActor *lamp;
    struct FieldActor *actor;
    u8 **globals;
    u8 *work;
    s32 *camera;
    s32 entrance;
    s32 leader;

    lamp = Engine_ActorGet(8);
    Data_0200add0 = (s16 *)Data_02001000;
    ToretoPalette_CaptureBank();
    lamp->motion_flags = 0;
    lamp->y.fixed = -0xa0000;
    {
        struct FieldActor *other = Engine_ActorGet(9);

        other->motion_flags = 0;
        other->y.fixed = -0xa0000;
    }
    Engine_ActorSetChildValue(9, 15);
    ToretoHeya_PlayGesture(0);
    if (Data_02000240_t[225][0] != 19)
        Call2(Engine_TaskAddCallback, 0x02009245, 0xc80);
    if (Engine_GameFlagIsSet(0x844)) {
        Engine_ActorSetPosition(9, 0, 0);
        Engine_ActorSetPosition(8, 0, 0);
    }
    if (Value1(Engine_GameFlagIsSet, 0x109))
        Local_020017ec();
    globals = (u8 **)0x03001e70;
    work = globals[0];
    camera = (s32 *)(work + 260);
    camera[2] += Iwram_MulQ16(*(s32 *)(work + 236) + 0xa00000, 0x1999);
    camera[3] += Iwram_MulQ16(*(s32 *)(work + 240) + 0x880000, 0x1999);
    camera[4] = 0xe666;
    camera[5] = 0xe666;
    Engine_GameFlagSet(0x201);
    Engine_GameFlagSet(0x20d);
    Engine_GameFlagSet(0x20f);
    Engine_GameFlagSet(0x213);
    Engine_TaskWait(1);
    SceneState_ApplyRectsByFlag844(0);
    *(s32 *)(globals[19] + 0x1c0) = 0x202;
    entrance = Data_02000240_t[225][0];
    leader = *(s32 *)Data_02000240_t[250];
    actor = Engine_ActorGet(leader);
    if (entrance == 50 || entrance == 40 || entrance == 30 || entrance == 20) {
        Engine_EventOpenScreen();
        Engine_ActorSetAnimation(leader, 27);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(leader), 0);
        Call2(Engine_ActorSetAttachedEffect, leader, 0x101);
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        actor->motion_flags = 2;
        actor->y.fixed = 0x640000;
        *(s32 *)((u8 *)actor + 20) = -0xa00000;
        *(s32 *)((u8 *)actor + 72) = 0x8000;
        Engine_AudioPlayCue(204);
        Main_0808a238((s32)Data_0000002d, entrance - 10);
        Engine_EventWait(20);
        actor->unknown_22 = 2;
        Engine_ActorSetSpritePriority(leader, 3);
        Engine_EventWait(2);
        Engine_ActorSetAttachedEffect(leader, 0x100);
        Engine_EventWait(8);
    } else if (entrance == 10) {
        if (!Value1(Engine_GameFlagIsSet, 0x109))
            ToretoHeya_RunLandingDustScene();
    } else if (entrance == 19) {
        SceneState_ApplyRectsByFlag844(1);
    }
    return 0;
}
