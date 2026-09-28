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

/* The ride's work, laid out in order just past the overlay's image, with the
 * actor records FOUR_ACTORS.C sets up. */
struct SpringRide TorebiIzumi_Ride = { 0 };
s32 TorebiIzumi_RideSide = 0;
s32 TorebiIzumi_RideUnknown[3] = { 0 };
u8 TorebiIzumi_ActorRecords[4 * 24] = { 0 };
s32 TorebiIzumi_RideFrame = 0;
s32 TorebiIzumi_RideEnded = 0;
s32 TorebiIzumi_RideResult = 0;

/* Launches the spring ride from side 0 or 1: plays the spring cue at frame
 * 50, starts the leader and the ride at frame 16, and waits for the ride to
 * report that it has ended; returns its result. */
s32 TorebiIzumi_RunSpringRide(s32 side)
{
    struct SpringRide *ride;

    ride = &TorebiIzumi_Ride;
    ride->y = 0;
    ride->unknown_14 = 0;
    ride->unknown_20 = 0;
    ride->unknown_2c = 0;
    TorebiIzumi_RideSide = side;
    TorebiIzumi_RideEnded = 0;
    ride->hold = 0xffff;
    for (TorebiIzumi_RideFrame = 0;; TorebiIzumi_RideFrame++) {
        if (TorebiIzumi_RideFrame == 50) {
            Engine_AudioPlayCue(300);
        }
        if (TorebiIzumi_RideFrame == 16) {
            Engine_ActorSetAnimation(gGameState.selected_actor, 29);
            ride->hold = 0;
            ride->speed = 0x14ccc;
            ride->lift = 0x40000;
            ride->fall = -0x20000;
            ride->x = 0x780000;
            ride->y = 0x100000;
            ride->z = 0x980000;
            ride->frames = 300;
            if (TorebiIzumi_RideSide == 1) {
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
        if (TorebiIzumi_RideEnded == 1) {
            break;
        }
    }
    return TorebiIzumi_RideResult;
}
