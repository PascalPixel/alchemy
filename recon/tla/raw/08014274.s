.syntax unified
	.thumb
	.global Func_08014274
	.thumb_func
Func_08014274:
	push {r5, r6, lr}
	ldr r3, .L_080142a8
	lsls r2, r0, #2
	adds r5, r2, r3
	cmp r0, #95
	bls .L_08014286
	movs r0, #1
	negs r0, r0
	b .L_080142a4
.L_08014286:
	movs r6, #255
	ldrh r3, [r5, #2]
	lsls r6, r6, #8
	adds r6, #255
	cmp r3, r6
	beq .L_080142a2
	bl Resource_ClearSlotReferences
	ldrh r3, [r5, #2]
	adds r2, r6, #0
	orrs r2, r3
	movs r3, #0
	strh r2, [r5, #2]
	strh r3, [r5]
.L_080142a2:
	movs r0, #0
.L_080142a4:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080142a8:
	.4byte ResourceTableEntries
