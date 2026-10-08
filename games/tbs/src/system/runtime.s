.syntax unified
	.arm
	.align 2

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
	ldrne	ip, [pc, #184]
	bne	.L_irq_dispatch
	ands	r0, r1, #1
	ldrne	ip, [pc, #168]
	bne	.L_irq_dispatch
	ands	r0, r1, #4
	ldrne	ip, [pc, #164]
	bne	.L_irq_dispatch
	ands	r0, r1, #0x40
	ldrne	ip, [pc, #168]
	bne	.L_irq_dispatch
	ands	r0, r1, #0x80
	ldrne	ip, [pc, #160]
	bne	.L_irq_dispatch
	ands	r0, r1, #0x10
	ldrne	ip, [pc, #136]
	bne	.L_irq_dispatch
	ands	r0, r1, #0x20
	ldrne	ip, [pc, #128]
	bne	.L_irq_dispatch
	ands	r0, r1, #0x1000
	ldrne	ip, [pc, #144]
	bne	.L_irq_dispatch
	ands	r0, r1, #0x2000
	ldr	ip, [pc, #136]
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
	.global IwramIrqModeWord
IwramIrqModeWord:
	orr	r3, r3, #0x92
	msr	CPSR_fc, r3
	pop	{r0, r1, r2, r3, r4, r5, lr}
	strh	r2, [r3]
	strh	r1, [r3, #8]
	msr	SPSR_fc, r0
	bx	lr
