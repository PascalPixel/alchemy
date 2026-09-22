.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_08108030, 0x08108030
	.global Overlay_08041204
Overlay_08041204:
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
	.4byte 0x08041205
