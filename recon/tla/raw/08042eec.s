.syntax unified
	.thumb
	.global Func_08042eec
	.thumb_func
Func_08042eec:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #10
	bl WaitFrames
	bl Func_080c8698
	movs r0, #128
	lsls r0, r0, #1
	movs r1, #30
	bl Func_081c0040
	adds r0, r5, #0
	movs r1, #0
	bl UiWork_Finalize
	adds r0, r5, #0
	pop {r5, pc}
