.syntax unified
	.thumb
	.global UiText_FormatNumber
	.thumb_func
UiText_FormatNumber:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #0
	adds r6, r1, #0
	adds r7, r0, #0
	mov r10, r2
	mov r11, r3
	cmp r6, #0
	bge .L_0803ae3a
	cmp r2, #0
	bne .L_0803ae38
	movs r3, #1
	mov r11, r3
.L_0803ae38:
	negs r6, r6
.L_0803ae3a:
	movs r3, #32
	strb r3, [r7]
	ldr r3, .L_0803aed4
	adds r5, r7, #0
	mov r8, r3
	adds r5, #12
	mov r9, r7
.L_0803ae48:
	movs r2, #10
	adds r0, r6, #0
	mov r1, r8
	negs r2, r2
	bl Math_UnsignedMulHighAdd
	adds r0, #48
	strb r0, [r5]
	mov r1, r8
	adds r0, r6, #0
	bl Math_UnsignedMulHigh
	subs r5, #1
	adds r6, r0, #0
	cmp r5, r9
	bne .L_0803ae48
	movs r0, #0
	strb r0, [r7, #13]
	movs r1, #32
	movs r0, #1
	movs r4, #45
	adds r2, r7, #0
	b .L_0803ae7a
.L_0803ae76:
	adds r2, #1
	adds r0, #1
.L_0803ae7a:
	cmp r0, #13
	beq .L_0803ae94
	ldrb r3, [r2, #1]
	cmp r3, #48
	bne .L_0803ae8c
	cmp r0, #12
	beq .L_0803ae76
	strb r1, [r2, #1]
	b .L_0803ae76
.L_0803ae8c:
	mov r3, r11
	cmp r3, #0
	beq .L_0803ae94
	strb r4, [r2]
.L_0803ae94:
	mov r3, r10
	cmp r3, #0
	bne .L_0803aeb6
	ldrb r3, [r7]
	movs r0, #0
	cmp r3, #32
	bne .L_0803aeb2
	adds r2, r7, #0
.L_0803aea4:
	adds r0, #1
	cmp r0, #12
	beq .L_0803aeb2
	adds r2, #1
	ldrb r3, [r2]
	cmp r3, #32
	beq .L_0803aea4
.L_0803aeb2:
	adds r0, r7, r0
	b .L_0803aec6
.L_0803aeb6:
	mov r3, r10
	cmp r3, #12
	bls .L_0803aec0
	movs r3, #12
	mov r10, r3
.L_0803aec0:
	mov r3, r10
	subs r0, r7, r3
	adds r0, #13
.L_0803aec6:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803aed4:
	.4byte 0x1999999a
