.syntax unified
	.thumb
	.global Func_080c9dc8
	.thumb_func
Func_080c9dc8:
	push {lr}
	ldr r3, .L_080c9ddc
	movs r2, #132
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Audio_PlayCue
	pop {pc}
.L_080c9ddc:
	.4byte gPartyState
