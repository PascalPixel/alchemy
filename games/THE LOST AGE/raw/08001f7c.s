.syntax unified
	.thumb
	.set sub_080178ac, 0x080178ac
	.global Overlay_08001f7c
Overlay_08001f7c:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #96
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	sub_080178ac
	add	sp, #96
	pop	{r5, r6, r7, pc}
