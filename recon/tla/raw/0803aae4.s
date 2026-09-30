.syntax unified
	.thumb
	.global UiText_RenderWideStringAtOffset
	.thumb_func
UiText_RenderWideStringAtOffset:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r7, r2, #0
	ldr r6, [r3, #60]
	lsls r3, r7, #16
	asrs r3, r3, #16
	adds r5, r0, #0
	sub sp, #4
	mov r10, r1
	movs r4, #0
	mov r9, r3
	cmp r5, #0
	beq .L_0803ab0c
	b .L_0803ac3a
.L_0803ab0c:
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #66
	adds r1, r6, r3
	ldrh r3, [r1]
	movs r2, #244
	lsls r2, r2, #4
	lsls r3, r3, #1
	adds r3, r3, r2
	strh r4, [r6, r3]
	adds r5, r6, r2
	ldrh r3, [r1]
	ldr r2, .L_0803ab30
	adds r3, #1
	ands r3, r2
	strh r3, [r1]
	b .L_0803ac3a
	.2byte 0x0000
.L_0803ab30:
	.4byte 0x000001ff
.L_0803ab34:
	cmp r4, #30
	bls .L_0803ab3c
	cmp r4, #176
	bne .L_0803abee
.L_0803ab3c:
	subs r1, r4, #3
	cmp r1, #26
	bls .L_0803ab44
	b .L_0803ac3a
.L_0803ab44:
	ldr r2, .L_0803ac58
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0803ab4c:
	.4byte .L_0803abe2
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803abdc
	.4byte .L_0803abb8
	.4byte .L_0803abc2
	.4byte .L_0803abcc
	.4byte .L_0803abd8
	.4byte .L_0803abd8
	.4byte .L_0803ac3a
	.4byte .L_0803abea
	.4byte .L_0803abea
	.4byte .L_0803ac3a
	.4byte .L_0803abd8
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803ac3a
	.4byte .L_0803abea
	.4byte .L_0803abd8
.L_0803abb8:
	movs r1, #240
	ldrh r2, [r5]
	lsls r1, r1, #4
	adds r1, #62
	b .L_0803abd4
.L_0803abc2:
	movs r1, #240
	ldrh r2, [r5]
	lsls r1, r1, #4
	adds r1, #60
	b .L_0803abd4
.L_0803abcc:
	movs r1, #240
	ldrh r2, [r5]
	lsls r1, r1, #4
	adds r1, #56
.L_0803abd4:
	adds r3, r6, r1
	strh r2, [r3]
.L_0803abd8:
	adds r5, #2
	b .L_0803ac3a
.L_0803abdc:
	bl UiWork_ResetCounters
	b .L_0803ac3a
.L_0803abe2:
	movs r2, #15
	mov r7, r9
	add r8, r2
	b .L_0803ac3a
.L_0803abea:
	adds r5, #2
	b .L_0803abd8
.L_0803abee:
	mov r3, r10
	ldrh r2, [r3, #22]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	bne .L_0803ac28
	ldrh r0, [r5]
	cmp r4, #32
	bls .L_0803ac28
	cmp r0, #32
	bls .L_0803ac28
	ldr r1, .L_0803ac5c
	adds r3, r4, #0
	adds r2, r0, #0
	subs r3, #32
	subs r2, #32
	lsls r3, r3, #5
	lsls r2, r2, #5
	ldrh r3, [r1, r3]
	ldrh r2, [r1, r2]
	movs r1, #240
	adds r3, r3, r2
	lsls r3, r3, #16
	lsls r1, r1, #12
	cmp r3, r1
	bhi .L_0803ac28
	lsls r3, r0, #8
	orrs r4, r3
	adds r5, #2
.L_0803ac28:
	movs r3, #0
	str r3, [sp, #0]
	adds r2, r7, #0
	mov r0, r10
	adds r1, r4, #0
	mov r3, r8
	bl Func_0803bde4
	adds r7, r7, r0
.L_0803ac3a:
	ldrh r4, [r5]
	adds r5, #2
	cmp r4, #255
	bls .L_0803ac44
	movs r4, #64
.L_0803ac44:
	cmp r4, #0
	beq .L_0803ac4a
	b .L_0803ab34
.L_0803ac4a:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803ac58:
	.4byte .L_0803ab4c
.L_0803ac5c:
	.4byte UiText_Glyphs
