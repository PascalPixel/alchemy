.syntax unified
	.thumb
	.global Func_080167d8
	.thumb_func
Func_080167d8:
	push {r5, r6, lr}
	ldr r1, .L_08016804
	adds r5, r0, #0
	ldrh r2, [r1]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, r5
	beq .L_080167fa
	adds r6, r1, #0
.L_080167ea:
	movs r0, #1
	bl WaitFrames
	ldrh r2, [r6]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, r5
	bne .L_080167ea
.L_080167fa:
	ldr r3, .L_08016808
	ldr r0, [r3]
	lsls r0, r0, #26
	lsrs r0, r0, #30
	pop {r5, r6, pc}
.L_08016804:
	.4byte Data_0300124c
.L_08016808:
	.4byte 0x04000128
