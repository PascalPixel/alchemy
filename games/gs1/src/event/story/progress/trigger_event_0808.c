#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
extern s32 Func_020041c2(s32, s32);
extern s32 Func_020041f4(void);
extern void Func_02004484(s32, s32);

void StoryProgress_TriggerEvent0808(void)
{
    u8 *workspace = Data_03001ebc;
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_020041c2(*level * 9, 10)) {
        if ((u32)Func_020041f4() < 0x8000) {
            Func_02004484(0x808, 3);
            *(s32 *)(workspace + 424) = 0;
        } else {
            *progress = *level;
        }
    }
}
