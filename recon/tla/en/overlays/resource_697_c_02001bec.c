/* Draft, differ 12 of 52: one register. The native keeps the 1 of the three byte-98 stores in r5 and builds the 0 of the three collision_flags stores in r6 (push {r5, r6, lr}); this form reuses r5 for the 0. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

void Func_02000d70(void);

void Func_02001bec(void)
{
    Map_CopyCells(11, 22, 11, 8, 11, 64);
    Map_CopyCells(79, 22, 11, 8, 79, 64);
    OBJECT_BYTE(Object_GetById(16), 98) = 1;
    OBJECT_BYTE(Object_GetById(17), 98) = 1;
    OBJECT_BYTE(Object_GetById(18), 98) = 1;
    Object_GetById(16)->collision_flags = 0;
    Object_GetById(17)->collision_flags = 0;
    Object_GetById(18)->collision_flags = 0;
    Func_02000d70();
    Func_020006b4(15);
    Func_0200062c(15);
}
