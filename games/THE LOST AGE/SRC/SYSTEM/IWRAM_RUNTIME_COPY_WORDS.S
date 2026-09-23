@ Word copier of TLA's resident runtime, behind a 4-byte Thumb entry
@ (bx pc): 256-byte blocks through eight-register ldm and stmia, the first
@ entered part-way into its unrolled run with add pc, pc, ip, lsr #2, then
@ single words; it saves r5-r9 without lr. GCC never jumps into unrolled
@ code and saves lr with any register. After the entry these 120 bytes
@ equal TBS SYSTEM/RUNTIME_COPY_WORDS.S (0x08001af8, run at 0x03001388).
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common IWRAM runtime word copier
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000ce8
	.set AlchemyRunAddress, 0x03000730
	.thumb
AlchemyUncredited_08000ce8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000ce8:
	push	{r5, r6, r7, r8, r9}
	ands	ip, r2, #224
	rsb	ip, ip, #240
	add	pc, pc, ip, lsr #2
.L_03000744:
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	subs	r2, r2, #0x100
	bpl	.L_03000744
	ands	r2, r2, #28
	beq	.L_030007a4
.L_03000794:
	ldm	r1!, {r3}
	stmia	r0!, {r3}
	subs	r2, r2, #4
	bgt	.L_03000794
.L_030007a4:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
