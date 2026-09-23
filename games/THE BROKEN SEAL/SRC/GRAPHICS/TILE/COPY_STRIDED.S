@ Gathers 8-byte rows from a bitmap whose rows lie r1 bytes apart into
@ consecutive 64-byte tiles: four rows (ldm of two registers each) per 32-byte
@ stmia r3!, {r4-fp}, two per tile. It runs from a RAM copy of exactly these
@ 124 bytes. It returns before saving anything when either
@ dimension rounds down to zero (bics; bicsne; bxeq lr), saves r5-fp without
@ lr and subtracts with sub r0, r0, r1; GCC saves lr with any register and
@ writes register subtraction as rsb. Every TBS and TLA edition has these
@ bytes.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank
.syntax unified
	.arm
	.global Func_0800a418
Func_0800a418:
	bics	r1, r1, #7
	bicsne	r2, r2, #7
	bxeq	lr
	push	{r5, r6, r7, r8, r9, sl, fp}
.L_0800a428:
	mov	ip, r1
.L_0800a42c:
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
	bgt	.L_0800a42c
	sub	r0, r0, r1
	subs	r2, r2, #8
	bgt	.L_0800a428
	pop	{r5, r6, r7, r8, r9, sl, fp}
	bx	lr
