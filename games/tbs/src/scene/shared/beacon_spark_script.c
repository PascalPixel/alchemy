/// @file
/// The Mercury and Venus Lighthouse aeries: the sparks that circle the beacon's
/// light.

/// A spark is drawn at seven fifths of its size for half a second, then gone.
s32 gBeacon_SparkScript[] = {
    SC_SET(SW_SCALE_Y, 7 * FIXED_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * FIXED_ONE / 5),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(30),
    SC_RELEASE,
};
