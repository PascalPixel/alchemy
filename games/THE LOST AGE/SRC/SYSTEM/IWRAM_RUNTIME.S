@ Interrupt dispatcher at the head of TLA's resident runtime. The routine at
@ 0x080133c4 copies 4,096 bytes with DMA3 from 0x080006b8 to 0x03000100
@ (control 0x84000400): the runtime's 4,000 bytes (this module through
@ IWRAM_RUNTIME_MIX_MODES.S) and the first 96 bytes of the byte-LZ decoder
@ after it, which land at 0x030010a0..0x03001100. It points the interrupt
@ vector at 0x03007ffc to this first word and fills the fourteen handler
@ words at 0x030001e4 from the ROM table at 0x080178b4 (control
@ 0x8400000e). The dispatcher saves SPSR with mrs, switches to System mode
@ with msr CPSR_fc around each handler and returns from it through a Thumb
@ stub (mov lr, r5; bx pc). mrs and msr have no C form. The runtime's data
@ carries no credit: the handler words (IWRAM_RUNTIME_IRQ_TABLE.S), its
@ transform matrix at 0x03000478 (IWRAM_RUNTIME_MATRIX.S), a filler word at
@ 0x0300087c (in IWRAM_RUNTIME_MIX.S) and the mixer's variables at
@ 0x03000ae8 (in IWRAM_RUNTIME_MIX_LANES.S), 0x03000af8
@ (IWRAM_RUNTIME_MIX_STATE.S), 0x03000d1c (IWRAM_RUNTIME_MIX_RENDER_STATE.S)
@ and 0x03000ee4 (in IWRAM_RUNTIME_MIX_RENDER.S). After their Thumb entries
@ its 16.16 multiply, fill, square root and bit-run helpers, decompressor
@ and word copier equal TBS SYSTEM/RUNTIME_MUL_Q16.S, RUNTIME_MATH.S,
@ RUNTIME_DECOMPRESS.S and RUNTIME_COPY_WORDS.S; several of its other
@ routines share runs with the TBS runtime modules but are laid out apart.
@ credit: handwritten — TLA IWRAM runtime interrupt dispatcher
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080006b8
	.set AlchemyRunAddress, 0x03000100
	.global IwramIrqMain
IwramIrqMain:
	mov	r3, #0x4000000
	ldr	r2, [r3, #512]!
	ldrh	r1, [r3, #8]
	mrs	r0, SPSR
	push	{r0, r1, r2, r3, r4, r5, lr}
	mvns	r0, r1, lsl #31
	strhmi	r0, [r3, #8]
	and	r1, r2, r2, lsr #16
	ands	r0, r1, #2
	ldrne	ip, [pc, #188]
	bne	.L_03000188
	ands	r0, r1, #1
	ldrne	ip, [pc, #172]
	bne	.L_03000188
	ands	r0, r1, #4
	ldrne	ip, [pc, #168]
	bne	.L_03000188
	ands	r0, r1, #64
	ldrne	ip, [pc, #172]
	bne	.L_03000188
	ands	r0, r1, #128
	ldrne	ip, [pc, #164]
	bne	.L_03000188
	ands	r0, r1, #16
	ldrne	ip, [pc, #140]
	bne	.L_03000188
	ands	r0, r1, #32
	ldrne	ip, [pc, #132]
	bne	.L_03000188
	ands	r0, r1, #0x1000
	ldrne	ip, [pc, #148]
	bne	.L_03000188
	ands	r0, r1, #0x2000
	ldr	ip, [pc, #140]
.L_03000188:
	bic	r2, r2, r0
	mov	r1, #0x2000
	orr	r1, r1, #196
	and	r1, r1, r2
	orr	r1, r1, r0, lsl #16
	str	r1, [r3]
	mrs	r3, CPSR
	bic	r3, r3, #223
	orr	r3, r3, #31
	msr	CPSR_fc, r3
	mov	r5, lr
	add	lr, pc, #1
	bx	ip
	.thumb
	mov	lr, r5
	bx	pc
	.arm
	mrs	r3, CPSR
	bic	r3, r3, #223
	orr	r3, r3, #146
	msr	CPSR_fc, r3
	pop	{r0, r1, r2, r3, r4, r5, lr}
	strh	r2, [r3]
	strh	r1, [r3, #8]
	msr	SPSR_fc, r0
	bx	lr
