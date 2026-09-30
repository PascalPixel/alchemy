.syntax unified
	.thumb
	.global Func_0811c594
	.thumb_func
Func_0811c594:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r1, #0
	ldrsh r0, [r7, r1]
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	cmp r3, #0
	bne .L_0811c5ae
	movs r0, #1
	negs r0, r0
	b .L_0811c646
.L_0811c5ae:
	adds r0, r7, #0
	bl Func_0811cd30
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r2, [r3]
	movs r1, #0
	ldrsh r3, [r7, r1]
	strh r0, [r7, #10]
	ldr r1, .L_0811c648
	cmp r3, #4
	bgt .L_0811c5cc
	movs r1, #128
	lsls r1, r1, #6
.L_0811c5cc:
	movs r3, #60
	str r1, [r2]
	str r3, [r2, #4]
	bl Func_08038118
	movs r2, #6
	ldrsh r3, [r7, r2]
	cmp r3, #2
	beq .L_0811c616
	cmp r3, #2
	bgt .L_0811c5ec
	cmp r3, #0
	beq .L_0811c624
	cmp r3, #1
	beq .L_0811c63a
	b .L_0811c624
.L_0811c5ec:
	cmp r3, #3
	beq .L_0811c608
	cmp r3, #99
	bne .L_0811c624
	ldr r0, .L_0811c64c
	bl Func_080381c8
	adds r0, r7, #0
	bl Func_0811c66c
	cmp r0, #0
	beq .L_0811c640
	movs r0, #1
	b .L_0811c646
.L_0811c608:
	movs r0, #45
	bl WaitFrames
	adds r0, r7, #0
	bl Func_0811c6cc
	b .L_0811c640
.L_0811c616:
	movs r0, #45
	bl WaitFrames
	adds r0, r7, #0
	bl Func_0811ca54
	b .L_0811c640
.L_0811c624:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r6, [r3]
	movs r5, #0
	str r5, [r6, #20]
	adds r0, r7, #0
	bl Func_0811ca54
	str r5, [r6, #20]
	b .L_0811c640
.L_0811c63a:
	adds r0, r7, #0
	bl Func_0811c710
.L_0811c640:
	bl Func_08038220
	movs r0, #0
.L_0811c646:
	pop {r5, r6, r7, pc}
.L_0811c648:
	.4byte 0xffffe000
.L_0811c64c:
	.4byte 0x00000c98
