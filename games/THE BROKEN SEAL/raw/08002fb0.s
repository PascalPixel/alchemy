.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_08002f40, 0x08002f40
	.set sub_08004938, 0x08004938
	.set sub_08005340, 0x08005340
	.set sub_080072fc, 0x080072fc
	.global Overlay_08002fb0
Overlay_08002fb0:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r1
	bl	sub_08002f40
	mov	r1, r8
	bl	sub_08005340
	mov	sl, r0
	ldr	r5, [pc, #52]
	adds	r0, r5, #0
	bl	sub_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #40]
	ldr	r0, [pc, #40]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	bl	sub_080072fc
	adds	r0, r6, #0
	bl	sub_08002df0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0000007c
	.4byte 0x040000d4
	.4byte 0x08002d5c
