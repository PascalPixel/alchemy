.syntax unified
	.thumb
	.global Func_0818ddfc
	.thumb_func
Func_0818ddfc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
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
	movs r1, #160
	ldrh r2, [r3, #10]
	movs r2, #236
	lsls r2, r2, #7
	adds r2, #64
	adds r0, r0, r2
	lsls r1, r1, #19
	ldr r2, .L_0818de38
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.2byte 0x0000
.L_0818de38:
	.4byte 0xa2600001
