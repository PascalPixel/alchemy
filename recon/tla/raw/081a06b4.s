.syntax unified
	.thumb
	.global Func_081a06b4
	.thumb_func
Func_081a06b4:
	push {lr}
	ldr r3, .L_081a06d8
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_081a06d6
	ldr r2, .L_081a06dc
	movs r1, #255
	ldrh r3, [r2, #8]
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	strh r3, [r2, #8]
	ldrh r3, [r2, #12]
	adds r3, r3, r1
	strh r3, [r2, #12]
.L_081a06d6:
	pop {pc}
.L_081a06d8:
	.4byte gFrameTick
.L_081a06dc:
	.4byte Data_03001120
