/* @file */
/* The Mercury and Venus Lighthouse aeries: the sparks that circle the beacon's
 */
/* light. */

/* A spark is drawn at seven fifths of its size for half a second, then gone. */
s32 Beacon_SparkScript[] = {
	SC_SET(SW_SCALE_Y, 7 * FIXED_ONE / 5),
	SC_SET(SW_SCALE_X, 7 * FIXED_ONE / 5),
	SC_ANIMATION(4), /* the spark sheet's own animation 4 */
	SC_WAIT(30),
	SC_RELEASE,
};
