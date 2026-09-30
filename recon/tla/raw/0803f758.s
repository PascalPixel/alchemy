.syntax unified
	.thumb
	.global Resource_ResetPendingTransfer
	.thumb_func
Resource_ResetPendingTransfer:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #72]
	movs r2, #195
	lsls r2, r2, #2
	adds r5, r3, r2
	ldrh r3, [r5, #10]
	cmp r3, #0
	beq .L_0803f776
	ldrh r0, [r5, #12]
	bl Resource_ResetEntry
	movs r3, #0
	strh r3, [r5, #10]
.L_0803f776:
	pop {r5, pc}
