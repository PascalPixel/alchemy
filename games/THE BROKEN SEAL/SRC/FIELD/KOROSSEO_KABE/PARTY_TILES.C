#include "TASK.H"

void SceneActor_PlacePartyAtSavedTiles(void)
{
    {
        s32 x = GameFlag_GetByte(896);
        s32 y = GameFlag_GetByte(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_GERALD, x, y);
    }
    {
        s32 x = GameFlag_GetByte(912);
        s32 y = GameFlag_GetByte(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_IVAN, x, y);
    }
    {
        s32 x = GameFlag_GetByte(928);
        s32 y = GameFlag_GetByte(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_MIA, x, y);
    }
}
