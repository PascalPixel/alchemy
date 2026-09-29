#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* The cave's scene start. Entering the first scene sets flag 0x144 and
   hides actor 11 while flag 0x9a0 is set. In the third, entrance 1 opens
   its passage, flags 0x9a2 and 0x9a5 move the actors their events have
   moved, and actor 12's sprite flags are cleared. */
s32 Scene_Initialize(void)
{
    struct FieldActor *actor;

    if (gGameState.scene == (s32)&SceneId_KaragoruDou1) {
        GameFlag_Set(0x144);
        if (GameFlag_IsSet(0x9a0) != 0) {
            Actor_SetPosition(11, 0, 0);
        }
    }
    if (gGameState.scene == (s32)&SceneId_KaragoruDou3) {
        if (gGameState.entrance == 1) {
            Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
        }
        if (GameFlag_IsSet(0x9a2) != 0) {
            Actor_SetPosition(8, 0x1b80000, 0x1340000);
            Actor_SetAnimation(8, 2);
            Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
        }
        if (GameFlag_IsSet(0x9a5) != 0) {
            Actor_SetPosition(9, 0, 0);
            Actor_SetPosition(10, 0x2b80000, 0x1200000);
            Actor_SetAnimation(10, 2);
        }
        actor = Actor_Get(12);
        Actor_SetSpriteFlags(actor, 0);
    }
    return 0;
}
