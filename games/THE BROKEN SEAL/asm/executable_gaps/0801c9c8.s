.syntax unified
.text
	.thumb
	.global Fragment_0801c9c8
	.thumb_func
Fragment_0801c9c8:
	movs r1, #128
	lsls r1, r1, #3
	movs r2, #0
	adds r3, r0, r1
	adds r1, #52
	strh r2, [r3, #10]
	adds r3, r0, r1
	adds r1, #52
	strh r2, [r3, #10]
	adds r3, r0, r1
	adds r1, #52
	strh r2, [r3, #10]
	adds r3, r0, r1
	adds r1, #52
	strh r2, [r3, #10]
	adds r3, r0, r1
	adds r1, #52
	strh r2, [r3, #10]
	adds r3, r0, r1
	adds r1, #52
	strh r2, [r3, #10]
	adds r3, r0, r1
	adds r1, #60
	strh r2, [r3, #10]
	adds r3, r0, r1
	adds r1, #2
	strh r2, [r3, #0]
	adds r3, r0, r1
	adds r1, #2
	strh r2, [r3, #0]
	adds r3, r0, r1
	adds r1, #2
	strh r2, [r3, #0]
	adds r3, r0, r1
	strh r2, [r3, #0]
	.inst.n 0x4b02
	adds r0, r0, r3
	strh r2, [r0, #0]
	bx lr
