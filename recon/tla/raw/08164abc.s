.syntax unified
	.thumb
	.global Func_08164abc
	.thumb_func
Func_08164abc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r5, #160
	ldr r7, .L_08164af8
	lsls r5, r5, #19
	mov r8, r0
	mov lr, r1
	mov r12, r2
	adds r5, #192
	movs r6, #0
.L_08164ad2:
	ldrh r2, [r5]
	movs r0, #31
	lsls r3, r2, #16
	lsrs r1, r3, #26
	lsrs r4, r3, #21
	ands r0, r2
	ands r1, r7
	ands r4, r7
	add r0, r8
	add r4, lr
	add r1, r12
	cmp r0, #31
	ble .L_08164aee
	movs r0, #31
.L_08164aee:
	cmp r4, #31
	ble .L_08164afc
	movs r4, #31
	b .L_08164afc
	.2byte 0x0000
.L_08164af8:
	.4byte 0x0000001f
.L_08164afc:
	cmp r1, #31
	ble .L_08164b02
	movs r1, #31
.L_08164b02:
	cmp r0, #0
	bge .L_08164b08
	movs r0, #0
.L_08164b08:
	cmp r4, #0
	bge .L_08164b0e
	movs r4, #0
.L_08164b0e:
	cmp r1, #0
	bge .L_08164b14
	movs r1, #0
.L_08164b14:
	lsls r3, r1, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r0
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #128
	bne .L_08164ad2
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
