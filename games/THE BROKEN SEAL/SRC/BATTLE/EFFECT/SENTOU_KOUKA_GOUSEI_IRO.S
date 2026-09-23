@ ARM colour-blend instruction templates (IroJun, IroGyaku) for the
@ battle-effect blitter generator, which DMA-copies one of them into the
@ routine it builds. They add colour bytes lane by lane against the mask the
@ generated routine keeps in sl and have no return: ARM templates, not
@ compiled code. Every TBS and TLA edition has these bytes (TLA 0x08197230).
@ credit: library — TBS/TLA common ARM colour-blend instruction templates for the battle-effect code generator
.syntax unified
.text
	.arm
	.set AlchemyLoadAddress, 0x080eefa4
	.global SentouKouka_IroJun
	.global SentouKouka_IroGyaku
SentouKouka_IroJun:
	ldr	r8, [r9]
	ldrb	r5, [r1], #1
	add	r8, r8, r5
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #8
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #16
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #24
	and	r5, r8, sl
	bic	r8, r8, sl
	sub	r5, r5, r5, lsr #6
	orr	r8, r8, r5
	str	r8, [r9], #4
SentouKouka_IroGyaku:
	ldr	r8, [r9]
	ldrb	r5, [r1], #-1
	add	r8, r8, r5
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #8
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #16
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #24
	and	r5, r8, sl
	bic	r8, r8, sl
	sub	r5, r5, r5, lsr #6
	orr	r8, r8, r5
	str	r8, [r9], #4
