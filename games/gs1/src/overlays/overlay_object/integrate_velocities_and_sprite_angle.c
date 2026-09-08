#include "types.h"

void OverlayObject_IntegrateVelocitiesAndSpriteAngle(u8 *rec)
{
    *(s32 *)(rec + 8) += *(s32 *)(rec + 68);
    *(s32 *)(rec + 12) += *(s32 *)(rec + 72);
    *(s32 *)(rec + 16) += *(s32 *)(rec + 76);
    *(s32 *)(rec + 24) += *(s32 *)(rec + 48);
    *(volatile s32 *)(rec + 28) += *(s32 *)(rec + 52);
    {
        u8 *sub = *(u8 *volatile *)(rec + 80);
        *(u16 *)(sub + 30) += *(u16 *)(rec + 100);
    }
}
