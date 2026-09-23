@ The cartridge header branches here. This ARM routine sets the IRQ and
@ system stacks through msr mode switches, points the IRQ vector at
@ 0x03007ffc to the interrupt dispatcher's ROM image at 0x080006b8 (the
@ runtime later repoints it to its IWRAM copy) and enters AgbMain through
@ bx. Neither approved compiler emits msr. It equals TBS SYSTEM/ENTRY.S
@ (0x080003c0) apart from the AgbMain pointer and the add r0, pc offset to
@ the runtime image.
@ credit: library — TBS/TLA common GBA ARM start-up entry
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000428
	.global Entry
Entry:
	mov	r0, #18
	msr	CPSR_fc, r0
	ldr	sp, [pc, #40]
	mov	r0, #31
	msr	CPSR_fc, r0
	ldr	sp, [pc, #24]
	ldr	r1, [pc, #28]
	add	r0, pc, #0x26c
	str	r0, [r1]
	ldr	r1, [pc, #20]
	mov	lr, pc
	bx	r1
	b	Entry
	.4byte	0x03007f00
	.4byte	0x03007fa0
	.4byte	0x03007ffc
	.4byte	0x0801319d
