.syntax unified
	.thumb
	.global Func_0814cb60
	.thumb_func
Func_0814cb60:
	push {r5, lr}
	movs r1, #192
	lsls r1, r1, #2
	adds r5, r0, #0
	adds r1, #2
	movs r0, #100
	bl Runtime_AllocateHeapBlock
	movs r1, #246
	lsls r1, r1, #7
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #240
	adds r2, r3, r1
	str r5, [r2]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	adds r3, r3, r2
	movs r2, #1
	str r2, [r3]
	ldr r2, .L_0814cbc8
	ldr r3, [r5]
	adds r0, r5, #0
	lsls r3, r3, #2
	subs r3, #4
	ldr r3, [r2, r3]
	mov lr, r3
	.2byte 0xf800
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	movs r0, #100
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
	.2byte 0x0000
.L_0814cbc8:
	.4byte Data_08197aa0
