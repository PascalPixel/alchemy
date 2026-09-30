.syntax unified
	.thumb
	.global Event_CallWithLastActiveObjectId
	.thumb_func
Event_CallWithLastActiveObjectId:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_080cacc0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Func_080ca6e8
	pop {r5, pc}
	.2byte 0x0000
