.syntax unified
	.thumb
	.global Func_080fa84c
	.thumb_func
Func_080fa84c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	adds r6, r0, #0
	bl ItemMenu_PosCategory
	ldr r0, [r5, #36]
	bl RenderOutput_RedrawSavedRectFar
	ldr r0, [r5, #36]
	adds r1, r6, #0
	movs r2, #0
	bl Func_081004b8
	pop {r5, r6, pc}
	.2byte 0x0000
