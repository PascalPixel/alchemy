.syntax unified
.text
	.thumb
	.global Fragment_080e72de
	.thumb_func
Fragment_080e72de:
	movs	r0, r0
	ldr	r3, [pc, #40]
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #44]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #40]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldrh	r2, [r3, #10]
	movs	r2, #252
	lsls	r2, r2, #5
	movs	r1, #160
	adds	r0, r0, r2
	lsls	r1, r1, #19
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001eec
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0xa2600001
