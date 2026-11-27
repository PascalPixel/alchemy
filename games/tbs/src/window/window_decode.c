/* The message decoder: articles, names and numbers written out. Part of
   window.c. */

/* The most letters of an article mnMessageAppendArticleName writes, and of a
   possessive or pronoun before an item's name. */
#define ARTICLE_MAX_LETTERS		 8
#define POSSESSIVE_MAX_LETTERS	 16
/* The highest character code outside Japanese: those fonts hold a byte's
   worth. */
#define CHARACTER_CODE_MAX		 255
/* The Japanese font's first kana (charmap_ja.tsv): its kana run to
   TEXT_LAST_FONT_CODE, and its kanji start at KANJI_FIRST_CODE (window.h). */
#define KANA_FIRST_CODE			 128
/* Halting speech breaks off with an ellipsis at its first space, or after this
   many codes. */
#define HALTING_SPEECH_MAX_CODES 10
/* MESSAGE_BUTTON_ICON's first character code. */
#define BUTTON_ICON_FIRST_CODE	 128
/* The value of an operand the message stores one above it, taken one down as a
   halfword. */
#define OPERAND_VALUE(stored)	 ((stored) + (Uint32)(Uint16)-1)

/* Copies the unit name at `unitName` into `out`, each byte widened to a
   character code, through the decoder's cursors nameBytes and nameOutput and
   its counter `i`. It is several statements, written only as a case's own. */
#define UNIT_NAME_WIDEN(out, unitName, i) \
	nameBytes = (unitName); \
	nameOutput = (out); \
	(i) = 0; \
	do { \
		*nameOutput = *nameBytes; \
		(i)++; \
		nameBytes++; \
		nameOutput++; \
	} while((i) < UNIT_NAME_SIZE)

#if LANGUAGE != LANGUAGE_JAPANESE
/* The article classes a name can take in each European language, and the bits
   of its class. */
#define ARTICLE_KINDS	  BY_LANGUAGE(8, 8, 8, 5, 8, 11)
#define ARTICLE_KIND_MASK BY_LANGUAGE(7, 7, 7, 15, 7, 15)

/* The article written before a name of each article class. */
typedef struct ARTICLE_TABLE {
	Sint8 *text[ARTICLE_KINDS];
} ARTICLE_TABLE;

#endif

/* The control codes of a message as stored (the PO catalogs' names), which
   mnMessageDecode expands into decoded text. */
enum {
	MESSAGE_END = 0,		/* kept as TEXT_END: the window stays open */
	MESSAGE_PAGE_BREAK = 1, /* kept as TEXT_PAGE */
	/* kept as TEXT_WAIT_KEY: a button closes the window */
	MESSAGE_END_WAIT = 2,
	MESSAGE_LINE_BREAK = 3, /* kept as TEXT_NEWLINE */
	MESSAGE_TEXT_COLOR = 8, /* kept with its operand as TEXT_COLOR */
	/* Kept with its operand as TEXT_STYLE: the style of sprite text, 1 outlined
	   (mnTextDrawGlyphPair), 2 to 5 a motion (mnGlyphDraw). */
	MESSAGE_TEXT_EFFECT = 9,
	MESSAGE_LINE_SPACING = 10, /* kept with its operand as TEXT_LINE_SPACING */
	MESSAGE_UNUSED_11 = 11,	   /* kept with its operand as TEXT_UNUSED_11 */
	MESSAGE_UNUSED_12 = 12,	   /* kept with its operand as TEXT_UNUSED_12 */
	/* German: a counted noun; Italian: a gender ending; Spanish and French: the
	   plural suffix */
	MESSAGE_LOCALIZED_WORD = 13,
	MESSAGE_UNUSED_14 = 14, /* kept with its two operands as TEXT_UNUSED_14 */
	/* Kept with its operand as TEXT_WINDOW_ANIMATION: the window animation to
	   run. */
	MESSAGE_WINDOW_EFFECT = 15,
	/* The name of the unit MainRoutineStateWork.leaderActorId names. */
	MESSAGE_PROTAGONIST_NAME = 16,
	MESSAGE_ACTOR_NAME = 17, /* the name of the unit its operand names */
	/* The argument codes write out the next message argument of their kind
	   (MESSAGE_ARGUMENT_). */
	MESSAGE_ARGUMENT_ACTOR_NAME = 18,
	MESSAGE_ARGUMENT_CLASS_NAME = 19,
	MESSAGE_ARGUMENT_ITEM_NAME = 20,
	MESSAGE_ARGUMENT_ABILITY_NAME = 21,
	/* The number argument, with a minus sign when negative; outside Japanese
	   any number but -1, 0 and 1 makes what follows plural. */
	MESSAGE_ARGUMENT_SIGNED_NUMBER = 22,
	MESSAGE_ARGUMENT_LOCATION_NAME = 23,
	/* A dash: TEXT_EM_DASH_LEAD and a hyphen, and outside English a space
	   after; the Japanese decoder keeps the code as it is. */
	MESSAGE_EM_DASH = 24,
	/* English: the plural suffix; German, Spanish, French and Italian: the
	   item's possessive */
	MESSAGE_PLURAL_SUFFIX = 25,
	/* A button drawn as two characters, codes 128 + 2 * n and the next for
	   operand value n; the Japanese decoder keeps the code as it is. */
	MESSAGE_BUTTON_ICON = 26,
	/* English: an apostrophe, and an s unless the name ended in s; German: an
	   apostrophe after s or z, else an s; nothing in Spanish, French and
	   Italian. The Japanese decoder keeps the code as it is. */
	MESSAGE_POSSESSIVE_SUFFIX = 27,
	/* The article class of the name it starts, in item and class names, which
	   mnMessageAppendArticleName reads and drops. */
	MESSAGE_ARTICLE_CLASS = 29,
	/* Kept as TEXT_END_ALT, which the message ends as it ends TEXT_END. */
	MESSAGE_END_NOW = 30,
};

