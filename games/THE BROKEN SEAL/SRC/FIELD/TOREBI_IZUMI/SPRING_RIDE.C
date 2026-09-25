#include "TYPES.H"
#include "FIELD_EVENT.H"

void OverlayObject_SetField54(s32 actor, s32 value);

struct SpringRide {
    u8 unknown_00[2];
    u16 hold;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_10[4];
    s32 unknown_14;
    u8 unknown_18[8];
    s32 unknown_20;
    u8 unknown_24[8];
    s32 unknown_2c;
    u8 unknown_30[16];
    s32 speed;
    s32 lift;
    s32 fall;
    s32 frames;
};

extern struct SpringRide Data_0200a070;
extern s32 Data_0200a0c0;
extern s32 Data_0200a130;
extern s32 Data_0200a134;
extern s32 Data_0200a138;

/* Launches the spring ride from side 0 or 1: plays the spring cue at frame
 * 50, starts the leader and the ride at frame 16, and waits for the ride to
 * report that it has ended; returns its result. */
s32 TorebiIzumi_RunSpringRide(s32 side)
{
    struct SpringRide *ride;

    ride = &Data_0200a070;
    ride->y = 0;
    ride->unknown_14 = 0;
    ride->unknown_20 = 0;
    ride->unknown_2c = 0;
    Data_0200a0c0 = side;
    Data_0200a134 = 0;
    ride->hold = 0xffff;
    for (Data_0200a130 = 0;; Data_0200a130++) {
        if (Data_0200a130 == 50) {
            Engine_AudioPlayCue(300);
        }
        if (Data_0200a130 == 16) {
            Engine_ActorSetAnimation(gGameState.selected_actor, 29);
            ride->hold = 0;
            ride->speed = 0x14ccc;
            ride->lift = 0x40000;
            ride->fall = -0x20000;
            ride->x = 0x780000;
            ride->y = 0x100000;
            ride->z = 0x980000;
            ride->frames = 300;
            if (Data_0200a0c0 == 1) {
                Engine_ObjectSetAnimation(Engine_ActorGet(16), 3);
                Engine_ObjectSetAnimation(Engine_ActorGet(17), 0);
                OverlayObject_SetField54(15, 1);
                OverlayObject_SetField54(14, 1);
                OverlayObject_SetField54(13, 1);
            } else {
                Engine_ObjectSetAnimation(Engine_ActorGet(11), 3);
                Engine_ObjectSetAnimation(Engine_ActorGet(12), 0);
                OverlayObject_SetField54(10, 1);
                OverlayObject_SetField54(9, 1);
                OverlayObject_SetField54(8, 1);
            }
        }
        Engine_TaskWait(1);
        if (Data_0200a134 == 1) {
            break;
        }
    }
    return Data_0200a138;
}
