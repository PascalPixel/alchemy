@ ARM colour-blend instruction templates (IroJun at 0x08197230, IroGyaku at
@ 0x08197268) for the battle-effect code generator, whose literal pool names
@ them (0x08196900 and 0x081968fc). They add colour bytes lane by lane
@ against the mask the generated routine keeps in sl and have no return:
@ templates copied into generated code, not compiled functions. These 112
@ bytes equal TBS BATTLE/EFFECT/SENTOU_KOUKA_GOUSEI_IRO.S (0x080eefa4) byte
@ for byte; PATCH_TEMPLATE.S follows them.
@ credit: library — TBS/TLA common ARM colour-blend instruction templates for the battle-effect code generator
.syntax unified
.text
	.arm
	.set AlchemyLoadAddress, 0x08197230
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
