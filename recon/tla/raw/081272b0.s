.syntax unified
	.thumb
	.global Func_081272b0
	.thumb_func
Func_081272b0:
	push {lr}
	movs r1, #128
	lsls r1, r1, #7
	ldr r3, .L_081272f0
	ldr r0, .L_081272f4
	mov lr, r3
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_081272ec
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r0, .L_081272f8
	bl Scheduler_RemoveCallback
	ldr r0, .L_081272fc
	bl Scheduler_RemoveCallback
	b .L_08127300
.L_081272ec:
	.4byte 0x00001341
.L_081272f0:
	.4byte IwramClearWords
.L_081272f4:
	.4byte 0x06004000
.L_081272f8:
	.4byte Func_08127038
.L_081272fc:
	.4byte Func_08126e00
.L_08127300:
	pop {pc}
	.2byte 0x0000
	.4byte 0x00004770
