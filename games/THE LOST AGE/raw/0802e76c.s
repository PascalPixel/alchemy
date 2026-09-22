.syntax unified
	.thumb
	.set sub_08021918, 0x08021918
	.set sub_0802296c, 0x0802296c
	.global Overlay_0802e76c
Overlay_0802e76c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	movs	r3, #192
	adds	r5, r0, #0
	lsls	r3, r3, #18
	adds	r0, r7, #0
	ldr	r6, [r3, #16]
	bl	sub_08021918
	movs	r3, #3
	ands	r3, r5
	lsls	r3, r3, #2
	adds	r3, #40
	mov	r8, r3
	movs	r5, #9
.L_0802e78e:
	mov	r3, r8
	ldr	r0, [r6, r3]
	subs	r5, #1
	strh	r7, [r0, #0]
	bl	sub_0802296c
	adds	r6, #56
	cmp	r5, #0
	bge.n	.L_0802e78e
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
