#ifndef ALCHEMY_FLAG_IDS_H
#define ALCHEMY_FLAG_IDS_H

/* Game flags shared by more than one scene. */
enum GameFlagId {
    /*
     * Set while the party keeps its position across a scene load: the loader
     * skips the entrance's position and facing, and scenes skip their
     * arrival scripts.
     */
    FLAG_KEEP_PARTY_POSITION = 0x109,
    /* Shows the location name when the next scene starts. */
    FLAG_SHOW_LOCATION_NAME = 0x12f,
    /* The Vault thieves are held in the jail. */
    FLAG_VAULT_THIEVES_JAILED = 0x855,
    /* The Lunpa chef has told the party about the prisoner in the fortress. */
    FLAG_LUNPA_HEARD_OF_PRISONER = 0x940,
    /* Lunpa's shops and inn trade with the party again. */
    FLAG_LUNPA_TRADE_REOPENED = 0x941,
    /* Hammet and Bunza have met again in West Lunpa Cave. */
    FLAG_LUNPA_CAVE_REUNION_SEEN = 0x94d,
    /* The party stayed behind when Hammet and Bunza left for Kalay. */
    FLAG_PARTY_STAYED_IN_LUNPA = 0x94f
};

#endif
