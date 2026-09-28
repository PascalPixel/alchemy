.syntax unified
	.thumb
	.global AudioCommand_InvokeSlot35
	.global Func_081c2328
	.thumb_func
AudioCommand_InvokeSlot35:
Func_081c2328:
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	_call_via_r1
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0200688c
