.syntax unified
	.thumb
	.global Text_LookupSymbolCode
Text_LookupSymbolCode:
	.global Text_LookupSymbol
	.thumb_func
Text_LookupSymbol:
	bx	pc
AlchemyUncredited_0803857e:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0803857e:
	lsr	r3, r1, #8
	ldr	ip, [pc, #80]
	add	ip, ip, r3, lsl #3
	ldm	ip, {r2, r4}
	ands	r1, r1, #255
	beq	.L_080385b0
.L_08038598:
	ldrb	r3, [r4], #1
	add	r2, r2, r3
	cmp	r3, #255
	beq	.L_08038598
	subs	r1, r1, #1
	bne	.L_08038598
.L_080385b0:
	mov	r3, #1
	ands	r4, r2, #3
	beq	.L_080385d0
	rsbs	r4, r3, r4, lsl #3
	bic	r2, r2, #3
	ldr	r3, [r2], #4
	rrx	r3, r3
	lsr	r3, r3, r4
.L_080385d0:
	mov	r1, #0
	stm	r0, {r1, r2, r3}
	bx	lr
	.4byte	Text_MessageBanks
	.global Text_LookupSymbolCodeSize
	.set Text_LookupSymbolCodeSize, . - Text_LookupSymbolCode
