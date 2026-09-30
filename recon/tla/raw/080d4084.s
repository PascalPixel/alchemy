.syntax unified
	.thumb
	.global Func_080d4084
	.thumb_func
Func_080d4084:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r5, r0, #0
	bl UiText_OpenMessageAtObject
	bl Func_080cdf5c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r7, r0, #0
	cmp r7, #0
	bne .L_080d40ba
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_080d3fb0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_080d40d4
.L_080d40ba:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r0, r5, #0
	adds r3, #1
	strh r3, [r2]
	adds r1, r6, #0
	bl Func_080d3fb0
.L_080d40d4:
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
