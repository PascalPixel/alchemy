#ifndef ALCHEMY_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_SOL_SANCTUM_LUNA_SOL_ROOMS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum InnerSanctumActor { ACTOR_KRADEN = ACTOR_FIRST_PLACED + 8 };

enum InnerSanctumFlag {
  FLAG_INNER_SANCTUM_ENTERED = 0x80a,
  FLAG_STATUE_LIGHT_1 = 0x80b,
  FLAG_STATUE_LIGHT_2,
  FLAG_STATUE_LIGHT_3,
  FLAG_STATUE_LIGHT_4,
  FLAG_LUNA_PICTURE_CHANGED_TO_SOL = 0x810,
  FLAG_STATUE_TRAP_SPRUNG = 0x813,
  FLAG_ISAAC_SEARCHING_FOR_KRADEN = 0x896
};

extern u16 gSolSanctumLunaSolRooms_Light1Timer;
extern s32 gSolSanctumLunaSolRooms_FlashState;
extern u16 gSolSanctumLunaSolRooms_Light2Timer;
extern u16 gSolSanctumLunaSolRooms_Light3Timer;
extern u16 gSolSanctumLunaSolRooms_Light4Timer;

#endif
