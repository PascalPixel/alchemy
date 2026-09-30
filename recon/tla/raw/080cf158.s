.syntax unified
	.thumb
	.global Func_080cf158
	.thumb_func
Func_080cf158:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #0
	cmp r3, #0
	beq .L_080cf17a
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
.L_080cf17a:
	pop {pc}
