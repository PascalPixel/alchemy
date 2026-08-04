#include "layout_guard.h"
#include "types.h"

struct BattleActor_080b6f44 {
    u8 padding00[6];
    u16 heading;
    u8 padding08[0x10];
    s32 scale_x;
    s32 scale_y;
    u8 padding20[0x35];
    u8 animation;
    u8 padding56[3];
    u8 render_mode;
};

struct BattleSpriteConfig_080b6f44 {
    struct BattleActor_080b6f44 *actor;
    u16 sprite_id;
    s16 secondary_id;
    s16 frame;
    u16 palette_mask;
    s32 x;
    s32 z;
    s32 alternate;
    s32 scale;
    u8 padding1c[4];
    s32 motion_x;
    s32 motion_z;
    s16 target_x;
    s16 target_z;
};

struct OwnerState_080b6f44 {
    u8 padding000[0x128];
    u8 sprite_type;
    u8 variant_id;
};

LAYOUT_OFFSET_GUARD(
    BattleActor080b6f44_Heading,
    struct BattleActor_080b6f44,
    heading,
    0x06);
LAYOUT_OFFSET_GUARD(
    BattleActor080b6f44_Scale,
    struct BattleActor_080b6f44,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    BattleActor080b6f44_Animation,
    struct BattleActor_080b6f44,
    animation,
    0x55);
LAYOUT_OFFSET_GUARD(
    BattleActor080b6f44_RenderMode,
    struct BattleActor_080b6f44,
    render_mode,
    0x59);
LAYOUT_OFFSET_GUARD(
    BattleSpriteConfig080b6f44_SpriteId,
    struct BattleSpriteConfig_080b6f44,
    sprite_id,
    0x04);
LAYOUT_OFFSET_GUARD(
    BattleSpriteConfig080b6f44_Position,
    struct BattleSpriteConfig_080b6f44,
    x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    BattleSpriteConfig080b6f44_Alternate,
    struct BattleSpriteConfig_080b6f44,
    alternate,
    0x14);
LAYOUT_OFFSET_GUARD(
    BattleSpriteConfig080b6f44_Scale,
    struct BattleSpriteConfig_080b6f44,
    scale,
    0x18);
LAYOUT_OFFSET_GUARD(
    BattleSpriteConfig080b6f44_Motion,
    struct BattleSpriteConfig_080b6f44,
    motion_x,
    0x20);
LAYOUT_OFFSET_GUARD(
    BattleSpriteConfig080b6f44_Target,
    struct BattleSpriteConfig_080b6f44,
    target_x,
    0x28);
LAYOUT_OFFSET_GUARD(
    OwnerState080b6f44_SpriteType,
    struct OwnerState_080b6f44,
    sprite_type,
    0x128);
LAYOUT_OFFSET_GUARD(
    OwnerState080b6f44_VariantId,
    struct OwnerState_080b6f44,
    variant_id,
    0x129);

struct BattleActor_080b6f44 *Func_080090c8(s32 kind, s32 x, s32 z);
void Func_08009098(struct BattleActor_080b6f44 *actor, const void *resource);
s32 Func_080044d0(s32 x, s32 y);
struct OwnerState_080b6f44 *Func_08077008(s32 owner);
s32 Func_08077078(struct OwnerState_080b6f44 *owner, s32 index);
s16 Func_080b6d30(s32 owner);
s16 Func_080b6eb4(s32 owner);
s32 Func_080c2384(s32 sprite_type);
s32 Func_080c23a0(s32 sprite_type);

static s32 VariantSpriteId_080b6f44(u8 sprite_type) {
    switch (sprite_type) {
    case 1:
        return 0x12d;
    case 2:
        return 0x12e;
    case 3:
        return 0x12f;
    case 5:
        return 0x131;
    default:
        return 0x12c;
    }
}

static s32 SpriteScale_080b6f44(u8 sprite_type) {
    switch (sprite_type) {
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x1e:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x5c:
    case 0x7a:
    case 0x7b:
    case 0x7c:
    case 0x7d:
    case 0x7e:
        return 0x0e666;

    case 0x4e:
    case 0x83:
        return 0x19999;

    case 0x59:
    case 0x92:
        return 0x18ccc;

    case 0x2f:
    case 0x30:
    case 0x6e:
    case 0x82:
    case 0x88:
    case 0x89:
    case 0x90:
        return 0x13333;

    case 0x93:
    case 0x95:
        return 0x1cccc;

    case 0x5e:
    case 0x8a:
    case 0x8d:
    case 0x91:
    case 0x94:
    case 0x96:
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
    case 0x9c:
    case 0x9d:
        return 0x18000;

    case 0x79:
        return 0x1b333;

    case 0x31:
    case 0x80:
    case 0x81:
        return 0x16666;

    case 0x34:
    case 0x55:
    case 0x69:
        return 0x14000;

    case 0x62:
        return 0x14ccc;

    case 0x84:
    case 0x85:
        return 0x10ccc;

    case 0x86:
    case 0x87:
        return 0x11999;

    default:
        return 0x10000;
    }
}

/*
 * Build one battle sprite descriptor and its backing actor. The owner's
 * normal or variant sprite IDs, per-species scale, alternate flag, heading,
 * animation state, and resource are initialized as one coherent operation.
 */
void Func_080b6f44(
    struct BattleSpriteConfig_080b6f44 *config,
    s32 owner,
    s32 x,
    s32 z
) {
    struct BattleActor_080b6f44 *actor =
        Func_080090c8(0xf000, (s32)((u32)x << 16), 0);
    struct OwnerState_080b6f44 *state = Func_08077008(owner);
    s32 sprite_id;
    s32 alternate = 0;
    s16 override_id = Func_080b6d30(owner);

    if (state->variant_id == 0) {
        sprite_id = Func_080c2384(state->sprite_type);
        if (override_id == 0) {
            alternate = Func_080c23a0(state->sprite_type);
        } else {
            sprite_id = override_id;
        }
    } else {
        sprite_id = VariantSpriteId_080b6f44(state->sprite_type);
        if ((u32)owner > 7) {
            alternate = 1;
        }
    }

    config->actor = actor;
    config->sprite_id = (u16)sprite_id;
    config->x = (s32)((u32)x << 16);
    config->z = (s32)((u32)z << 16);
    config->alternate = alternate;
    config->scale = SpriteScale_080b6f44(state->sprite_type);
    config->secondary_id = Func_080b6eb4(owner);
    config->frame = 0;
    config->palette_mask = 0x01fe;
    config->motion_x = 0;
    config->motion_z = 0;
    config->target_x = 0;
    config->target_z = 0;

    if (state->sprite_type <= 1 && Func_08077078(state, 1) == 0x0f) {
        config->sprite_id = (u16)((state->sprite_type == 0 ? 0xf0 : 0xf1) * 2);
        config->secondary_id = 0;
    }

    actor->heading = (u16)(Func_080044d0(z / 8, x) + 0x8000);
    actor->render_mode = 3;
    actor->animation = 2;
    actor->scale_x = state->variant_id == 0 ? 0x14ccc : 0x10000;
    actor->scale_y = actor->scale_x;
    Func_08009098(actor, (const void *)0x080C5938);
}
