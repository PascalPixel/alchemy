.syntax unified
	.thumb
	.global Overlay_080d2aa4
Overlay_080d2aa4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #172
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	strh	r2, [r3, #0]
	ldr	r2, [pc, #16]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r2, r4
	strh	r0, [r3, #0]
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r2, r0
	strh	r1, [r3, #0]
	bx	lr
	.4byte 0x02000240
