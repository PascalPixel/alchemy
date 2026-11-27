/* How far each field Psynergy reaches. */

#include "field/field.h"
#include "game/game.h"

/* clang-format off */
/* One row a field Psynergy, with one more for the things Tremor's shaking */
/* reaches; the last row, ended by -1, is the reach of a kind not listed. */
const FIELD_PSYNERGY_REACH FieldPsynergyReach[] = {
	{FIELD_PSYNERGY_PLY,                           16, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_FROST,                         16, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_LIFT,                          16, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_CARRY,                         16, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_GROWTH,                        16, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_MIND_READ,                     32, 16, DEGtoANG(45)},
	{FIELD_PSYNERGY_MOVE,                          32, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_WHIRLWIND,                     35, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_DOUSE,                         32, 24, DEGtoANG(33.75)},
	{FIELD_PSYNERGY_HALT,                          32, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_FORCE,                         48, 8,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_CATCH,                         48, 48, DEGtoANG(33.75)},
	{FIELD_PSYNERGY_BLAZE,                         16, 64, DEGtoANG(33.75)},
	{FIELD_PSYNERGY_LASH,                          16, 0,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_GRIND,                         32, 16, DEGtoANG(45)},
	{FIELD_PSYNERGY_TREMOR,                        32, 16, DEGtoANG(45)},
	{FIELD_PSYNERGY_POUND,                         32, 12, DEGtoANG(45)},
	{REACH_SHAKEN_OBJECTS + FIELD_PSYNERGY_TREMOR, 32, 32, DEGtoANG(45)},
	{FIELD_PSYNERGY_SCOOP,                         16, 0,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_PARCH,                         32, 0,  DEGtoANG(33.75)},
	{FIELD_PSYNERGY_CYCLONE,                       0,  0,  0},
	{-1,                                           16, 8,  DEGtoANG(33.75)},
};
/* clang-format on */
