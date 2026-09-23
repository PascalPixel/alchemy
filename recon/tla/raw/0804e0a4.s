.syntax unified
	.thumb
	.set sub_08016cfc, 0x08016cfc
	.set sub_080ad150, 0x080ad150
	.global Func_0804e0a4
	.thumb_func
Func_0804e0a4:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r5, r1, #0
	mov	r8, r0
	lsls	r0, r5, #2
	adds	r0, r0, r5
	adds	r6, r2, #0
	lsls	r0, r0, #2
	adds	r0, r0, r6
	adds	r0, #48
	bl	sub_08016cfc
	mov	r0, r8
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_080ad150
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.align 2, 0
