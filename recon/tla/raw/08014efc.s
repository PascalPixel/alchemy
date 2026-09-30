.syntax unified
	.thumb
	.global Func_08014efc
	.thumb_func
Func_08014efc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	ldr r0, [r5]
	sub sp, #56
	bl Trig_Sin
	adds r7, r0, #0
	ldr r0, [r5]
	movs r6, #128
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
	mov r10, r0
	ldr r6, .L_0801501c
	ldr r0, [sp, #0]
	mov r1, r10
	mov lr, r6
	.2byte 0xf800
	add r2, sp, #8
	str r0, [r2]
	ldr r0, [sp, #0]
	mov r1, r9
	mov r8, r2
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	mov r2, r11
	str r0, [r3, #4]
	negs r3, r2
	mov r2, r8
	str r3, [r2, #8]
	mov r1, r11
	adds r0, r7, #0
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
	mov r3, r8
	subs r5, r5, r0
	str r5, [r3, #12]
	mov r1, r11
	adds r0, r7, #0
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
	mov r2, r8
	adds r5, r5, r0
	str r5, [r2, #16]
	ldr r1, [sp, #0]
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	str r0, [r3, #20]
	ldr r0, [sp, #4]
	mov r1, r11
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	adds r5, r0, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	adds r5, r5, r0
	str r5, [r2, #24]
	ldr r0, [sp, #4]
	mov r1, r11
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	adds r5, r0, #0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	subs r5, r5, r0
	str r5, [r3, #28]
	ldr r1, [sp, #0]
	ldr r0, [sp, #4]
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	movs r3, #0
	str r0, [r2, #32]
	str r3, [r2, #36]
	str r3, [r2, #40]
	str r3, [r2, #44]
	mov r0, r8
	ldr r3, .L_08015020
	mov lr, r3
	.2byte 0xf800
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0801501c:
	.4byte IwramMulQ16
.L_08015020:
	.4byte IwramTransformMatrix
