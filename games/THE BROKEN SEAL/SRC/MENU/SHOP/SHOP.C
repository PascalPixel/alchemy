#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "UI.H"
#include "NODE_CHAIN.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "SHOP.H"

s32 BattleFx_GetResourceIdFar(s32);
void UiIcon_PrepareObjectFar(void *);
s32 Menu_RunConfirmSelectionAtFar(s32, s32, s32);
extern u8 Data_03001f2c[];

/* ui/message/show_and_wait.c */
void UiWork_FinalizePendingCoreFar(void);
void UiText_OpenMessageWindowFar(s32, s32, s32, s32);
extern u8 MsgWeaponShopWelcome[];
extern u8 MsgArmorShopWelcome[];
extern u8 MsgItemShopWelcome[];
extern u8 MsgWarriorShopWelcome[];

extern u16 RomBytes_080b413c[];

/* shop/sel/fill.c */
struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

extern u16 RomBytes_080b4100[];

/* shop/draw/glyphs.c */
extern u8 Shop_GlyphBytes[];
s32 Math_Div(s32, s32);
s32 Math_Mod(s32, s32);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *src);
u8 *RenderOutput_CreateFar(s32 no, u32 flags, s32 window, s32 x, s32 y);
void Shop_CopyGlyphs(s32 arg0, s32 arg1, u32 arg2);
extern u8 gEventWork[];
void BattleFx_ApplyColorToTargetBufferFar(s32, s32);
void BattleFx_StartBufferInterpolationFar(s32);
extern u8 Data_03001ebc[];

struct SpriteAttr {
    unsigned y : 8;
    unsigned affine : 2;
    unsigned blend_mode : 2;
    unsigned mosaic : 1;
    unsigned full_color : 1;
    unsigned shape : 2;
    unsigned x : 9;
    unsigned affine_index : 5;
    unsigned size : 2;
};

struct ShopCursorSprite {
    u8 unknown_00[6];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    struct SpriteAttr oam;
};

struct Half {
    u16 v;
};

void UiMessage_ShowAndWait(s32 arg0)
{
    s32 *state = *(s32 **)((u32)&Data_03001f2c);
    s32 value = BattleFx_GetResourceIdFar(*(u16 *)&state[233]);
    s32 result = arg0;
    s8 mode;

    UiWork_FinalizePendingCoreFar();
    mode = *(s8 *)((u8 *)state + 0x3a9);
    if (mode == 2)
        result += MsgArmorShopWelcome - MsgWeaponShopWelcome;
    if (mode == 0)
        result += MsgItemShopWelcome - MsgWeaponShopWelcome;
    if (*(s8 *)&state[235] != 0)
        result += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
    UiText_OpenMessageWindowFar(result, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
}

/* ui/message/show_and_restore_state.c */
void UiMessage_ShowAndRestoreState(s32 message_id)
{
    s32 variant;
    s32 no;
    s8 mode;
    s8 flag;
    u8 saved;
    void *state;
    u8 **slot;

    state = *(void **)((u32)&Data_03001f2c);
    slot = (u8 **)((u8 *)state + 0x380);
    saved = (*slot)[5];
    no = message_id;
    variant = BattleFx_GetResourceIdFar(FIELD_AT_OFFSET(state, u16 *, 0x3A4));
    mode = FIELD_AT_OFFSET(state, s8 *, 0x3A9);
    if (mode == 2) {
        no += (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome;
    }
    if (mode == 0) {
        no += (s32)MsgItemShopWelcome - (s32)MsgWeaponShopWelcome;
    }
    flag = FIELD_AT_OFFSET(state, u8 *, 0x3AC);
    if (flag != 0) {
        no += (s32)MsgWarriorShopWelcome - (s32)MsgWeaponShopWelcome;
    }
    (*slot)[5] = 0xDU;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(no, 5, 0, (variant << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void **, 0x380), u8 *, 5) = saved;
}

/* ui/message/show_choice.c */
s32 UiMessage_ShowChoice(s32 arg0)
{
    u8 **slot = (u8 **)(*(u8 **)((u32)&Data_03001f2c) + 0x380);
    u8 saved = (*slot)[5];
    UiIcon_PrepareObjectFar(*slot);
#if defined(TBS_EDITION_DE)
    arg0 = Menu_RunConfirmSelectionAtFar(6, 5, arg0);
#else
    arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
#endif
    (*slot)[5] = saved;
    return arg0;
}

/* ui/message/show_choice_variant.c */
s32 UiMessage_ShowChoiceVariant(s32 arg0)
{
    u8 **slot = (u8 **)(*(u8 **)((u32)&Data_03001f2c) + 0x380);
    u8 saved = (*slot)[5];
    UiIcon_PrepareObjectFar(*slot);
    arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
    (*slot)[5] = saved;
    return arg0;
}

struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *state)
{
    struct NodeChainNode *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}

void Shop_FillSelector(s32 count, s32 selector, u8 *base)
{
    u32 shifted = selector << 4;
    u16 *offset;

    selector = shifted + 1;

    if (count > 0) {
        offset = RomBytes_080b4100;
        do {
            struct Record_080b06c0 *record = (struct Record_080b06c0 *)(base + *offset++);
            record->values[0] = selector;
            record->values[4] = selector;
            record->values[8] = selector;
            record->values[12] = selector;
            record->values[16] = selector;
            record->values[20] = selector;
            count--;
        } while (count != 0);
    }
}

/* 4行分の非0バイトを指定配置へ順にコピーする。 */
void Shop_CopyGlyphs(s32 arg0, s32 arg1, u32 arg2)
{
    u8 *src = Shop_GlyphBytes + ((u32)arg0 << 5);
    u8 *dst =
        (u8 *)((u32)arg1 + RomBytes_080b413c[arg2] + 2);
    s32 count = 3;

    do {
        if (*src != 0) {
            dst[0] = *src++;
            if (*src != 0) {
                dst[1] = *src++;
                if (*src != 0) {
                    dst[30] = *src++;
                    if (*src != 0) dst[31] = *src++;
                }
            }
        }
        dst += 4;
        count--;
    } while (count >= 0);
}

/* Builds a sprite showing a price of up to five digits, least significant
   digit first, over the blank price tiles. */
u8 *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y)
{
    u8 *buf;
    s32 slot;
    u8 *sprite;

    buf = Runtime_AllocateBlock(14, 0x400);
    sprite = 0;
    Dma_Set(Shop_PriceTiles, buf, 0x84000040, (volatile u32 *)0x040000d4);
    Shop_CopyGlyphs(Math_Mod(value, 10), buf, 0);
    value = Math_Div(value, 10);
    if (value != 0) {
        Shop_CopyGlyphs(Math_Mod(value, 10), buf, 1);
        value = Math_Div(value, 10);
        if (value != 0) {
            Shop_CopyGlyphs(Math_Mod(value, 10), buf, 2);
            value = Math_Div(value, 10);
            if (value != 0) {
                s32 last;

                Shop_CopyGlyphs(Math_Mod(value, 10), buf, 3);
                last = Math_Div(value, 10);
                if (last != 0)
                    Shop_CopyGlyphs(Math_Mod(last, 10), buf, 4);
            }
        }
    }
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 0x100, buf);
        sprite = RenderOutput_CreateFar(slot, 0x80008000, window, x, y);
    }
    Runtime_ReleaseHeapBlock(14);
    return sprite;
}

