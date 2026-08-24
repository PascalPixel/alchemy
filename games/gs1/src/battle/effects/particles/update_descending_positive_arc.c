#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000001;

void Object_Destroy(void);

void BattleEffect_UpdateDescendingParticlePositiveArc(void *arg0) {
    u8 *object;
    u8 *source;
    s32 position;
    s32 threshold;
    s32 speed;

    object = arg0;
    threshold = *(s32 *)(object + 0x14) + 0xA0000;
    source = *(u8 **)(object + 0x68);
    if (Data_02000240[237] == (s32)&Value_00000001)
        threshold = *(s32 *)(object + 0x14) + 0x40000;

    position = *(s32 *)(object + 0x0C);
    if (position <= threshold) {
        Object_Destroy();
        return;
    }

    speed = *(s32 *)(object + 0x18) + 0xC00;
    if (speed > 0x10000)
        speed = 0x10000;
    *(s32 *)(object + 0x18) = speed;
    *(s32 *)(object + 0x1C) = speed;
    *(s32 *)(object + 8) = *(s32 *)(source + 8);
    *(s32 *)(object + 0x0C) = position + (s32)0xFFFE0000;
    *(s32 *)(object + 0x10) =
        *(s32 *)(source + 0x10) + ((0x10000 - speed) * 5) + 0x90000;
}
