.syntax unified
	.thumb
	.set sub_080072e8, 0x080072e8
	.global AudioCommand_InvokeSlot34
	.global Func_080fa678
	.thumb_func
AudioCommand_InvokeSlot34:
Func_080fa678:
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	sub_080072e8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02004088