/* How a name argument is written ({argument_item_name:n} and the like, the
   catalogs showing the form one above): with or without its article, and
   singular or plural. */
typedef enum ARTICLE_FORM {
	ARTICLE_FORM_BARE = 0,	  /* the name alone */
	ARTICLE_FORM_ARTICLE = 1, /* the name after its article ("a Herb") */
	/* English: the name with its plural ending ("Herbs"); the other European
	   languages: the plural name after its article. */
	ARTICLE_FORM_PLURAL = 2,
	/* ARTICLE_FORM_ARTICLE or ARTICLE_FORM_PLURAL, by the number argument
	   written before it. */
	ARTICLE_FORM_BY_NUMBER = 3,
	/* Outside English: the plural name alone. */
	ARTICLE_FORM_BARE_PLURAL = 4,
} ARTICLE_FORM;
#if LANGUAGE == LANGUAGE_SPANISH
/* In Spanish, the form's bit asking for the indefinite article rather than the
   definite one, above the form. */
#define ARTICLE_FORM_INDEFINITE 8
#define ARTICLE_FORM_MASK		7
#endif

/* The state of the text just written, which an article or ending after it
   follows: a sentence's end capitalises the next article, and a name ending in
   s (in German, s or z) takes a different possessive or plural ending. */
enum {
	LAST_LETTER_OTHER = 0,
	LAST_LETTER_S = 1,
	LAST_LETTER_SENTENCE_END = 2,
};

/* mnMessageAppendArticleName copies a name into the decoded text at
   writePosition, as articleForm asks, and returns the position after it. In a
   Japanese battle message the name is set off by a space on each side, typed at
   the default line spacing with the spacing 8 after it. */

#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
/* The German, Spanish, French and Italian form: the bare forms take no article,
   the others the article of the name's class, capitalised at the start of a
   sentence. */
Uint32 mnMessageAppendArticleName(Sint32 inBattleMessage, Uint16 *name,
	Uint32 writePosition, Uint16 *entries, Sint32 articleForm, Sint32 plural,
	Sint32 *lastLetter)
{
	struct ARTICLE_TABLE table;
#if LANGUAGE == LANGUAGE_SPANISH
	struct ARTICLE_TABLE indefiniteTable;
	Uint16				 form;
	Sint16				 indefinite;
#endif
	Sint32 kind;
	Uint16 head;
	Uint16 code;
	Sint8 *p;
	Sint8  letter;
	Sint32 letterIndex;

#if LANGUAGE == LANGUAGE_SPANISH
	form = articleForm & ARTICLE_FORM_MASK;
	indefinite = articleForm & ARTICLE_FORM_INDEFINITE;
#endif
	if(inBattleMessage != 0) {
		TEXT_APPEND(entries, writePosition, ' ');
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
	}
	/* A name's leading article class (MESSAGE_ARTICLE_CLASS and its operand) is
	   never copied. */
#if LANGUAGE == LANGUAGE_SPANISH
	if(form == ARTICLE_FORM_BARE || form == ARTICLE_FORM_BARE_PLURAL) {
#else
	if(articleForm == ARTICLE_FORM_BARE ||
		articleForm == ARTICLE_FORM_BARE_PLURAL) {
#endif
		head = name[0];
		if(head == MESSAGE_ARTICLE_CLASS)
			name += 2;
	}

	else {
		kind = 0;
		table = Article_Table;
#if LANGUAGE == LANGUAGE_SPANISH
		indefiniteTable = Article_Indefinite;
#endif
		if(name[0] == MESSAGE_ARTICLE_CLASS) {
			kind = name[1];
			name += 2;
		}
#if LANGUAGE == LANGUAGE_SPANISH
		if(indefinite == 0)
			p = table.text[kind & ARTICLE_KIND_MASK];
		else
			p = indefiniteTable.text[kind & ARTICLE_KIND_MASK];
#else
		p = table.text[kind & ARTICLE_KIND_MASK];
#endif
		/* The first letter is capitalised at the start of a sentence. */
		for(letterIndex = 0; letterIndex < ARTICLE_MAX_LETTERS; letterIndex++) {
			if((letter = *p++) == 0)
				break;
			if(letterIndex == 0 && *lastLetter == LAST_LETTER_SENTENCE_END)
				letter -= 'a' - 'A';
			TEXT_APPEND(entries, writePosition, letter);
		}
	}
	while(*name != 0) {
		code = *name++;
		TEXT_APPEND(entries, writePosition, code);
#if LANGUAGE == LANGUAGE_GERMAN
		if(code == 's' || code == 'S' || code == 'z' || code == 'Z')
			*lastLetter = LAST_LETTER_S;
		else
			*lastLetter = LAST_LETTER_OTHER;
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
		if(code == 's' || code == 'S')
			*lastLetter = LAST_LETTER_S;
		else
			*lastLetter = LAST_LETTER_OTHER;
#endif
	}
	if(inBattleMessage != 0) {
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, 8);
		TEXT_APPEND(entries, writePosition, ' ');
	}
	return writePosition;
}
#endif

