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
