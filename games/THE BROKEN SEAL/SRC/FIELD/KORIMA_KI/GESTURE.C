#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Play a numbered gesture: actor 10 selects actor 8's twelve, any other actor 9's six; then wait twelve frames. */
void KorimaKi_PlayGesture(s32 actor, s32 gesture)
{
    if (actor == 10) {
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
        case 6:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 8);
            break;
        case 8:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 9);
            break;
        case 9:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 10);
            break;
        case 10:
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
        case 7:
        case 11:
            Engine_ActorSetAnimation(8, 6);
            break;
        }
    } else {
        switch (gesture) {
        case 0:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            break;
        case 1:
            Engine_ActorSetAnimation(9, 1);
            break;
        case 2:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 5);
            break;
        case 3:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 4);
            break;
        case 4:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 1);
            break;
        case 5:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 2);
            break;
        }
    }
    Engine_TaskWait(12);
}
