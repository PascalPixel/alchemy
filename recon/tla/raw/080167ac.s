.syntax unified
	.thumb
	.global SerialRuntime_RemoveIrqHandlers
	.thumb_func
SerialRuntime_RemoveIrqHandlers:
	push {lr}
	ldr r2, .L_080167d0
	ldr r3, .L_080167cc
	movs r0, #7
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_08013438
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_08013438
	b .L_080167d4
	.2byte 0x0000
.L_080167cc:
	.4byte 0x00000000
.L_080167d0:
	.4byte Data_030011b8
.L_080167d4:
	pop {pc}
	.2byte 0x0000
