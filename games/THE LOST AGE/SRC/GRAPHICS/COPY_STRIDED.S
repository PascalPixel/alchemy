@ Strided copy kernel, copied and run as one 128-byte unit (size and source
@ {0x80, 0x080217a4} in its loader's literal pool at 0x08022c70). Behind a
@ 4-byte Thumb entry (bx pc) the ARM body returns before saving anything
@ when either dimension rounds down to zero (bics; bicsne; bxeq lr), saves
@ r5-fp without lr and subtracts with sub r0, r0, r1; GCC saves lr with any
@ register and writes register subtraction as rsb. After that entry its
@ 124-byte body equals TBS GRAPHICS/TILE/COPY_STRIDED.S (0x0800a418) byte for
@ byte.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, strided copy kernel
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080217a4
	.global Render_CopyStrided
	.thumb_func
Render_CopyStrided:
AlchemyUncredited_080217a4:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080217a4:
	bics	r1, r1, #7
	bicsne	r2, r2, #7
	bxeq	lr
	push	{r5, r6, r7, r8, r9, sl, fp}
.L_080217b8:
	mov	ip, r1
.L_080217bc:
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0!, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	subs	ip, ip, #8
	subgt	r0, r0, r1, lsl #3
	bgt	.L_080217bc
	sub	r0, r0, r1
	subs	r2, r2, #8
	bgt	.L_080217b8
	pop	{r5, r6, r7, r8, r9, sl, fp}
	bx	lr
