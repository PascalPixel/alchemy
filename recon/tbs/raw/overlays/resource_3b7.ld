/* resource_3b7: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. The overlay's
   own work follows the image. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/QUESTIONS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/WALK_TO_SPRING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/RULES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/OPEN_SCENE.o"(.text)
        "*/resource_3b7_overlay.o"(.text.x020084bc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/RISE_AND_FADE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/OFFER_WHEELS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/PAY_TO_PLAY.o"(.text)
        "*/resource_3b7_overlay.o"(.text.x02008ac8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/TOPIC_PICK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/PLACE_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/FIELD54.o"(.text)
        "*/resource_3b7_overlay.o"(.text.x02008e5c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/FOUR_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/SPRING_RIDE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b7_overlay.o"(.rodata.part1)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/SPRING_RIDE.o"(.data .bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