/* Copies the saved tile block at work + 0xe00 back into the scene's map and
   the work area, then refreshes the scene layer. */
void Shop_RestoreSceneTiles(s32 layer)
{
    u32 *scene = (u32 *)gEventWork;
    u8 *work = (u8 *)scene[5];
    u8 *map = (u8 *)scene[0];
    u8 *saved = work + 0xe00;

    Dma_Set(saved, map + 0x236, 0x84000150, (volatile u32 *)0x040000d4);
    Dma_Set(saved, work + 0x380, 0x840002a0, (volatile u32 *)0x040000d4);
    BattleFx_ApplyColorToTargetBufferFar(layer, 1);
    BattleFx_StartBufferInterpolationFar(16);
}

void Shop_InitEffect(void)
{
    BattleFx_ApplyColorToTargetBufferFar(*(s32 *)((u32)&Data_03001ebc) + 0x236, 1);
    BattleFx_StartBufferInterpolationFar(0x10);
}

/* ShopCursor_Advance: step the cursor sprite one frame of a linear tween
   from its position toward its target over kind frames, refreshing the
   sprite's position and OAM coordinates, and stop when the tween ends.
   FAKEMATCH: the zero that ends the tween is a one-halfword struct, which
   keeps it a pool constant loaded before the second division as in the ROM. */
void ShopCursor_Advance(struct ShopCursor *cursor)
{
    struct ShopCursorSprite *sprite;
    s32 kind;
    s32 step;
    s32 x;
    s32 y;
    struct Half zero;

    if (cursor == NULL)
        return;
    kind = cursor->kind;
    if (kind == 0)
        return;
    sprite = (struct ShopCursorSprite *)cursor->anchor;
    step = (s8)++cursor->active;
    x = cursor->x + (cursor->target_x - (s16)cursor->x) * step / kind;
    sprite->x = x;
    zero.v = 0;
    sprite->oam.x = x;
    y = cursor->y + (cursor->target_y - (s16)cursor->y) * step / kind;
    sprite->y = y;
    sprite->oam.y = y;
    if (step == kind) {
        cursor->kind = zero.v;
        cursor->active = zero.v;
    }
}
