.syntax unified
	.thumb
	.global Func_080eb930
	.thumb_func
Func_080eb930:
	push {lr}
	ldr r0, .L_080eb958
	bl Func_08014644
	ldr r0, .L_080eb95c
	bl Func_08014644
	movs r0, #1
	bl WaitFrames
	movs r0, #180
	bl Runtime_ReleaseHeapBlock
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #240
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_080eb958:
	.4byte Func_080eb6a0
.L_080eb95c:
	.4byte Func_080eb594
