/* Korima's tree: the overlay's first entry stages the tree actors and sets
 * Retreat to return the party to the Korima bridge. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "KORIMAKI.H"

void PaletteScene_AdjustPaletteWindow(s32 step);

s32 Object_ReplaceResourceEntry(struct FieldSprite *sprite, s32 previous);

struct Spark {
    u8 unknown_00[0x64];
    u16 phase;
    u16 angle;
};

/* Stage the tree actors: sizes, sprite flags and priorities, heights and collision. */
s32 KorimaKi_PrepareActors(void)
{
    struct FieldActor *first;
    struct FieldActor *third;
    struct FieldActor *second;
    u8 zero;

    first = Engine_ActorGet(10);
    third = Engine_ActorGet(14);
    second = Engine_ActorGet(11);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(14, 15);
    gEventWork->start_transition = 0x204;
    gGameState.retreat_scene = (s32)&SceneId_KorimaHashi;
    gGameState.retreat_entrance = 4;
    /* FAKEMATCH: the heights are cleared through a u8 local zero, which the
     * compiler loads from the pool. */
    zero = 0;
    if (!Engine_GameFlagIsSet(0x845))
        PaletteScene_AdjustPaletteWindow(3);
    Engine_ActorGet(8)->radius = 6;
    Engine_ActorGet(9)->radius = 6;
    Engine_ActorGet(12)->radius = 6;
    Engine_ActorGet(13)->radius = 6;
    Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(11), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(8), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
    Engine_ActorSetSpritePriority(8, 2);
    Engine_ActorSetSpritePriority(14, 2);
    Engine_ActorSetSpritePriority(9, 2);
    first->motion_flags = zero;
    first->y.fixed = 0x1c0000;
    second->motion_flags = zero;
    second->y.fixed = 0x1c0000;
    third->motion_flags = zero;
    third->y.fixed = 0x1c0000;
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorGet(8)->collision_flags |= 8;
    Engine_ActorGet(9)->collision_flags |= 8;
    Engine_ActorGet(10)->collision_flags |= 8;
    Engine_ActorGet(11)->collision_flags |= 8;
    Engine_ActorGet(14)->collision_flags |= 8;
    return 0;
}

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Engine_ObjectDispatchRelease(frame);
            }
        }
    }
    return 1;
}

void PaletteScene_SpawnEffect(void)
{

    struct PaletteEffect *effect;
    struct EffectSprite *sprite;
    s32 phase;
    s32 effect_flags;
    s32 sprite_flags;
    s32 spawn_x = 0x01460000;
    s32 spawn_y = 0x00200000;
    s32 spawn_z = 0x00c00000;
    s32 target_x = 0x01460000;
    s32 target_z = 0x00f00000;

    phase = gFrameCount & 3;
    if (phase != 0) return;
    if (gKorimaKiSparkSound != 0) Audio_PlayCue(200);
    effect = (struct PaletteEffect *)Engine_ObjectCreate(26, spawn_x, spawn_y, spawn_z);
    if (effect == 0) return;
    sprite = effect->sprite;
    sprite->state = phase;
    effect_flags = 0xfe;
    effect_flags &= effect->flags;
    effect->flags = effect_flags;
    sprite_flags = ~12;
    sprite_flags &= sprite->flags;
    sprite_flags |= 4;
    sprite->flags = sprite_flags;
    effect->progress = 0x1999;
    effect->rate_x = 0x40000;
    effect->rate_y = 0x40000;
    effect->mode = phase;
    Object_SetAnimation(effect, 2);
    Engine_ObjectSetPosition((struct FieldActor *)effect, target_x, 0, target_z);
    Object_SetScript(effect, gKorimaKiEffectScript);
}

/* Steps the shared transition counter, firing at 0 and at 20 and wrapping at
 * 30. */
void PaletteScene_AdvanceTransition(void)
{
    s32 step = gKorimaKiTransitionStep;

    if (step == 0) {
        KorimaPalette_Restore(0);
        ColorBuffer_Interpolate(20);
    } else if (step == 20) {
        KorimaPalette_Restore(1);
        ColorBuffer_Interpolate(8);
    }
    step = gKorimaKiTransitionStep + 1;
    gKorimaKiTransitionStep = step;
    if (step == 30) {
        gKorimaKiTransitionStep = 0;
    }
}