#if LANGUAGE == LANGUAGE_ENGLISH
/* The English article classes mnMessageAppendArticleName chooses between
   (Article_Table) when a name's class does not name one. */
enum {
	ARTICLE_CLASS_NONE = 0,
	ARTICLE_CLASS_A = 1,
	ARTICLE_CLASS_AN = 2,
};

/* The English form: the singular takes "a" or "an" (by the name's first letter,
   unless its class names an article), and the plural adds "s", or "es" after an
   s. */
Uint32 mnMessageAppendArticleName(Sint32 inBattleMessage, Uint16 *name,
	Uint32 writePosition, Uint16 *entries, Sint32 articleForm, Sint32 plural,
	Sint32 *lastLetter)
{
	struct ARTICLE_TABLE table;
	Sint32				 kind;
	Uint16				 head;
	Uint16				 code;
	Uint8				*p;
	Sint8				 letter;
	Sint32				 letterIndex;

	if(inBattleMessage != 0) {
		TEXT_APPEND(entries, writePosition, ' ');
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
	}
	if(articleForm == ARTICLE_FORM_ARTICLE ||
		(articleForm == ARTICLE_FORM_BY_NUMBER && plural == 0)) {
		kind = 0;
		table = Article_Table;
		head = name[0];
		/* The class operand is stored one above its value. */
		if(head == MESSAGE_ARTICLE_CLASS) {
			kind = name[1] - 1;
			name += 2;
		}
		if(kind == ARTICLE_CLASS_NONE) {
			if(head == 'A' || head == 'I' || head == 'U' || head == 'E' ||
				head == 'O')
				kind = ARTICLE_CLASS_AN;
			else
				kind = ARTICLE_CLASS_A;
		}
		p = table.text[kind & 7];
		for(letterIndex = 0; letterIndex < ARTICLE_MAX_LETTERS; letterIndex++) {
			if((letter = *p++) == 0)
				break;
			TEXT_APPEND(entries, writePosition, letter);
		}
	} else {
		head = name[0];
		if(head == MESSAGE_ARTICLE_CLASS)
			name += 2;
	}
	while(*name != 0) {
		code = *name++;
		TEXT_APPEND(entries, writePosition, code);
		if(code == 'S' || code == 's')
			*lastLetter = LAST_LETTER_S;
		else
			*lastLetter = LAST_LETTER_OTHER;
	}
	if(articleForm == ARTICLE_FORM_PLURAL ||
		(articleForm == ARTICLE_FORM_BY_NUMBER && plural != 0)) {
		if(*lastLetter != LAST_LETTER_OTHER) {
			TEXT_APPEND(entries, writePosition, 'e');
		}
		TEXT_APPEND(entries, writePosition, 's');
	}
	if(inBattleMessage != 0) {
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, 8);
		TEXT_APPEND(entries, writePosition, ' ');
	}
	return writePosition;
}
#endif

/* What the decode flags of mnMessageSetDecodeFlags put between characters. */
#define ENTRY_NARROW_GAP TEXT_PAUSE_SHORT
/* With spacingAfter (English and Japanese): TEXT_MARK_VOICED after a kana or
   the first kanji (KANA_FIRST_CODE to KANJI_FIRST_CODE), other than the kana
   marks and the Japanese full stop, comma and middle dot, unless the code after
   it is a kana mark. */
#define ENTRY_WIDE_GAP	 TEXT_MARK_VOICED

#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_GERMAN || \
	LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || \
	LANGUAGE == LANGUAGE_ITALIAN

#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#if LANGUAGE == LANGUAGE_FRENCH
extern Sint8 LocalizedWord_ItemPronoun[];
#else
/* The possessive written before an item's name, chosen by the item's gender or
   by number. */
typedef struct ITEM_POSSESSIVE_TABLE {
	Sint8 *text[ITEM_POSSESSIVE_TEXT_COUNT];
} ITEM_POSSESSIVE_TABLE;
extern const struct ITEM_POSSESSIVE_TABLE LocalizedWord_ItemPossessives;
extern const Sint8						  LocalizedWord_ItemGenders[];
extern Uint8							 *LocalizedWord_CountedNouns[];
#endif
#endif

/* Decodes message script into the ring of decoded text (WIN_STATE.textBuf) and
   returns where it starts; script -1 returns the last message's start again. */
Sint32 mnMessageDecode(Sint32 script, Sint32 clear)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   code = 0;
#if LANGUAGE == LANGUAGE_ENGLISH
	Sint32 lineStart = 1;
#endif
	Sint32 quote = 0;
	Uint32 start = state->textBufEnd;
	Uint32 decodedCount = 0;
	Sint32 plural = 0;
#if LANGUAGE != LANGUAGE_GERMAN && LANGUAGE != LANGUAGE_ITALIAN
	Bool running = TRUE;
#endif
#if LANGUAGE == LANGUAGE_ENGLISH
	Uint32 writePosition = start;
	Uint32 previousCode;
	Sint32 lastLetter = LAST_LETTER_OTHER;
