.syntax unified
	.thumb
	.global Func_080fb6d4
	.thumb_func
Func_080fb6d4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Item_Get
	ldrb r3, [r0, #12]
	cmp r3, #2
	bne .L_080fb730
	bl Random16
	movs r3, #128
	lsls r3, r3, #6
	cmp r0, r3
	bcs .L_080fb730
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r5, r2
	subs r2, #174
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrh r1, [r3]
	bl Inventory_BreakFar
	movs r0, #138
	bl Audio_PlayCue
	movs r2, #1
	ldr r0, .L_080fb734
	negs r2, r2
	movs r1, #0
	bl Func_080f8ce8
	ldr r0, [r5, #48]
	bl RenderOutput_RedrawSavedRectFar
.L_080fb730:
	pop {r5, pc}
	.2byte 0x0000
.L_080fb734:
	.4byte 0x000010b7
