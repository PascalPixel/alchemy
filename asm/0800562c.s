@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800562c
	.thumb_func
Func_0800562c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	ip, r2
	ldr	r2, [pc, #56]
	lsrs	r2, r2, #2
	lsls	r3, r2, #2
	adds	r6, r1, #0
	movs	r4, #132
	mov	r1, sp
	subs	r1, r1, r3
	lsls	r4, r4, #24
	mov	r7, sp
	adds	r5, r0, #0
	mov	r8, sp
	ldr	r3, [pc, #40]
	mov	sp, r1
	ldr	r0, [pc, #40]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	r2, ip
	bl	Func_08007318
	mov	sp, r8
	mov	sp, r7
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000005c
	.4byte 0x040000d4
	.4byte 0x0800203c