#else
#if LANGUAGE == LANGUAGE_SPANISH
	struct ITEM_POSSESSIVE_TABLE possessives;
	Sint32						 form;
#endif
	Sint32 lastLetter = LAST_LETTER_SENTENCE_END;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
	Sint32 running = TRUE;
#endif
	Sint8 *word;
	Sint8  letter;
	Uint32 writePosition = start;
#endif
	Uint16 *entry = state->textBuf;
	Bool	overflow = FALSE;
	Sint32 (*decode)(MESSAGE_READER *);
	Uint32 nextPosition;
	Sint32 articleForm;
	Uint32 j;
	Uint32 i;
	Sint32 value;
	Sint32 argument;
#if LANGUAGE == LANGUAGE_ITALIAN
	Sint32 ending;
#endif
#if FIX_SIGNED_OVERFLOW
	Uint32 magnitude;
#else
	Sint32 magnitude;
#endif
	Uint8		  *numberText;
	Uint8		  *nameBytes;
	Uint16		  *nameOutput;
	Uint16		  *p;
	Uint16		   name[MESSAGE_NAME_HALFWORDS];
	Uint8		   numberBuffer[TEXT_NUMBER_BUFFER_SIZE];
	MESSAGE_READER reader;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
	struct ITEM_POSSESSIVE_TABLE possessives;
	Sint32						 kind;
