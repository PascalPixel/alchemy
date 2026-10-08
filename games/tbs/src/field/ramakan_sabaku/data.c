#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 RamakanSabaku_SafePoints1[] = {
	PIXELS(384), PIXELS(128),
	PIXELS(288), PIXELS(688),
	PIXELS(608), PIXELS(720),
};

s32 RamakanSabaku_SafePoints2[] = {
	PIXELS(704), PIXELS(96),
	PIXELS(448), PIXELS(464),
	PIXELS(832), PIXELS(592),
	PIXELS(128), PIXELS(752),
	PIXELS(464), PIXELS(896),
};

s32 RamakanSabaku_SafePointsOther[] = {
	PIXELS(192), PIXELS(224),
	PIXELS(192), PIXELS(352),
};
