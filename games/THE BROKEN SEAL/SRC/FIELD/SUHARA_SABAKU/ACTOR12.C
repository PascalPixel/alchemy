/* The Suhara desert: actor 12's place and the effect state byte. */
#include "SABAKU.H"

void PlaceActorTwelveWhenFlagClear(void)
{
    if (GameFlag_IsSet(2487) == 0) {
        GameFlag_Set(526);
        PlaceActor(12, 240 << 15, 206 << 18);
        Engine_ActorEnableActionCallback(12, gSuharaSabakuActor12Action);
    }
}

void SceneState_SetStateByte52(void)
{
    u8 *state = *(u8 **)gEffectWork;
    state[52] = 1;
}
