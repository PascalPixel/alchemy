#include "TYPES.H"
#include "CALL.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
s32 StagedActor_FillGridAttributeRectangle();
s32 Engine_GameFlagIsSet();
void Engine_EventWait();
void Engine_AudioPlayCue();
void Engine_GameFlagSet();
void Engine_EventEnd();

/* Crossbone Isle: block actor 10's cell and, the first time it reaches
 * column 16 (flag 0x204), drop it into the floor with cue 159. */
void TakaraHashira_DropActorTen(void)
{
    u8 *rec7;
    s32 rec8;

    rec7 = Engine_ActorGet(10);
    Engine_EventBegin();
    StagedActor_FillGridAttributeRectangle(2, (*(s32 *)((s32)rec7 + 8) >> 20), (*(s32 *)((s32)rec7 + 16) >> 20), 1, 1, 255);
    if ((*(s32 *)((s32)rec7 + 8) >> 20) == 16) {
        rec8 = Value1(Engine_GameFlagIsSet, 0x204);
        if (rec8 == 0) {
            Engine_EventWait(10);
            Engine_AudioPlayCue(159);
            rec7[85] = rec8;
            *(s32 *)((s32)rec7 + 20) = -0x20000;
            *(s32 *)((s32)rec7 + 12) = -0x20000;
            Engine_GameFlagSet(0x204);
        }
    }
    Engine_EventEnd();
}
