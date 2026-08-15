#include "fixed_point_position.h"

#define CalculateFixedPointPositionDistance Func_02000030

s32 CalculateFixedPointPositionDistance(
    const struct FixedPointPosition *first_position,
    const struct FixedPointPosition *second_position)
{
    s32 delta_x = (first_position->x - second_position->x) >> 16;
    s32 delta_y = (first_position->y - second_position->y) >> 16;
    s32 delta_z = (first_position->z - second_position->z) >> 16;
    s32 distance_x_squared = delta_x * delta_x;
    s32 distance_y_squared = delta_y * delta_y;
    s32 distance_z_squared = delta_z * delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(
        distance_x_squared + distance_y_squared + distance_z_squared);
}
