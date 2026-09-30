.syntax unified
	.thumb
	.global Func_080e6388
	.thumb_func
Func_080e6388:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	ldr r3, [r7, #24]
	movs r1, #7
	asrs r3, r3, #2
	ands r3, r1
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_080e63c4
	adds r6, r0, #0
	ands r2, r3
	ldrh r1, [r6, #8]
	ldr r3, .L_080e63c8
	sub sp, #12
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
	ble .L_080e63cc
	adds r3, r2, #0
	b .L_080e63cc
	.2byte 0x0000
.L_080e63c4:
	.4byte 0x000003ff
.L_080e63c8:
	.4byte 0xfffffc00
.L_080e63cc:
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
	adds r0, r7, #0
	movs r1, #60
	movs r2, #0
	bl BattleFx_IntegrateVector3
	add sp, #12
	pop {r5, r6, r7, pc}
