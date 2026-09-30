.syntax unified
	.thumb
	.global Func_08179e6c
	.thumb_func
Func_08179e6c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #96]
	movs r7, #7
	mov r8, r3
	bl Func_0815b410
	ldr r3, .L_08179eb0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_08179eb4
	movs r2, #160
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08179eb8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08179ebc
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_08179ec0
	subs r2, #2
	strh r3, [r2]
	movs r3, #255
	movs r1, #0
	mov r12, r3
	b .L_08179ec4
.L_08179eb0:
	.4byte 0x00000784
.L_08179eb4:
	.4byte 0x00000000
.L_08179eb8:
	.4byte 0x0000004e
.L_08179ebc:
	.4byte 0x00001010
.L_08179ec0:
	.4byte 0x00003f44
.L_08179ec4:
	adds r5, r1, #0
	movs r4, #0
	lsls r6, r1, #1
	ands r5, r7
.L_08179ecc:
	lsls r0, r1, #1
	cmp r1, #31
	ble .L_08179edc
	mov r3, r12
	subs r0, r3, r6
	cmp r1, #95
	bgt .L_08179edc
	movs r0, #63
.L_08179edc:
	adds r2, r4, #0
	cmp r4, #0
	bge .L_08179ee4
	adds r2, r4, #7
.L_08179ee4:
	asrs r2, r2, #3
	adds r3, r1, #0
	cmp r1, #0
	bge .L_08179eee
	adds r3, r1, #7
.L_08179eee:
	asrs r3, r3, #3
	lsls r2, r2, #4
	adds r2, r2, r3
	adds r3, r4, #0
	ands r3, r7
	lsls r2, r2, #3
	adds r2, r2, r3
	lsls r2, r2, #3
	adds r2, r2, r5
	mov r3, r8
	adds r4, #1
	strb r0, [r3, r2]
	cmp r4, #128
	bne .L_08179ecc
	adds r1, #1
	cmp r1, #128
	bne .L_08179ec4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
