#include "TYPES.H"

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

extern u8 *Data_03001ebc;

s32 Render_ProjectPoint(struct EffectVector *position, struct EffectVector *result);

/* Camera: turn a world position into screen coordinates in place. In the
   projected camera mode (3) the point goes through the projection; otherwise
   it is taken relative to the camera's whole-unit x and z, with height folded
   into z. Either way the height is cleared. */
void Camera_WorldToScreen(struct EffectVector *position)
{
    u8 **data = &Data_03001ebc;

    if (*(s16 *)(*data + 0x19e) == 3) {
        struct EffectVector result;

        Render_ProjectPoint(position, &result);
        position->x = result.x << 16;
        position->z = result.y << 16;
        position->y = 0;
    } else {
        u8 *state = *(u8 **)((u8 *)data - 76);
        s32 x = *(s32 *)(state + 228) & 0xffff0000;
        s32 z = *(s32 *)(state + 232) & 0xffff0000;

        position->x -= x;
        position->z -= position->y;
        position->z -= z;
        position->y = 0;
    }
}
