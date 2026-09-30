.syntax unified
	.thumb
	.global Func_080dcf54
	.thumb_func
Func_080dcf54:
	ldr r3, .L_080dcf68
	ldr r1, .L_080dcf6c
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrh r0, [r1, r3]
	bx lr
	.2byte 0x0000
.L_080dcf68:
	.4byte gInput
.L_080dcf6c:
	.4byte BattleFx_CyclePatternWords
