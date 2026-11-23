/// @file
/// Kolima and Kolima Forest: a falling sparkle of Tret's curse that strikes a
/// Psynergy field over the party.

/// A sparkle bursts on a field: it doubles in size and is gone.
s32 gKolima_SparkleBurstScript[] = {
    SC_SET(SW_SCALE_X, 2 * FIXED_ONE),
    SC_SET(SW_SCALE_Y, 2 * FIXED_ONE),
    SC_WAIT(10),
    SC_RELEASE,
};
