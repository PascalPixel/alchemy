#include "TYPES.H"

void Engine_ActorSetAnimation();
void Object_SetActionById();

/* Pose one of the actors 18 to 26 for its scene role. */
void FuneHeya_PoseSceneActor(s32 id)
{
    switch (id) {
    case 19:
        Engine_ActorSetAnimation(id, 6);
        Object_SetActionById(id, 8);
        break;
    case 18:
    case 20:
        Engine_ActorSetAnimation(id, 5);
        Object_SetActionById(id, 16);
        break;
    case 22:
    case 23:
        Engine_ActorSetAnimation(id, 5);
        Object_SetActionById(id, 20);
        break;
    case 24:
        Engine_ActorSetAnimation(id, 10);
        Object_SetActionById(id, 8);
        break;
    case 21:
    case 25:
        Engine_ActorSetAnimation(id, 5);
        Object_SetActionById(id, 4);
        break;
    case 26:
        Engine_ActorSetAnimation(id, 9);
        Object_SetActionById(id, 4);
        break;
    }
}
