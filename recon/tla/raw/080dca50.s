.syntax unified
	.thumb
	.global Func_080dca50
	.thumb_func
Func_080dca50:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #88]
	bl Func_080dcadc
	ldr r0, .L_080dca80
	bl Scheduler_RemoveCallback
	movs r3, #164
	lsls r3, r3, #2
	adds r5, r5, r3
	ldrh r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl Func_080dcf2c
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #88
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_080dca80:
	.4byte Func_080dcb44
