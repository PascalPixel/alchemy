	.include "games/common/include/game/ed_asm.h"
.syntax unified
	.arm

	.global IwramExpandBitRuns
IwramExpandBitRuns:
	.ifeq EDITION_INTERNATIONAL
	add	ip, r0, #24
	.else
	add	ip, r0, #28
	.endif
.L_expand_halfword:
	mov	r4, r1
	ldrh	r3, [r0], #2
	bic	r3, r3, #3
	lsls	r3, r3, #17
	bcc	.L_expand_zero
.L_expand_store:
	strb	r2, [r4], #1
.L_expand_shift:
	lsls	r3, r3, #1
	bcs	.L_expand_store
.L_expand_zero:
	add	r4, r4, #1
	bne	.L_expand_shift
	add	r1, r1, #16
	cmp	r0, ip
	bne	.L_expand_halfword
	bx	lr
