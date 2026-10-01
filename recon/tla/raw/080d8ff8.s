.syntax unified
	.thumb
	.global Func_080d8ff8
	.thumb_func
Func_080d8ff8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r1, [sp, #32]
	str r2, [sp, #28]
	str r3, [sp, #24]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r6, [r3]
	ldr r2, [sp, #68]
	ldr r3, [sp, #32]
	ldr r1, [sp, #72]
	subs r2, r2, r3
	asrs r3, r2, #8
	str r2, [sp, #20]
	ldr r2, [sp, #28]
	lsls r0, r0, #5
	subs r1, r1, r2
	str r1, [sp, #16]
	adds r6, r6, r0
	adds r6, #12
	ldr r2, [r6, #28]
	adds r0, r3, #0
	asrs r1, r1, #8
	mov r8, r2
	bl ArcTan2
	adds r5, r0, #0
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r0, r5, #0
	bl Trig_Sin
	str r0, [sp, #12]
	adds r0, r5, #0
	bl Trig_Cos
	str r0, [sp, #8]
	movs r2, #0
	movs r3, #4
	ldrsh r7, [r6, r3]
	mov r10, r2
	cmp r10, r7
	bgt .L_080d90ee
	ldr r3, [sp, #76]
	ldr r2, [sp, #24]
	subs r3, r3, r2
	str r3, [sp, #4]
	movs r3, #0
	str r3, [sp, #0]
	mov r11, r3
	mov r9, r3
.L_080d906c:
	mov r2, r10
	lsls r0, r2, #15
	adds r1, r7, #0
	bl __divsi3
	bl Trig_Sin
	ldr r3, .L_080d9100
	ldr r1, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_080d9100
	ldr r1, [sp, #8]
	adds r6, r0, #0
	mov lr, r2
	.2byte 0xf800
	adds r1, r7, #0
	adds r5, r0, #0
	mov r0, r9
	bl __divsi3
	ldr r3, [sp, #32]
	mov r2, r8
	adds r0, r3, r0
	adds r0, r0, r5
	str r0, [r2, #4]
	ldr r1, [sp, #12]
	ldr r3, .L_080d9100
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	adds r1, r7, #0
	adds r5, r0, #0
	mov r0, r11
	bl __divsi3
	ldr r2, [sp, #28]
	mov r3, r8
	adds r0, r2, r0
	adds r0, r0, r5
	str r0, [r3, #8]
	ldr r0, [sp, #0]
	adds r1, r7, #0
	bl __divsi3
	ldr r2, [sp, #24]
	mov r3, r8
	adds r0, r2, r0
	movs r2, #1
	str r0, [r3, #12]
	strb r2, [r3, #18]
	ldr r3, [r3]
	ldr r2, [sp, #0]
	mov r8, r3
	ldr r3, [sp, #4]
	adds r2, r2, r3
	str r2, [sp, #0]
	ldr r2, [sp, #16]
	ldr r3, [sp, #20]
	add r11, r2
	movs r2, #1
	add r10, r2
	add r9, r3
	cmp r10, r7
	ble .L_080d906c
.L_080d90ee:
	movs r0, #0
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d9100:
	.4byte IwramMulQ16
