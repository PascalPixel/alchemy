	.section .rodata
@ Effect tables and object scripts after gBattleEffect_CyclePatternWords, in
@ the order of The Broken Seal's object/fxdata.c. Object scripts are words
@ in this game's command numbers: 0x00 wait, 0x01 sync, 0x0d loop,
@ 0x16 set, 0x17 add, 0x26 free, 0x2e call; a label is 0x80000000 | n << 16
@ and a branch to it 0xc0000000 | n << 16.

@ A copy of The Broken Seal's gWorldMap_CursorDirectionAngles: the
@ direction of each pad state in eight ways, 0xffff for none
	.global gBattleEffect_EightWayHeadingWords
gBattleEffect_EightWayHeadingWords:
	.2byte 0xffff, 0x0000, 0x8000, 0xffff
	.2byte 0xc000, 0xe000, 0xa000, 0xc000
	.2byte 0x4000, 0x2000, 0x6000, 0x4000
	.2byte 0xffff, 0x0000, 0x8000, 0xffff

@ A copy of The Broken Seal's gBattleEffect_QuadrantModes
	.global Data_080f0ee4
Data_080f0ee4:
	.byte 3, 1, 2, 1

@ The Broken Seal's gBattleEffect_BurstParticleObjectScript: wait 60 frames
@ and free
	.global gData_080f0ee8
gData_080f0ee8:
	.4byte 0x00, 60
	.4byte 0x26
@ The Broken Seal's gUnused_BattleFxBytes: the same four bytes as the
@ quadrant modes, which no code reads
	.byte 3, 1, 2, 1

@ The Broken Seal's gBattleEffect_HeavyImpactScript: sync, call Func_080de154,
@ free
	.global gData_080f0ef8
gData_080f0ef8:
	.4byte 0x01
	.4byte 0x2e, Func_080de154
	.4byte 0x26
@ The Broken Seal's short, medium and long effect scripts: wait 24, 46
@ or 128 frames and free
	.4byte 0x00, 24
	.4byte 0x26
	.4byte 0x00, 46
	.4byte 0x26
	.4byte 0x00, 128
	.4byte 0x26

@ A copy of The Broken Seal's gBattleEffect_RandomChildValues
	.global gBattleEffect_RandomChildValues
gBattleEffect_RandomChildValues:
	.byte 0, 0, 1, 2, 3, 4, 5, 6

@ Read by Func_080e1678: eight offsets in 16.16 fixed point
	.global Data_080f0f34
Data_080f0f34:
	.4byte 0x1000, 0, -0x1000, -0x2000
	.4byte -0x3000, -0x2000, -0x1000, 0

@ Read by Func_080e1678: four values in 16.16 fixed point
	.global Data_080f0f54
Data_080f0f54:
	.4byte 0xd000, 0x9000, 0x5000, 0x3000

@ Read by Func_080e2264: 21 {x, y} cell offsets within two of a centre,
@ the corners left out
	.global Data_080f0f64
Data_080f0f64:
	.byte -1, -2, 0, -2, 1, -2
	.byte -2, -1, -1, -1, 0, -1, 1, -1, 2, -1
	.byte -2, 0, -1, 0, 0, 0, 1, 0, 2, 0
	.byte -2, 1, -1, 1, 0, 1, 1, 1, 2, 1
	.byte -1, 2, 0, 2, 1, 2
	.balign 4

@ The four colours (BGR555) Func_080e3d04 passes to
@ Field_StartColorTransition
	.global Data_080f0f90
Data_080f0f90:
	.4byte 0x1090, 0x304c, 0x098c, 0x2942

@ The 16 colours Func_080e4d9c writes to OBJ palettes 14 and 15
	.global Data_080f0fa0
Data_080f0fa0:
	.incbin "games/tla/src/field/event_commands/fxpal.gbapal"

@ Item ids; Func_080e716c picks one of the 16 at random
	.global gData_080f0fc0
gData_080f0fc0:
	.2byte 0xb4, 0xb4, 0xb4, 0xb4, 0xbb, 0xbb, 0xbb, 0xe2
	.2byte 0xe2, 0xe3, 0xe3, 0xe4, 0xe4, 0xe4, 0xb5, 0xb5

@ Read by Func_080e989c: 32 steps down from 31, 32 zeros and 32 steps up
	.global gData_080f0fe0
gData_080f0fe0:
	.byte 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16
	.byte 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.byte 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31

@ Read by Func_080ea14c: an object script that sets the scale to 1 and
@ then squashes and stretches it for ever, as the variant motion scripts
@ (gActor_VariantScripts) do; work 9 is the x scale, 10 the y
	.global Data_080f1040
Data_080f1040:
	.4byte 0x16, 9, 0x10000		@ set
	.4byte 0x16, 10, 0x10000	@ set
	.4byte 0x80010000		@ label 1
	.4byte 0x17, 9, -0x4000		@ add
	.4byte 0x17, 10, 0x2000		@ add
	.4byte 0x00, 2			@ wait
	.4byte 0x17, 9, 0x2000		@ add
	.4byte 0x17, 10, -0x1000	@ add
	.4byte 0x00, 2			@ wait
	.4byte 0x17, 9, 0x1000		@ add
	.4byte 0x17, 10, -0x800		@ add
	.4byte 0x00, 2			@ wait
	.4byte 0x17, 9, 0x1000		@ add
	.4byte 0x17, 10, -0x800		@ add
	.4byte 0x00, 4			@ wait
	.4byte 0x17, 9, -0x1000		@ add
	.4byte 0x17, 10, -0x1000	@ add
	.4byte 0x0d, 0xffff, 0xc0010000	@ loop for ever to label 1
