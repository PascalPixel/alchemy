.syntax unified
	.thumb
	.global Func_080cf6fc
	.thumb_func
Func_080cf6fc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #124]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #57
	adds r3, r0, r2
	ldrb r2, [r3]
	movs r4, #128
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #5
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r1, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r1, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	lsls r4, r4, #19
	ldrh r2, [r3, #10]
	ldr r1, .L_080cf770
	ldrh r2, [r4]
	orrs r2, r1
	strh r2, [r4]
	ldrh r1, [r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r1, [r2]
	adds r0, #2
	ldrh r1, [r0]
	adds r2, #2
	strh r1, [r2]
	adds r0, #2
	ldrh r2, [r0]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #64
	strh r2, [r1]
	adds r0, #2
	ldrh r4, [r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #66
	strh r4, [r2]
	b .L_080cf774
.L_080cf770:
	.4byte 0x00006000
.L_080cf774:
	movs r4, #160
	adds r2, #2
	strh r4, [r2]
	adds r2, #2
	strh r4, [r2]
	adds r0, #2
	ldr r2, .L_080cf788
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_080cf788:
	.4byte 0xa6600001
