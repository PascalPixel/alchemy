.syntax unified
	.thumb
	.global Func_0803a60c
	.thumb_func
Func_0803a60c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #132
	adds r3, r5, r2
	movs r6, #0
	adds r2, #2
	strh r6, [r3]
	adds r3, r5, r2
	strh r6, [r3]
	adds r7, r0, #0
	adds r0, r1, #0
	movs r1, #1
	sub sp, #16
	bl UiText_BuildRenderEntries
	movs r2, #244
	adds r1, r0, #0
	lsls r3, r1, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r3, [r5, r3]
	movs r0, #0
	cmp r3, #0
	beq .L_0803a662
	cmp r7, #0
	beq .L_0803a662
	movs r3, #1
	str r3, [sp, #4]
	adds r0, r7, #0
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl Func_0803954c
	adds r6, r0, #0
	movs r0, #0
	cmp r6, #0
	beq .L_0803a662
	adds r0, r6, #0
.L_0803a662:
	add sp, #16
	pop {r5, r6, r7, pc}
	.2byte 0x0000
