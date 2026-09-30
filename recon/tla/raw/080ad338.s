.syntax unified
	.thumb
	.global Func_080ad338
	.thumb_func
Func_080ad338:
	push {lr}
	bl Func_080addf0
	movs r0, #0
	bl Game_ResetForNewGameFar
	pop {pc}
	.2byte 0x0000
