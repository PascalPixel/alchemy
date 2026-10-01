#include "TOPIC.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

void OverlayObject_SetField54(s32 actor, s32 value);

/* The ride's work, laid out in order just past the overlay's image, with the
 * actor records FOUR_ACTORS.C sets up. */
struct SpringRide TorebiIzumi_Ride = { 0 };

s32 TorebiIzumi_RideSide = 0;

s32 TorebiIzumi_RideUnknown[3] = { 0 };

u8 TorebiIzumi_ActorRecords[4 * 24] = { 0 };

s32 TorebiIzumi_RideFrame = 0;
s32 TorebiIzumi_RideEnded = 0;
s32 TorebiIzumi_RideResult = 0;

/*
 * The 148-byte owner includes its eight-word literal pool: those words lie
 * past the return and are read only by the pc-relative loads.
 * Field names are descriptive only: the 24-byte record stride and the cleared
 * halfwords at +14..+20 are read off the stores alone, and the second heading
 * is 0x0001 rather than a multiple of 0x4000 -- the byte is certain, its
 * meaning is not.
 */
void SceneState_InitFourActorRecordsAndInstallTask(void)
{
    struct SpringRide *ride = &TorebiIzumi_Ride;
    s32 i = 0;
    u8 *xtbl;
    u16 *htbl;
    u8 *rec;
    u8 *ztbl;

    xtbl = (u8 *)TorebiIzumi_ActorTileX;
    rec = TorebiIzumi_ActorRecords;
    htbl = (u16 *)TorebiIzumi_ActorHeadings;
    ztbl = (u8 *)TorebiIzumi_ActorTileZ;

    do {
        *(s32 *)(rec + 0) = (s32)*xtbl << 16;
        *(s32 *)(rec + 8) = (s32)*ztbl << 16;
        *(s32 *)(rec + 4) = 0;
        *(u16 *)(rec + 12) = *htbl;
        *(u16 *)(rec + 14) = 0;
        *(u16 *)(rec + 16) = 0;
        *(u16 *)(rec + 18) = 0;
        *(u16 *)(rec + 20) = 0;

        i++;
        xtbl++;
        ztbl++;
        htbl++;
        rec += 24;
    } while (i != 4);

    ride->x = (s32)0xffe20000;      /* -30.0 in 16.16 */
    ride->y = 0;
    ride->z = 0x640000;             /* 200 << 15, i.e. 100.0 */
    ride->speed = 0;
    ride->lift = 0;
    ride->fall = 0;
    ride->frames = 0;

    /* r0 carries each lookup's result straight into the retag call. */
    Object_SetMode(Object_GetById(20), 2);
    Object_SetMode(Object_GetById(21), 2);

    /* The locals keep the task and its rate built rather than folded. */
    {
        s32 budget = 0xc83;
        void (*task)(void) = FieldScene_RunSecondaryScript;

        Engine_TaskAddCallback(task, budget);
    }
}

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
                Object_SetMode(Object_GetById(16), 3);
                Object_SetMode(Object_GetById(17), 0);
                OverlayObject_SetField54(15, 1);
                OverlayObject_SetField54(14, 1);
                OverlayObject_SetField54(13, 1);
            } else {
                Object_SetMode(Object_GetById(11), 3);
                Object_SetMode(Object_GetById(12), 0);
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
