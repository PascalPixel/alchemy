.syntax unified
	.thumb
	.global Func_081a6094
	.thumb_func
Func_081a6094:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	movs r5, #0
	cmp r5, r6
	bcs .L_081a60b8
	ldr r7, .L_081a60bc
.L_081a60a0:
	movs r0, #1
	bl WaitFrames
	bl Random16
	ldr r3, [r7, #4]
	movs r0, #1
	cmp r3, #0
	bne .L_081a60ba
	adds r5, #1
	cmp r5, r6
	bcc .L_081a60a0
.L_081a60b8:
	movs r0, #0
.L_081a60ba:
	pop {r5, r6, r7, pc}
.L_081a60bc:
	.4byte gInput
