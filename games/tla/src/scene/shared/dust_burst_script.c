/* The script of a cloud of dust that bursts from breaking stone (scDustBurst)
   and of a vent's puff, which the scenes with rubble and Taopo Swamp and Aqua
   Rock keep a copy of.

   An includer whose overlay places the script after its pictures defines
   DUST_SCRIPT_SECTION as LATE_DATA first. */

#ifndef DUST_SCRIPT_SECTION
#define DUST_SCRIPT_SECTION
#endif

/* It lasts 44 ticks, then stops and is gone. */
DUST_SCRIPT_SECTION Uint32 DustBurstScript[] = {
	SC_WAIT(44),
	SC_SET(SW_HOOK, 0),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};
