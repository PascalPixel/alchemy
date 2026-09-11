#include "types.h"
#include "scene.h"
#include "abi/audio/music/player/step_volume_and_pitch_toward_targets.h"

extern u8 gOv;
extern u16 gOv2;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;
extern u16 gOv6;
extern u16 gOv7;
extern u8 gOv8[];
extern u8 gOv9[];

void Audio_Run(void)
{
    s32 delta;

    if (gOv != 0) {
        if (gOv == 1) {
            if (gOv8[4] == 0) {
                gOv = 0;
                gOv6 = 0x100;
            }
        } else {
            gOv -= 1;
        }
    }
    if ((s16)gOv6 != (s16)gOv2) {
        delta = (s16)gOv6 - (s16)gOv2;
        if (delta > 0) {
            gOv2 = gOv2 + gOv4;
        } else {
            gOv2 = gOv2 - gOv4;
        }
        if ((((s16)gOv6 - (s16)gOv2) ^ delta) < 0) {
            gOv2 = gOv6;
        }
        Audio_Place(gOv9, 255, gOv2);
    }
    if ((s16)gOv5 != (s16)gOv7) {
        delta = (s16)gOv5 - (s16)gOv7;
        if (delta > 0) {
            gOv7 = gOv7 + gOv3;
        } else {
            gOv7 = gOv7 - gOv3;
        }
        if ((((s16)gOv5 - (s16)gOv7) ^ delta) < 0) {
            gOv7 = gOv5;
        }
        Audio_Apply(gOv9, gOv7);
        Audio_Place2(gOv9, 255, (s16)((s16)gOv7 * 12 - 3072));
    }
    Audio_Run2();
}
