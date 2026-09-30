.syntax unified
	.thumb
	.global UiText_MeasureStringVariant
	.thumb_func
UiText_MeasureStringVariant:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #92
	str r1, [sp, #8]
	mov r8, r3
	ldr r3, .L_08018c54
	add r1, sp, #28
	ldr r5, [r3]
	movs r3, #15
	str r3, [r1]
	str r3, [r1, #4]
	str r3, [r1, #8]
	str r3, [r1, #12]
	str r3, [r1, #16]
	str r3, [r1, #20]
	str r3, [r1, #24]
	str r3, [r1, #28]
	str r3, [r1, #32]
	str r3, [r1, #36]
	str r3, [r1, #40]
	str r3, [r1, #44]
	str r3, [r1, #48]
	str r3, [r1, #52]
	str r3, [r1, #56]
	str r3, [r1, #60]
	add r3, sp, #20
	mov lr, r2
	mov r9, r3
	movs r2, #0
	add r3, sp, #12
	str r2, [sp, #4]
	mov r10, r2
	movs r4, #0
	movs r6, #0
	movs r7, #0
	mov r12, r2
	mov r11, r3
.L_08018aa4:
	movs r2, #235
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r5, r3]
	ldr r3, .L_08018c58
	adds r0, #1
	ands r0, r3
	cmp r2, #31
	bls .L_08018ae0
	cmp r2, #32
	bne .L_08018ac2
	adds r4, #5
	adds r6, #1
	b .L_08018aa4
.L_08018ac2:
	ldr r3, .L_08018c5c
	subs r2, #32
	lsls r2, r2, #5
	ldrh r2, [r3, r2]
	ldr r3, .L_08018c60
	adds r3, r5, r3
	ldrh r3, [r3]
	str r3, [sp, #0]
	cmp r3, #1
	beq .L_08018ada
	cmp r3, #5
	bne .L_08018adc
.L_08018ada:
	adds r2, #1
.L_08018adc:
	adds r4, r4, r2
	b .L_08018aa4
.L_08018ae0:
	cmp r2, #28
	bhi .L_08018aa4
	lsls r3, r2, #2
	ldr r2, .L_08018c64
	ldr r3, [r3, r2]
	mov pc, r3
.L_08018aec:
	.4byte .L_08018b92
	.4byte .L_08018bac
	.4byte .L_08018aa4
	.4byte .L_08018b60
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018bde
	.4byte .L_08018bce
	.4byte .L_08018bde
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018bc6
	.4byte .L_08018bde
	.4byte .L_08018aa4
	.4byte .L_08018bde
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018aa4
	.4byte .L_08018bc6
.L_08018b60:
	mov r3, r9
	mov r2, r12
	adds r6, #1
	strh r6, [r3, r2]
	mov r3, r11
	strh r4, [r3, r2]
	cmp r7, #0
	bne .L_08018b76
	cmp r10, r4
	bcs .L_08018b76
	mov r10, r4
.L_08018b76:
	ldr r3, [sp, #4]
	cmp r3, #2
	bhi .L_08018b84
	adds r3, #1
	str r3, [sp, #4]
	lsls r3, r3, #1
	mov r12, r3
.L_08018b84:
	lsls r2, r7, #2
	ldr r3, [r1, r2]
	adds r3, #15
	movs r6, #0
	movs r4, #0
	str r3, [r1, r2]
	b .L_08018aa4
.L_08018b92:
	mov r2, r9
	mov r3, r12
	adds r6, #1
	strh r6, [r2, r3]
	mov r2, r11
	strh r4, [r2, r3]
	cmp r7, #0
	bne .L_08018ba8
	cmp r10, r4
	bcs .L_08018ba8
	mov r10, r4
.L_08018ba8:
	adds r7, #1
	b .L_08018be6
.L_08018bac:
	mov r2, r9
	mov r3, r12
	adds r6, #1
	strh r6, [r2, r3]
	mov r2, r11
	strh r4, [r2, r3]
	cmp r7, #0
	bne .L_08018bc2
	cmp r10, r4
	bcs .L_08018bc2
	mov r10, r4
.L_08018bc2:
	adds r7, #1
	b .L_08018aa4
.L_08018bc6:
	ldr r3, .L_08018c58
	adds r0, #1
	ands r0, r3
	b .L_08018bde
.L_08018bce:
	movs r2, #235
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r5, r3]
	ldr r3, .L_08018c60
	adds r3, r5, r3
	strh r2, [r3]
.L_08018bde:
	ldr r3, .L_08018c58
	adds r0, #1
	ands r0, r3
	b .L_08018aa4
.L_08018be6:
	ldr r2, .L_08018c68
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08018bf4
	movs r3, #2
	add r10, r3
.L_08018bf4:
	movs r4, #0
	cmp r4, r7
	bcs .L_08018c20
	adds r0, r1, #0
	adds r5, r0, #0
.L_08018bfe:
	cmp r4, #0
	bne .L_08018c0a
	ldr r3, [r5]
	mov r2, lr
	str r3, [r2]
	b .L_08018c18
.L_08018c0a:
	mov r2, lr
	ldr r3, [r2]
	ldr r2, [r0]
	cmp r3, r2
	bcs .L_08018c18
	mov r3, lr
	str r2, [r3]
.L_08018c18:
	adds r4, #1
	adds r0, #4
	cmp r4, r7
	bcc .L_08018bfe
.L_08018c20:
	ldr r3, [sp, #8]
	mov r2, r10
	str r2, [r3]
	mov r3, r10
	adds r3, #19
	lsrs r3, r3, #3
	lsls r3, r3, #3
	subs r3, #16
	mov r2, r8
	mov r10, r3
	cmp r2, #0
	beq .L_08018c9a
	movs r6, #0
	movs r5, #0
.L_08018c3c:
	mov r2, r9
	ldrh r3, [r5, r2]
	cmp r3, #1
	bhi .L_08018c6c
	ldr r3, .L_08018c50
	mov r2, r8
	strh r3, [r2]
	movs r3, #2
	b .L_08018c8e
	.2byte 0x0000
.L_08018c50:
	.4byte 0x00000000
.L_08018c54:
	.4byte Data_03001e8c
.L_08018c58:
	.4byte 0x000001ff
.L_08018c5c:
	.4byte UiText_Glyphs
.L_08018c60:
	.4byte 0x00000eac
.L_08018c64:
	.4byte .L_08018aec
.L_08018c68:
	.4byte 0x00000ea4
.L_08018c6c:
	mov r2, r11
	ldrh r3, [r5, r2]
	mov r2, r10
	subs r0, r2, r3
	subs r0, #4
	cmp r0, #0
	bge .L_08018c7c
	movs r0, #0
.L_08018c7c:
	mov r3, r9
	ldrh r1, [r5, r3]
	lsls r0, r0, #8
	subs r1, #1
	bl FixedPoint_Ratio
	mov r2, r8
	movs r3, #2
	strh r0, [r2]
.L_08018c8e:
	add r8, r3
	ldr r2, [sp, #4]
	adds r6, #1
	adds r5, #2
	cmp r6, r2
	bls .L_08018c3c
.L_08018c9a:
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
