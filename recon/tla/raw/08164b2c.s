.syntax unified
	.thumb
	.global Func_08164b2c
	.thumb_func
Func_08164b2c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r10, r0
	mov lr, r2
	ldr r3, [r3, #36]
	movs r0, #160
	ldr r2, .L_08164b74
	movs r7, #160
	lsls r0, r0, #3
	lsls r7, r7, #19
	adds r0, #108
	mov r8, r1
	adds r7, #192
	adds r5, r3, r0
	movs r6, #0
	mov r12, r2
.L_08164b54:
	ldrh r2, [r5]
	mov r0, r12
	lsls r3, r2, #16
	lsrs r1, r3, #26
	lsrs r4, r3, #21
	ands r1, r0
	ands r4, r0
	movs r0, #31
	ands r0, r2
	add r0, r10
	add r4, r8
	add r1, lr
	cmp r0, #31
	ble .L_08164b78
	movs r0, #31
	b .L_08164b78
.L_08164b74:
	.4byte 0x0000001f
.L_08164b78:
	cmp r4, #31
	ble .L_08164b7e
	movs r4, #31
.L_08164b7e:
	cmp r1, #31
	ble .L_08164b84
	movs r1, #31
.L_08164b84:
	cmp r0, #0
	bge .L_08164b8a
	movs r0, #0
.L_08164b8a:
	cmp r4, #0
	bge .L_08164b90
	movs r4, #0
.L_08164b90:
	cmp r1, #0
	bge .L_08164b96
	movs r1, #0
.L_08164b96:
	lsls r3, r1, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r0
	adds r6, #1
	strh r3, [r7]
	adds r5, #2
	adds r7, #2
	cmp r6, #128
	bne .L_08164b54
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
