@ Why assembly: syHeapSlotRelease branches on lsrs flags without cmp/cmn
@ (lsrs r3, r1, #22 / beq); alchemy-gcc's Thumb cbranchsi4 and
@ negated_cbranchsi4 always emit a compare.
@ Why syHaltOnGamePakRemoved is assembly:
@ swi 0x30000 is a BIOS software interrupt (Halt), and b . - 8 branches back
@ to the same swi.
@ Why syStopOnGamePakRemoved is assembly:
@ it has a reachable exit with no return or interworking branch: when
@ (IE & IF & 0x2000) is zero, its final bne falls straight into the Thumb
@ syInit, the next object's first function, while the CPU is still
@ in ARM mode. Neither a procedure return nor a tail call can have this linkage.
@ syHeapRelease, as syHeapSlotRelease before it:
@ hand-written, scratching r4 for the 0x03000000 base. The ARM after it is
@ ☀️'s heap_release.s tail: a halt (SWI 3) and syStopOnGamePakRemoved,
@ which every edition here keeps (☀️ only DE and ES), falling through on
@ no cartridge interrupt as shipped.
	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global syHeapSlotRelease
	.thumb_func
syHeapSlotRelease:
	movs	r4, #3
	lsls	r4, r4, #24
	ldr	r1, [r0, r4]
	lsrs	r3, r1, #22
	beq.n	.L_08013160
	movs	r2, #0
	str	r2, [r0, r4]
	movs	r0, #4
	ands	r3, r0
	str	r1, [r3, r4]
.L_08013160:
	bx	lr
AlchemyUncredited08013162:
	.2byte	0x0000
AlchemyUncreditedEnd08013162:

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syHeapRelease
	.thumb_func
syHeapRelease:
	movs	r4, #3
	lsls	r4, r4, #24
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
AlchemyUncredited08013172:
	.2byte	0x0000
AlchemyUncreditedEnd08013172:
	.arm
	.global syHaltOnGamePakRemoved
syHaltOnGamePakRemoved:
	swi	0x30000
	b	. - 8
	.global syStopOnGamePakRemoved
syStopOnGamePakRemoved:
	mov	ip, #0x04000000
	add	r3, ip, #0x200
	ldr	r2, [r3]
	and	r1, r2, r2, lsr #16
	mov	r2, #0
	ands	r0, r1, #0x2000
	strbne	r0, [r3, #-0x17c]
.L_gamepak_stop:
	bne	.L_gamepak_stop
