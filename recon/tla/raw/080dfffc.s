.syntax unified
	.thumb
	.global Func_080dfffc
	.thumb_func
Func_080dfffc:
	push {r5, lr}
	adds r5, r0, #0
	adds r5, #100
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #80
	bne .L_080e0018
	movs r3, #0
	str r3, [r0, #108]
	movs r0, #136
	bl Audio_PlayCue
	ldrh r2, [r5]
.L_080e0018:
	adds r3, r2, #1
	strh r3, [r5]
	pop {r5, pc}
	.2byte 0x0000
