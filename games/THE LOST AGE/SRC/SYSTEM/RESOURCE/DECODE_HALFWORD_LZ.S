@ Halfword LZ decoder run from the stack. Its loader (0x08015830) takes 0x57
@ from its literal pool at 0x08015874, reserves (0x57 + 4) & ~3 = 88 bytes of
@ stack, has DMA3 copy (0x57 + 1) / 4 = 22 words, exactly these 88 bytes, and
@ calls the copy. Behind a 4-byte Thumb entry (bx pc) the ARM body keeps a
@ sentinel in its flag word, tests it with lsls and bcc, refills it through
@ the carry (adc) and returns mid-body with bxeq lr. GCC never branches on the
@ carry out of a shift and uses adc only for 64-bit sums. After that entry
@ its 84-byte body equals TBS SYSTEM/RESOURCE/DECODE_HALFWORD_LZ.S
@ (0x08002298) byte for byte, as in every TBS and TLA edition.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM halfword-LZ decoder copied to the stack
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08001ffc
	.global Resource_DecodeHalfwordLz
	.thumb_func
Resource_DecodeHalfwordLz:
AlchemyUncredited_08001ffc:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001ffc:
	mov	r3, #0x80000000
	b	.L_0800200c
.L_08002008:
	strh	r2, [r1], #2
.L_0800200c:
	lsls	r3, r3, #1
.L_08002010:
	ldrh	r2, [r0], #2
	bcc	.L_08002008
	bne	.L_08002028
	adc	r3, r2, r2
	lsls	r3, r3, #16
	b	.L_08002010
.L_08002028:
	cmp	r2, #0
	bxeq	lr
	sub	ip, r1, r2, lsr #5
	sub	ip, ip, r2, lsr #5
	and	r2, r2, #31
	add	r2, r2, #2
.L_08002040:
	ldrh	r4, [ip], #2
	strh	r4, [r1], #2
	subs	r2, r2, #1
	bne	.L_08002040
	b	.L_0800200c
