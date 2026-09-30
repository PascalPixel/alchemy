.syntax unified
	.thumb
	.global Func_080430c4
	.thumb_func
Func_080430c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	movs r1, #60
	sub sp, #16
	adds r5, r0, #0
	bl Math_ModU
	movs r1, #60
	adds r6, r0, #0
	adds r0, r5, #0
	bl Math_DivU
	movs r3, #100
	adds r5, r0, #0
	movs r1, #60
	adds r0, r6, #0
	muls r0, r3
	bl Math_DivU
	movs r1, #60
	mov r11, r0
	adds r0, r5, #0
	bl Math_ModU
	movs r1, #60
	mov r9, r0
	adds r0, r5, #0
	bl Math_DivU
	movs r1, #60
	adds r5, r0, #0
	bl Math_ModU
	movs r1, #60
	adds r6, r0, #0
	adds r0, r5, #0
	bl Math_DivU
	mov r10, sp
	adds r1, r0, #0
	movs r2, #3
	mov r0, r10
	bl UiText_FormatNumber
	ldrb r3, [r0]
	adds r0, #1
	strb r3, [r7]
	adds r5, r7, #1
	ldrb r3, [r0]
	adds r6, #100
	strb r3, [r5]
	adds r5, #1
	ldrb r3, [r0, #1]
	adds r1, r6, #0
	strb r3, [r5]
	movs r3, #58
	adds r5, #1
	strb r3, [r5]
	mov r0, r10
	movs r2, #2
	mov r8, r3
	bl UiText_FormatNumber
	ldrb r3, [r0]
	adds r5, #1
	strb r3, [r5]
	adds r5, #1
	ldrb r3, [r0, #1]
	movs r2, #2
	strb r3, [r5]
	mov r3, r8
	adds r5, #1
	strb r3, [r5]
	movs r3, #100
	add r9, r3
	mov r1, r9
	mov r0, r10
	bl UiText_FormatNumber
	ldrb r3, [r0]
	adds r5, #1
	strb r3, [r5]
	adds r5, #1
	ldrb r3, [r0, #1]
	movs r2, #2
	strb r3, [r5]
	mov r3, r8
	adds r5, #1
	strb r3, [r5]
	movs r3, #100
	add r11, r3
	mov r0, r10
	mov r1, r11
	bl UiText_FormatNumber
	ldrb r3, [r0]
	adds r5, #1
	strb r3, [r5]
	adds r5, #1
	ldrb r3, [r0, #1]
	add sp, #16
	strb r3, [r5]
	movs r3, #0
	adds r0, r7, #0
	strb r3, [r5, #1]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
