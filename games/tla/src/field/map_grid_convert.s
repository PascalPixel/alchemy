.syntax unified
	.thumb
	.global Render_ConvertMapCode
Render_ConvertMapCode:
	.global Render_ConvertMap
	.thumb_func
Render_ConvertMap:
	bx	pc
AlchemyUncredited_08021706:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08021706:
	push	{r5, r6, r7, r8}
	mov	r5, #0xff000000
	orr	r5, r5, r5, lsr #4
	orr	r4, r5, r5, lsr #8
	add	r4, r4, #1
	mov	r6, #0x4000
.L_08021720:
	ldrh	ip, [r0], #2
	cmp	r5, ip, lsl #20
	bne	.L_08021738
	add	ip, ip, r4
	cmp	r4, #0
	addne	r4, r4, #1
.L_08021738:
	strh	ip, [r1], #4
	subs	r6, r6, #1
	bne	.L_08021720
	add	r0, r2, #0x4000
	sub	r1, r1, #0x10000
	add	r1, r1, #2
	mov	r5, #255
	orr	r5, r5, r5, lsl #16
	mov	r8, #0x1000
.L_0802175c:
	ldr	r4, [r2], #4
	ldr	ip, [r0], #4
	and	r3, r4, r5
	and	r6, ip, r5
	orr	r3, r3, r6, lsl #8
	and	r7, r4, r5, lsl #8
	and	r6, ip, r5, lsl #8
	orr	r7, r6, r7, lsr #8
	strh	r3, [r1], #4
	strh	r7, [r1], #4
	ror	r3, r3, #16
	ror	r7, r7, #16
	strh	r3, [r1], #4
	strh	r7, [r1], #4
	subs	r8, r8, #1
	bne	.L_0802175c
	pop	{r5, r6, r7, r8}
	bx	lr
	.global Render_ConvertMapCodeSize
	.set Render_ConvertMapCodeSize, . - Render_ConvertMapCode
