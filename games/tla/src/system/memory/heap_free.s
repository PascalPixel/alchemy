@ Heap_Release, as HeapSlot_Release (release_heap_block.s) before it:
@ hand-written, scratching r4 for the 0x03000000 base. The ARM after it is
@ ☀️'s heapfree.s tail: a halt (SWI 3) and Irq_StopOnGamePakRemoved,
@ which every edition here keeps (☀️ only DE and ES), falling through on
@ no cartridge interrupt as shipped.
.syntax unified
	.thumb
	.global Heap_Release
	.thumb_func
Heap_Release:
	movs	r4, #3
	lsls	r4, r4, #24
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
AlchemyUncredited_08013172:
	.2byte	0x0000
AlchemyUncreditedEnd_08013172:
	.arm
	.global Irq_HaltOnGamePakRemoved
Irq_HaltOnGamePakRemoved:
	swi	0x30000
	b	. - 8
	.global Irq_StopOnGamePakRemoved
Irq_StopOnGamePakRemoved:
	mov	ip, #0x04000000
	add	r3, ip, #0x200
	ldr	r2, [r3]
	and	r1, r2, r2, lsr #16
	mov	r2, #0
	ands	r0, r1, #0x2000
	strbne	r0, [r3, #-0x17c]
.L_gamepak_stop:
	bne	.L_gamepak_stop
