@ Cosine of a 16-bit angle (a quarter turn is 0x4000) in 16.16 fixed point:
@ 0x0800231c adds a quarter turn, rounds to one of the 256 entries of the
@ quarter-wave sine table in SINE_TABLE.S, mirrors and negates by quadrant,
@ and branches back into 0x0800230c for the angles on an axis. It reaches
@ the table PC-relatively with the Thumb add r3, pc, #12, which neither
@ approved compiler emits, and the branches at 0x08002316, 0x08002328,
@ 0x0800232c and 0x0800233c test carry or zero left by lsrs, where both
@ approved compilers put a cmp before every conditional branch. TLA
@ LIB/TRIG.S is a different routine over a different table.
@ credit: handwritten — Q16 cosine lookup
.syntax unified
	.thumb
	.global Func_0800230c
	.thumb_func
Func_0800230c:
	lsrs	r2, r0, #14
	movs	r0, #1
	ands	r0, r2
	lsls	r0, r0, #16
	lsrs	r2, r2, #2
	bcc.n	.L_helper_return
	negs	r0, r0
.L_helper_return:
	bx	lr
	.global Func_0800231c
	.thumb_func
Func_0800231c:
	movs	r1, #64
	lsls	r1, r1, #8
	adds	r0, r0, r1
	adds	r0, #32
	lsls	r1, r0, #18
	lsrs	r1, r1, #24
	beq.n	Func_0800230c
	lsrs	r2, r0, #15
	bcc.n	.L_index
	movs	r3, #128
	lsls	r3, r3, #1
	subs	r1, r3, r1
.L_index:
	lsls	r1, r1, #1
	add	r3, pc, #12
	ldrh	r0, [r3, r1]
	lsrs	r2, r2, #1
	bcc.n	.L_return
	negs	r0, r0
	bx	lr
.L_return:
	bx	lr
