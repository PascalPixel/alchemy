#include "TYPES.H"
#include "CALL.H"

void Engine_ActorFaceDirection();
void Engine_ActorShowEmote();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorJump();
void Object_SetModeById();

extern u8 *gWork;
extern s16 Data_02000240[];

/* Frames actor 13 has idled, in the overlay's own work past its image. */
s32 GomaHashira_Actor13Frames;

/* Drive actor 13's idle routine from a frame counter. */
void GomaHashira_DriveActor13Idle(void)
{
    u8 *work;
    s32 t;

    work = gWork;
    t = ++GomaHashira_Actor13Frames;
    switch (t) {
    case 60:
        Call3(Engine_ActorFaceDirection, 13, 0x2000, 0);
        Engine_ActorShowEmote(13, 2, 0);
        break;
    case 180:
        Engine_ActorStartRepeatedMotion(13, 3);
        break;
    case 240:
    case 270:
        Engine_ActorJump(13, 4, 0);
        break;
    case 480:
        Object_SetModeById(13, 4);
        break;
    }
    /* FAKEMATCH: the 99 goes through the counter variable so it is built with
     * movs instead of a halfword pool constant. */
    if (Data_02000240[282] == 0)
        *(u16 *)(work + 0x182) = t = 99;
}
