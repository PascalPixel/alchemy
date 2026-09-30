.syntax unified
	.thumb
	.global Func_0811d7e8
	.thumb_func
Func_0811d7e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldrh r3, [r6]
	sub sp, #20
	adds r5, r1, #0
	movs r0, #2
	cmp r3, #7
	bls .L_0811d802
	movs r0, #1
.L_0811d802:
	mov r10, sp
	mov r1, r10
	bl Func_0811a39c
	ldr r3, .L_0811d898
	mov r8, r0
	movs r1, #104
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	ldrh r3, [r6]
	movs r0, #0
	cmp r3, #7
	bhi .L_0811d820
	movs r0, #1
.L_0811d820:
	bl Func_0811f4d4
	bl Battle_GetTaggedSlotValue
	movs r3, #2
	str r3, [r5, #80]
	movs r3, #14
	str r3, [r5, #84]
	ldrh r3, [r6, #6]
	adds r2, r5, #0
	adds r2, #74
	strb r0, [r5]
	strh r3, [r2]
	mov r3, r8
	cmp r3, #0
	ble .L_0811d878
	movs r6, #1
	adds r0, r5, #0
	adds r3, r5, #3
	movs r7, #0
	mov r9, r6
	adds r0, #31
	subs r2, #57
	mov r12, r3
	mov r1, r10
	mov r4, r8
.L_0811d854:
	ldrh r6, [r1]
	subs r4, #1
	mov lr, r6
	mov r3, lr
	mov r6, r12
	strb r3, [r6]
	mov r6, r9
	movs r3, #1
	strb r7, [r2]
	adds r1, #2
	strb r6, [r0]
	add r12, r3
	strb r7, [r2, #28]
	strb r6, [r0, #28]
	adds r2, #1
	adds r0, #1
	cmp r4, #0
	bne .L_0811d854
.L_0811d878:
	mov r3, r8
	strb r3, [r5, #1]
	movs r3, #148
	adds r3, #255
	str r3, [r5, #76]
	movs r3, #128
	lsls r3, r3, #8
	adds r3, #26
	str r3, [r5, #88]
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811d898:
	.4byte IwramClearWords
