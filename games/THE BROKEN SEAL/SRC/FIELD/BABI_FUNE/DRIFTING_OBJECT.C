#include "TYPES.H"

s32 Engine_RandomNext();
void Engine_ObjectSetAnimation();
void Engine_ObjectSetScript();

/* Lemurian ship: while the wait lasts, drift the object randomly sideways
 * and upwards; when it ends and a trigger is pending, start its script. */
void BabiFune_UpdateDriftingObject(u8 *obj)
{
    u16 *wait = (u16 *)(obj + 100);
    s32 left = *wait;

    if (*(s16 *)wait != 0) {
        s32 drift;

        *wait = left - 1;
        drift = Engine_RandomNext();
        drift -= Engine_RandomNext();
        *(s32 *)(obj + 8) += drift;
        *(s32 *)(obj + 12) += 0xcccc;
    } else if (*(s16 *)(obj + 102) != 0) {
        *(s16 *)(obj + 102) = 0;
        Engine_ObjectSetAnimation(obj, 1);
        {
            u16 *p = (u16 *)(obj + 94);
            s32 delay = 20;

            *p = delay;
        }
        Engine_ObjectSetScript(obj, 0x020094c4);
    }
}
