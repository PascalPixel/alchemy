.syntax unified
	.thumb
	.global Func_0803f86c
	.thumb_func
Func_0803f86c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #20
	ldr r6, [r3, #108]
	ldr r2, .L_0803f8f4
	movs r3, #8
	str r3, [sp, #16]
	str r3, [sp, #12]
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_080c8648
	ldr r3, .L_0803f8f8
	adds r5, r0, #0
	adds r5, r5, r3
	add r0, sp, #4
	add r1, sp, #16
	add r2, sp, #12
	add r3, sp, #8
	str r0, [sp, #0]
	adds r0, r5, #0
	bl UiText_GetResourceDimensions
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	movs r0, #30
	movs r1, #10
	subs r0, r0, r2
	subs r1, r1, r3
	movs r4, #2
	asrs r1, r1, #1
	asrs r0, r0, #1
	str r1, [sp, #12]
	str r4, [sp, #0]
	str r0, [sp, #16]
	bl UiWindow_Create
	movs r2, #135
	lsls r2, r2, #2
	adds r1, r0, #0
	adds r3, r6, r2
	str r1, [r3]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResource
	movs r3, #136
	lsls r3, r3, #2
	adds r2, r6, r3
	movs r1, #144
	movs r3, #90
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0803f8fc
	bl Scheduler_AddOrUpdateCallback
	add sp, #20
	pop {r5, r6, pc}
.L_0803f8f4:
	.4byte gPartyState
.L_0803f8f8:
	.4byte 0x00000e58
.L_0803f8fc:
	.4byte Func_0803f900
