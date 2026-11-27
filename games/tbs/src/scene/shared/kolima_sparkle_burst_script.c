// Kolima and Kolima Forest: a falling sparkle of Tret's curse that strikes a Psynergy field over
// the party.

// A sparkle bursts on a field: it doubles in size and is gone.
Sint32 KolimaSparkleBurstScript[] = {
    SC_SET(SW_SCALE_X, toFIXED(2)),
    SC_SET(SW_SCALE_Y, toFIXED(2)),
    SC_WAIT(10),
    SC_RELEASE,
};
