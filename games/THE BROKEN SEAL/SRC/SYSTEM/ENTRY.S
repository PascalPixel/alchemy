@ Start-up code entered from the branch at 0x08000000. It switches the CPU to
@ IRQ mode and then to System mode with msr CPSR_fc, loading each mode's stack
@ pointer, stores 0x08000770 in the interrupt vector at 0x03007ffc and calls the
@ Thumb routine at 0x08002e00 with mov lr, pc; bx r1, starting over if that
@ returns. Neither approved compiler emits msr or loads sp from a literal.
@ Every TBS and TLA edition has this routine; TLA SYSTEM/ENTRY.S differs only
@ in its call target and in the add r0, pc offset to its runtime image.
@ credit: library — TBS/TLA common GBA ARM start-up entry
.syntax unified
.text
	.arm
	.global Entry_080003c0
Entry_080003c0:
	mov	r0, #18
	msr	CPSR_fc, r0
	ldr	sp, [pc, #40]
	mov	r0, #31
	msr	CPSR_fc, r0
	ldr	sp, [pc, #24]
	ldr	r1, [pc, #28]
	add	r0, pc, #908
	str	r0, [r1]
	ldr	r1, [pc, #20]
	mov	lr, pc
	bx	r1
	b Entry_080003c0
	.4byte 0x03007f00
	.4byte 0x03007fa0
	.4byte 0x03007ffc
	.4byte 0x08002e01
