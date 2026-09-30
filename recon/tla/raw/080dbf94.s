.syntax unified
	.thumb
	.global Func_080dbf94
	.thumb_func
Func_080dbf94:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	mov r10, r1
	ldr r1, [r3, #16]
	sub sp, #12
	ldr r3, [r1, #8]
	mov r8, r2
	mov r2, sp
	str r3, [r2]
	adds r5, r0, #0
	ldr r3, [r1, #12]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r2, #4]
	movs r6, #0
	ldr r3, [r1, #16]
	mov r9, r2
	str r3, [r2, #8]
	mov r11, r6
.L_080dbfd0:
	mov r2, r9
	ldr r3, [r2]
	mov r0, r10
	subs r3, r3, r0
	adds r0, r6, #0
	muls r0, r3
	movs r1, #10
	bl Math_Div
	add r0, r10
	str r0, [r5, #8]
	mov r2, r9
	ldr r3, [r2, #4]
	mov r0, r8
	subs r3, r3, r0
	adds r0, r6, #0
	muls r0, r3
	movs r1, #10
	bl Math_Div
	add r0, r8
	str r0, [r5, #12]
	mov r2, r9
	ldr r3, [r2, #8]
	movs r1, #10
	subs r3, r3, r7
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	adds r0, r7, r0
	str r0, [r5, #16]
	movs r1, #10
	mov r0, r11
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #9
	adds r0, r0, r3
	str r0, [r5, #24]
	str r0, [r5, #28]
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_080dc040
	adds r6, #1
	add r11, r0
	cmp r6, #10
	ble .L_080dbfd0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080dc040:
	.4byte 0xffff4000
