.syntax unified
	.thumb
	.global Camera_FollowLeaderInBounds
	.thumb_func
Camera_FollowLeaderInBounds:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0800dcc0
	ldr r1, [r3]
	adds r3, r1, #0
	adds r3, #236
	ldr r3, [r3]
	movs r2, #240
	lsls r2, r2, #15
	mov r8, r0
	adds r7, r3, r2
	ldr r0, [r0, #104]
	adds r3, r1, #0
	adds r3, #240
	ldr r2, [r0, #12]
	ldr r3, [r3]
	movs r4, #192
	adds r3, r3, r2
	lsls r4, r4, #15
	adds r6, r3, r4
	adds r3, r1, #0
	adds r3, #244
	ldr r3, [r3]
	ldr r5, .L_0800dcc4
	adds r4, r3, r5
	adds r3, r1, #0
	adds r3, #248
	ldr r3, [r3]
	adds r3, r3, r2
	ldr r2, .L_0800dcc8
	adds r1, r3, r2
	mov r2, r8
	adds r2, #85
	movs r3, #0
	sub sp, #8
	strb r3, [r2]
	cmp r0, #0
	bne .L_0800db48
	b .L_0800dca0
.L_0800db48:
	ldr r3, [r0]
	cmp r3, #0
	bne .L_0800db50
	b .L_0800dca0
.L_0800db50:
	ldr r3, [r0, #8]
	ldr r5, [r0, #12]
	mov r11, r3
	ldr r0, [r0, #16]
	movs r3, #128
	lsls r3, r3, #24
	mov r2, r8
	str r0, [sp, #4]
	str r3, [r2, #56]
	str r3, [r2, #60]
	str r3, [r2, #64]
	cmp r11, r7
	bge .L_0800db6c
	mov r11, r7
.L_0800db6c:
	ldr r3, [sp, #4]
	cmp r3, r6
	bge .L_0800db74
	str r6, [sp, #4]
.L_0800db74:
	cmp r11, r4
	ble .L_0800db7a
	mov r11, r4
.L_0800db7a:
	ldr r4, [sp, #4]
	cmp r4, r1
	ble .L_0800db82
	str r1, [sp, #4]
.L_0800db82:
	mov r3, r8
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0800db9c
	mov r4, r8
	mov r3, r11
	str r3, [r4, #8]
	str r5, [r4, #12]
	ldr r5, [sp, #4]
	str r5, [r4, #16]
	b .L_0800dca0
.L_0800db9c:
	mov r2, r8
	ldr r3, [r2, #8]
	mov r4, r11
	subs r0, r4, r3
	cmp r0, #0
	bge .L_0800dbac
	ldr r2, .L_0800dccc
	adds r0, r0, r2
.L_0800dbac:
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r4, [sp, #4]
	asrs r0, r0, #16
	mov r10, r0
	subs r0, r4, r3
	cmp r0, #0
	bge .L_0800dbc0
	ldr r2, .L_0800dccc
	adds r0, r0, r2
.L_0800dbc0:
	asrs r6, r0, #16
	mov r3, r10
	mov r0, r10
	muls r0, r3
	adds r3, r6, #0
	muls r3, r6
	adds r0, r0, r3
	ldr r3, .L_0800dcd0
	bl _call_via_r3
	mov r4, r8
	ldr r3, [r4, #8]
	mov r2, r11
	subs r2, r2, r3
	ldr r3, [r4, #12]
	subs r5, r5, r3
	mov r9, r5
	ldr r3, [r4, #16]
	movs r5, #128
	ldr r4, [sp, #4]
	lsls r7, r0, #16
	lsls r5, r5, #15
	mov r10, r2
	subs r6, r4, r3
	cmp r7, r5
	bge .L_0800dc16
	ldr r4, .L_0800dcd4
	mov r0, r10
	mov r1, r10
	movs r0, r0
	mov r12, pc
	bx r4
	adds r3, r0, #0
	adds r1, r6, #0
	adds r0, r6, #0
	movs r0, r0
	mov r12, pc
	bx r4
	adds r3, r3, r0
	adds r0, r3, #0
	bl FixedSqrt
	adds r7, r0, #0
.L_0800dc16:
	adds r1, r7, #0
	cmp r7, #0
	bge .L_0800dc1e
	adds r1, r7, #7
.L_0800dc1e:
	mov r2, r8
	ldr r3, [r2, #48]
	asrs r5, r1, #3
	cmp r5, r3
	ble .L_0800dc2a
	adds r5, r3, #0
.L_0800dc2a:
	movs r3, #128
	lsls r3, r3, #7
	cmp r7, r3
	bge .L_0800dc3e
	mov r5, r8
	mov r4, r11
	str r4, [r5, #8]
	ldr r2, [sp, #4]
	str r2, [r5, #16]
	b .L_0800dc7c
.L_0800dc3e:
	cmp r7, r5
	ble .L_0800dc6e
	ldr r3, .L_0800dcd8
	mov r1, r10
	mov r11, r3
	adds r0, r7, #0
	bl _call_via_fp
	ldr r3, .L_0800dcd4
	adds r1, r5, #0
	movs r0, r0
	mov r12, pc
	bx r3
	adds r1, r6, #0
	str r3, [sp, #0]
	mov r10, r0
	adds r0, r7, #0
	bl _call_via_fp
	adds r1, r5, #0
	ldr r3, [sp, #0]
	mov r12, pc
	bx r3
	adds r6, r0, #0
.L_0800dc6e:
	mov r4, r8
	ldr r3, [r4, #8]
	add r3, r10
	str r3, [r4, #8]
	ldr r3, [r4, #16]
	adds r3, r3, r6
	str r3, [r4, #16]
.L_0800dc7c:
	mov r3, r9
	cmp r3, #0
	bge .L_0800dc84
	negs r3, r3
.L_0800dc84:
	movs r5, #128
	lsls r5, r5, #8
	cmp r3, r5
	ble .L_0800dc98
	mov r3, r9
	cmp r3, #0
	bge .L_0800dc94
	adds r3, #3
.L_0800dc94:
	asrs r3, r3, #2
	mov r9, r3
.L_0800dc98:
	mov r2, r8
	ldr r3, [r2, #12]
	add r3, r9
	str r3, [r2, #12]
.L_0800dca0:
	mov r4, r8
	ldrh r3, [r4, #4]
	mov r5, r8
	adds r3, #1
	movs r0, #1
	strh r3, [r5, #4]
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_0800dcc0:
	.4byte gMapWork
.L_0800dcc4:
	.4byte 0xff880000
.L_0800dcc8:
	.4byte 0xffc00000
.L_0800dccc:
	.4byte 0x0000ffff
.L_0800dcd0:
	.4byte IwramSqrt
.L_0800dcd4:
	.4byte IwramMulQ16ReturnIp
.L_0800dcd8:
	.4byte IwramRatioMulQ14