#endif

	if(script == -1) {
		start = state->lastMsgStart;
	} else {
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbol,
			Text_DecodeSymbolCodeSize);
		/* The decoder runs from a copy in RAM, one symbol at a time. */
		decode = (Sint32 (*)(MESSAGE_READER *))HEAP_SLOT_BLOCK(
			HEAP_SLOT_MESSAGE_DECODER);
		mnMessageFind(&reader, script);

		for(; running != 0 && decodedCount <= TEXT_BUFFER_MASK;
			decodedCount++) {
#if LANGUAGE == LANGUAGE_ENGLISH
			previousCode = code;
#endif
			code = (Uint32)decode(&reader);
			/* A symbol past the font's characters becomes '@'. */
			if(code > CHARACTER_CODE_MAX)
				code = '@';

			/* Once halting speech has broken off, the rest of the message is
			   skipped, though its arguments are still taken from the table. */
			if(overflow != 0) {
				if(code < TEXT_SPACE) {
					switch(code) {
					case MESSAGE_ARGUMENT_CLASS_NAME:
						decode(&reader);
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_CLASS, clear);
						break;
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = FALSE;
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_NUMBER, clear);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						decode(&reader);
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, clear);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_LOCATION, clear);
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
						decode(&reader);
						break;
					case MESSAGE_ACTOR_NAME:
						decode(&reader);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						decode(&reader);
						break;
					case MESSAGE_ARTICLE_CLASS:
						decode(&reader);
						break;
					case MESSAGE_PAGE_BREAK:
						/* The message ends at its first page, waiting for a
						   button. */
						running = FALSE;
						code = TEXT_WAIT_KEY;
						break;
					case MESSAGE_PROTAGONIST_NAME:
						break;
					case -1:
						break;
					default:
						break;
					}
				}
			} else {
				/* Halting speech pauses before each code. */
				if(state->haltingSpeech != 0
#if LANGUAGE == LANGUAGE_ENGLISH
					&& lineStart == 0 && code != TEXT_MARK_VOICED &&
					code != TEXT_MARK_SEMIVOICED
#endif
				) {
					TEXT_APPEND(entry, writePosition, ENTRY_NARROW_GAP);
				}

#if LANGUAGE == LANGUAGE_ENGLISH
				if(state->spacingAfter != 0 && lineStart == 0 &&
					code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED &&
					previousCode <= KANJI_FIRST_CODE &&
					previousCode >= KANA_FIRST_CODE &&
					previousCode != TEXT_MARK_VOICED &&
					previousCode != TEXT_MARK_SEMIVOICED &&
					previousCode != TEXT_SPACE &&
					previousCode != TEXT_JAPANESE_MIDDLE_DOT &&
					previousCode != TEXT_JAPANESE_FULL_STOP &&
					previousCode != TEXT_JAPANESE_COMMA) {
					TEXT_APPEND(entry, writePosition, ENTRY_WIDE_GAP);
				}

#endif
				/* A character. */
				if(code >= TEXT_SPACE) {
					if(state->haltingSpeech != 0 &&
						(code == TEXT_SPACE ||
							decodedCount > HALTING_SPEECH_MAX_CODES)) {
						TEXT_APPEND(entry, writePosition, '.');
						TEXT_APPEND(entry, writePosition, '.');
						TEXT_APPEND(entry, writePosition, '.');
						overflow = TRUE;
						if(decodedCount > HALTING_SPEECH_MAX_CODES)
							code = TEXT_SPACE;
					}
					/* Every other double quote, the opening one, becomes
					   TEXT_OPENING_QUOTE. */
					if(code == '"') {
						quote ^= 1;
						if(quote != 0)
							code = TEXT_OPENING_QUOTE;
					}
					TEXT_APPEND(entry, writePosition, (Uint16)code);
#if LANGUAGE == LANGUAGE_ENGLISH
					lineStart = 0;
#else
					if(code != ' '
#if LANGUAGE == LANGUAGE_SPANISH
						&& code != TEXT_INVERTED_EXCLAMATION_MARK &&
						code != TEXT_INVERTED_QUESTION_MARK
#endif
					) {
						lastLetter = LAST_LETTER_OTHER;
						if(code == '.' || code == '!' || code == '?')
							lastLetter = LAST_LETTER_SENTENCE_END;
						if(code == 's' || code == 'S'
#if LANGUAGE == LANGUAGE_GERMAN
							|| code == 'z' || code == 'Z'
#endif
						)
							lastLetter = LAST_LETTER_S;
					}
#endif
				} else {
					/* A control code: write out names, numbers and articles,
					   and keep style codes with their operands. */
					switch(code) {
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = FALSE;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
					case MESSAGE_ARTICLE_CLASS:
						TEXT_APPEND(entry, writePosition, (Uint16)code);
						TEXT_APPEND(entry, writePosition,
							OPERAND_VALUE(decode(&reader)));
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						value = (Sint32)mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_NUMBER, clear);
						magnitude = value;
						if(value < 0)
#if FIX_SIGNED_OVERFLOW
							/* Signed -value overflows for INT_MIN; its unsigned
							   magnitude still gives the correct singular/plural
							   comparison. */
							magnitude = 0U - (Uint32)value;
#else
							magnitude = -value;
#endif
						plural = 1;
						if(magnitude <= 1)
							plural = 0;
						numberText = mnStringFormatNum(numberBuffer, value, 0);
						for(i = numberText - numberBuffer;
							i != TEXT_NUMBER_BUFFER_SIZE &&
							numberBuffer[i] != 0;
							i++) {
							TEXT_APPEND(entry, writePosition, numberBuffer[i]);
						}
						break;
					case MESSAGE_ARGUMENT_CLASS_NAME:
						articleForm = decode(&reader) - 1;
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_CLASS, clear);
						mnMessageDecodeRaw(argument + MsgClassName, name,
							MESSAGE_NAME_HALFWORDS);
						writePosition =
							mnMessageAppendArticleName(0, name, writePosition,
								entry, articleForm, plural, &lastLetter);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						articleForm = decode(&reader) - 1;
#if LANGUAGE == LANGUAGE_SPANISH
						form = articleForm & ARTICLE_FORM_MASK;
#endif
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ITEM, clear);
						/* The argument is an inventory code, the item in its
						   low bits. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
						argument &= INVENTORY_ITEM_ID_MASK;
#if LANGUAGE == LANGUAGE_SPANISH
						if(form == ARTICLE_FORM_PLURAL ||
							(form == ARTICLE_FORM_BY_NUMBER && plural != 0) ||
							form == ARTICLE_FORM_BARE_PLURAL)
#else
						if(articleForm == ARTICLE_FORM_PLURAL ||
							(articleForm == ARTICLE_FORM_BY_NUMBER &&
								plural != 0) ||
							articleForm == ARTICLE_FORM_BARE_PLURAL)
#endif
							mnMessageDecodeRaw(argument + MsgItemPluralName,
								name, MESSAGE_NAME_HALFWORDS);
						else
							mnMessageDecodeRaw(argument + MsgItemName, name,
								MESSAGE_NAME_HALFWORDS);
#else
						mnMessageDecodeRaw(
							(argument & INVENTORY_ITEM_ID_MASK) + MsgItemName,
							name, MESSAGE_NAME_HALFWORDS);
#endif
						writePosition =
							mnMessageAppendArticleName(0, name, writePosition,
								entry, articleForm, plural, &lastLetter);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						mnMessageDecodeRaw(argument + MsgAbilityName, name,
							MESSAGE_NAME_HALFWORDS);
						p = name;
						nextPosition = writePosition;
						while(*p != 0) {
							entry[nextPosition] = *p;
							p++;
							nextPosition =
								(nextPosition + 1) & TEXT_BUFFER_MASK;
						}
						writePosition = nextPosition;
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageDecodeRaw(
							fdAreaGetNameIndexStub(
								mnMessageArgumentLookup(
									MESSAGE_ARGUMENT_LOCATION, clear),
								1) +
								MsgDebugAreaName,
							name, MESSAGE_NAME_HALFWORDS);
						p = name;
						nextPosition = writePosition;
						while(*p != 0) {
							entry[nextPosition] = *p;
							p++;
							nextPosition =
								(nextPosition + 1) & TEXT_BUFFER_MASK;
						}
						writePosition = nextPosition;
						break;
					case MESSAGE_PROTAGONIST_NAME:
						UNIT_NAME_WIDEN(name,
							gmUnitGetStub(MainRoutineStateWork.leaderActorId)
								->name,
							i);
						writePosition = mnMessageAppendArticleName(
							0, name, writePosition, entry, 0, 0, &lastLetter);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						articleForm = decode(&reader) - 1;
						UNIT_NAME_WIDEN(name,
							gmUnitGetStub(mnMessageArgumentLookup(
											  MESSAGE_ARGUMENT_ACTOR, clear))
								->name,
							j);
						writePosition =
							mnMessageAppendArticleName(0, name, writePosition,
								entry, articleForm, plural, &lastLetter);
						break;
					case MESSAGE_ACTOR_NAME:
						argument = decode(&reader) - 1;
						UNIT_NAME_WIDEN(name, gmUnitGetStub(argument)->name, j);
						writePosition = mnMessageAppendArticleName(
							0, name, writePosition, entry, 0, 0, &lastLetter);
						break;
					case MESSAGE_BUTTON_ICON:
						argument = ((decode(&reader) - 1) << 1) +
							BUTTON_ICON_FIRST_CODE;
						TEXT_APPEND(entry, writePosition, argument);
						TEXT_APPEND(entry, writePosition, argument + 1);
						break;
					case MESSAGE_EM_DASH:
						TEXT_APPEND(entry, writePosition, TEXT_EM_DASH_LEAD);
						TEXT_APPEND(entry, writePosition, '-');
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
						TEXT_APPEND(entry, writePosition, ' ');
#endif
						break;
#if LANGUAGE == LANGUAGE_GERMAN
					case MESSAGE_LOCALIZED_WORD:
						/* A counted noun, singular or plural, by its kind. */
						articleForm = decode(&reader) - 1;
						for(nameBytes = LocalizedWord_CountedNouns
								[(articleForm & 7) * 2 + plural];
							*nameBytes != 0; nameBytes++) {
							TEXT_APPEND(entry, writePosition, *nameBytes);
						}
						break;
