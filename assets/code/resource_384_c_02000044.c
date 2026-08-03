#include "resource_384.h"

extern s16 Data_02000240[];
extern u8 Data_0200835c[];
extern u8 Data_020083bc[];

#define DefaultActorLayout     Data_0200835c
#define SpecialStepActorLayout Data_020083bc

/* Scene-state halfword 225 is the current scene step. */
u8 *Func_02000044(void)
{
    if (Data_02000240[225] == RESOURCE384_SPECIAL_STEP) {
        return SpecialStepActorLayout;
    }
    return DefaultActorLayout;
}
