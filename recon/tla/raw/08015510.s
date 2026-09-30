.syntax unified
	.thumb
	.global Func_08015510
	.thumb_func
Func_08015510:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r4, r1, #0
	ldr r1, .L_080156d8
	sub sp, #68
	str r0, [sp, #32]
	str r2, [sp, #28]
	ldr r0, .L_080156dc
	add r2, sp, #36
	mov r10, r2
	mov r9, r1
	movs r3, #128
	adds r1, r2, #0
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r11, r0
	adds r3, #212
	ldr r0, .L_080156e0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [sp, #32]
	ldr r2, [r4]
	ldr r3, [r0]
	subs r2, r2, r3
	str r2, [sp, #24]
	ldr r2, [r4, #4]
	ldr r3, [r0, #4]
	subs r2, r2, r3
	str r2, [sp, #20]
	ldr r2, [r4, #8]
	ldr r3, [r0, #8]
	ldr r0, [sp, #20]
	subs r2, r2, r3
	str r2, [sp, #16]
	asrs r3, r0, #8
	ldr r2, [sp, #24]
	ldr r0, [sp, #16]
	asrs r1, r2, #8
	asrs r2, r0, #8
	str r2, [sp, #0]
	str r2, [sp, #4]
	adds r0, r1, #0
	adds r2, r3, #0
	mov lr, r10
	.2byte 0xf800
	ldr r1, .L_080156e4
	mov lr, r1
	.2byte 0xf800
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #24
	mov lr, r9
	.2byte 0xf800
	lsrs r5, r0, #15
	negs r5, r5
	ldr r0, [sp, #24]
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	adds r1, r5, #0
	str r0, [sp, #24]
	ldr r0, [sp, #20]
	mov lr, r11
	.2byte 0xf800
	adds r1, r5, #0
	str r0, [sp, #20]
	ldr r0, [sp, #16]
	mov lr, r11
	.2byte 0xf800
	ldr r2, [sp, #24]
	str r0, [sp, #16]
	negs r2, r2
	str r2, [sp, #8]
	ldr r0, [sp, #20]
	adds r1, r0, #0
	mov lr, r11
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #9
	subs r0, r3, r0
	adds r5, r3, #0
	cmp r0, #0
	ble .L_080155d8
	ldr r3, .L_080156e4
	mov lr, r3
	.2byte 0xf800
	adds r1, r0, #0
	movs r0, #128
	lsls r1, r1, #8
	lsls r0, r0, #24
	mov lr, r9
	.2byte 0xf800
	lsls r5, r0, #1
.L_080155d8:
	ldr r0, [sp, #16]
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	adds r1, r5, #0
	str r0, [sp, #12]
	ldr r0, [sp, #8]
	mov lr, r11
	.2byte 0xf800
	str r0, [sp, #8]
	ldr r1, [sp, #8]
	ldr r0, [sp, #20]
	mov lr, r11
	.2byte 0xf800
	ldr r1, [sp, #12]
	mov r8, r0
	ldr r0, [sp, #16]
	mov lr, r11
	.2byte 0xf800
	ldr r1, [sp, #8]
	adds r5, r0, #0
	ldr r0, [sp, #24]
	mov lr, r11
	.2byte 0xf800
	ldr r1, [sp, #12]
	subs r6, r5, r0
	ldr r0, [sp, #20]
	mov lr, r11
	.2byte 0xf800
	negs r7, r0
	str r7, [sp, #0]
	str r7, [sp, #4]
	adds r2, r6, #0
	adds r3, r6, #0
	mov r1, r8
	mov r0, r8
	mov lr, r10
	.2byte 0xf800
	ldr r1, .L_080156e4
	mov lr, r1
	.2byte 0xf800
	adds r1, r0, #0
	movs r0, #128
	lsls r1, r1, #8
	lsls r0, r0, #24
	mov lr, r9
	.2byte 0xf800
	lsls r5, r0, #1
	adds r1, r5, #0
	mov r0, r8
	mov lr, r11
	.2byte 0xf800
	adds r1, r5, #0
	mov r8, r0
	adds r0, r6, #0
	mov lr, r11
	.2byte 0xf800
	adds r1, r5, #0
	adds r6, r0, #0
	adds r0, r7, #0
	mov lr, r11
	.2byte 0xf800
	ldr r2, [sp, #32]
	ldr r3, [sp, #32]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	ldr r1, [sp, #12]
	mov r11, r2
	adds r7, r0, #0
	ldr r2, [sp, #28]
	ldr r0, [sp, #32]
	mov r9, r3
	movs r3, #0
	ldr r5, [r0, #8]
	str r1, [r2]
	str r3, [r2, #12]
	ldr r0, [sp, #8]
	str r0, [r2, #24]
	ldr r1, [sp, #12]
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r2, r5, #0
	ldr r3, [sp, #8]
	mov r0, r11
	mov lr, r10
	.2byte 0xf800
	ldr r1, [sp, #28]
	mov r2, r8
	negs r0, r0
	str r0, [r1, #36]
	str r2, [r1, #4]
	str r6, [r1, #16]
	str r7, [r1, #28]
	mov r2, r9
	str r5, [sp, #0]
	str r7, [sp, #4]
	mov r1, r8
	adds r3, r6, #0
	mov r0, r11
	mov lr, r10
	.2byte 0xf800
	ldr r3, [sp, #28]
	negs r0, r0
	str r0, [r3, #40]
	ldr r0, [sp, #24]
	str r0, [r3, #8]
	ldr r1, [sp, #20]
	mov r0, r11
	str r1, [r3, #20]
	ldr r2, [sp, #16]
	str r2, [r3, #32]
	ldr r1, [sp, #24]
	str r5, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #20]
	mov r2, r9
	mov lr, r10
	.2byte 0xf800
	ldr r3, [sp, #28]
	negs r0, r0
	str r0, [r3, #44]
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080156d8:
	.4byte IwramUnsignedDivide
.L_080156dc:
	.4byte IwramMulQ16
.L_080156e0:
	.4byte Math_DotProductQ16Code
.L_080156e4:
	.4byte IwramFillWords + 0x74
