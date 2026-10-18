@ MemFree, as Runtime_ReleaseHeapBlock (release_heap_block.s) before it:
@ hand-written, scratching r4 for the 0x03000000 base. The ARM after it is
@ ☀️'s heapfree.s tail: a halt (SWI 3) and Runtime_StopOnGamePakIrq,
@ which every edition here keeps (☀️ only DE and ES), falling through on
@ no cartridge interrupt as shipped.
.syntax unified
	.thumb
	.global MemFree
	.thumb_func
MemFree:
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
	.global Runtime_HaltOnGamePakIrq
Runtime_HaltOnGamePakIrq:
	swi	0x30000
	b	. - 8
	.global Runtime_StopOnGamePakIrq
Runtime_StopOnGamePakIrq:
	mov	ip, #0x04000000
	add	r3, ip, #0x200
	ldr	r2, [r3]
	and	r1, r2, r2, lsr #16
	mov	r2, #0
	ands	r0, r1, #0x2000
	strbne	r0, [r3, #-0x17c]
.L_gamepak_stop:
	bne	.L_gamepak_stop
