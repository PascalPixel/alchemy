.syntax unified
	.thumb
.L_081c05e8:
	movs	r1, #178
	movs	r2, #255
	b.n	.L_081c05f8
AlchemyUncredited_081c05ee:
	.2byte	0x0000
AlchemyUncreditedEnd_081c05ee:
	.global SoundDriver_MidiKeyToFrequency
	.thumb_func
SoundDriver_MidiKeyToFrequency:
	ldr	r0, [r0, #4]
	mov	ip, r4
	cmp	r1, #179
	bge.n	.L_081c05e8
.L_081c05f8:
	add	r4, pc, #136
	ldrb	r4, [r1, r4]
	lsls	r1, r4, #28
	lsrs	r1, r1, #25
	add	r3, pc, #32
	adds	r1, r1, r3
	ldmia	r1, {r1, r3}
	lsrs	r4, r4, #4
	lsrs	r1, r4
	lsrs	r3, r4
	movs	r4, #0
	lsls	r2, r2, #24
	bx	pc
AlchemyUncredited_081c0612:
	nop
	.arm
AlchemyUncreditedEnd_081c0612:
	umlalne	r4, r1, r3, r2
	umull	r4, r0, r1, r0
	mov	r4, ip
	bx	lr
