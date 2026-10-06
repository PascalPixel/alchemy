.syntax unified
	.thumb
	.global EffectRuntime_StopCurrentObject
	.thumb_func
EffectRuntime_StopCurrentObject:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r1, #0
	ldr r0, [r3, #16]
	bl BattleFx_SetObjectTickMode
	movs r0, #1
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
