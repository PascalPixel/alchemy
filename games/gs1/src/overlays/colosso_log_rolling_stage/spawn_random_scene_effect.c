#include "colosso_log_rolling_stage.h"

typedef struct StageEffect {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[20];
    s32 vertical_motion;
    u8 reserved_2c[41];
    u8 state;
} StageEffect;

extern u8 Data_0200ce50[];

extern u32 Func_02008568(void);
extern s32 Func_02008584(void);
extern s32 Func_0200858a(void);
extern void Func_020085a6(s32, s32, s32 *);
extern StageEffect *Func_0200863a(s32, s32, s32, s32);
extern void Func_020086b6(StageEffect *, s32);
extern void Func_0200864e(StageEffect *, s32);
extern void Func_02008656(StageEffect *, s32);

void ColossoLogRollingStage_SpawnRandomSceneEffect(StageEffect *source)
{
    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Func_02008568();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Func_02008584();
        radius = Func_0200858a();
        Func_020085a6(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Func_0200863a(285, x, y, z);
        }
        if (effect != 0) {
            effect->state = 0;
            Func_020086b6(effect, 0);
            Func_0200864e(effect, (s32)Data_0200ce50);
            Func_0200864e(effect, 1);
            Func_02008656(effect, 0);
        }
    }
}
