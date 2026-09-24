#include "TYPES.H"

void Engine_ActorSetAnimation();
void Main_0808a108();

/* Pose one of the actors 18 to 26 for its scene role. */
void FuneHeya_Func020047cc(s32 id)
{
    switch (id) {
    case 19:
        Engine_ActorSetAnimation(id, 6);
        Main_0808a108(id, 8);
        break;
    case 18:
    case 20:
        Engine_ActorSetAnimation(id, 5);
        Main_0808a108(id, 16);
        break;
    case 22:
    case 23:
        Engine_ActorSetAnimation(id, 5);
        Main_0808a108(id, 20);
        break;
    case 24:
        Engine_ActorSetAnimation(id, 10);
        Main_0808a108(id, 8);
        break;
    case 21:
    case 25:
        Engine_ActorSetAnimation(id, 5);
        Main_0808a108(id, 4);
        break;
    case 26:
        Engine_ActorSetAnimation(id, 9);
        Main_0808a108(id, 4);
        break;
    }
}
