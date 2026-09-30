.syntax unified
	.thumb
	.global Func_0815a044
	.thumb_func
Func_0815a044:
	push {lr}
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_0815a054
	movs r1, #6
	bl BattlePres_RunBurstScene
	b .L_0815a066
.L_0815a054:
	cmp r3, #1
	bne .L_0815a060
	movs r1, #7
	bl BattlePres_RunBurstScene
	b .L_0815a066
.L_0815a060:
	movs r1, #8
	bl BattlePres_RunBurstScene
.L_0815a066:
	pop {pc}
