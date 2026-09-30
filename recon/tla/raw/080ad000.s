.syntax unified
	.thumb
	.global Resource_FarCall005
Resource_FarCall005:
	.global Trade_GetOfferStateFar
	.thumb_func
Trade_GetOfferStateFar:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xd349
	.2byte 0x080a
	.global Owner_RecalculateStatsFar
	.thumb_func
Owner_RecalculateStatsFar:
	.global BattleUnit_Recalculate
	.thumb_func
BattleUnit_Recalculate:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xd3f9
	.2byte 0x080a
	.global Item_Get
	.thumb_func
Item_Get:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xec05
	.2byte 0x080a
	.global Func_080ad018
	.thumb_func
Func_080ad018:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xeca5
	.2byte 0x080a
	.global Func_080ad020
	.thumb_func
Func_080ad020:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xed6d
	.2byte 0x080a
	.global Func_080ad028
	.thumb_func
Func_080ad028:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xedf9
	.2byte 0x080a
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xee99
	.2byte 0x080a
	.global Func_080ad038
	.thumb_func
Func_080ad038:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080aeec9
