@ Bit commands that end the second decoder unit (0x081a04d0). They share the
@ decoder's registers (source r0, bit count r2, bit word r3, value r5 and
@ the constant 7 in fp) instead of arguments, refill r3 from r0 when r2 runs
@ out and return through mov pc, lr or movpl pc, lr. Updating r5 and
@ reading fp without saving them are beyond both approved compilers. These
@ 272 bytes equal TBS GRAPHICS/TILE/BIT_COMMANDS.S (0x080b5258) and
@ BATTLE/EFFECT/SENTOU_KOUKA_TENKAI_COMMANDS.S (0x080f0144).
@ credit: library — TBS/TLA common ARM model/bit-stream decoder bank, bit commands
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x081a03c0
	.global SentouKouka_TenkaiCommands2
SentouKouka_TenkaiCommands2:
.L_081a03c0:
	lsr	r3, r3, #2
	subs	r2, r2, #2
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_081a03dc:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	add	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_081a0404:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	sub	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_081a042c:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	add	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_081a0454:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	sub	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_081a047c:
	mov	ip, #15
	ands	ip, ip, r3, lsr #4
	add	ip, ip, #11
	rsbcs	ip, ip, #0
	add	r5, r5, ip, lsl #25
	lsr	r3, r3, #8
	subs	r2, r2, #8
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_081a04ac:
	ror	r5, r3, #10
	and	r5, r5, #0xfe000000
	lsr	r3, r3, #10
	subs	r2, r2, #10
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
