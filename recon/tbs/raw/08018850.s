.syntax unified
	.thumb
	.global UiText_MeasureEntryDimensions
	.thumb_func
UiText_MeasureEntryDimensions:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	str r1, [sp, #12]
	str r2, [sp, #8]
	adds r6, r3, #0
	ldr r3, .L_080189f0
	ldr r4, [r3]
	ldr r3, .L_080189f4
	movs r2, #0
	mov r10, r2
	mov lr, r3
	movs r2, #24
	movs r3, #16
	movs r1, #15
	movs r5, #0
	add r2, sp
	add r3, sp
	mov r11, r1
	movs r7, #0
	movs r1, #0
	mov r8, r2
	mov r12, r5
	mov r9, r3
.L_0801888a:
	movs r2, #235
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r4, r3]
	ldr r3, .L_080189f8
	adds r0, #1
	ands r0, r3
	cmp r2, #31
	bls .L_080188c6
	cmp r2, #32
	bne .L_080188a8
	adds r1, #5
	adds r5, #1
	b .L_0801888a
.L_080188a8:
	ldr r3, .L_080189fc
	subs r2, #32
	lsls r2, r2, #5
	ldrh r2, [r3, r2]
	ldr r3, .L_08018a00
	adds r3, r4, r3
	ldrh r3, [r3]
	str r3, [sp, #0]
	cmp r3, #1
	beq .L_080188c0
	cmp r3, #5
	bne .L_080188c2
.L_080188c0:
	adds r2, #1
.L_080188c2:
	adds r1, r1, r2
	b .L_0801888a
.L_080188c6:
	cmp r2, #28
	bhi .L_0801888a
	lsls r3, r2, #2
	mov r2, lr
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080188d4:
	.4byte .L_080189a2
	.4byte .L_080189a2
	.4byte .L_0801888a
	.4byte .L_08018948
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801897e
	.4byte .L_08018986
	.4byte .L_0801897e
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_08018978
	.4byte .L_0801897e
	.4byte .L_0801888a
	.4byte .L_0801897e
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_0801888a
	.4byte .L_08018978
.L_08018948:
	mov r3, r8
	mov r2, r12
	adds r5, #1
	strh r5, [r3, r2]
	mov r3, r9
	strh r1, [r3, r2]
	cmp r7, r1
	bcs .L_0801895a
	adds r7, r1, #0
.L_0801895a:
	mov r3, r10
	cmp r3, #2
	bhi .L_0801896a
	movs r1, #1
	add r10, r1
	mov r2, r10
	lsls r2, r2, #1
	mov r12, r2
.L_0801896a:
	ldr r2, .L_080189f4
	movs r3, #15
	movs r5, #0
	movs r1, #0
	add r11, r3
	mov lr, r2
	b .L_0801888a
.L_08018978:
	ldr r3, .L_080189f8
	adds r0, #1
	ands r0, r3
.L_0801897e:
	ldr r3, .L_080189f8
	adds r0, #1
	ands r0, r3
	b .L_0801888a
.L_08018986:
	movs r2, #235
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r4, r3]
	ldr r3, .L_08018a00
	adds r3, r3, r4
	strh r2, [r3]
	ldr r3, .L_080189f8
	ldr r2, .L_080189f4
	adds r0, #1
	ands r0, r3
	mov lr, r2
	b .L_0801888a
.L_080189a2:
	mov r3, r8
	mov r2, r12
	adds r5, #1
	strh r5, [r3, r2]
	mov r3, r9
	strh r1, [r3, r2]
	cmp r7, r1
	bcs .L_080189b4
	adds r7, r1, #0
.L_080189b4:
	ldr r1, .L_08018a04
	adds r3, r4, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080189c0
	adds r7, #2
.L_080189c0:
	ldr r2, [sp, #12]
	str r7, [r2]
	ldr r1, [sp, #8]
	mov r3, r11
	str r3, [r1]
	adds r3, r7, #0
	adds r3, #19
	lsrs r7, r3, #3
	lsls r3, r7, #3
	adds r7, r3, #0
	subs r7, #16
	cmp r6, #0
	beq .L_08018a3e
	movs r2, #0
	movs r5, #0
.L_080189de:
	mov r1, r8
	ldrh r3, [r5, r1]
	cmp r3, #1
	bhi .L_08018a08
	ldr r3, .L_080189ec
	strh r3, [r6]
	b .L_08018a34
.L_080189ec:
	.4byte 0x00000000
.L_080189f0:
	.4byte gWindowWork
.L_080189f4:
	.4byte .L_080188d4
.L_080189f8:
	.4byte 0x000001ff
.L_080189fc:
	.4byte UiText_Glyphs
.L_08018a00:
	.4byte 0x00000eac
.L_08018a04:
	.4byte 0x00000ea4
.L_08018a08:
	mov r1, r9
	ldrh r3, [r5, r1]
	subs r0, r7, r3
	subs r0, #4
	cmp r0, #0
	bge .L_08018a16
	movs r0, #0
.L_08018a16:
	mov r3, r8
	ldrh r1, [r5, r3]
	lsls r0, r0, #8
	subs r1, #1
	str r2, [sp, #4]
	bl FixedPoint_Ratio
	movs r1, #192
	lsls r1, r1, #4
	ldr r2, [sp, #4]
	cmp r0, r1
	bls .L_08018a32
	movs r0, #128
	lsls r0, r0, #2
.L_08018a32:
	strh r0, [r6]
.L_08018a34:
	adds r6, #2
	adds r2, #1
	adds r5, #2
	cmp r2, r10
	bls .L_080189de
.L_08018a3e:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
