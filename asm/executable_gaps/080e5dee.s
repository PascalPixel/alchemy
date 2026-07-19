.syntax unified
.text
	.thumb
	.global Fragment_080e5dee
	.thumb_func
Fragment_080e5dee:
	movs	r0, r0
	cmp	r2, #86
	lsls	r1, r0, #8
	asrs	r6, r2, #5
	lsls	r1, r0, #8
	ands	r0, r0
	lsls	r1, r0, #8
	movs	r0, r0
	lsls	r1, r0, #8
	ldrb	r0, [r5, #0]
	movs	r0, r0
	.inst.n 0xedd4
	.inst.n 0x080e
	.inst.n 0xede2
	.inst.n 0x080e
	asrs	r2, r2, #23
	movs	r0, r0
	lsls	r0, r5, #5
	lsls	r0, r0, #12
	subs	r7, #63
	subs	r7, #63
	adds	r0, #129
	movs	r0, r0
	cmp	r5, #128
	lsls	r1, r0, #8
	ldr	r3, [pc, #0]
	lsls	r1, r0, #8
	.4byte 0x020158d2
