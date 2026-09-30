.syntax unified
	.thumb
	.global Func_080e9f68
	.thumb_func
Func_080e9f68:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	sub sp, #12
	mov r8, r3
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #85
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080ea01a
	movs r3, #234
	lsls r3, r3, #5
	add r3, r8
	ldr r3, [r3]
	mov r6, sp
	str r3, [r6]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #68
	add r3, r8
	ldr r3, [r3]
	movs r5, #232
	str r3, [r6, #8]
	movs r3, #232
	lsls r3, r3, #5
	lsls r5, r5, #5
	adds r3, #76
	adds r5, #72
	add r3, r8
	add r5, r8
	ldr r0, [r3]
	adds r2, r6, #0
	ldr r1, [r5]
	bl Func_0801489c
	movs r7, #132
	ldr r3, [r6]
	lsls r7, r7, #5
	add r7, r8
	str r3, [r7, #12]
	movs r2, #192
	ldr r3, [r6, #8]
	lsls r2, r2, #3
	str r3, [r7, #16]
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	movs r3, #0
	mov r10, r3
.L_080e9fda:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_080ea00a
	mov r2, r10
	cmp r2, #0
	beq .L_080e9fee
	ldr r3, [r6]
	str r3, [r7, #12]
	ldr r3, [r6, #8]
	str r3, [r7, #16]
.L_080e9fee:
	ldr r3, [r7]
	movs r1, #0
	str r3, [r6]
	ldr r3, [r7, #4]
	adds r2, r6, #0
	str r3, [r6, #8]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #80
	add r3, r8
	ldr r0, [r3]
	bl Func_0801489c
	ldr r3, [r7, #24]
.L_080ea00a:
	adds r3, #1
	str r3, [r7, #24]
	movs r3, #1
	add r10, r3
	mov r2, r10
	adds r7, #28
	cmp r2, #47
	ble .L_080e9fda
.L_080ea01a:
	movs r7, #132
	movs r5, #174
	lsls r7, r7, #5
	lsls r5, r5, #5
	movs r3, #47
	add r7, r8
	add r5, r8
	mov r10, r3
.L_080ea02a:
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #84
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080ea062
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_080ea05c
	movs r1, #128
	adds r0, r7, #0
	lsls r1, r1, #11
	bl Func_080e9ee4
	ldr r3, [r7]
	adds r0, r5, #0
	str r3, [r5, #12]
	ldr r3, [r7, #4]
	str r3, [r5, #16]
	bl Func_080eb01c
	ldr r3, [r7, #24]
.L_080ea05c:
	adds r3, #1
	str r3, [r7, #24]
	b .L_080ea0bc
.L_080ea062:
	cmp r3, #2
	bne .L_080ea080
	movs r1, #192
	adds r0, r7, #0
	lsls r1, r1, #11
	bl Func_080e9ee4
	ldr r3, [r7]
	adds r0, r5, #0
	str r3, [r5, #12]
	ldr r3, [r7, #4]
	str r3, [r5, #16]
	bl Func_080eb01c
	b .L_080ea0bc
.L_080ea080:
	cmp r3, #3
	bne .L_080ea0bc
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #88
	add r3, r8
	ldr r2, [r7]
	ldr r3, [r3]
	cmp r2, r3
	bne .L_080ea0a4
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #92
	add r3, r8
	ldr r2, [r7, #4]
	ldr r3, [r3]
	cmp r2, r3
	beq .L_080ea0bc
.L_080ea0a4:
	movs r1, #192
	adds r0, r7, #0
	lsls r1, r1, #11
	bl Func_080e9ee4
	ldr r3, [r7]
	adds r0, r5, #0
	str r3, [r5, #12]
	ldr r3, [r7, #4]
	str r3, [r5, #16]
	bl Func_080eb01c
.L_080ea0bc:
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	adds r5, #40
	adds r7, #28
	cmp r3, #0
	bge .L_080ea02a
	movs r6, #232
	movs r7, #128
	lsls r6, r6, #5
	movs r5, #184
	lsls r7, r7, #4
	adds r6, #86
	lsls r5, r5, #4
	movs r2, #31
	add r7, r8
	add r6, r8
	add r5, r8
	mov r10, r2
.L_080ea0e4:
	ldr r0, [r7, #24]
	cmp r0, #19
	bhi .L_080ea130
	movs r1, #5
	bl __divsi3
	ldrh r1, [r6]
	movs r3, #7
	ands r3, r0
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_080ea124
	ldr r2, .L_080ea128
	ands r1, r3
	ldrh r3, [r5, #8]
	adds r0, r5, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	adds r1, r7, #0
	bl Func_080eb298
	adds r0, r7, #0
	movs r1, #63
	ldr r2, .L_080ea12c
	bl BattleFx_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
	b .L_080ea130
	.2byte 0x0000
.L_080ea124:
	.4byte 0x000003ff
.L_080ea128:
	.4byte 0xfffffc00
.L_080ea12c:
	.4byte 0xffff8000
.L_080ea130:
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r2, r10
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_080ea0e4
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