#endif
#if LANGUAGE == LANGUAGE_ITALIAN
					case MESSAGE_LOCALIZED_WORD:
						ending = decode(&reader) - 1;
						if(ending == 0) {
							if(plural == 0)
								entry[writePosition] = 'o';
							else
								entry[writePosition] = 'i';
						} else if(plural == 0)
							entry[writePosition] = 'a';
						else
							entry[writePosition] = 'e';
						writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
						break;
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
					case MESSAGE_LOCALIZED_WORD:
#elif LANGUAGE == LANGUAGE_ENGLISH
					case MESSAGE_PLURAL_SUFFIX:
#endif
#if LANGUAGE != LANGUAGE_GERMAN && LANGUAGE != LANGUAGE_ITALIAN
						if(plural != 0) {
							if(lastLetter != LAST_LETTER_OTHER) {
								TEXT_APPEND(entry, writePosition, 'e');
							}
							TEXT_APPEND(entry, writePosition, 's');
						}
						break;
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
					case MESSAGE_PLURAL_SUFFIX:
						/* The item argument is only looked at (clear 0), so the
						   item's name code after the possessive still takes
						   it. */
#if LANGUAGE == LANGUAGE_SPANISH
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
						possessives = LocalizedWord_ItemPossessives;
						word = possessives.text[plural];
#else
						argument =
							mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
						argument &= INVENTORY_ITEM_ID_MASK;
						possessives = LocalizedWord_ItemPossessives;
						kind = LocalizedWord_ItemGenders[argument];
						word = possessives.text[kind & 3];
#endif
						for(value = 0; (Uint32)value < POSSESSIVE_MAX_LETTERS;
							value++) {
							if((letter = *word++) == 0)
								break;
							if(value == 0 &&
								lastLetter == LAST_LETTER_SENTENCE_END)
								letter -= 'a' - 'A';
							TEXT_APPEND(entry, writePosition, letter);
						}
						break;
#endif
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_FRENCH
					case MESSAGE_PLURAL_SUFFIX:
#if LANGUAGE == LANGUAGE_GERMAN
						argument =
							mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
						argument &= INVENTORY_ITEM_ID_MASK;
						possessives = LocalizedWord_ItemPossessives;
						kind = LocalizedWord_ItemGenders[argument];
						word = possessives.text[kind & 3];
#else
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
						word = LocalizedWord_ItemPronoun;
#endif
						for(i = 0; i < POSSESSIVE_MAX_LETTERS; i++) {
							if((letter = *word++) == 0)
								break;
							if(i == 0 && lastLetter == LAST_LETTER_SENTENCE_END)
								letter -= 'a' - 'A';
							TEXT_APPEND(entry, writePosition, letter);
						}
						lastLetter = LAST_LETTER_OTHER;
						break;
#endif
					case MESSAGE_POSSESSIVE_SUFFIX:
#if LANGUAGE == LANGUAGE_GERMAN
						if(lastLetter == LAST_LETTER_S)
							entry[writePosition] = '\'';
						else
							entry[writePosition] = 's';
						writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#elif LANGUAGE == LANGUAGE_ENGLISH
						TEXT_APPEND(entry, writePosition, '\'');
						if(lastLetter == LAST_LETTER_OTHER) {
							TEXT_APPEND(entry, writePosition, 's');
						}
#endif
						break;
					case -1:
						break;
					case MESSAGE_PAGE_BREAK:
					case MESSAGE_LINE_BREAK:
#if LANGUAGE == LANGUAGE_ENGLISH
						lineStart = 1;
#endif
						/* Keep the break in the decoded text. Fall through. */
					default:
						TEXT_APPEND(entry, writePosition, (Uint16)code);
#if LANGUAGE == LANGUAGE_ENGLISH
						if(code == 's' || code == 'S')
							lastLetter = LAST_LETTER_S;
						else
							lastLetter = LAST_LETTER_OTHER;
