/* differ 2: ☀️ UiMessage_ShowAndWait; the native loads the shop work before building the keeper offset; only ARM9TDMI load scheduling differs */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "dma.h"
#include "resource.h"
#include "render_input.h"
#include "mem.h"
#include "ram_buffer.h"
#include "io_reg.h"

#include "input.h"
#include "text/msg_ids.h"

struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

struct ShopRuntime {
    u8 unknown_000[0x4dc];
    struct RenderOutput *anchor;
    u8 unknown_4e0[0x1a];
    u16 keeper_resource;
    u8 unknown_4fc[0xa];
    s8 shop_type;
};

void WaitFrames(s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePendingCoreFar(void);

void Func_081084f4(s32 message)
{
    struct ShopRuntime *state = Ram_HeapSlots->menu_runtime;
    s32 value = BattleFx_GetResourceIdFar(state->keeper_resource);
    s8 mode;

    UiWork_FinalizePendingCoreFar();
    mode = state->shop_type;
    if (mode == 3)
        message += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
    if (mode == 2)
        message += MsgArmorShopWelcome - MsgWeaponShopWelcome;
    if (mode == 0)
        message += MsgItemShopWelcome - MsgWeaponShopWelcome;
    UiText_OpenMessageWindowFar(message, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
}
