#include "TYPES.H"
#include "FIELD_EVENT.H"

struct ShrineWork {
    u8 unknown_0000[0xe5a];
    u16 colors[3];
    u8 unknown_0e60[0x2a00 - 0xe60];
    u8 lamps[4];
};

void Main_08015210(s32 message, s32 a1, s32 a2);
s32 Local_020027d4(void);

/* Shrine room: the party faces the altar, the room fades to blue, three of the four lamps light, and the scene exits by the altar's answer. */
void ShindenHeya_RunAltarScene(void)
{
    struct ShrineWork *work;
    u8 zero;

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 0);
    Engine_ActorSetAnimation(1, 0);
    Engine_ActorSetAnimation(11, 0);
    Engine_ActorSetAnimation(12, 0);
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorSetAnimation(9, 0);
    Engine_ActorSetAnimation(10, 0);
    Engine_ColorBufferApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(180);
    work = *(struct ShrineWork **)0x03001ed0;
    work->colors[0] = 0x7c00;
    work->colors[1] = 0x7c00;
    work->colors[2] = 0x7c00;
    /* FAKEMATCH: the first lamp is cleared through a u8 local zero, which
     * the compiler loads from the pool. */
    zero = 0;
    work->lamps[0] = zero;
    work->lamps[1] = 1;
    work->lamps[2] = 1;
    work->lamps[3] = 1;
    Engine_EventWait(1);
    Main_08015210(0x116d, 1, 0);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    Engine_EventWait(60);
    if (Local_020027d4() == 0) {
        Engine_EventEnd();
        Engine_EventRequestExit(20);
    } else {
        Engine_EventEnd();
        Engine_EventRequestExit(50);
    }
}
