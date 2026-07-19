.syntax unified
.text
	.thumb
	.global Fragment_08091e6c
	.thumb_func
Fragment_08091e6c:
	ldr	r3, [pc, #32]
	movs	r2, #184
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #28]
	movs	r4, #226
	strh	r2, [r3, #0]
	ldr	r2, [pc, #24]
	lsls	r4, r4, #1
	adds	r3, r2, r4
	strh	r0, [r3, #0]
	movs	r0, #227
	lsls	r0, r0, #1
	adds	r3, r2, r0
	strh	r1, [r3, #0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001ebc
	.4byte 0x000003e7
	.4byte 0x02000240
	.inst.n 0x4b03
	.inst.n 0x22b8
	.inst.n 0x681b
	.inst.n 0x0052
	.inst.n 0x189b
	.inst.n 0x8018
	.inst.n 0x4770
	.inst.n 0x0000
	.inst.n 0x1ebc
	.inst.n 0x0300
