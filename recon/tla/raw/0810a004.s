.syntax unified
	.thumb
	.global Func_0810a004
	.thumb_func
Func_0810a004:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	cmp r5, #0
	beq .L_0810a034
	bl RenderOutput_PrepareForRedrawFar
	adds r0, r5, #0
	movs r1, #8
	movs r2, #0
	adds r3, r6, #0
	bl Func_0810bf98
	adds r0, r6, #0
	bl Inventory_CountFar
	cmp r0, #0
	bne .L_0810a034
	ldr r0, .L_0810a038
	adds r1, r5, #0
	movs r2, #8
	movs r3, #20
	bl UiText_DrawResourceFar
.L_0810a034:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0810a038:
	.4byte 0x0000123e
