.syntax unified
	.thumb
	.global Func_0802c864
	.thumb_func
Func_0802c864:
	movs r2, #168
	movs r3, #128
	lsls r2, r2, #8
	lsls r3, r3, #19
	adds r2, #11
	adds r3, #14
	strh r2, [r3]
	movs r2, #170
	lsls r2, r2, #8
	adds r2, #15
	subs r3, #2
	strh r2, [r3]
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #130
	subs r3, #2
	strh r2, [r3]
	adds r3, #202
	ldr r0, .L_0802c894
	ldr r1, .L_0802c898
	ldr r2, .L_0802c89c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_0802c894:
	.4byte gMapCellBuffer
.L_0802c898:
	.4byte 0x06006000
.L_0802c89c:
	.4byte 0x84002800
