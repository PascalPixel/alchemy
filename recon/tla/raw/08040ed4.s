.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_08016cfc, 0x08016cfc
	.set sub_080ad028, 0x080ad028
	.set sub_080c8268, 0x080c8268
	.set sub_08108030, 0x08108030
	.global Func_08040ed4
	.thumb_func
Func_08040ed4:
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl	sub_08016cfc
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016cfc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #170
	bl	sub_08016cfc
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
	adds	r3, r3, r2
	movs	r2, #141
	strh	r2, [r3, #0]
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl	sub_080c8268
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x0062
	.2byte 0x0000
	push	{lr}
	movs	r0, #184
	adds	r0, #255
	bl	sub_080ad028
	movs	r0, #220
	lsls	r0, r0, #1
	bl	sub_080ad028
	movs	r0, #186
	adds	r0, #255
	bl	sub_080ad028
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_08016cfc
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
	adds	r3, r3, r2
	movs	r2, #141
	strh	r2, [r3, #0]
	ldr	r0, [pc, #12]
	movs	r1, #9
	bl	sub_080c8268
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x00b6
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #76
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #225
	bl	sub_08016cfc
	movs	r0, #133
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_08016cfc
	ldr	r0, [pc, #8]
	movs	r1, #30
	bl	sub_080c8268
	pop	{pc}
	.2byte 0x00f4
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #36
	bl	sub_08016cfc
	movs	r0, #190
	lsls	r0, r0, #1
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #197
	bl	sub_08016cfc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl	sub_08016cfc
	movs	r0, #156
	lsls	r0, r0, #4
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #193
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #194
	bl	sub_08016cfc
	ldr	r0, [pc, #8]
	movs	r1, #1
	bl	sub_080c8268
	pop	{pc}
	movs	r0, r0
	.2byte 0x010e
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	movs	r1, #1
	bl	sub_080c8268
	pop	{pc}
	.2byte 0x0129
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	movs	r1, #20
	bl	sub_080c8268
	pop	{pc}
	.2byte 0x011e
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r0, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	adds	r0, r0, r3
	bl	sub_08108030
	pop	{pc}
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #208
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	.4byte 0x8500018a
	.4byte 0x08041005
