@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08016018
	.thumb_func
Func_08016018:
	push	{r5, r6, lr}
	mov	r6, r9
	push	{r6}
	adds	r6, r0, #0
	ldr	r1, [pc, #160]
	movs	r0, #15
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r5, r0, #0
	mov	r4, sp
	str	r3, [r4, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #144]
	adds	r1, r5, #0
	ldr	r2, [pc, #144]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #144]
	adds	r3, r5, r1
	movs	r1, #1
	strb	r1, [r3, #0]
	ldr	r3, [pc, #140]
	adds	r2, r5, r3
	movs	r3, #99
	strh	r3, [r2, #0]
	ldr	r2, [pc, #136]
	adds	r3, r5, r2
	strb	r1, [r3, #0]
	ldr	r3, [pc, #132]
	adds	r2, r5, r3
	movs	r3, #15
	strb	r3, [r2, #0]
	ldr	r3, [pc, #128]
	adds	r0, r4, #0
	str	r3, [r4, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #124]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	Func_08015ef4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #112]
	bl	Func_080041d8
	adds	r0, r6, #0
	bl	Func_08017464
	add	r1, sp, #4
	mov	r9, r1
	ldr	r0, [pc, #100]
	movs	r1, #128
	bl	Func_08015fb8
	add	r2, sp, #4
	mov	r9, r2
	movs	r1, #129
	ldr	r0, [pc, #92]
	bl	Func_08015fb8
	add	r3, sp, #4
	mov	r9, r3
	movs	r1, #130
	ldr	r0, [pc, #84]
	bl	Func_08015fb8
	ldr	r1, [pc, #80]
	movs	r2, #4
	movs	r3, #2
	adds	r5, r5, r1
.L0:
	subs	r3, #1
	strb	r2, [r5, #0]
	subs	r5, #1
	cmp	r3, #0
	bge.n	.L0
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000012fc
	.4byte 0x040000d4
	.4byte 0x850004bf
	.4byte 0x00000ea3
	.4byte 0x000012b6
	.4byte 0x00000ea5
	.4byte 0x00000ea7
	.4byte 0xf000f000
	.4byte 0x85000140
	.4byte 0x080160fd
	.4byte 0x0000f013
	.4byte 0x0000f014
	.4byte 0x0000f015
	.4byte 0x00000da2
