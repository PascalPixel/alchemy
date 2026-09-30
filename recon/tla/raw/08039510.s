.syntax unified
	.thumb
	.global RenderOutput_Release
	.thumb_func
RenderOutput_Release:
	push {r5, lr}
	adds r5, r0, #0
	bl RenderOutput_ReleaseFree
	ldrb r3, [r5, #4]
	cmp r3, #0
	beq .L_08039540
	ldrb r0, [r5, #14]
	bl Resource_ResetEntry
	ldrb r3, [r5, #4]
	cmp r3, #2
	bne .L_08039540
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #60]
	ldrb r3, [r5, #25]
	movs r2, #155
	lsrs r3, r3, #4
	lsls r2, r2, #5
	lsls r3, r3, #1
	adds r3, r3, r2
	ldr r2, .L_08039548
	strh r2, [r1, r3]
.L_08039540:
	movs r3, #0
	strb r3, [r5, #5]
	pop {r5, pc}
	.2byte 0x0000
.L_08039548:
	.4byte 0x000003e7
