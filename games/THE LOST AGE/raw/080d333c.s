.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.global Overlay_080d333c
Overlay_080d333c:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #48]
	ldr	r3, [r6, #8]
	ldr	r7, [r6, #52]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	movs	r2, #128
	adds	r3, r3, r7
	str	r3, [r6, #16]
	ldr	r3, [r6, #12]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #12]
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #48]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_080d336e
	adds	r3, #15
.L_080d336e:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #52]
	pop	{r5, r6, r7, pc}
