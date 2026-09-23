@ Interrupt dispatcher at the head of the resident runtime. The start-up
@ routine at 0x0800300c copies the 5,120-byte runtime (this module through
@ RUNTIME_COPY_WORDS.S) with DMA3 from 0x08000770 to 0x03000000, points the
@ interrupt vector at 0x03007ffc at this first word and fills the handler
@ words of RUNTIME_IRQ_TABLE.S from the ROM table at 0x08007320. The
@ dispatcher saves SPSR with mrs, picks one handler by priority, switches
@ to System mode with msr CPSR_fc around the call (add lr, pc, #0; bx ip)
@ and back. mrs and msr have no C form.
@ credit: handwritten — IWRAM runtime interrupt dispatcher
.syntax unified
	.arm
	.align 2
	.set AlchemyLoadAddress, 0x08000770
	.set AlchemyRunAddress, 0x03000000

	.global IwramIrqMain
IwramIrqMain:
	mov	r3, #0x04000000
	ldr	r2, [r3, #0x200]!
	ldrh	r1, [r3, #8]
	mrs	r0, SPSR
	push	{r0, r1, r2, r3, r4, r5, lr}
	mov	r0, #1
	strh	r0, [r3, #8]
	and	r1, r2, r2, lsr #16
	ands	r0, r1, #2
	ldrne	ip, [pc, #184]	@ IwramIrqHBlank
	bne	.L_irq_dispatch
	ands	r0, r1, #1
	ldrne	ip, [pc, #168]	@ IwramIrqVBlank
	bne	.L_irq_dispatch
	ands	r0, r1, #4
	ldrne	ip, [pc, #164]	@ IwramIrqVCount
	bne	.L_irq_dispatch
	ands	r0, r1, #0x40
	ldrne	ip, [pc, #168]	@ IwramIrqTimer3
	bne	.L_irq_dispatch
	ands	r0, r1, #0x80
	ldrne	ip, [pc, #160]	@ IwramIrqSerial
	bne	.L_irq_dispatch
	ands	r0, r1, #0x10
	ldrne	ip, [pc, #136]	@ IwramIrqTimer1
	bne	.L_irq_dispatch
	ands	r0, r1, #0x20
	ldrne	ip, [pc, #128]	@ IwramIrqTimer2
	bne	.L_irq_dispatch
	ands	r0, r1, #0x1000
	ldrne	ip, [pc, #144]	@ IwramIrqKeypad
	bne	.L_irq_dispatch
	ands	r0, r1, #0x2000
	ldr	ip, [pc, #136]	@ IwramIrqGamePak
.L_irq_dispatch:
	strh	r0, [r3, #2]
	mov	r1, #0x20c0
	bic	r2, r2, r0
	and	r1, r1, r2
	strh	r1, [r3]
	mrs	r3, CPSR
	bic	r3, r3, #0xdf
	orr	r3, r3, #0x1f
	msr	CPSR_fc, r3
	mov	r5, lr
	add	lr, pc, #0
	bx	ip
	mov	lr, r5
	mrs	r3, CPSR
	bic	r3, r3, #0xdf
	orr	r3, r3, #0x92
	msr	CPSR_fc, r3
	pop	{r0, r1, r2, r3, r4, r5, lr}
	strh	r2, [r3]
	strh	r1, [r3, #8]
	msr	SPSR_fc, r0
	bx	lr
