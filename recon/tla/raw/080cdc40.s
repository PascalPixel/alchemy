.syntax unified
	.thumb
	.global Func_080cdc40
	.thumb_func
Func_080cdc40:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r0, r1, #0
	adds r6, r2, #0
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #0
	cmp r5, #0
	beq .L_080cdc70
	movs r0, #124
	bl Audio_PlayCue
	movs r1, #4
	adds r0, r5, #0
	bl Object_SetMode
	movs r0, #12
	bl WaitFrames
	adds r0, r7, #0
	adds r1, r6, #0
	bl BattleFx_StartRandomParticleEmitter
.L_080cdc70:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
