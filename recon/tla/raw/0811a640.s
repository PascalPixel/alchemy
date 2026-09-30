.syntax unified
	.thumb
	.global Func_0811a640
	.thumb_func
Func_0811a640:
	push {r5, lr}
	adds r5, r0, #0
	movs r4, #0
	ldr r0, .L_0811a670
.L_0811a648:
	lsls r1, r4, #1
	ldrh r2, [r0, r1]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r2
	cmp r5, r3
	bne .L_0811a65e
	ldrh r3, [r0, r1]
	lsrs r0, r3, #9
	b .L_0811a66e
.L_0811a65e:
	lsls r3, r2, #16
	movs r2, #1
	asrs r3, r3, #16
	negs r2, r2
	adds r4, #1
	cmp r3, r2
	bne .L_0811a648
	movs r0, #6
.L_0811a66e:
	pop {r5, pc}
.L_0811a670:
	.4byte Data_0812cad0
