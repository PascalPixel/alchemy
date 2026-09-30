.syntax unified
	.thumb
	.global Func_08039f94
	.thumb_func
Func_08039f94:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r1, [sp, #0]
	adds r7, r0, #0
	movs r2, #24
	ldrsh r6, [r7, r2]
	movs r3, #26
	ldrsh r0, [r7, r3]
	ldrh r3, [r7, #8]
	add r5, sp, #4
	adds r1, r3, #0
	muls r1, r6
	ldr r3, .L_0803a050
	subs r2, r0, r6
	lsls r1, r1, #16
	lsls r0, r0, #17
	mov r8, r3
	str r1, [r5]
	str r0, [r5, #4]
	mov r10, r2
	mov lr, r8
	.2byte 0xf800
	movs r2, #12
	ldrsh r3, [r7, r2]
	str r0, [r5, #8]
	asrs r0, r0, #16
	adds r0, r0, r3
	ldrh r3, [r7, #8]
	mov r9, r0
	mov r1, r10
	muls r1, r3
	lsls r1, r1, #16
	str r1, [r5]
	ldr r0, [r5, #4]
	mov lr, r8
	.2byte 0xf800
	ldrh r3, [r7, #10]
	str r0, [r5, #8]
	asrs r0, r0, #15
	adds r1, r3, #0
	muls r1, r6
	mov r11, r0
	movs r3, #26
	ldrsh r0, [r7, r3]
	lsls r1, r1, #16
	lsls r0, r0, #17
	str r1, [r5]
	str r0, [r5, #4]
	mov lr, r8
	.2byte 0xf800
	movs r2, #14
	ldrsh r3, [r7, r2]
	str r0, [r5, #8]
	asrs r0, r0, #16
	adds r6, r0, r3
	ldrh r3, [r7, #10]
	ldr r0, [r5, #4]
	mov r1, r10
	muls r1, r3
	lsls r1, r1, #16
	str r1, [r5]
	mov lr, r8
	.2byte 0xf800
	str r0, [r5, #8]
	asrs r5, r0, #15
	adds r3, r5, #0
	mov r0, r9
	adds r1, r6, #0
	mov r2, r11
	bl Func_0803a084
	ldr r3, [sp, #0]
	cmp r3, #0
	beq .L_0803a040
	mov r2, r9
	mov r3, r11
	strh r2, [r7, #28]
	strh r6, [r7, #30]
	strh r3, [r7, #32]
	strh r5, [r7, #34]
.L_0803a040:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803a050:
	.4byte IwramRatioMulQ14
