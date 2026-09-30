.syntax unified
	.thumb
	.global Func_080e1650
	.thumb_func
Func_080e1650:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #248
	ldr r3, [r3]
	adds r3, #164
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	ldr r0, .L_080e1674
	bl Scheduler_RemoveCallback
	movs r0, #248
	bl Runtime_ReleaseHeapBlock
	pop {pc}
	.2byte 0x0000
.L_080e1674:
	.4byte Func_080e150c
