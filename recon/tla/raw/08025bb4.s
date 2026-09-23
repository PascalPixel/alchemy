.syntax unified
	.thumb
	.global Script_SetOrCompareUnsignedHalfword
	.global Func_08025bb4
	.thumb_func
Script_SetOrCompareUnsignedHalfword:
Func_08025bb4:
	push	{lr}
	adds	r4, r2, #0
	cmp	r1, #0
	bne.n	.L_08025bc0
	strh	r4, [r0, #6]
	b.n	.L_08025be0
.L_08025bc0:
	cmp	r1, #1
	bne.n	.L_08025bcc
	ldrh	r3, [r0, #6]
	adds	r3, r3, r4
	strh	r3, [r0, #6]
	b.n	.L_08025be0
.L_08025bcc:
	ldrh	r2, [r0, #6]
	lsls	r3, r4, #16
	lsrs	r3, r3, #16
	movs	r1, #0
	cmp	r2, r3
	bne.n	.L_08025bda
	movs	r1, #1
.L_08025bda:
	adds	r3, r0, #0
	adds	r3, #87
	strb	r1, [r3, #0]
.L_08025be0:
	pop	{pc}
