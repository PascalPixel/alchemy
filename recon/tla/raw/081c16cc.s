.syntax unified
	.thumb
	.global Func_081c16cc
	.thumb_func
Func_081c16cc:
	ldr r3, [r0, #44]
	cmp r3, #0
	beq .L_081c16ea
	ldr r1, [r0, #52]
	ldr r2, [r0, #48]
	cmp r2, #0
	beq .L_081c16de
	str r1, [r2, #52]
	b .L_081c16e0
.L_081c16de:
	str r1, [r3, #32]
.L_081c16e0:
	cmp r1, #0
	beq .L_081c16e6
	str r2, [r1, #48]
.L_081c16e6:
	movs r1, #0
	str r1, [r0, #44]
.L_081c16ea:
	bx lr
