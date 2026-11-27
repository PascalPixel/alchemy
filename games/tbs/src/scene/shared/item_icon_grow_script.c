/* @file */
/* The Sanctum and Sol Sanctum's Elemental Star room: how an Elemental Star's */
/* icon appears. */

/* An item icon's entrance: drawn at half size, it grows to full size over 8 */
/* frames, holds for 16, then swells to one and a half times its size and back
 */
/* twice. */
s32 ItemIcon_GrowScript[] = {
	SC_SET(SW_SCALE_X, FIXED_ONE / 2),
	SC_SET(SW_SCALE_Y, FIXED_ONE / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 4),
	SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 4),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 7 * FIXED_ONE / 8),
	SC_SET(SW_SCALE_Y, 7 * FIXED_ONE / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 15 * FIXED_ONE / 16),
	SC_SET(SW_SCALE_Y, 15 * FIXED_ONE / 16),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_WAIT(16),
	SC_SET(SW_SCALE_X, 11 * FIXED_ONE / 8),
	SC_SET(SW_SCALE_Y, 11 * FIXED_ONE / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 2),
	SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 9 * FIXED_ONE / 8),
	SC_SET(SW_SCALE_Y, 9 * FIXED_ONE / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 11 * FIXED_ONE / 8),
	SC_SET(SW_SCALE_Y, 11 * FIXED_ONE / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 2),
	SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 9 * FIXED_ONE / 8),
	SC_SET(SW_SCALE_Y, 9 * FIXED_ONE / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_WAIT(2),
	SC_END,
};
