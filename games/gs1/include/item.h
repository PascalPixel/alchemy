#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

#include "layout_guard.h"

struct ItemEffect {
    u8 kind;                        /* 0x00 */
    s8 amount;                      /* 0x01 */
    u8 reserved_02[2];
};

LAYOUT_SIZE_GUARD(ItemEffect_Size, struct ItemEffect, 0x04);
LAYOUT_OFFSET_GUARD(ItemEffect_Kind, struct ItemEffect, kind, 0x00);
LAYOUT_OFFSET_GUARD(ItemEffect_Amount, struct ItemEffect, amount, 0x01);

struct ItemDefinition {
    s16 price;                      /* 0x00 */
    u8 type;                        /* 0x02 */
    u8 flags;                       /* 0x03 */
    u16 equip_mask;                 /* 0x04 */
    u16 icon;                       /* 0x06 */
    s16 primary_bonus;              /* 0x08 */
    s8 secondary_bonus;             /* 0x0a */
    u8 secondary_flags;             /* 0x0b */
    u8 use_type;                    /* 0x0c */
    u8 reserved_0d;
    u16 description_message;        /* 0x0e */
    u8 reserved_10[4];
    u8 element;                     /* 0x14 */
    u8 reserved_15[3];
    struct ItemEffect effects[4];   /* 0x18 */
    u16 action_id;                  /* 0x28 */
    u8 reserved_2a[2];
};

LAYOUT_SIZE_GUARD(ItemDefinition_Size, struct ItemDefinition, 0x2c);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_Price,
    struct ItemDefinition,
    price,
    0x00);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_Type,
    struct ItemDefinition,
    type,
    0x02);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_Flags,
    struct ItemDefinition,
    flags,
    0x03);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_EquipMask,
    struct ItemDefinition,
    equip_mask,
    0x04);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_Icon,
    struct ItemDefinition,
    icon,
    0x06);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_PrimaryBonus,
    struct ItemDefinition,
    primary_bonus,
    0x08);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_SecondaryBonus,
    struct ItemDefinition,
    secondary_bonus,
    0x0a);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_SecondaryFlags,
    struct ItemDefinition,
    secondary_flags,
    0x0b);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_UseType,
    struct ItemDefinition,
    use_type,
    0x0c);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_DescriptionMessage,
    struct ItemDefinition,
    description_message,
    0x0e);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_Element,
    struct ItemDefinition,
    element,
    0x14);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_Effects,
    struct ItemDefinition,
    effects,
    0x18);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_ActionId,
    struct ItemDefinition,
    action_id,
    0x28);

struct ItemDefinition *Func_08078414(s32 item);
struct ItemDefinition *Func_08077018(s32 item);
s32 Func_08077218(s32 owner, s32 item);
s32 Func_0807842c(s32 owner, s32 item);
s32 Func_0807845c(s32 owner, s32 item);
s32 Func_08078480(s32 item);
u8 Func_08078a8c(s32 item);
s32 Func_08078aa0(s32 counter, s32 delta);
s32 Func_08078ad0(s32 item, s32 delta);

#define Item_Get                   Func_08077018
#define Item_GetDirect             Func_08078414
#define Item_CanOwnerEquip         Func_08077218
#define Item_CanOwnerEquipDirect   Func_0807842c
#define Item_IsCompatibleWithOwner Func_0807845c
#define Item_GetEquipmentGroup     Func_08078480
#define Item_GetTargetMode         Func_08078a8c
#define ItemCounter_Adjust         Func_08078aa0
#define Item_AdjustCounter         Func_08078ad0

#endif
