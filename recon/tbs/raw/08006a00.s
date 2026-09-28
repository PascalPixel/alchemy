@ Flash library code whose C (recon/tbs/en/main/08006a00.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_08006a00
	.type	 Func_08006a00,function
	.thumb_func
Func_08006a00:
	push	{r4, r5, lr}
	lsl	r0, r0, #24
	lsr	r0, r0, #24
	ldr	r1, .L3
	lsl	r2, r0, #1
	add	r2, r2, r0
	lsl	r2, r2, #1
	ldr	r0, [r1]
	add	r2, r2, r0
	ldr	r1, .L3+4
	ldr	r3, .L3+8
	ldrh	r0, [r3]
	strh	r0, [r1]
	mov	r5, #0
	strh	r5, [r3]
	ldr	r4, .L3+12
	ldr	r0, .L3+16
	ldrb	r0, [r0]
	mov	r1, #8
	lsl	r1, r1, r0
	ldrh	r0, [r4]
	orr	r0, r0, r1
	strh	r0, [r4]
	mov	r0, #1
	strh	r0, [r3]
	ldr	r0, .L3+20
	strb	r5, [r0]
	ldr	r1, .L3+24
	ldrh	r0, [r2]
	strh	r0, [r1]
	add	r2, r2, #2
	ldr	r3, .L3+28
	ldr	r0, [r3]
	ldrh	r1, [r2]
	strh	r1, [r0]
	add	r0, r0, #2
	str	r0, [r3]
	ldrh	r1, [r2, #2]
	strh	r1, [r0]
	sub	r0, r0, #2
	str	r0, [r3]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
.L4:
	.align	2, 0
.L3:
	.word	Data_02004c18
	.word	Data_02004c2c
	.word	Data_04000208
	.word	Data_04000200
	.word	gFlashTimerNum
	.word	gFlashTimeoutFlag
	.word	gFlashTimerCount
	.word	gFlashTimerReg
.Lfe1:
	.size	 Func_08006a00,.Lfe1-Func_08006a00

