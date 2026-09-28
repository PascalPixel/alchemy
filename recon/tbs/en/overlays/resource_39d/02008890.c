/* Draft of resource_39d 0x02008890 (MeasureFixedPointPositionDistance),
 * from games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/PRESENT.C. Remaining
 * difference: it calls the IWRAM square root through its register, which
 * the C spells only with the routine's number: calling IwramSqrt by name
 * compiles to a direct call. The listing keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot)0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}
