#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"

struct Position { s32 x, y, z; };
struct Effect_080b2f4c { u8 filler[0x48]; };

u32 Random16(void);
#define Rand Random16
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);

void Func_080b0840(s32 address);
void Func_080041d8(s32 callback, s32 flags);
void Func_0808a528(struct Effect_080b2f4c *effect, s32 mode, s32 x, s32 z);
void Func_0808a520(struct Effect_080b2f4c *effect, s32 callback);
void Func_0808a518(struct Effect_080b2f4c *effect, s32 value);
void Func_08009248(s32 object, u32 frame_offset);
void Func_080b04c4(void);
void Func_0808a530(struct Effect_080b2f4c *effect);
void Func_08004278(u32 callback);
void Func_08009280(s32 object, s32 arg);
void Func_080b0894(void);
void Func_080b2ffc(void);
void BattleEffect_UpdateRadialMotion(struct Effect_080b2f4c *effect);

extern s8 Data_080b4ab2[];

#define Shop_RunPartyMemberIconBurst Func_080b3050

void Shop_RunPartyMemberIconBurst(s32 member)
{
    u8 *shop;
    s8 saved_kind;
    struct Position position;
    struct Effect_080b2f4c *effect;
    s32 i;

    shop = (u8 *)ADDR_03001F2C;
    saved_kind = (*(s8 **)(shop + 0x380))[5];
    *(u8 *)(shop + 0x3ab) = 0xff;
    (*(s8 **)(shop + 0x380))[5] = 13;
    Audio_PlayCue(Data_080b4ab2[*(s8 *)(shop + 0x3aa)]);
    Func_080b0840(0x00202108);
    Func_08009280(*(s32 *)(shop + 0x114 + member * 4), 0);
    WaitFrames(20);
    Func_080041d8(0x080b2ffd, 0xc80);

    position.x = (s32)*(s16 *)(shop + 0x134 + member * 2) << 16;
    position.z = ((s32)*(s16 *)(shop + 0x144 + member * 2) << 16) + (s32)0xfff40000;

    effect = (struct Effect_080b2f4c *)(shop + 0x3b0);
    for (i = 0; i <= 17; i++) {
        Func_0808a528(effect, 0x11c, position.x, position.z);
        Func_0808a520(effect, 0x080b2f4d);
        Func_0808a518(effect, 7);
        Func_08009248(
            *(s32 *)((u8 *)effect + 0),
            (Rand() * 7) >> 16);
        *(s32 *)((u8 *)effect + 44) = 0xb333;
        *(s32 *)((u8 *)effect + 40) = 0xb333;
        WaitFrames(3);
        if (i == 5) {
            *(u8 *)(shop + 0x3f5) = (u8)member;
        }
        effect = (struct Effect_080b2f4c *)((u8 *)effect + 0x48);
    }

    Func_080b04c4();
    {
        u8 *entry = shop + 0x3f0;
        for (i = 23; i >= 0; i--) {
            if (*(s8 *)(entry + 5) != 0) {
                *(u8 *)(entry + 0) = 2;
            }
            entry += 0x48;
        }
    }

    WaitFrames(20);
    Audio_PlayCue(126);
    *(u8 *)(shop + 0x3ab) = 0xff;
    Func_08009248(*(s32 *)(shop + 0x114 + member * 4), 0);
    WaitFrames(20);

    {
        u8 *flag_entry = shop + 0x3f0;
        struct Effect_080b2f4c *entry2 =
            (struct Effect_080b2f4c *)(shop + 0x3b0);
        for (i = 23; i >= 0; i--) {
            if (*(u8 *)(flag_entry + 0) != 0) {
                Func_0808a530(entry2);
            }
            flag_entry += 0x48;
            entry2 = (struct Effect_080b2f4c *)((u8 *)entry2 + 0x48);
        }
    }

    Func_08004278(0x080b2ffd);
    Func_08009280(*(s32 *)(shop + 0x114 + member * 4), 16);
    Func_080b0894();
    WaitFrames(30);
    (*(s8 **)(shop + 0x380))[5] = saved_kind;
}
