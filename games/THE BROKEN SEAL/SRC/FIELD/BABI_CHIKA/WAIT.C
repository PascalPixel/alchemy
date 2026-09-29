/* Waits and effect motion. */
#include "BABI.H"

s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Actor_Get(ACTOR_PARTY_LEADER);

    if (ref->w12 > (s32)0xffd00000
        && (((Record* (*)())Object_GetById)(8)->w16 >> 20) == 10) {
        record->w8 = ((Record* (*)())Object_GetById)(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = ((Record* (*)())Object_GetById)(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}

void SceneActor_WaitValueBelowLimit(struct Track02001038 *track)
{
    s32 cnt = 60;
    s32 limit;

    for (;;) {
        if (cnt != 0) {
            s32 value;

            Task_Wait(1);
            value = track->value;
            limit = track->limit;
            cnt--;
            if (value <= limit) {
                break;
            }
            continue;
        }
        limit = track->limit;
        break;
    }
    track->state = 0;
    track->value = limit;
    track->mark = (s32) 0x80000000;
}

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Effect_AdvanceMotion(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}