#endif
						break;
					}
				}
			}
		}

		/* The code that ended the message goes in too (TEXT_END, TEXT_WAIT_KEY
		   or TEXT_END_ALT, or the last code of a message cut at the ring's
		   length), then a 0; the next message starts after both. */
		TEXT_APPEND(entry, writePosition, (Uint16)code);
		entry[writePosition] = 0;
		nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
		state->textBufEnd = (Uint16)nextPosition;
		syHeapSlotRelease(HEAP_SLOT_MESSAGE_DECODER);
		state->lastMsgStart = (Uint16)start;
	}

	if(clear != 0)
		mnMessageArgumentClearAll();

	return start;
}

#endif

#if LANGUAGE == LANGUAGE_JAPANESE

/* The most codes the Japanese decoder takes of the name it gives the Mystery
   Man or the Mystery Woman in battle. */
#define MYSTERY_PERSON_NAME_CODES 10

/* The Japanese form: Japanese has no articles, so the name is copied as it
   is. */
extern __inline__ Uint32 mnMessageAppendArticleName(Sint32 inBattleMessage,
	Uint16 *name, Uint32 writePosition, Uint16 *entries, Sint32 articleForm,
	Sint32 plural, Sint32 *lastLetter)
{
	Uint16 code;

	if(inBattleMessage != 0) {
		TEXT_APPEND(entries, writePosition, ' ');
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
	}
	while(*name != 0) {
		code = *name++;
		TEXT_APPEND(entries, writePosition, code);
	}
	if(inBattleMessage != 0) {
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, 8);
		TEXT_APPEND(entries, writePosition, ' ');
	}
	return writePosition;
}

/* The Japanese mnMessageDecode: the European decoder's loop without its
   articles, plurals, possessives, dashes, button icons and opening quotes. What
   only this one does: halting speech breaks off with ・・・。 rather than
   "...", a battle message doubles each space and frames each name with spaces
   and line spacing codes (mnMessageAppendArticleName above), and in battle the
   Mystery Man and Mystery Woman take their names from MsgMysteryPersonName. */
