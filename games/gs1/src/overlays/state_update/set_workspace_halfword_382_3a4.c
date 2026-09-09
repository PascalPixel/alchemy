#include "types.h"

/*
 * Write 0x1018 into the halfword at scene workspace + 382. The 24-byte owner
 * includes its two-word literal pool holding 0x03001ebc and 0x00001018, both
 * read by pc-relative loads in the row. The displacement is built as 191 << 1
 * and is a plain displacement, never used as a value. 0x1018 is loaded from
 * the pool rather than built. No arguments and no return value.
 */

extern u8 *Data_03001ebc;   /* The scene workspace pointer. */
extern u8 Value_00001018;   /* The pooled constant 0x1018. */

void SceneState_SetWorkspaceHalfword382To1018(void)
{
    *(u16 *)(Data_03001ebc + (191 << 1)) = (int)&Value_00001018;
}
