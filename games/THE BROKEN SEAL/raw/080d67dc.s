.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080b5040, 0x080b5040
	.global Overlay_080d67dc
Overlay_080d67dc:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #64]
	movs	r1, #128
	ldr	r6, [r3, #124]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	strh	r3, [r1, #0]
	mov	r8, r1
	ldr	r1, [pc, #52]
	movs	r3, #32
	strh	r3, [r1, #6]
	movs	r3, #201
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldrh	r1, [r2, #0]
	movs	r0, #1
	movs	r2, #24
	sub	sp, #4
	bl	sub_080b5040
	movs	r5, #0
	mov	r4, sp
	str	r5, [r4, #0]
	ldr	r3, [pc, #24]
	adds	r0, r4, #0
	adds	r1, r6, #0
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r5, [r4, #0]
	b.n	.L_080d6834
	.4byte 0x00000001
	.4byte 0x03001e74
	.4byte 0x03001ad0
	.4byte 0x040000d4
	.2byte 0x1000
	.2byte 0x8500
.L_080d6834:
	adds	r0, r4, #0
	ldr	r1, [pc, #52]
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	mov	r1, r8
	ldr	r2, [pc, #32]
	strh	r3, [r1, #0]
	movs	r3, #120
	str	r3, [r2, #16]
	add	sp, #4
	b.n	.L_080d687c
	.4byte 0x0000100e
	.4byte 0x00003f46
	.4byte 0x00007741
	.4byte 0x06004000
	.4byte 0x85001000
	.4byte 0x04000052
	.2byte 0x1ce0
	.2byte 0x0300
.L_080d687c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
