@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801f818
	.thumb_func
Func_0801f818:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #0
	ldr	r5, [pc, #356]
	sub	sp, #28
	mov	r8, r0
	bl	Func_08077300
	ldr	r2, [pc, #352]
	ldr	r3, [pc, #352]
	str	r0, [r2, #0]
	movs	r0, #128
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #348]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	str	r1, [r2, #4]
	str	r1, [r3, #0]
	ldr	r3, [pc, #344]
	ldr	r0, [pc, #344]
	ldrb	r1, [r3, #0]
	adds	r3, r2, r0
	strb	r1, [r3, #0]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r0, [r2, #0]
	bl	Func_08077008
	adds	r7, r5, #0
	adds	r6, r0, #0
	adds	r1, r5, #0
	subs	r7, #16
	adds	r2, r6, #0
	movs	r5, #11
.L0:
	ldrb	r3, [r2, #0]
	subs	r5, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r5, #0
	bge.n	.L0
	ldrb	r3, [r6, #15]
	ldr	r5, [pc, #280]
	strb	r3, [r7, #28]
	ldr	r3, [r5, #4]
	movs	r2, #224
	str	r3, [r7, #32]
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r2, #2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	Func_0808a5d0
	strh	r0, [r7, #30]
	ldr	r0, [pc, #268]
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	strb	r3, [r7, #29]
	ldr	r3, [r5, #16]
	movs	r0, #0
	str	r3, [r7, #36]
	bl	Func_08077290
	adds	r3, r7, #0
	adds	r3, #40
	strb	r0, [r3, #0]
	movs	r0, #1
	bl	Func_08077290
	adds	r3, r7, #0
	adds	r3, #41
	strb	r0, [r3, #0]
	movs	r0, #2
	bl	Func_08077290
	adds	r3, r7, #0
	adds	r3, #42
	strb	r0, [r3, #0]
	movs	r0, #3
	bl	Func_08077290
	adds	r3, r7, #0
	adds	r3, #43
	mov	r6, sp
	strb	r0, [r3, #0]
	movs	r5, #0
	adds	r0, r6, #0
	bl	Func_08077158
	ldrh	r3, [r6, r5]
	cmp	r3, #255
	beq.n	.L1
	adds	r1, r7, #0
	adds	r0, r6, #0
	adds	r1, #44
	movs	r2, #0
.L2:
	ldrh	r3, [r2, r0]
	adds	r5, #1
	strb	r3, [r1, #0]
	adds	r2, #2
	adds	r1, #1
	cmp	r5, #3
	bgt.n	.L1
	ldrh	r3, [r2, r6]
	cmp	r3, #255
	bne.n	.L2
.L1:
	movs	r1, #1
	adds	r2, r5, #0
	negs	r1, r1
	adds	r3, r1, #0
	adds	r2, #44
	strb	r3, [r7, r2]
	ldr	r0, [pc, #156]
	ldr	r2, [pc, #132]
	adds	r3, r2, r0
	ldrb	r1, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #52
	strb	r1, [r3, #0]
	ldr	r1, [pc, #148]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #53
	strb	r3, [r1, #0]
	ldr	r3, [pc, #140]
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	adds	r2, r7, #0
	adds	r2, #49
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #50
	movs	r0, #0
	strb	r0, [r3, #0]
	movs	r5, #48
	adds	r6, r3, #0
.L4:
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L3
	ldrb	r3, [r6, #0]
	adds	r3, #1
	strb	r3, [r6, #0]
.L3:
	adds	r5, #1
	cmp	r5, #127
	ble.n	.L4
	movs	r0, #32
	bl	Func_080770c0
	negs	r3, r0
	orrs	r3, r0
	adds	r2, r7, #0
	lsrs	r3, r3, #31
	adds	r2, #51
	strb	r3, [r2, #0]
	ldr	r3, [pc, #44]
	movs	r1, #242
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #72]
	movs	r5, #0
	lsls	r1, r1, #2
	strh	r3, [r7, #54]
	b.n	.L5
.L6:
	ldmia	r2!, {r3}
	adds	r5, #1
	add	r8, r3
.L5:
	cmp	r5, r1
	blt.n	.L6
	mov	r0, r8
	str	r0, [r7, #60]
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000000
	.4byte 0x02000240
	.4byte 0x03001c9c
	.4byte 0x02001000
	.4byte 0x03001d08
	.4byte 0x0000022a
	.4byte 0x00000129
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x0000020f
	.4byte 0x02000040
