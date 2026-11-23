/// @file
/// The script of a piece of rubble or a cloud of dust thrown out as stone
/// breaks or a pillar crumbles, which Dehkan Plateau, the Izumo ruins, Magma
/// Rock, the Trial Road and Tundaria Tower keep a copy of (Dust_Scatter,
/// Dust_Scatter).
///
/// An includer whose overlay places the script after its pictures defines
/// DUST_SCRIPT_SECTION as LATE_DATA first.

#ifndef DUST_SCRIPT_SECTION
#define DUST_SCRIPT_SECTION
#endif

/// It lasts 33 frames, then stops and is gone.
DUST_SCRIPT_SECTION u32 gDust_ScatterScript[] = {
    SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0),
    SC_WAIT(1),  SC_RELEASE,
};
