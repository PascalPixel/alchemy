/* Draft: FieldScene_DrawThreeCaptionWindow, resource_3cd, all five
 * international editions. Complete native extent: 80 bytes including pool.
 * 2026-10-04: ordinary source scores 2; only sub/add sp use 4 rather than 36.
 * The native frame addresses only the outgoing fifth argument at sp+0;
 * UiWindow_Create returns an owned window, and all three glyph pointers are
 * globals. No pointer into the other 32 bytes escapes. The former unread
 * buf[32] reservation was removed from credit, not retained as a device.
 * Separate declaration/assignment: still 2. A loop over the three rows: 1882.
 * 2026-10-04: an eight-entry window array gives the 36-byte frame but stores
 * the window and reloads it for each row; three text pointers in a local
 * array give a 16-byte frame with the stores. Only an unread 32-byte local
 * reproduces the frame alone.
 * Japanese has ItemLevel_RunMotionTest at this position and is unaffected.
 */
#include "TYPES.H"
struct UiWindow;
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawStringInWindow(u8 *text, struct UiWindow *window, s32 x, s32 y);
extern u8 gItemLevelGlyphsUpper[], gItemLevelGlyphsLower[], gItemLevelGlyphsMarks[];
void FieldScene_DrawThreeCaptionWindow(void)
{
    struct UiWindow *window = UiWindow_Create(0, 13, 30, 6, 2);
    UiText_DrawStringInWindow(gItemLevelGlyphsUpper, window, 0, 0);
    UiText_DrawStringInWindow(gItemLevelGlyphsLower, window, 0, 8);
    UiText_DrawStringInWindow(gItemLevelGlyphsMarks, window, 0, 16);
}
