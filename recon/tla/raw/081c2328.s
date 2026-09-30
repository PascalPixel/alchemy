.syntax unified
	.thumb
	.global AudioCommand_InvokeSlot35
	.thumb_func
AudioCommand_InvokeSlot35:
	.global Func_081c2328
Func_081c2328:
	push {lr}
	ldr r1, .L_081c2338
	ldr r1, [r1]
	bl _call_via_r1
	pop {r0}
	bx r0
	.2byte 0x0000
.L_081c2338:
	.4byte Data_0200688c
