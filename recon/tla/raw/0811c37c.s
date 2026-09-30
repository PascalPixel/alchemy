.syntax unified
	.thumb
	.global Func_0811c37c
	.thumb_func
Func_0811c37c:
	push {r5, lr}
	adds r5, r0, #0
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	bl Func_08128194
	lsls r0, r0, #24
	lsrs r3, r0, #8
	cmp r3, #0
	bne .L_0811c3b6
	adds r0, r5, #0
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	bl Func_081280fc
	movs r3, #192
	lsls r3, r3, #13
	cmp r0, #0
	bne .L_0811c3b6
	movs r3, #192
	lsls r3, r3, #14
.L_0811c3b6:
	adds r0, r3, #0
	pop {r5, pc}
	.2byte 0x0000
