#include "TYPES.H"

void Main_08000128();
void Main_080001b8();
void Engine_ObjectDispatchRelease();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

/* Swirl the object around its origin for 80 frames, shrinking it over the
 * first 40, then release it. */
struct Swirl {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 scaleX;
    s32 scaleY;
    u8 pad20[24];
    s32 ox;
    s32 oy;
    s32 oz;
    u8 pad44[12];
    u8 *sprite;
    u8 pad54[16];
    s16 timer;
    s16 angle;
};

void ToretoHeya_UpdateSwirlObject(struct Swirl *obj)
{
    struct Vec pos;
    s32 t;

    t = obj->timer;
    if (t <= 79) {
        pos.x = obj->ox;
        pos.y = obj->oy;
        pos.z = obj->oz;
        {
            s32 a = obj->angle;

            Main_08000128(t << 16, ((t * 3) << 8) + a, &pos);
        }
        obj->x = pos.x;
        obj->y = pos.y;
        obj->z = pos.z;
        if (obj->timer <= 39) {
            obj->scaleX += -0x51e;
            obj->scaleY += -0x51e;
        }
        obj->timer++;
    } else {
        Main_080001b8(obj->sprite[28]);
        Engine_ObjectDispatchRelease(obj);
    }
}
