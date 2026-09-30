.syntax unified
	.thumb
	.global Func_080d9194
	.thumb_func
Func_080d9194:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r1, [sp, #12]
	str r2, [sp, #8]
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	ldr r1, [sp, #48]
	mov r10, r3
	movs r3, #0
	str r3, [sp, #0]
	ldr r3, [sp, #12]
	ldr r6, [sp, #52]
	subs r1, r1, r3
	ldr r3, [sp, #8]
	ldr r2, [sp, #56]
	subs r6, r6, r3
	ldr r3, [sp, #4]
	mov r9, r0
	subs r0, r2, r3
	ldr r2, .L_080d929c
	asrs r1, r1, #8
	mov r8, r2
	asrs r7, r0, #8
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	asrs r6, r6, #8
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r7, #0
	adds r6, r0, #0
	adds r0, r7, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r6
	adds r5, r5, r0
	ldr r3, .L_080d92a0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r1, #6
	bl Math_Div
	mov r3, r9
	lsls r3, r3, #5
	mov r9, r3
	add r10, r9
	adds r0, #1
	movs r2, #40
	add r2, r10
	mov r11, r0
	mov r3, r11
	mov r9, r2
	mov r2, r10
	movs r6, #128
	strh r3, [r2, #16]
	movs r3, #0
	movs r7, #0
	lsls r6, r6, #11
	mov r10, r3
	b .L_080d9270
.L_080d9226:
	adds r0, r7, #0
	bl Trig_Cos
	adds r1, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, [sp, #12]
	adds r0, r2, r0
	str r0, [r5, #4]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r5, #8]
	bl Trig_Sin
	adds r1, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, [sp, #4]
	movs r3, #2
	adds r0, r2, r0
	str r0, [r5, #12]
	mov r2, r9
	movs r0, #192
	strb r3, [r5, #18]
	adds r1, r6, #0
	str r5, [r2]
	lsls r0, r0, #11
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r7, r7, r0
	movs r3, #1
	lsls r0, r0, #2
	mov r9, r5
	adds r6, r6, r0
	add r10, r3
.L_080d9270:
	cmp r10, r11
	bgt .L_080d9284
	bl Func_080d8d40
	adds r5, r0, #0
	cmp r5, #0
	bne .L_080d9226
	movs r2, #1
	negs r2, r2
	str r2, [sp, #0]
.L_080d9284:
	movs r3, #0
	mov r2, r9
	str r3, [r2]
	ldr r0, [sp, #0]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d929c:
	.4byte IwramMulQ16
.L_080d92a0:
	.4byte IwramFillWords + 0x74
