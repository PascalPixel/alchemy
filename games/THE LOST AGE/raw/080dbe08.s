.syntax unified
	.thumb
	.set sub_080dbb78, 0x080dbb78
	.set sub_080dbde8, 0x080dbde8
	.global Overlay_080dbe08
Overlay_080dbe08:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	mov	r9, r3
	adds	r6, r0, #0
	mov	r8, r1
	mov	sl, r2
	bl	sub_080dbde8
	mov	r1, r8
	adds	r5, r0, #0
	mov	r2, sl
	adds	r0, r6, #0
	bl	sub_080dbb78
	mov	r3, r9
	orrs	r5, r3
	strb	r5, [r0, #3]
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
	adds	r0, r5, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
