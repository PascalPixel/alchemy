.syntax unified
	.thumb
	.global Func_080e2d40
	.thumb_func
Func_080e2d40:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #92]
	ldr r0, .L_080e2d60
	bl Scheduler_RemoveCallback
	movs r3, #216
	lsls r3, r3, #5
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Resource_ResetEntry
	pop {r5, pc}
	.2byte 0x0000
.L_080e2d60:
	.4byte Func_080e2c38
