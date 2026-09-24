#include "IWRAM_CALL.H"

struct OrbitEffect {
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[20];
    s32 scale;
    u8 unknown_2c[28];
    s32 speed;
    u8 unknown_4c[24];
    u16 timer;
    u16 pause;
    struct OrbitEffect *anchor;
    void (*update)(struct OrbitEffect *);
};

s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
u32 Random16(void);
void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect);

/* Circles the anchor for 121 frames, then hands over to a random wander. */
void BattleFx_CircleAnchor(struct OrbitEffect *effect)
{
    struct OrbitEffect *anchor;
    s32 angle;
    s32 radius;
    s32 dx;
    s32 dz;

    anchor = effect->anchor;
    radius = 0x80000;
    angle = effect->angle;
    dx = Iwram_MulQ16(radius, Trig_Cos(angle));
    dz = Iwram_MulQ16(radius, Trig_Sin(angle));
    effect->x = anchor->x + dx;
    effect->z = anchor->z + dz;
    effect->angle += 0x800;
    if (++effect->timer == 121) {
        effect->update = BattleFx_WanderAroundAnchor;
        effect->timer = 0;
        effect->pause = 0;
        effect->speed = 0x1999;
        effect->scale = 0x30000;
        effect->angle = Random16();
    }
}
