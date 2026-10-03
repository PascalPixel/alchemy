.syntax unified
	.thumb
	.global Func_080e13e8
	.thumb_func
Func_080e13e8:
	push {r5, r6, lr}
	ldr r6, .L_080e141c
	adds r5, r0, #0
	ldr r0, [r6]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_080e140a
	movs r1, #6
	lsrs r0, r0, #1
	bl __umodsi3
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildValuesFar
	ldr r0, [r6]
.L_080e140a:
	movs r3, #15
	ands r3, r0
	cmp r3, #0
	bne .L_080e1418
	adds r0, r5, #0
	bl Func_080e1448
.L_080e1418:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080e141c:
	.4byte gFrameCount
