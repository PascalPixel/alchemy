.syntax unified
	.thumb
	.set sub_08009080, 0x08009080
	.set sub_08009150, 0x08009150
	.set sub_080091e0, 0x080091e0
	.set sub_08009220, 0x08009220
	.set sub_0809163c, 0x0809163c
	.set sub_080916b0, 0x080916b0
	.set sub_08091750, 0x08091750
	.set sub_08092054, 0x08092054
	.set sub_080920e8, 0x080920e8
	.set sub_08092158, 0x08092158
	.set sub_080923c4, 0x080923c4
	.set sub_08092adc, 0x08092adc
	.global FieldEffect_UpdateGridPlacement
	.global Func_08093e28
	.thumb_func
FieldEffect_UpdateGridPlacement:
Func_08093e28:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #340]
	movs	r1, #250
	mov	r8, r0
	lsls	r1, r1, #1
	add	r1, r8
	ldr	r0, [r1, #0]
	mov	fp, r1
	sub	sp, #24
	bl	sub_08092054
	adds	r6, r0, #0
	ldr	r3, [pc, #320]
	movs	r2, #10
	ldrsh	r5, [r6, r2]
	movs	r1, #18
	ldrsh	r7, [r6, r1]
	ands	r5, r3
	ands	r7, r3
	movs	r0, #8
	movs	r2, #8
	adds	r0, r0, r5
	adds	r2, r2, r7
	mov	sl, r0
	mov	r9, r2
	bl	sub_080916b0
	movs	r3, #249
	lsls	r3, r3, #1
	add	r8, r3
	mov	r0, r8
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_08093f2e
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_08093e82
	adds	r3, r5, #0
	adds	r3, #23
.L_08093e82:
	asrs	r2, r3, #4
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_08093e8e
	adds	r3, r7, #0
	adds	r3, #23
.L_08093e8e:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r2, r3
	ldr	r1, [pc, #252]
	ldr	r0, [pc, #256]
	lsls	r3, r3, #2
	adds	r2, r3, r1
	adds	r3, r3, r0
	ldrb	r2, [r2, #2]
	ldrb	r3, [r3, #2]
	cmp	r2, r3
	bne.n	.L_08093f72
	ldr	r3, [r6, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r1, [pc, #236]
	ldr	r3, [r6, #12]
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl	sub_08009220
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_08093f72
	mov	r2, fp
	ldr	r0, [r2, #0]
	mov	r1, sl
	mov	r2, r9
	bl	sub_08092158
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #48]
	movs	r1, #192
	mov	r3, fp
	movs	r2, #0
	ldr	r0, [r3, #0]
	lsls	r1, r1, #8
	bl	sub_08092adc
	mov	r1, fp
	ldr	r0, [r1, #0]
	bl	sub_080920e8
	adds	r3, r6, #0
	adds	r3, #90
	movs	r5, #1
	strb	r5, [r3, #0]
	subs	r3, #5
	strb	r7, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_080091e0
	adds	r0, r6, #0
	movs	r1, #13
	bl	sub_08009080
	mov	r2, sl
	lsls	r1, r2, #16
	ldr	r3, [pc, #144]
	ldr	r2, [r6, #12]
	mov	r0, r9
	adds	r2, r2, r3
	lsls	r3, r0, #16
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	adds	r0, r6, #0
	bl	sub_08009150
	mov	r1, fp
	ldr	r0, [r1, #0]
	bl	sub_080923c4
	mov	r2, r8
	strb	r5, [r2, #0]
	b.n	.L_08093f6a
.L_08093f2e:
	adds	r0, r6, #0
	movs	r1, #10
	bl	sub_08009080
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	ldr	r3, [r6, #12]
	adds	r0, r6, #0
	str	r3, [r6, #20]
	movs	r1, #1
	bl	sub_080091e0
	movs	r0, #6
	bl	sub_0809163c
	movs	r5, #0
	mov	r3, r8
	adds	r2, r6, #0
	strb	r5, [r3, #0]
	adds	r2, #90
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
.L_08093f6a:
	bl	sub_08091750
	movs	r0, #0
	b.n	.L_08093f7a
.L_08093f72:
	bl	sub_08091750
	movs	r0, #1
	negs	r0, r0
.L_08093f7a:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x0000fff0
	.4byte 0x02010000
	.4byte 0x02010200
	.4byte 0xfff00000
