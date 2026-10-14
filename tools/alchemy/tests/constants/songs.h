// alchemy camelotize constants: song and sound names, as #defines the
// assembler reads too and as enum constants only C reads.
#ifndef SONGS_H
#define SONGS_H

#define SONG_NONE (-1)
#define MUS_VALE 2
#define MUS_SANCTUM 74

#ifndef __ASSEMBLER__
enum
{
	SE_SANCTUM_REVIVE = 82,
	SE_SANCTUM_CURE = 84,
	SE_SANCTUM_BLESS = 88,
	SE_MENU_CURSOR = 111,
	SE_MENU_CONFIRM,
	SE_MENU_CANCEL
};
#endif

#endif
