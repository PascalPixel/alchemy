.syntax unified
	.thumb
	.global Func_080ec14c
	.thumb_func
Func_080ec14c:
	push {r5, lr}
	ldr r5, .L_080ec168
	ldrh r0, [r5]
	bl Resource_ResetEntry
	ldrh r0, [r5, #2]
	bl Resource_ResetEntry
	ldr r0, [r5, #28]
	movs r1, #2
	bl UiWork_FinalizeFar
	pop {r5, pc}
	.2byte 0x0000
.L_080ec168:
	.4byte Data_0202a000
