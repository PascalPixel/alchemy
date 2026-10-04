@ Uncredited owned-ROM disassembly; ordinary draft: ../en/main/080175c0.c.
.syntax unified
	.text
	.balign 4, 0
	.thumb
	.global UiText_OpenEntryMessage
	.type UiText_OpenEntryMessage, %function
	.thumb_func
UiText_OpenEntryMessage:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #84]
	ldr r2, [pc, #84]
	ldr r5, [r3]
	movs r6, #0
	adds r3, r5, r2
	adds r2, #2
	strh r6, [r3]
	adds r3, r5, r2
	strh r6, [r3]
	adds r7, r0, #0
	adds r0, r1, #0
	movs r1, #1
	sub sp, #16
	bl UiText_BuildRenderEntries
	movs r2, #235
	adds r1, r0, #0
	lsls r3, r1, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r3, [r5, r3]
	movs r0, #0
	cmp r3, #0
	beq .L0
	cmp r7, #0
	beq .L0
	movs r3, #1
	str r3, [sp, #4]
	adds r0, r7, #0
	movs r2, #0
	movs r3, #0
	str r6, [sp, #0]
	bl UiText_QueueRenderEntries
	adds r6, r0, #0
	movs r0, #0
	cmp r6, #0
	beq .L0
	adds r0, r6, #0
.L0:
	add sp, #16
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte gWindowWork
	.4byte 0x000012f4
	.size UiText_OpenEntryMessage, .-UiText_OpenEntryMessage
