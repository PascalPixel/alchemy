.syntax unified
	.thumb
	.global Func_080d3860
	.thumb_func
Func_080d3860:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080d3880
	adds r3, r0, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_080d3884
	bl Object_SetCallback
	adds r0, r6, #0
	bl Object_RefreshSelectorById
.L_080d3880:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080d3884:
	.4byte Data_080f3300
