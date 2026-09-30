.syntax unified
	.thumb
	.global ResetMotionRecordGroup
	.thumb_func
ResetMotionRecordGroup:
	push {lr}
	cmp r0, #0
	beq .L_0811b9fa
	movs r1, #0
	adds r0, #40
	movs r2, #3
.L_0811b9ec:
	ldmia r0!, {r3}
	cmp r3, #0
	beq .L_0811b9f4
	str r1, [r3, #16]
.L_0811b9f4:
	subs r2, #1
	cmp r2, #0
	bge .L_0811b9ec
.L_0811b9fa:
	pop {pc}
