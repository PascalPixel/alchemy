@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b010c
	.thumb_func
Func_080b010c:
	push	{r5, lr}
	movs	r1, #167
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #4
	bl	Func_080048b0
	adds	r5, r0, #0
	bl	Func_0808a460
	movs	r3, #0
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #160]
	ldr	r2, [pc, #164]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r2, r5, r3
	movs	r3, #12
	strb	r3, [r2, #0]
	ldr	r2, [pc, #152]
	adds	r0, r5, r2
	bl	Func_08077158
	ldr	r2, [pc, #148]
	adds	r3, r5, r2
	strb	r0, [r3, #0]
	bl	Func_08004080
	movs	r2, #228
	lsls	r2, r2, #2
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #132]
	movs	r1, #128
	bl	Func_08003fa4
	bl	Func_08004080
	ldr	r2, [pc, #124]
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #124]
	movs	r1, #128
	bl	Func_08003fa4
	bl	Func_08004080
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #108]
	movs	r1, #128
	bl	Func_08003fa4
	bl	Func_08004080
	ldr	r2, [pc, #100]
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #96]
	movs	r1, #128
	bl	Func_08003fa4
	bl	Func_08004080
	ldr	r2, [pc, #88]
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #88]
	movs	r1, #128
	bl	Func_08003fa4
	bl	Func_08004080
	movs	r3, #230
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r2, [pc, #72]
	strh	r0, [r5, #0]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #64]
	bl	Func_080041d8
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x8500029c
	.4byte 0x0000036e
	.4byte 0x000003a7
	.4byte 0x080b3940
	.4byte 0x00000392
	.4byte 0x080b3b40
	.4byte 0x080b3bc0
	.4byte 0x00000396
	.4byte 0x080b39c0
	.4byte 0x0000039a
	.4byte 0x080b3a40
	.4byte 0x080b3ac0
	.4byte 0x080b00f5
