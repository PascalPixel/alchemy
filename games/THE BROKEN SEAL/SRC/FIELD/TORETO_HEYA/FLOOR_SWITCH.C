#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_080091b8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Main_0808a238(s32 map, s32 entrance);

extern s32 Data_02000240_t[];
extern s16 *Data_0200add0;
extern u8 Data_0000002d[];
extern u8 Data_00002000[];

/* Spelled through these wrappers, the constants go straight into the
 * argument registers instead of a shared pseudo. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* A floor switch: when the leader steps onto a new entrance tile, copy its
 * cell; the first time, just set its flag, otherwise drop the leader through
 * the floor to map 0x2d at the entrance, spinning as it falls. */
void ToretoHeya_HandleFloorSwitch(s32 flag, s32 src_x, s32 src_y, s32 entrance)
{
    s32 dest_x;
    s32 dest_y;
    s32 leader;
    struct FieldActor *actor;
    s32 cnt;

    dest_x = (Data_02000240_t[119] >> 20) + 64;
    dest_y = Data_02000240_t[121] >> 20;
    leader = Data_02000240_t[125];
    actor = Engine_ActorGet(leader);
    if (entrance == *Data_0200add0)
        return;
    *Data_0200add0 = entrance;
    if (Engine_GameFlagIsSet(flag) == 0) {
        Main_080091b8(src_x, src_y, 1, 1, dest_x, dest_y);
        Engine_GameFlagSet(flag);
        return;
    }
    *Data_0200add0 = -1;
    Main_080091b8(src_x, src_y + 1, 1, 1, dest_x, dest_y);
    Engine_AudioPlayCue(206);
    Engine_EventBegin();
    Main_0808a238((s32)Data_0000002d, entrance);
    Engine_ActorSetAnimation(leader, 27);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(leader), 0);
    Call2(Engine_ActorSetAttachedEffect, leader, 0x101);
    Engine_EventWait(30);
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    actor->motion_flags = 2;
    *(s32 *)((u8 *)actor + 20) = -0xa00000;
    *(s32 *)((u8 *)actor + 72) = 0x8000;
    Engine_AudioPlayCue(204);
    Engine_EventWait(3);
    actor->unknown_22 = 2;
    Engine_ActorSetSpritePriority(leader, 3);
    for (cnt = 29; cnt >= 0; cnt--) {
        /* FAKEMATCH: the step is a HImode pool constant, which orders it
         * first in the pool and dumps the pool before the epilogue. */
        s32 step = (u16)(u32)Data_00002000;

        actor->facing += step;
        Engine_TaskWait(1);
    }
    if (entrance != 50)
        Engine_GameFlagSet(0x122);
}
