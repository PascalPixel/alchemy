#include "types.h"
#include "menu.h"
#include "render_input.h"
#include "ram_buffer.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "sound.h"
#include "system.h"
#include "dma.h"
#include "resource.h"
#include "mem.h"
#include "io_reg.h"
#include "input.h"
#include "text/msg_ids.h"

struct ShopRuntime {
    u8 unknown_000[0x4dc];
    struct RenderOutput *anchor;
    u8 unknown_4e0[0x1a];
    u16 keeper_resource;
    u8 unknown_4fc[0xa];
    s8 shop_type;
};

struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePendingCoreFar(void);

void Func_0810857c(s32 message_id)
{
    s32 variant;
    s32 no;
    s8 mode;
    u8 saved;
    struct ShopRuntime *state;
    struct RenderOutput **slot;

    /* FAKEMATCH: the one-pass block loads the shop work before the anchor offset is built (differ 2 without it). */
    do {
        state = Ram_HeapSlots->menu_runtime;
    } while (0);
    slot = &state->anchor;
    saved = (*slot)->active;
    no = message_id;
    variant = BattleFx_GetResourceIdFar(state->keeper_resource);
    mode = state->shop_type;
    if (mode == 3)
        no += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
    if (mode == 2)
        no += MsgArmorShopWelcome - MsgWeaponShopWelcome;
    if (mode == 0)
        no += MsgItemShopWelcome - MsgWeaponShopWelcome;
    (*slot)->active = 0xD;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(no, 5, 0, (variant << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    state->anchor->active = saved;
}

s32 Func_08108630(s32 arg0)
{
    struct RenderOutput **slot = &((struct ShopRuntime *)Ram_HeapSlots->menu_runtime)->anchor;
    u8 saved = (*slot)->active;

    UiIcon_PrepareObjectFar(*slot);
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    arg0 = Menu_RunConfirmSelectionAtFar(6, 5, arg0);
#else
    arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
#endif
    (*slot)->active = saved;
    return arg0;
}

s32 Func_08108660(s32 arg0)
{
    struct RenderOutput **slot = &((struct ShopRuntime *)Ram_HeapSlots->menu_runtime)->anchor;
    u8 saved = (*slot)->active;

    UiIcon_PrepareObjectFar(*slot);
    arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
    (*slot)->active = saved;
    return arg0;
}
