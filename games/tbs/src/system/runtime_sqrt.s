@ C2: movs r1, r0 copies and tests without a shift. The compilers' copy-and-test
@ (*movsi_compare0) is subs r1, r0, #0; a register-to-register movs comes only
@ with a shift.
@ Secondary: the sign exit that follows is bxmi lr. *movsi_compare0 sets plain
@ CC, where a signed < 0 test is lt; mi is the CC_NOOV spelling, which a
@ copy-and-test never has.
.syntax unified
	.arm

	.global IwramSqrt
IwramSqrt:
	movs	r1, r0
	mov	r0, #0
	bxmi	lr
	mov	r2, #0x8000
.L_sqrt_coarse:
	add	r0, r0, r2
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2
	add	r0, r0, r2, lsr #1
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2, lsr #1
	lsrs	r2, r2, #2
	bne	.L_sqrt_coarse
	bx	lr