Sint32 mnMessageDecode(Sint32 script, Sint32 clear)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   code = 0;
	Sint32	   lineStart = 1;
	Uint32	   start = state->textBufEnd;
	Uint32	   decodedCount = 0;
	Bool	   running = TRUE;
	Uint32	   writePosition = start;
	Uint16	  *entry = state->textBuf;
	Uint8	   inBattleMessage = state->battleMode & WINDOW_BATTLE_MESSAGE;
	Sint32 (*decode)(MESSAGE_READER *);
	Bool		   overflow = FALSE;
	Uint32		   previousCode;
	Uint32		   nextPosition;
	Uint32		   i;
	Uint32		   j;
	Sint32		   argument;
	Sint32		   message;
	Sint32		   value;
	Uint8		  *numberText;
	Uint8		  *nameBytes;
	Uint16		  *nameOutput;
	UNIT		  *unit;
	Uint16		   name[MESSAGE_NAME_HALFWORDS];
	Uint8		   numberBuffer[TEXT_NUMBER_BUFFER_SIZE];
	MESSAGE_READER reader;

	if(script == -1) {
		start = state->lastMsgStart;
	} else {
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbol,
			Text_DecodeSymbolCodeSize);
		/* The decoder runs from a copy in RAM, one symbol at a time. */
		decode = (Sint32 (*)(MESSAGE_READER *))HEAP_SLOT_BLOCK(
			HEAP_SLOT_MESSAGE_DECODER);
		mnMessageFind(&reader, script);
		for(; running != 0 && decodedCount <= TEXT_BUFFER_MASK;
			decodedCount++) {
			previousCode = code;
			code = (Uint32)decode(&reader);
			/* Once halting speech has broken off, the rest of the message is
			   skipped, though its arguments are still taken from the table. */
			if(overflow != 0) {
				if(code < TEXT_SPACE) {
					switch(code) {
					case MESSAGE_ARGUMENT_CLASS_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_CLASS, clear);
						break;
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = FALSE;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
						decode(&reader);
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_NUMBER, clear);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, clear);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_LOCATION, clear);
						break;
					case MESSAGE_ACTOR_NAME:
						decode(&reader);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						break;
					case MESSAGE_PAGE_BREAK:
						running = FALSE;
						code = TEXT_WAIT_KEY;
						break;
					case MESSAGE_PROTAGONIST_NAME:
						break;
					case -1:
						break;
					default:
						break;
					}
				}
			} else {
				/* Halting speech pauses before each character. */
				if(state->haltingSpeech != 0 && lineStart == 0 &&
					code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
					TEXT_APPEND(entry, writePosition, ENTRY_NARROW_GAP);
				}
				if(state->spacingAfter != 0 && lineStart == 0 &&
					code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED &&
					previousCode <= KANJI_FIRST_CODE &&
					previousCode >= KANA_FIRST_CODE &&
					previousCode != TEXT_MARK_VOICED &&
					previousCode != TEXT_MARK_SEMIVOICED &&
					previousCode != TEXT_SPACE &&
					previousCode != TEXT_JAPANESE_MIDDLE_DOT &&
					previousCode != TEXT_JAPANESE_FULL_STOP &&
					previousCode != TEXT_JAPANESE_COMMA) {
					TEXT_APPEND(entry, writePosition, ENTRY_WIDE_GAP);
				}
				/* A character. */
				if(code >= TEXT_SPACE) {
					/* A battle message doubles each space. The ellipsis here is
					   ・・・。. */
					if(inBattleMessage != 0 && code == ' ') {
						TEXT_APPEND(entry, writePosition, code);
					}
					if(state->haltingSpeech != 0 &&
						(code == TEXT_SPACE ||
							decodedCount > HALTING_SPEECH_MAX_CODES)) {
						TEXT_APPEND(
							entry, writePosition, TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(
							entry, writePosition, TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(
							entry, writePosition, TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(
							entry, writePosition, TEXT_JAPANESE_FULL_STOP);
						overflow = TRUE;
						if(decodedCount > HALTING_SPEECH_MAX_CODES)
							code = TEXT_SPACE;
					}
					TEXT_APPEND(entry, writePosition, (Uint16)code);
					lineStart = 0;
				} else {
					/* A control code: write out names and numbers, and keep
					   style codes with their operands. */
					switch(code) {
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = FALSE;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
						TEXT_APPEND(entry, writePosition, (Uint16)code);
						TEXT_APPEND(entry, writePosition,
							OPERAND_VALUE(decode(&reader)));
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						value = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_NUMBER, clear);
						numberText = mnStringFormatNum(numberBuffer, value, 0);
						i = numberText - numberBuffer;
						while(i != TEXT_NUMBER_BUFFER_SIZE &&
							numberBuffer[i] != 0) {
							TEXT_APPEND(entry, writePosition, numberBuffer[i]);
							i++;
						}
						break;
					case MESSAGE_ARGUMENT_CLASS_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_CLASS, clear);
						mnMessageDecodeRaw(argument + MsgClassName, name,
							MESSAGE_NAME_HALFWORDS);
						writePosition =
							mnMessageAppendArticleName(inBattleMessage, name,
								writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ITEM, clear);
						mnMessageDecodeRaw(
							(argument & INVENTORY_ITEM_ID_MASK) + MsgItemName,
							name, MESSAGE_NAME_HALFWORDS);
						writePosition =
							mnMessageAppendArticleName(inBattleMessage, name,
								writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						mnMessageDecodeRaw(argument + MsgAbilityName, name,
							MESSAGE_NAME_HALFWORDS);
						writePosition =
							mnMessageAppendArticleName(inBattleMessage, name,
								writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageDecodeRaw(
							fdAreaGetNameIndexStub(
								mnMessageArgumentLookup(
									MESSAGE_ARGUMENT_LOCATION, clear),
								1) +
								MsgDebugAreaName,
							name, MESSAGE_NAME_HALFWORDS);
						writePosition =
							mnMessageAppendArticleName(inBattleMessage, name,
								writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_PROTAGONIST_NAME:
						UNIT_NAME_WIDEN(name,
							gmUnitGetStub(MainRoutineStateWork.leaderActorId)
								->name,
							i);
						writePosition =
							mnMessageAppendArticleName(inBattleMessage, name,
								writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						unit = gmUnitGetStub(mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ACTOR, clear));
						/* In battle ENEMY_MYSTERY_MAN is called Mystery Man and
						   ENEMY_MYSTERY_WOMAN Mystery Woman
						   (MsgMysteryPersonName and the next). */
						if(state->battleMode != WINDOW_FIELD &&
							(unit->id == ENEMY_MYSTERY_MAN ||
								unit->id == ENEMY_MYSTERY_WOMAN)) {
							message = MsgMysteryPersonName;
							if(unit->id == ENEMY_MYSTERY_WOMAN)
								message++;
							mnMessageDecodeRaw(
								message, name, MYSTERY_PERSON_NAME_CODES);
							writePosition =
								mnMessageAppendArticleName(inBattleMessage,
									name, writePosition, entry, 0, 0, 0);
						} else {
							UNIT_NAME_WIDEN(name, unit->name, j);
							writePosition =
								mnMessageAppendArticleName(inBattleMessage,
									name, writePosition, entry, 0, 0, 0);
						}
						break;
					case MESSAGE_ACTOR_NAME:
						argument = decode(&reader) - 1;
						UNIT_NAME_WIDEN(name, gmUnitGetStub(argument)->name, j);
						writePosition =
							mnMessageAppendArticleName(inBattleMessage, name,
								writePosition, entry, 0, 0, 0);
						break;
					case -1:
						break;
					case MESSAGE_PAGE_BREAK:
					case MESSAGE_LINE_BREAK:
						lineStart = 1;
						/* Keep the break in the decoded text. Fall through. */
					default:
						TEXT_APPEND(entry, writePosition, (Uint16)code);
						break;
					}
				}
			}
		}
		TEXT_APPEND(entry, writePosition, (Uint16)code);
		entry[writePosition] = 0;
		nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
		state->textBufEnd = (Uint16)nextPosition;
		syHeapSlotRelease(HEAP_SLOT_MESSAGE_DECODER);
		state->lastMsgStart = (Uint16)start;
	}
	if(clear != 0)
		mnMessageArgumentClearAll();
	return start;
}

#undef MYSTERY_PERSON_NAME_CODES
#endif

#undef ARTICLE_MAX_LETTERS
#undef POSSESSIVE_MAX_LETTERS
#undef CHARACTER_CODE_MAX
#undef KANA_FIRST_CODE
#undef HALTING_SPEECH_MAX_CODES
#undef BUTTON_ICON_FIRST_CODE
#undef OPERAND_VALUE
#undef UNIT_NAME_WIDEN
