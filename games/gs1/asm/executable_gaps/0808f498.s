.syntax unified
.text
	.thumb
	.global Fragment_0808f498
	.thumb_func
Fragment_0808f498:
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #100]
	ldr	r0, [r3, #0]
	adds	r3, r0, r2
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #84]
	ldr	r2, [pc, #84]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #80]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r4, #128
	lsls	r4, r4, #19
	ldrh	r2, [r3, #10]
	ldr	r1, [pc, #48]
	ldrh	r2, [r4, #0]
	orrs	r2, r1
	strh	r2, [r4, #0]
	ldrh	r1, [r0, #0]
	ldr	r2, [pc, #60]
	strh	r1, [r2, #0]
	adds	r0, #2
	ldrh	r1, [r0, #0]
	adds	r2, #2
	strh	r1, [r2, #0]
	adds	r0, #2
	ldrh	r2, [r0, #0]
	ldr	r1, [pc, #48]
	strh	r2, [r1, #0]
	adds	r0, #2
	ldrh	r4, [r0, #0]
	ldr	r2, [pc, #44]
	strh	r4, [r2, #0]
	movs	r4, #160
	adds	r2, #2
	strh	r4, [r2, #0]
	adds	r2, #2
	strh	r4, [r2, #0]
	b .L_0808f51c
	.4byte 0x00006000
	.4byte 0x03001ecc
	.4byte 0x00000539
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x04000048
	.4byte 0x04000040
	.4byte 0x04000042
.L_0808f51c:
	adds	r0, #2
	ldr	r2, [pc, #8]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.inst.n 0x0000
	.4byte 0xa6600001
