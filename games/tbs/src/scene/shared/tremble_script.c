/* @file */
/* The trembling of Imil's freezing young man and of the Tolbi ship's lookout */
/* once the sea monster has landed beside him. */

/* An actor trembles: squashed narrow and a little tall, then sprung back over
 */
/* three frames, every four frames for as long as the script runs. */
s32 Actor_TrembleScript[] = {
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_ADD(SW_SCALE_X, -5 * FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 16),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 32),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
