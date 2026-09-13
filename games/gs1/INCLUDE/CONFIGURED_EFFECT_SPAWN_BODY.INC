    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;

    party = GetPartyEffect(0);
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = SpawnEffect(options->kind, x, y, z);
    } else {
        effect = SpawnEffect(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;
    SetEffectVariant(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    SetEffectDescriptor(effect, EffectDescriptorTable[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;
    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        SetEffectMode(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct EffectDescriptor *descriptor =
            EffectDescriptorTable[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = ScaleEffectDeltaFromAccumulated(
                first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = ScaleEffectDeltaFromOrigin(
                first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }
        effect->rate34 = ScaleEffectVerticalDelta(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        SetEffectCallbackMode(effect, 1);
        SetEffectCallbackArgument(effect, options->callback_arg);
    }
    if ((flags & 0x400000) != 0) {
        block->angle = options->angle;
    }
    if ((flags & 0x800000) != 0) {
        *tag = options->step;
    }
    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
