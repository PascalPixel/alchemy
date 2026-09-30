.syntax unified
	.thumb
	.global Func_08164a4c
	.thumb_func
Func_08164a4c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r5, #160
	ldr r7, .L_08164a88
	lsls r5, r5, #19
	mov r8, r0
	mov lr, r1
	mov r12, r2
	adds r5, #2
	movs r6, #0
.L_08164a62:
	ldrh r2, [r5]
	movs r0, #31
	lsls r3, r2, #16
	lsrs r1, r3, #26
	ands r1, r7
	lsrs r4, r3, #21
	ands r4, r7
	ands r0, r2
	add r1, r8
	add r4, lr
	add r0, r12
	cmp r1, #31
	ble .L_08164a7e
	movs r1, #31
.L_08164a7e:
	cmp r4, #31
	ble .L_08164a8c
	movs r4, #31
	b .L_08164a8c
	.2byte 0x0000
.L_08164a88:
	.4byte 0x0000001f
.L_08164a8c:
	cmp r0, #31
	ble .L_08164a92
	movs r0, #31
.L_08164a92:
	cmp r1, #0
	bge .L_08164a98
	movs r1, #0
.L_08164a98:
	cmp r4, #0
	bge .L_08164a9e
	movs r4, #0
.L_08164a9e:
	cmp r0, #0
	bge .L_08164aa4
	movs r0, #0
.L_08164aa4:
	lsls r3, r1, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r0
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #63
	bne .L_08164a62
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
