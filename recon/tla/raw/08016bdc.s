.syntax unified
	.thumb
	.global Func_08016bdc
	.thumb_func
Func_08016bdc:
	push	{r5, r6, lr}
	movs	r0, #3
	sub	sp, #4
	bl	Audio_PlayCue
	bl	Func_08016180
	ldr	r2, [pc, #152]
	ldr	r3, [pc, #152]
	movs	r1, #19
.L_08016bf0:
	subs	r1, #1
	strh	r3, [r2, #0]
	subs	r2, #2
	subs	r3, #1
	cmp	r1, #0
	bge.n	.L_08016bf0
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	ldr	r1, [pc, #136]
	ldr	r2, [pc, #136]
	bl	Bios_CpuSet
	movs	r0, #3
	bl	Func_080167d8
.L_08016c10:
	ldr	r0, [pc, #120]
	bl	Func_08016854
	ldr	r6, [pc, #124]
.L_08016c18:
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08016c2e
	movs	r0, #128
	movs	r1, #160
	lsls	r0, r0, #20
	lsls	r1, r1, #2
	bl	Func_0801680c
.L_08016c2e:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08016c42
	movs	r1, #160
	ldr	r0, [pc, #92]
	lsls	r1, r1, #2
	bl	Func_0801680c
.L_08016c42:
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08016c5c
	movs	r5, #156
	lsls	r5, r5, #6
	adds	r5, #15
.L_08016c52:
	subs	r5, #1
	bl	0x08016bd8
	cmp	r5, #0
	bge.n	.L_08016c52
.L_08016c5c:
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08016c7a
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #28]
	ldr	r1, [pc, #44]
	adds	r2, #160
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_08016c10
.L_08016c7a:
	movs	r0, #1
	bl	WaitFrames
	b.n	.L_08016c18
	movs	r0, r0
	.4byte 0x06002426
	.4byte 0xfffff093
	.4byte 0x02010000
	.4byte 0x05000100
	.4byte 0x03001150
	.4byte 0x08001000
	.4byte 0x020055d0
	.2byte 0x1000
	.2byte 0x0600
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	push	{lr}
	cmp	r0, #7
	bhi.n	.L_08016cb6
	movs	r3, #166
	lsls	r3, r3, #1
	ldr	r2, [pc, #44]
	muls	r3, r0
	adds	r0, r3, r2
	b.n	.L_08016cda
.L_08016cb6:
	adds	r3, r0, #0
	subs	r3, #128
	cmp	r3, #5
	bhi.n	.L_08016cd8
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #216
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_08016cd8
	movs	r3, #166
	lsls	r3, r3, #1
	muls	r3, r0
	adds	r3, r2, r3
	ldr	r2, [pc, #12]
	adds	r0, r3, r2
	b.n	.L_08016cda
.L_08016cd8:
	movs	r0, #0
.L_08016cda:
	pop	{pc}
	.4byte 0x02000520
	.2byte 0x5a00
	.2byte 0xffff
