.syntax unified
	.thumb
	.global Func_080e13b0
	.thumb_func
Func_080e13b0:
	push {r5, lr}
	ldr r3, .L_080e13e4
	movs r2, #2
	ldr r3, [r3]
	adds r5, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080e13c8
	movs r1, #7
	bl Animation_ApplyChildValuesFar
	b .L_080e13d0
.L_080e13c8:
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValuesFar
.L_080e13d0:
	ldr r3, .L_080e13e4
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_080e13e2
	adds r0, r5, #0
	bl Func_080e1448
.L_080e13e2:
	pop {r5, pc}
.L_080e13e4:
	.4byte gFrameCount
