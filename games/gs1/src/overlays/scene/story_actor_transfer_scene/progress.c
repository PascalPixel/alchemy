/*
 * Story-progress triggers -- resource_371.  Each fires one event once the
 * progress word has reached nine tenths of the level word; each seventy-two
 * byte owner includes its three pool words.
 */
#include "types.h"

#define StoryProgress_TriggerEvent0808 Func_02000030
#define StoryProgress_TriggerEvent0809 Func_0200008c
#define StoryProgress_TriggerEvent080A Func_020000d4
extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_020041c2(s32, s32);
s32 Func_020041f4(void);
void Func_02004484(s32, s32);
s32 Func_0200421c();
void Func_020044d2();
s32 Func_02004264();
void Func_0200451a();

/*
 * The progress word is Data_02000240[284] read as a whole word and the level
 * word is the workspace at +428; both offsets are built in one register, so
 * the order in which the locals are declared is what reproduces the
 * reference.  Each Func_ name labels the call word of one call site rather
 * than a runtime address, so the same service appears under several names,
 * and the declarations stay old-style because the interfaces are unknown and
 * argument counts differ between call sites.
 */
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

void StoryProgress_TriggerEvent0809(void)
{
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_0200421c(*level * 9, 10)) {
        Func_020044d2(0x809, 42);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_02004264(*level * 9, 10)) {
        Func_0200451a(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}
