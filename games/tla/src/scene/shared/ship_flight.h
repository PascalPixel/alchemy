/// @file
/// The ship's looks in flight, which the World Map and the staff credits each
/// set through their own Actor_SetFlightMode.
#ifndef ALCHEMY_SCENE_SHARED_SHIP_FLIGHT_H
#define ALCHEMY_SCENE_SHARED_SHIP_FLIGHT_H

/// Actor_SetFlightMode's mode: the ship plain, flashing as it powers up, or
/// glowing in flight.
enum ShipFlightMode {
  SHIP_LANDED = 0,
  SHIP_POWERING_UP = 1,
  SHIP_IN_FLIGHT = 2
};

#endif
