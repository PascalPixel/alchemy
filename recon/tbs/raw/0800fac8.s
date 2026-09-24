.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_08004938, 0x08004938
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080072fc, 0x080072fc
	.global Tilemap_ConvertBuffer
	.global Func_0800fac8
	.thumb_func
Tilemap_ConvertBuffer:
Func_0800fac8:
.L_0800fac8:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r0, r5, #0
	bl	sub_08004970
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #72]
	adds	r2, r5, #0
	mov	r8, r0
	bl	sub_080072f0
	ldr	r5, [pc, #64]
	adds	r0, r5, #0
	bl	sub_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #56]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #32]
	mov	r2, r8
	bl	sub_080072fc
	adds	r0, r6, #0
	bl	sub_08002df0
	mov	r0, r8
	bl	sub_08002df0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001388
	.4byte 0x02010000
	.4byte 0x0000009c
	.4byte 0x040000d4
	.4byte 0x0800a37c
	.2byte 0x8000
	.2byte 0x0201
