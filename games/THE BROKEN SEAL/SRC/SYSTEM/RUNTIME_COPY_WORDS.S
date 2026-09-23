@ Word copier of the resident runtime: 256-byte blocks through eight-
@ register ldm and stmia, the first block entered part-way into its
@ unrolled run with add pc, pc, ip, lsr #2, then single words. It saves
@ r5-r9 without lr. GCC never jumps into unrolled code and saves lr with
@ any register. These 120 bytes equal TLA SYSTEM/IWRAM_RUNTIME_COPY_WORDS.S
@ after its Thumb entry (0x08000cec, run at 0x03000734).
@ credit: library — TBS/TLA common IWRAM runtime word copier
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08001af8
	.set AlchemyRunAddress, 0x03001388

@ 二百五十六バイト単位で語列を転送する常駐処理。
	.global IwramCopyWords
IwramCopyWords:
	push	{r5, r6, r7, r8, r9}
	ands	ip, r2, #224, 0
	rsb	ip, ip, #240, 0
	add	pc, pc, ip, lsr #2
.L_audio_1398:
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
	subs	r2, r2, #256
	bpl	.L_audio_1398
	ands	r2, r2, #28, 0
	beq	.L_audio_13f8
.L_audio_13e8:
	ldm	r1!, {r3}
	stmia	r0!, {r3}
	subs	r2, r2, #4, 0
	bgt	.L_audio_13e8
.L_audio_13f8:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
