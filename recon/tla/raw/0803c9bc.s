.syntax unified
	.thumb
	.global UiText_CopyMessageString
	.thumb_func
UiText_CopyMessageString:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #66
	adds r5, r2, #0
	adds r2, r6, r3
	movs r3, #0
	adds r7, r1, #0
	strh r3, [r2]
	movs r1, #1
	bl UiText_BuildRenderEntries
	subs r5, #1
	movs r0, #0
	cmp r0, r5
	bcs .L_0803ca0e
	movs r2, #244
	lsls r2, r2, #4
	ldrh r3, [r6, r2]
	strh r3, [r7]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0803ca0e
	mov r12, r5
	adds r2, r6, r2
	movs r4, #0
.L_0803c9f6:
	adds r0, #1
	adds r4, #2
	cmp r0, r12
	bcs .L_0803ca12
	adds r2, #2
	ldrh r3, [r2]
	adds r1, r4, #0
	strh r3, [r1, r7]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0803c9f6
	b .L_0803ca14
.L_0803ca0e:
	movs r1, #0
	b .L_0803ca14
.L_0803ca12:
	lsls r1, r0, #1
.L_0803ca14:
	ldr r3, .L_0803ca1c
	strh r3, [r1, r7]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803ca1c:
	.4byte 0x00000000
