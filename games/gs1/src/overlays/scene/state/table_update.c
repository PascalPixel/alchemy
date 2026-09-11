/*
 * Overlay resource_37c: in-image table getters and the scene entry that posts
 * the setup request and sets the view scale.
 */

#include "types.h"

#define Resource37c_Run Func_02000054
#include "resource_37c.h"
#include "resource_37c_runtime.h"

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *Func_02000030(void)
{
    return RESOURCE37C_PRIMARY_TABLE;   /* image offset 0xc8 */
}

/* Table slot with no data: reads nothing and returns zero. */
s32 Func_02000038(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *Func_0200003c(void)
{
    return RESOURCE37C_SECONDARY_TABLE;   /* image offset 0x110 */
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *Func_02000044(void)
{
    return RESOURCE37C_TERTIARY_TABLE;   /* image offset 0x11c */
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *Func_0200004c(void)
{
    return RESOURCE37C_QUATERNARY_TABLE;   /* image offset 0x134 */
}

s32 Resource37c_Run(void)
{
    /*
     * The three 16.16 scale values must stay separate locals built in the
     * entry block; sharing them turns two of the three into register copies.
     */
    s32 scale_x = 0x10000;
    s32 scale_y = 0x10000;
    s32 scale_z = 0x10000;

    RESOURCE37C_RUNTIME->setup_request_1c0 = 0x204;
    Resource37c_SetSetupFlag(0x144);
    if (Resource37c_TestSetupGate(0x814) != 0) {
        Resource37c_QueueSoundCue(0x8D);
        Resource37c_SetViewScale(scale_x, scale_y, scale_z);
        Resource37c_FinalizeSetup();
    }
    return 0;
}
