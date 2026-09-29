/* The entrance opens: the scene restarts at its entrance 31. */
#include "ENTRANCE.H"
#include "SCENE_IDS.H"

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 weight);

void MakyuriIriguchi_OpenEntrance(void)
{
    s32 game;

    Event_Begin();
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_Wait(20);
    gEventWork->start_transition = 0x200;
    Party_SetFields1ceAnd1d0((s32)&SceneId_MakyuriIriguchi, 31);
    game = (s32)&gGameState;
    /* FAKEMATCH: the do/while loads the game state's address ahead of the
     * offset for the byte store. */
    do {
        *(u8 *)(game + 0x22b) = 3;
        BattleFx_SetWeightedResult(36, 1);
    } while (0);
    Event_End();
}
