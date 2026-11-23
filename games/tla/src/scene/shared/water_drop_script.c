/// @file
/// The script of a drop of water thrown up where the Apojii Islands' and Aqua
/// Rock's water splashes, which both overlays keep a copy of.

/// A water drop rises a quarter of a pixel a frame for 31 frames, then is
/// released.
u32 gWaterDropScript[] = {
    SC_ADD(SW_Y, toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};
