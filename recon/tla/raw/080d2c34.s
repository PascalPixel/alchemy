.syntax unified
	.thumb
	.balign 4
	.global Func_080d2c34
	.thumb_func
Func_080d2c34:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	ldr	r3, [pc, #36]
	mov	ip, r3
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, ip
	strh	r0, [r3, #0]
	movs	r3, #241
	lsls	r3, r3, #1
	add	r3, ip
	strh	r1, [r3, #0]
	movs	r3, #172
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	strh	r3, [r2, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02000240