/* Play a numbered gesture: actor 10 selects actor 8's twelve, any other actor 9's six; then wait twelve frames. */
void KorimaKi_PlayGesture(s32 actor, s32 gesture)
{
    if (actor == 10) {
        switch (gesture) {
        case 0:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 3);
            break;
        case 1:
            Engine_ActorSetAnimation(8, 1);
            break;
        case 2:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 5);
            break;
        case 3:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 4);
            break;
        case 4:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 1);
            break;
        case 5:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 2);
            break;
        case 6:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 8);
            break;
        case 8:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 9);
            break;
        case 9:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 10);
            break;
        case 10:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 8);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 8);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 6);
            break;
        case 7:
        case 11:
            Engine_ActorSetAnimation(8, 6);
            break;
        }
    } else {
        switch (gesture) {
        case 0:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            break;
        case 1:
            Engine_ActorSetAnimation(9, 1);
            break;
        case 2:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 5);
            break;
        case 3:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 4);
            break;
        case 4:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 1);
            break;
        case 5:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 2);
            break;
        }
    }
    Engine_TaskWait(12);
}

void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect)
{
    s32 position[3];
    s32 step = effect->step;
    s32 heading;

    if (step <= 119) {
        position[0] = effect->anchor_x;
        position[1] = effect->anchor_y;
        position[2] = effect->anchor_z;
        heading = effect->heading;
        Vector_AddPolarOffset(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Resource_ResetEntry(effect->owner[0x1c]);
        Engine_ObjectDispatchRelease(effect);
    }
}

/* Every ten frames up to forty, ring the centre with six orbiting sparks; the
 * frame counter wraps after 120. */
void KorimaKi_SpawnOrbitSparks(void)
{
    struct FieldActor *spark;
    s32 previous;
    u32 i;

    previous = 0;
    switch (gKorimaKiSparkCount) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        for (i = 0; i <= 5; i++) {
            spark = Engine_ObjectCreate(0x11d, gKorimaKiSparkOrigin[0], gKorimaKiSparkOrigin[1], gKorimaKiSparkOrigin[2]);
            if (spark != 0) {
                previous = Object_ReplaceResourceEntry(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 1;
                Engine_ActorSetSpriteFlags(spark, 0);
                Engine_ObjectSetAnimation(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = (i * 60 << 16) / 360;
                spark->target_x = gKorimaKiSparkOrigin[0];
                spark->target_y = gKorimaKiSparkOrigin[1];
                spark->target_z = gKorimaKiSparkOrigin[2];
                spark->speed = 0x19999;
                spark->update = (void (*)(union FieldObject *))PaletteScene_AdvanceOrbit;
            }
        }
        break;
    }
    if (++gKorimaKiSparkCount > 120) {
        gKorimaKiSparkCount = 0;
    }
}

/* Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names. */
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    s32 slot = PartyInventory_FindOwner(key);

    if (slot != -1) {
        s32 index = Inventory_Find(slot, key);

        if (index != -1) {
            Owner_GetState(slot)->values[index] = value;
        }
    }
}

/* Applies the adjustment to palette RAM, skipping two protected windows. */
void PaletteScene_AdjustPaletteWindow(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next_phase;
    KorimaPalette_SaveFirst();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 second_window;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            second_window = (index + 0xff3f) << 16;
            if (second_window > 0x70000)
                palette[index] = PaletteScene_AdjustColor(palette[index], adjustment);
        }
        next_phase = phase + 0x10000;
        phase = next_phase;
    } while (next_phase <= 0x00df0000);
    KorimaPalette_Capture(); KorimaPalette_SaveSecond(); ColorBuffer_ApplyTarget(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and blue
 * fall. Control jumps over a mask literal inside the span and rejoins before
 * the common return, so the literal belongs to this owner.
 */
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Math_Divide(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Math_Divide(green, adjustment));
    blue = (s16)(blue - Math_Divide(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
