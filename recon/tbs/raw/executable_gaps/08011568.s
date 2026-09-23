.syntax unified
.text
	.thumb
	.global Func_08011568
	.thumb_func
Func_08011568:
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #20]
	strh	r2, [r3, #0]
	adds	r3, #202
	ldr	r0, [pc, #16]
	ldr	r1, [pc, #20]
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x00000682
	.4byte 0x0400000a
	.4byte 0x02010000
	.4byte 0x06006a00
	.4byte 0x84002580
