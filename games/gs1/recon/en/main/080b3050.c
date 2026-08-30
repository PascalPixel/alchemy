#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"
#include "shop.h"

struct Position { s32 x, y, z; };
struct Effect_080b2f4c { u8 filler[0x48]; };
struct ShopBurstRuntime {
    u8 unknown_000[0x134];
    s16 member_x[8];
    s16 member_z[8];
};

u32 Random16(void);
#define Rand Random16
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);

void Func_080b0840(s32 address);
void Func_080041d8(void (*callback)(void), s32 flags);
void Func_0808a528(struct Effect_080b2f4c *effect, s32 mode, s32 x, s32 z);
void Func_0808a520(
    struct Effect_080b2f4c *effect,
    void (*callback)(struct Effect_080b2f4c *));
void Func_0808a518(struct Effect_080b2f4c *effect, s32 value);
void Func_08009248(s32 object, u32 frame_offset);
void Func_080b04c4(void);
void Func_0808a530(struct Effect_080b2f4c *effect);
void Func_08004278(void (*callback)(void));
void Func_08009280(s32 object, s32 arg);
void Func_080b0894(void);
void Func_080b2ffc(void);
void BattleEffect_UpdateRadialMotion(struct Effect_080b2f4c *effect);

extern s8 Data_080b4ab2[];

#define Shop_RunPartyMemberIconBurst Func_080b3050

void Shop_RunPartyMemberIconBurst(s32 member)
{
    struct ShopRuntime *shop;
    struct ShopBurstRuntime *burst;
    s8 saved_kind;
    struct Position position;
    struct Effect_080b2f4c *effect;
    s32 callback_flags;
    s32 i;

    shop = SHOP_RUNTIME;
    burst = (struct ShopBurstRuntime *)shop;
    saved_kind = shop->cursor.anchor->kind;
    *(u8 *)((u8 *)shop + 0x3ab) = 0xff;
    shop->cursor.anchor->kind = 13;
    Audio_PlayCue(Data_080b4ab2[shop->party_action]);
    Func_080b0840(0x00202108);
    Func_08009280((s32)shop->party_member_icons[member], 0);
    WaitFrames(20);
    callback_flags = 0xc80;
    Func_080041d8(Func_080b2ffc, callback_flags);

    position.x = (s32)burst->member_x[member] << 16;
    position.z = ((s32)burst->member_z[member] << 16) + (s32)0xfff40000;

    i = 0;
    effect = (struct Effect_080b2f4c *)((u8 *)shop + 0x3b0);
    do {
        Func_0808a528(effect, 0x11c, position.x, position.z);
        Func_0808a520(effect, BattleEffect_UpdateRadialMotion);
        Func_0808a518(effect, 7);
        Func_08009248(
            *(s32 *)((u8 *)effect + 0),
            (Rand() * 7) >> 16);
        *(s32 *)((u8 *)effect + 44) = 0xb333;
        *(s32 *)((u8 *)effect + 40) = 0xb333;
        WaitFrames(3);
        if (i == 5) {
            *(u8 *)((u8 *)shop + 0x3ab) = (u8)member;
        }
        i++;
        effect = (struct Effect_080b2f4c *)((u8 *)effect + 0x48);
    } while (i <= 17);

    Func_080b04c4();
    {
        u8 active_mode = 2;
        u8 *entry = (u8 *)shop + 0x3f0;
        for (i = 23; i >= 0; i--) {
            if (*(s8 *)(entry + 5) != 0) {
                *(u8 *)(entry + 0) = active_mode;
            }
            entry += 0x48;
        }
    }

    WaitFrames(20);
    Audio_PlayCue(126);
    *(u8 *)((u8 *)shop + 0x3ab) = 0xff;
    Func_08009248((s32)shop->party_member_icons[member], 0);
    WaitFrames(20);

    {
        u8 *flag_entry = (u8 *)shop + 0x3f5;
        struct Effect_080b2f4c *entry2 =
            (struct Effect_080b2f4c *)((u8 *)shop + 0x3b0);
        for (i = 23; i >= 0; i--) {
            s32 flag = *flag_entry << 24;

            flag_entry += 0x48;
            if (flag != 0) {
                Func_0808a530(entry2);
            }
            entry2 = (struct Effect_080b2f4c *)((u8 *)entry2 + 0x48);
        }
    }

    Func_08004278(Func_080b2ffc);
    Func_08009280((s32)shop->party_member_icons[member], 16);
    Func_080b0894();
    WaitFrames(30);
    shop->cursor.anchor->kind = saved_kind;
}
