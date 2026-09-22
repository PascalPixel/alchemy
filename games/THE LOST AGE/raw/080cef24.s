.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_080cad84, 0x080cad84
	.global Overlay_080cef24
Overlay_080cef24:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080cef60
	ldrh	r3, [r6, #6]
	movs	r7, #15
	mov	r8, r3
.L_080cef3a:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldrh	r3, [r6, #6]
	lsrs	r0, r0, #4
	lsrs	r5, r5, #4
	subs	r5, r5, r0
	adds	r3, r3, r5
	strh	r3, [r6, #6]
	movs	r0, #1
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080cef3a
	mov	r3, r8
	strh	r3, [r6, #6]
.L_080cef60:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
