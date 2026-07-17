@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801ca1c
	.thumb_func
Func_0801ca1c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r7, [pc, #160]
	ldr	r2, [pc, #160]
	adds	r3, r7, r2
	mov	r9, r0
	ldrb	r0, [r3, #0]
	movs	r1, #24
	adds	r0, #12
	sub	sp, #4
	bl	Func_080022fc
	ldr	r2, [pc, #148]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	subs	r3, #7
	lsls	r5, r5, #18
	mov	sl, r3
	asrs	r5, r5, #16
	ldr	r3, [pc, #136]
	adds	r0, r5, #0
	movs	r1, #96
	mov	r8, r3
	bl	Func_080022fc
	lsls	r0, r0, #16
	mov	r2, r8
	asrs	r0, r0, #16
	ldrb	r6, [r2, r0]
	adds	r0, r5, #0
	movs	r1, #96
	adds	r0, #32
	bl	Func_080022fc
	adds	r5, #64
	mov	r3, r8
	ldrb	r7, [r3, r0]
	movs	r1, #96
	adds	r0, r5, #0
	bl	Func_080022fc
	mov	r3, r8
	ldrb	r2, [r3, r0]
	add	r6, sl
	add	r7, sl
	add	r2, sl
	cmp	r6, #0
	bge.n	.L0
	movs	r6, #0
.L0:
	cmp	r7, #0
	bge.n	.L1
	movs	r7, #0
.L1:
	cmp	r2, #0
	bge.n	.L2
	movs	r2, #0
.L2:
	cmp	r6, #31
	ble.n	.L3
	movs	r6, #31
.L3:
	cmp	r7, #31
	ble.n	.L4
	movs	r7, #31
.L4:
	cmp	r2, #31
	ble.n	.L5
	movs	r2, #31
.L5:
	ldr	r3, [pc, #48]
	add	r3, r9
	strh	r6, [r3, #0]
	movs	r3, #175
	lsls	r3, r3, #3
	add	r3, r9
	strh	r7, [r3, #0]
	ldr	r3, [pc, #40]
	add	r3, r9
	strh	r2, [r3, #0]
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x08036750
	.4byte 0x00000576
	.4byte 0x0000057a
