.syntax unified
	.thumb
	.global Func_08015384
	.thumb_func
Func_08015384:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r1, [sp, #16]
	str r2, [sp, #12]
	adds r5, r0, #0
	ldr r0, [r5]
	bl Trig_Sin
	str r0, [sp, #8]
	movs r6, #128
	ldr r0, [r5]
	lsls r6, r6, #7
	adds r0, r0, r6
	bl Trig_Sin
	str r0, [sp, #4]
	ldr r0, [r5, #4]
	bl Trig_Sin
	mov r11, r0
	ldr r0, [r5, #4]
	adds r0, r0, r6
	bl Trig_Sin
	str r0, [sp, #0]
	ldr r0, [r5, #8]
	bl Trig_Sin
	mov r9, r0
	ldr r0, [r5, #8]
	adds r0, r0, r6
	bl Trig_Sin
	ldr r2, [sp, #12]
	mov r10, r0
	ldr r6, .L_08015508
	ldr r0, [sp, #0]
	mov r1, r10
	ldr r7, [r2]
	mov lr, r6
	.2byte 0xf800
	adds r1, r0, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	add r3, sp, #20
	str r0, [r3]
	ldr r0, [sp, #0]
	mov r1, r9
	mov r8, r3
	mov lr, r6
	.2byte 0xf800
	adds r1, r0, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	mov r3, r11
	str r0, [r2, #4]
	negs r1, r3
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	str r0, [r2, #8]
	ldr r3, [sp, #12]
	ldr r0, [sp, #8]
	mov r1, r11
	ldr r7, [r3, #4]
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	adds r5, r0, #0
	ldr r0, [sp, #4]
	mov lr, r6
	.2byte 0xf800
	subs r5, r5, r0
	adds r1, r5, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	str r0, [r2, #12]
	ldr r0, [sp, #8]
	mov r1, r11
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	adds r5, r0, #0
	ldr r0, [sp, #4]
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r1, r5, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	str r0, [r3, #16]
	ldr r1, [sp, #0]
	ldr r0, [sp, #8]
	mov lr, r6
	.2byte 0xf800
	adds r1, r0, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	str r0, [r2, #20]
	ldr r3, [sp, #12]
	ldr r0, [sp, #4]
	mov r1, r11
	ldr r7, [r3, #8]
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	adds r5, r0, #0
	ldr r0, [sp, #8]
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r1, r5, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	str r0, [r2, #24]
	ldr r0, [sp, #4]
	mov r1, r11
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	adds r5, r0, #0
	ldr r0, [sp, #8]
	mov lr, r6
	.2byte 0xf800
	subs r5, r5, r0
	adds r1, r5, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	str r0, [r3, #28]
	ldr r1, [sp, #0]
	ldr r0, [sp, #4]
	mov lr, r6
	.2byte 0xf800
	adds r1, r0, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	str r0, [r2, #32]
	ldr r2, [sp, #16]
	mov r0, r8
	ldr r3, [r2]
	mov r2, r8
	str r3, [r2, #36]
	ldr r2, [sp, #16]
	ldr r3, [r2, #4]
	mov r2, r8
	str r3, [r2, #40]
	ldr r2, [sp, #16]
	ldr r3, [r2, #8]
	mov r2, r8
	str r3, [r2, #44]
	ldr r3, .L_0801550c
	mov lr, r3
	.2byte 0xf800
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08015508:
	.4byte IwramMulQ16
.L_0801550c:
	.4byte IwramTransformMatrix
