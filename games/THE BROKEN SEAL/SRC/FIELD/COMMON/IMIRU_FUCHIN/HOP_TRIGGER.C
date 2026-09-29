#include "IMIRU_FUCHIN.H"

/* Hop the leader across the gap the touched trigger stands for. */
void ImiruFuchin_HopOnTrigger(void)
{
    s32 trigger = gEventWork->touched_trigger;

    if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
        if (trigger == 17) {
            ImiruFuchin_HopBy(0, -32);
        } else {
            ImiruFuchin_HopBy(-32, 0);
        }
    }
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && trigger == 25 && GameFlag_IsSet(0x309)) {
        ImiruFuchin_HopBy(0, 32);
    }
}
