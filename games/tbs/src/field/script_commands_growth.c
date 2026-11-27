// Growth: green sparks fly from the caster, and a sprout grows tall. Part of script_commands.c.

#include "script_commands_internal.h"

#define GROWTH_PARTICLE_MAX 24 // particles round the target

// When Growth is cast on a target, particles fly from in front of the caster to points round the
// target, which then takes the sprite priority of the ground under it.
void fdPsynergyGrowthOnTarget(Sint32 unused)
{
    ACTOR *target;

    target = ((FIELD_EFFECT_STATE *)FieldEffectStateWork)->target;
    if (target != NULL)
    {
        fdEffectSpawnBurstParticleField();
        // A sprout grows tall: sprout.tsv's animation 2, pictures 0 to 6.
        fdActorSelectAnimationStub(target, SPROUT_ANIMATION_GROW);
        target->collisionFlags = 0;
        fdActorSetSpriteFlagsStub(target, 0);
        target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        sySynch(10);
        sySoundPlayStub(SE_RECOVERY);
        sySynch(40);
        fdPsynergyFadeFromCastColor();
    }
}

// When Growth is cast with no target, particles fly out from in front of the caster, and then five
// bits of vine (SPRITE_VINE) fall 4 to 10 pixels round the Psynergy's position.
void fdPsynergyGrowth(void)
{
    FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
    VECTOR3             pos;
    Sint32              i;

    fdEffectSpawnBurstParticleField();
    sySoundPlayStub(SE_HEAVY_IMPACT);
    for (i = 0; i < 5; i++)
    {
        ACTOR *particle;
        FIXED  radius;

        pos.x  = effectState->x;
        pos.z  = effectState->z;
        radius = (syRandom() * 6) + toFIXED(4);
        syVectorAddPolarOffset(radius, syRandom(), (FIXED *)&pos);
        pos.y    = effectState->y;
        particle = fdActorSpawn(SPRITE_VINE, pos.x, pos.y, pos.z);
        if (particle != 0)
        {
            fdActorStartScriptStub(particle, GrowthLeafScript);
            particle->motionFlags = ACTOR_FALLS;
        }
        sySynch(((syRandom() * 2) >> 16) + 2);
    }
    sySynch(30);
    fdPsynergyFadeFromCastColor();
}

// Sends GROWTH_PARTICLE_MAX green sparks, one every 2 ticks, from in front of the caster to points
// round the Psynergy's position.
void fdEffectSpawnBurstParticleField(void)
{
    FIELD_EFFECT_STATE *effectState;
    ACTOR              *caster;
    Sint32              pos[3];
    Sint32              particleIndex;

    effectState = FieldEffectStateWork;
    caster      = effectState->caster;
    fdPsynergyFadeToCastColor();

    for (particleIndex = 0; particleIndex < GROWTH_PARTICLE_MAX; particleIndex++)
    {
        ACTOR              *particle;
        ANIMATION_SPR_PART *image;
        ANIMATION_SPR_PART *shadow;
        FIXED               radius;

        if (effectState->angle == FACING_SOUTH)
        {
            SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + toFIXED(10), caster->z.fixed);
        }
        else if (effectState->angle == FACING_NORTH)
        {
            SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + toFIXED(24), caster->z.fixed);
        }
        else
        {
            SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + toFIXED(10), caster->z.fixed);
            syVectorAddPolarOffset(toFIXED(10), effectState->angle, pos);
        }

        // The shadow part takes the image's colour mode, shape, size, tile and palette, so the
        // shadow (turned on by SPRITE_FLAG_SHADOW below) is drawn with the particle's picture. BUG:
        // the copy comes before the NULL test, so a failed spawn (a full ActorsWork) is read
        // through.
        particle          = fdActorSpawn(SPRITE_PARTICLE, pos[0], pos[1], pos[2]);
        image             = &particle->sprite->image;
        shadow            = &particle->sprite->shadow;
        shadow->fullColor = image->fullColor;
        shadow->shape     = image->shape;
        shadow->size      = image->size;
        shadow->tile      = image->tile;
        shadow->palette   = image->palette;

        if (particle != 0)
        {
            particle->scaleY       = toFIXED(7) / 10;
            particle->scaleX       = toFIXED(7) / 10;
            particle->acceleration = toFIXED(3) / 2;
            particle->speedLimit   = toFIXED(3) / 2;
            particle->motionFlags  = 0;
            fdActorSetSpriteColorModeStub(particle, SPRITE_COLOR_GREEN_TINT);
            fdActorSelectAnimationStub(particle, 7);
            // From the shared script's SC_SYNC: released once it arrives.
            fdActorStartScriptStub(particle, FieldEffectSharedParticleScript + 1);
            fdActorSetSpriteFlagsStub(particle, SPRITE_FLAG_SHADOW);

            // Destination: 4 to 10 pixels from the Psynergy's position.
            SET_VECTOR(pos, effectState->x, effectState->y, effectState->z);
            if (effectState->angle == FACING_NORTH)
                syVectorAddPolarOffset(toFIXED(14), effectState->angle, pos);
            radius = syRandom() * 6 + toFIXED(4);
            syVectorAddPolarOffset(radius, syRandom(), pos);
            fdActorSetMoveTargetStub(particle, pos[0], pos[1], pos[2]);
        }
        sySoundPlayStub(SE_PSYNERGY_SPARKLE);
        sySynch(2);
    }
    sySynch(8);
}

#undef GROWTH_PARTICLE_MAX
