.syntax unified
	.thumb
	.global ResourceTable_GetLongestFreeBlockRun
	.global Func_08003ed4
	.thumb_func
ResourceTable_GetLongestFreeBlockRun:
Func_08003ed4:
	push	{lr}
	movs	r2, #128
	ldr	r4, [pc, #36]
	movs	r1, #0
	movs	r0, #0
	lsls	r2, r2, #2
.L_08003ee0:
	ldrb	r3, [r4, #0]
	adds	r4, #1
	cmp	r3, #255
	beq.n	.L_08003eec
	movs	r1, #0
	b.n	.L_08003ef4
.L_08003eec:
	adds	r1, #1
	cmp	r0, r1
	bge.n	.L_08003ef4
	adds	r0, r1, #0
.L_08003ef4:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_08003ee0
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001810
