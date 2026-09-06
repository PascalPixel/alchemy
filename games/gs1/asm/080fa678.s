@ Command slot 34 wrapper; reference call uses the handler in r1.
.syntax unified
.thumb
.global AudioCommand_InvokeSlot34
.global Func_080fa678
.thumb_func
AudioCommand_InvokeSlot34:
Func_080fa678:
    push {lr}
    ldr r1, .Lcommand_slot
    ldr r1, [r1]
    bl Func_080072e8
    pop {r0}
    bx r0
.balign 4, 0
.Lcommand_slot:
    .4byte 0x02004088
