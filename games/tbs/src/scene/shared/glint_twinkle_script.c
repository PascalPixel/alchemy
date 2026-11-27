// Mercury Lighthouse, its entrance and Venus Lighthouse: the glints that twinkle in rising water
// and spray.

// A SPRITE_GLINT's life: it shrinks from full size to a quarter and back, four times over 24 ticks
// each, then is released.
Sint32 GlintTwinkleScript[] = {
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, toFIXED(5) / 8),
    SC_SET(SW_SCALE_X, toFIXED(5) / 8),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, toFIXED(1) / 4),
    SC_SET(SW_SCALE_X, toFIXED(1) / 4),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, toFIXED(5) / 8),
    SC_SET(SW_SCALE_X, toFIXED(5) / 8),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};
