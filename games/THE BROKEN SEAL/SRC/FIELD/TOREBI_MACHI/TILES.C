/* Tolbi town: actor 9's reset and its map tiles. */
#include "MACHI.H"

void FieldScene_ResetActor9AndDrawTiles(void)
{
    struct Actor *actor = Engine_ActorGet(9);
    if (actor != 0) {
        Engine_ActorSetSpriteFlags(actor, 0);
        actor->field23 = 2;
        actor->field55 = 0;
    }
    Engine_ActorSetAnimation(9, 5);
    {
        s32 v5 = 34;
        s32 v6 = 16;
        Engine_MapCopyCellAttributes(36, 16, 1, 1, v5, v6);
    }
    Engine_GameFlagSet(0x201);
}
