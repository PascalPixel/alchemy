.syntax unified
	.thumb
	.global Func_080e716c
	.thumb_func
Func_080e716c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #92]
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #48
	ldr r2, [r2, #108]
	adds r3, r3, r1
	ldr r7, [r3]
	ldr r3, .L_080e7230
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Owner_GetState
	adds r5, r0, #0
	movs r0, #79
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080e71c0
	movs r0, #234
	adds r0, #255
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	bl Func_080200c0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080e7228
	movs r3, #79
	lsls r3, r3, #2
	adds r3, #20
	mov r1, r8
	str r6, [r1, r3]
.L_080e71c0:
	bl Random16
	movs r2, #192
	lsls r2, r2, #8
	cmp r0, r2
	bcs .L_080e71f0
	ldr r5, .L_080e7234
	bl Random16
	lsls r0, r0, #4
	lsrs r0, r0, #16
	lsls r0, r0, #1
	ldrh r5, [r5, r0]
	adds r0, r5, #0
	bl Func_080ad2f0
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080e7204
	movs r5, #128
	lsls r5, r5, #8
	adds r5, #1
	b .L_080e7204
.L_080e71f0:
	bl Random16
	ldrb r3, [r5, #15]
	muls r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	adds r5, r3, #1
	movs r3, #128
	lsls r3, r3, #8
	orrs r5, r3
.L_080e7204:
	ldr r3, [r7, #8]
	movs r0, #79
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r1, r5, #0
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	bl Func_080d3b28
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #52
	add r3, r8
	strh r5, [r3]
	movs r0, #79
	bl Func_080e70f8
.L_080e7228:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e7230:
	.4byte gPartyState
.L_080e7234:
	.4byte Data_080f0fc0
