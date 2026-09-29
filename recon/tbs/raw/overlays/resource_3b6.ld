/* resource_3b6: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/FACING_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/PROMPTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/ACTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/WATCHING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/CHEF.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/STAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/FLAGGED.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/REMEMBER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/MESSAGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/IMPORT.o"(.text)
        "*/resource_3b6_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
