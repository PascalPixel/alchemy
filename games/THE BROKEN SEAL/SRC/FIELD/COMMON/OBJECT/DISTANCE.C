/* The whole-pixel distance between two fixed-point positions, through the
 * resident square root. Linked into the field overlays that measure it on
 * its own. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x * delta_x;
    s32 delta_y_squared = delta_y * delta_y;
    s32 delta_z_squared = delta_z * delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}
