.syntax unified
	.thumb
	.global Func_080d0748
	.thumb_func
Func_080d0748:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	beq .L_080d0784
	cmp r2, #0
	beq .L_080d0764
	ldrh r2, [r4, #20]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r4, #20]
.L_080d0764:
	cmp r1, #0
	beq .L_080d0774
	ldrh r2, [r4, #20]
	movs r3, #251
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r4, #20]
.L_080d0774:
	cmp r0, #0
	beq .L_080d0784
	ldrh r2, [r4, #20]
	movs r3, #247
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r4, #20]
.L_080d0784:
	pop {pc}
	.2byte 0x0000
