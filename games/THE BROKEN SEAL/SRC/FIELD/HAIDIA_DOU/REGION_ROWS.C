#include "HAIDIA.H"

void FieldScene_ConfigureRegionAtRow15(void)
{
    Map_CopyCells(16, 15, 1, 1, 15, 15);
}

/* Configure the matching 16x15 scene rectangle at row 17. */
void FieldScene_ConfigureRegionAtRow17(void)
{
    Map_CopyCells(16, 17, 1, 1, 15, 15);
}

/*
 * resource_3a6 owner at 0x020010c8, 24 bytes: open the scene scheduler,
 * initialize it, close it, then run the preceding tile-27 rain sequence.
 */
