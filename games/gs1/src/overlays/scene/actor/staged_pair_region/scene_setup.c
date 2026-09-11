#include "types.h"

#define FieldScene_ConfigureRegionAtRow15 Func_02000dcc
#define FieldScene_ConfigureRegionAtRow17 Func_02000de8
#define FieldScene_RunInitBracketThenSequence Func_020010c8
#define FieldScene_RunActor8ZeroStep Func_02001378
void Func_02002a44();
void Func_02002a60();
void Func_02002d78();
void Func_02001478();
void Func_02002d88();
void Func_02002050();
void Func_02003028(void);
void Func_02003080(s32 actor, s32 mode);
void Func_0200303c(void);

/* Contiguous unnamed leaf-owner run for resource_3a6. */

/* Configure the 16x15 scene rectangle at row 15. */
void FieldScene_ConfigureRegionAtRow15(void)
{
    Func_02002a44(16, 15, 1, 1, 15, 15);
}

/* Configure the matching 16x15 scene rectangle at row 17. */
void FieldScene_ConfigureRegionAtRow17(void)
{
    Func_02002a60(16, 17, 1, 1, 15, 15);
}

/*
 * resource_3a6 owner at 0x020010c8, 24 bytes: open the scene scheduler,
 * initialize it, close it, then run the preceding tile-27 rain sequence.
 */
void FieldScene_RunInitBracketThenSequence(void)
{
    Func_02002d78();
    Func_02001478();
    Func_02002d88();
    Func_02002050();
}

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */
void FieldScene_RunActor8ZeroStep(void)
{
    Func_02003028();
    Func_02003080(8, 0);
    Func_0200303c();
}
