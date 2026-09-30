.syntax unified
	.thumb
	.global Func_080e46ac
	.thumb_func
Func_080e46ac:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	ldr r2, [r7, #24]
	sub sp, #12
	adds r6, r0, #0
	adds r1, r3, #0
	cmp r2, #31
	bhi .L_080e4726
	movs r3, #7
	asrs r2, r2, #2
	ands r2, r3
	ldr r3, .L_080e46ec
	lsls r2, r2, #3
	adds r2, r1, r2
	ands r2, r3
	ldrh r1, [r6, #8]
	ldr r3, .L_080e46f0
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #8]
	movs r2, #200
	ldr r3, [r6, #20]
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r3, [r6, #20]
	cmp r3, r2
	ble .L_080e46f4
	adds r3, r2, #0
	b .L_080e46f4
.L_080e46ec:
	.4byte 0x000003ff
.L_080e46f0:
	.4byte 0xfffffc00
.L_080e46f4:
	str r3, [r6, #20]
	str r3, [r6, #24]
	ldr r3, [r7]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r7, #4]
	adds r0, r5, #0
	str r3, [r5, #4]
	ldr r3, [r7, #8]
	str r3, [r5, #8]
	bl Func_080dc390
	ldr r3, [r5]
	adds r0, r6, #0
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	bl Func_080eb01c
	movs r2, #0
	adds r0, r7, #0
	movs r1, #60
	bl BattleFx_IntegrateVector2
	ldr r2, [r7, #24]
.L_080e4726:
	adds r3, r2, #1
	str r3, [r7, #24]
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
