#include "TYPES.H"
#include "CALL.H"

s32 Engine_ActorGet();
void Engine_ObjectSetPosition();
void Engine_ActorWaitForMove();
void Engine_MapCopyCellAttributes();

/* Crossbone Isle: speed actor 13 up, switch the blend mode of actors 13 and
 * 14 and raise actor 14 before copying the map cells. */
void TakaraHashira_RaiseActorFourteen(void)
{
    s32 rec8;
    s32 record;
    s32 v5;

    rec8 = Engine_ActorGet(14);
    v5 = 128;
    record = Engine_ActorGet(13);
    *(s32 *)(record + 24) = (v5 << 9);
    record = Engine_ActorGet(13);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Engine_ActorGet(13);
    { s8 *p = *(s8 **)(record + 80); p[9] = (p[9] & -13) | 8; }
    { s8 *p = *(s8 **)(rec8 + 80); p[9] = (p[9] & -13) | 8; }
    *(s32 *)(rec8 + 52) = 0x6666;
    *(s32 *)(rec8 + 48) = 0xcccc;
    Call4(Engine_ObjectSetPosition, rec8, *(s32 *)(rec8 + 8), 0x200000, *(s32 *)(rec8 + 16));
    Engine_ActorWaitForMove(14);
    Call6(Engine_MapCopyCellAttributes, 20, 14, 1, 1, 22, 16);
}
