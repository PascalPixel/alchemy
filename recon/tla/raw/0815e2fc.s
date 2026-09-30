.syntax unified
	.thumb
	.global Func_0815e2fc
	.thumb_func
Func_0815e2fc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #240
	adds r3, r3, r2
	ldr r3, [r3]
	movs r2, #36
	ldrsh r3, [r3, r2]
	cmp r3, #127
	ble .L_0815e31a
	ldr r3, .L_0815e31c
	ldr r3, [r3]
.L_0815e31a:
	pop {pc}
.L_0815e31c:
	.4byte gInput
