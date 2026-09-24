#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Play one of actor 8's numbered gestures, then wait twelve frames. */
void ToretoHeya_PlayGesture(s32 gesture)
{
    switch (gesture) {
    case 0:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        break;
    case 1:
        Engine_ActorSetAnimation(8, 1);
        break;
    case 2:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 5);
        break;
    case 3:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 4);
        break;
    case 4:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 1);
        break;
    case 5:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 2);
        break;
    case 7:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        break;
    case 8:
        Engine_ActorSetAnimation(8, 6);
        break;
    case 9:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 9);
        break;
    case 10:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 10);
        break;
    case 11:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 6);
        break;
    case 12:
        Engine_ActorSetAnimation(8, 6);
        break;
    }
    Engine_TaskWait(12);
}
