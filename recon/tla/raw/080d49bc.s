.syntax unified
	.thumb
	.global Func_080d49bc
	.thumb_func
Func_080d49bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r1, [r6, #104]
	cmp r1, #0
	beq .L_080d4a48
	ldr r2, [r1, #8]
	ldr r3, [r6, #8]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080d49dc
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r0, r0, r2
.L_080d49dc:
	ldr r2, [r1, #16]
	ldr r3, [r6, #16]
	asrs r5, r0, #16
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080d49f0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080d49f0:
	asrs r0, r0, #16
	mov r8, r0
	mov r2, r8
	mov r3, r8
	muls r3, r2
	adds r0, r5, #0
	muls r0, r5
	adds r0, r0, r3
	ldr r3, .L_080d4a50
	mov lr, r3
	.2byte 0xf800
	adds r3, r6, #0
	adds r3, #100
	movs r2, #0
	ldrsh r7, [r3, r2]
	cmp r0, r7
	blt .L_080d4a40
	lsls r0, r5, #20
	adds r1, r7, #0
	bl Math_Div
	ldr r5, [r6, #8]
	mov r3, r8
	adds r5, r5, r0
	adds r1, r7, #0
	lsls r0, r3, #20
	bl Math_Div
	ldr r3, [r6, #16]
	adds r1, r5, #0
	adds r3, r3, r0
	ldr r2, [r6, #12]
	adds r0, r6, #0
	bl Object_SetPosition
	adds r0, r6, #0
	movs r1, #2
	bl Object_SetMode
	b .L_080d4a48
.L_080d4a40:
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
.L_080d4a48:
	movs r0, #1
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080d4a50:
	.4byte IwramFillWords + 0x74
