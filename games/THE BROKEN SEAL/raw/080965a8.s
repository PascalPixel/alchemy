.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08003f3c, 0x08003f3c
	.set sub_08009080, 0x08009080
	.set sub_080090c8, 0x080090c8
	.set sub_080090d0, 0x080090d0
	.set sub_080091e0, 0x080091e0
	.set sub_08009240, 0x08009240
	.set sub_08009248, 0x08009248
	.set sub_08092054, 0x08092054
	.set sub_08092560, 0x08092560
	.set sub_080925cc, 0x080925cc
	.set sub_08092adc, 0x08092adc
	.set sub_080958a8, 0x080958a8
	.set sub_080958e4, 0x080958e4
	.set sub_08096c48, 0x08096c48
	.set sub_080974d8, 0x080974d8
	.set sub_0809ba70, 0x0809ba70
	.set sub_0809ba7c, 0x0809ba7c
	.set sub_0809ba90, 0x0809ba90
	.set sub_080b0048, 0x080b0048
	.set sub_080b0050, 0x080b0050
	.set sub_080f9010, 0x080f9010
	.global Overlay_080965a8
Overlay_080965a8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #52
	adds	r5, r0, #0
	bl	sub_08092054
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080965c6
	b.n	.L_080967be
.L_080965c6:
	bl	sub_080958a8
	ldr	r3, [pc, #516]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #516]
	str	r3, [sp, #4]
	bl	sub_080b0048
	movs	r0, #30
	bl	sub_080030f8
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	movs	r0, #173
	bl	sub_080f9010
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_080925cc
	movs	r0, #175
	bl	sub_080f9010
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_080925cc
	movs	r0, #20
	bl	sub_080030f8
	movs	r0, #152
	bl	sub_080f9010
	movs	r1, #3
	movs	r2, #14
	adds	r0, r5, #0
	bl	sub_08092560
	movs	r0, #152
	bl	sub_080f9010
	movs	r1, #5
	movs	r2, #16
	adds	r0, r5, #0
	bl	sub_08092560
	movs	r0, #152
	bl	sub_080f9010
	movs	r1, #7
	movs	r2, #18
	adds	r0, r5, #0
	bl	sub_08092560
	movs	r0, #20
	bl	sub_080030f8
	ldr	r3, [r6, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	mov	r9, r1
	add	r1, sp, #20
	movs	r3, #0
	str	r1, [sp, #0]
	mov	sl, r3
	mov	r8, r6
	mov	fp, r1
	movs	r7, #7
.L_08096654:
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	mov	r0, r9
	bl	sub_080090c8
	ldr	r3, [sp, #0]
	adds	r5, r0, #0
	stmia	r3!, {r5}
	adds	r2, r3, #0
	str	r2, [sp, #0]
	cmp	r5, #0
	beq.n	.L_080966be
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r2, r5, #0
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	subs	r2, #50
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #324]
	str	r3, [r5, #108]
	ldrh	r3, [r6, #6]
	movs	r1, #9
	strh	r3, [r5, #6]
	bl	sub_08009240
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08009080
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_080091e0
	mov	r1, sl
	ldr	r0, [r5, #80]
	bl	sub_08096c48
	mov	r1, r8
	str	r1, [r5, #104]
	mov	sl, r0
	mov	r8, r5
.L_080966be:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L_08096654
	mov	r2, sl
	ldrb	r2, [r2, #28]
	movs	r0, #153
	mov	r8, r2
	bl	sub_080f9010
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r7, #14
.L_080966da:
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r6, #12]
	movs	r0, #1
	subs	r7, #1
	bl	sub_080030f8
	cmp	r7, #0
	bge.n	.L_080966da
	adds	r0, r6, #0
	bl	sub_080090d0
	mov	r5, fp
	movs	r7, #7
.L_080966fa:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	sub_080090d0
	cmp	r7, #0
	bge.n	.L_080966fa
	mov	r2, r8
	cmp	r2, #96
	beq.n	.L_08096712
	mov	r0, r8
	bl	sub_08003f3c
.L_08096712:
	movs	r0, #10
	bl	sub_080030f8
	ldr	r5, [pc, #192]
	movs	r3, #250
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl	sub_08092adc
	movs	r0, #20
	bl	sub_080030f8
	ldr	r0, [r5, #0]
	bl	sub_08092054
	movs	r1, #28
	bl	sub_08009080
	movs	r0, #20
	bl	sub_080030f8
	ldr	r3, [r6, #8]
	add	r5, sp, #8
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	bl	sub_080974d8
	ldr	r6, [sp, #4]
	movs	r7, #23
	adds	r6, #88
.L_0809675e:
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	movs	r1, #240
	bl	sub_0809ba90
	adds	r0, r6, #0
	ldr	r1, [pc, #112]
	bl	sub_0809ba7c
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_0809ba70
	ldr	r0, [r6, #0]
	movs	r1, #9
	bl	sub_08009248
	subs	r7, #1
	movs	r0, #1
	bl	sub_080030f8
	adds	r6, #72
	cmp	r7, #0
	bge.n	.L_0809675e
	movs	r0, #120
	bl	sub_080030f8
	ldr	r2, [sp, #4]
	movs	r1, #2
	adds	r2, #152
	movs	r7, #23
.L_0809679e:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080967a8
	strb	r1, [r2, #0]
.L_080967a8:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L_0809679e
	movs	r0, #50
	bl	sub_080030f8
	bl	sub_080b0050
	bl	sub_080958e4
.L_080967be:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x00204084
	.4byte 0x08096575
	.4byte 0x02000240
	.4byte 0x0809641d
