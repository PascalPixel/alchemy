# Alchemy's build and contributor workflows share the alchemy executable.
# Asset packages are described by manifests under recon/; the shared codecs behind
# `alchemy build assets` are named for their formats, not for any resource.

GCC296_CFLAGS := -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi \
                 -fno-builtin -nostdinc -ffreestanding \
                 -fcall-used-r4 -I"games/THE BROKEN SEAL/INCLUDE"

TOOLS := tools
CARGO ?= cargo
export CARGO_TARGET_DIR := $(CURDIR)/tools/out/cargo-target
# Bootstrap's native arm-none-eabi binutils come first for every recipe and test.
export PATH := $(CURDIR)/tools/out/binutils/bin:$(PATH)
CARGO_RUN := $(CARGO) run --offline --quiet --release --manifest-path

BUILD := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- build
ASSETS := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- build assets
CHECK := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- check
COMPILER := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml --
OVERLAY := $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- overlay

HOSTS := alchemy psynergy
PORTABLE_TOOLS := alchemy psynergy
TARGET ?= tbs-en
TARGET_GAME := $(firstword $(subst -, ,$(TARGET)))
TARGET_GAME_DIR := $(if $(filter tbs,$(TARGET_GAME)),THE BROKEN SEAL,$(if $(filter tla,$(TARGET_GAME)),THE LOST AGE,$(TARGET_GAME)))
FULL_REPORT = out/$(TARGET)/full/rebuilt.json
FULL_ROM = out/$(TARGET)/full/rebuilt.gba
OWNER_INVENTORY = out/$(TARGET)/full/rebuilt.owner-inventory.json
REPORT_DIR = out/$(TARGET)/reports
HISTORICAL_TARGETS := tbs-ja tbs-en tbs-de tbs-es tbs-fr tbs-it \
	tla-ja tla-en tla-de tla-es tla-fr tla-it
.PHONY: help verify audit reports test lint lint-production lint-all-targets build-tools tool-tests tooling-index-check \
	build-claimed build-asm build-assets build-full build-rom \
	standard-check compiler-source-check corpus-check \
	full-rom-check tla-assets-check tla-owners-check overlay-check declared-tu-check owner-inventory-check strict-tu-check siblings-check \
	source-tracking-check index-sync-check publication-tree-check check-owners progress progress-report progress-check progress-subject \
	correspondence correspondence-check edition-builds edition-builds-check \
	coverage coverage-check native-format-check review-images-check clean clean-preview
.PHONY: targets $(HISTORICAL_TARGETS)

help:
	@printf '%s\n' \
		'make bootstrap        build/install missing compiler dependencies' \
		'make bootstrap BUNDLE=/path/to/bundle   install an approved bundle' \
		'make verify-clean     verify after deleting generated output' \
		'make verify           fast byte-exact production gate' \
		'make audit            exhaustive editions, candidates, and reports audit' \
		'make reports          refresh analysis reports and coverage figures' \
		'make targets          compile shared source for all 12 historical targets' \
		'make tbs-ja           compile one edition-qualified source target' \
		'make build-rom        rebuild the ROM' \
		'make build-full       rebuild and compare every owned byte' \
		'make full-rom-check   prove the complete tbs-en ROM byte-exact' \
		'make tla-assets-check prove every tla-en asset region byte-exact' \
		'make tla-owners-check prove every exact tla-en owner and shared source' \
		'make overlay-check    audit every exact overlay owner' \
		'make declared-tu-check prove declared production translation-unit contracts' \
		'make owner-inventory-check prove registered owner production coverage' \
		'make strict-tu-check  prove strict production TU composition and owner coverage' \
		'make siblings-check   report twin families; reject address names in instanced sources' \
		'make source-tracking-check reject ignored or untracked Proven C' \
		'make build-assets     rebuild source assets' \
		'make test             focused Rust tests and policy checks' \
		'make tooling-index-check prove every tool is indexed exactly once' \
		'make progress         print byte-exact progress' \
		'make progress-subject print the required commit prefix' \
		'make correspondence   match exact EN owners across TBS editions' \
		'make edition-builds   relink exact EN C across TBS editions' \
		'make coverage         refresh coverage data and figures'

build-claimed:
	$(BUILD) claimed --target $(TARGET)

build-asm:
	$(BUILD) asm

build-assets: prepare-inputs
	$(ASSETS)

build-full: prepare-inputs
	$(BUILD) full --target $(TARGET)

build-rom: prepare-inputs
	$(BUILD) rom --target $(TARGET)

full-rom-check: build-full
	@cmp roms/$(TARGET).gba $(FULL_ROM)
	@grep -Fq '"target": "$(TARGET)"' $(FULL_REPORT)
	@grep -Fq '"verification": "rom"' $(FULL_REPORT)
	@grep -Fq '"byte_identical": true' $(FULL_REPORT)
	@grep -Fq '"unowned_bytes": 0' $(FULL_REPORT)
	@grep -Fq '"rom_fallback_bytes": 0' $(FULL_REPORT)
	@printf 'full ROM contract ok: %s\n' '$(TARGET)'

# The Lost Age has no full ROM contract yet; every asset region its manifest
# owns is rebuilt from source and compared with the tla-en ROM.
TLA_ASSETS = out/tla-en/assets
tla-assets-check: prepare-inputs
	$(ASSETS) --target tla-en -o $(TLA_ASSETS) roms/tla-en.gba
	@grep -Fq '"verification": "rom"' $(TLA_ASSETS)/manifest.json
	@printf 'asset contract ok: tla-en\n'

# Every owner the TLA register gives a source, and with them the TLA half of
# every shared source in games/COMMON, scores byte-exact over its audited
# extent against the tla-en ROM.
tla-owners-check:
	$(CHECK) tla-owners roms/tla-en.gba

overlay-check:
	$(OVERLAY) audit --all

declared-tu-check: full-rom-check
	@grep -Fq '"declared_main_translation_units_strict": true' $(FULL_REPORT)
	@printf 'declared translation-unit production contract ok\n'

owner-inventory-check: full-rom-check
	@test -s $(OWNER_INVENTORY)
	@grep -Fq '"complete_registered_identity_coverage": true' $(OWNER_INVENTORY)
	@printf 'registered owner production inventory ok\n'

strict-tu-check: declared-tu-check owner-inventory-check overlay-check
	@printf 'strict production TU contracts ok; historical original boundaries remain unknown\n'

# Missed siblings (R1) and copied modules (R2) are reported until their waves
# close; semantic spelling in instanced sources (R3) is enforced.
siblings-check: full-rom-check
	$(CHECK) siblings

targets: $(HISTORICAL_TARGETS)

$(HISTORICAL_TARGETS):
	$(BUILD) claimed --target $@ --compile-only --output out/$@/compile

.PHONY: dashboard dashboard-service-install dashboard-restart
dashboard:
	$(COMPILER) dashboard --bind 127.0.0.1:4650

# The service runs the binary it started with; restart it after tooling merges
# or the dashboard keeps computing DONE under the old rules.
dashboard-restart:
	@launchctl kickstart -k 'gui/$(shell id -u)/com.pascalpixel.alchemy-dashboard'

dashboard-service-install:
	@mkdir -p '$(HOME)/Library/LaunchAgents' '$(CURDIR)/out'
	@sed -e 's|@ALCHEMY_ROOT@|$(CURDIR)|g' \
		-e 's|@CARGO@|$(shell command -v $(CARGO))|g' \
		tools/alchemy/src/dashboard/com.pascalpixel.alchemy-dashboard.plist.in \
		> '$(HOME)/Library/LaunchAgents/com.pascalpixel.alchemy-dashboard.plist'
	@launchctl bootout 'gui/$(shell id -u)/com.pascalpixel.alchemy-dashboard' 2>/dev/null || true
	@launchctl bootstrap 'gui/$(shell id -u)' '$(HOME)/Library/LaunchAgents/com.pascalpixel.alchemy-dashboard.plist'

progress:
	$(CHECK) progress

progress-check: full-rom-check
	$(CHECK) progress --check

progress-report: full-rom-check | $(REPORT_DIR)
	$(CHECK) progress --write-report

progress-subject:
	$(CHECK) progress --subject

$(REPORT_DIR):
	@mkdir -p $@

correspondence: build-full | $(REPORT_DIR)
	$(COMPILER) cross-edition --all --object-dir out/tbs-en/full/claimed/obj \
		--write $(REPORT_DIR)/exact-correspondence.json \
		--edition-build $(REPORT_DIR)/exact-main-builds.json
	$(COMPILER) cross-edition --all-overlays \
		--write $(REPORT_DIR)/exact-overlay-correspondence.json \
		--edition-build $(REPORT_DIR)/exact-overlay-builds.json

correspondence-check: correspondence
	@set -e; \
	main='$(REPORT_DIR)/exact-correspondence.json'; \
	overlay='$(REPORT_DIR)/exact-overlay-correspondence.json'; \
	main_matched=$$(sed -n 's/.*"matched_owners": \([0-9][0-9]*\),/\1/p' "$$main" | head -n 1); \
	main_unresolved=$$(sed -n 's/.*"unresolved_owners": \([0-9][0-9]*\),/\1/p' "$$main" | head -n 1); \
	overlay_matched=$$(sed -n 's/.*"matched_owners": \([0-9][0-9]*\),/\1/p' "$$overlay" | head -n 1); \
	overlay_unresolved=$$(sed -n 's/.*"unresolved_owners": \([0-9][0-9]*\),/\1/p' "$$overlay" | head -n 1); \
	test -n "$$main_matched" -a -n "$$main_unresolved" \
		-a -n "$$overlay_matched" -a -n "$$overlay_unresolved"; \
	printf 'cross-edition correspondence ok: main=%s/%s overlay=%s/%s matched/unresolved\n' \
		"$$main_matched" "$$main_unresolved" "$$overlay_matched" "$$overlay_unresolved"

edition-builds: correspondence
	@printf 'edition build report: %s\n' '$(REPORT_DIR)/exact-main-builds.json'

edition-builds-check: correspondence-check
	@printf 'cross-edition edition-build audit ok\n'

coverage: | $(REPORT_DIR)
	$(CHECK) coverage --write

coverage-check:
	$(CHECK) coverage --check

source-tracking-check: prepare-inputs
	$(CHECK) source-tracking

index-sync-check:
	@git diff --quiet --ignore-submodules -- || { \
		printf 'unstaged tracked changes found; stage the tree before make verify\n'; exit 1; \
	}
	@set -e; untracked=$$(git ls-files --others --exclude-standard); \
	test -z "$$untracked" || { \
		printf 'untracked files found; stage or ignore them before make verify:\n%s\n' "$$untracked"; exit 1; \
	}
	@printf 'index and worktree agree\n'

publication-tree-check:
	$(CHECK) publication --tree

check-owners: source-tracking-check
	$(CHECK) owners

corpus-check:
	@test -f "recon/tbs/project.json"
	@test -f "recon/tla/project.json"
	@if test -d draft; then \
		printf 'legacy draft/ directory found; use recon/tbs/<edition>/\n'; \
		exit 1; \
	fi
	@if find "recon/tbs/semantic" -maxdepth 1 -name '*.c' -print | grep -q .; then \
		printf 'source hypotheses belong in recon/tbs/<edition>/, not recon/tbs/semantic/ metadata\n'; \
		exit 1; \
	fi
	@roots=$$(git ls-files -- games | cut -d/ -f2 | grep -vx COMMON | LC_ALL=C sort -u | tr '\n' '|'); \
	test "$$roots" = 'THE BROKEN SEAL|THE LOST AGE|' || { \
		printf 'games/ holds only the two game roots and the COMMON shared source root, found: %s\n' "$$roots"; exit 1; \
	}
	@scaffolding=$$(git ls-files -- games | cut -d/ -f3 | grep '[a-z]' | LC_ALL=C sort -u | tr '\n' '|'); \
	test -z "$$scaffolding" || { \
		printf 'games/ holds only the uppercase Camelot-shaped tree; move reconstruction scaffolding to recon/<game>/: %s\n' "$$scaffolding"; exit 1; \
	}
	@roots=$$(git ls-files -- recon | cut -d/ -f2 | LC_ALL=C sort -u | tr '\n' '|'); \
	test "$$roots" = 'tbs|tla|' || { \
		printf 'recon/ holds only the tbs and tla scaffolding roots, found: %s\n' "$$roots"; exit 1; \
	}
	@printf 'corpus ok: two shared-source games, 12 edition targets, games/COMMON shared source only, scaffolding under recon/\n'

build-tools:
	@set -e; for host in $(HOSTS); do \
		printf '  build %-20s' "$$host"; \
		$(CARGO) build --offline --quiet --release \
			--manifest-path $(TOOLS)/$$host/Cargo.toml; \
		printf ' ok\n'; \
	done

tool-tests:
	$(CARGO) test --offline --quiet --release --workspace \
		--manifest-path $(TOOLS)/Cargo.toml
	$(COMPILER) match --acceptance-test

tooling-index-check:
	@$(CHECK) publication --documents
	@set -eu; \
	actual=$$(mktemp /tmp/alchemy-tool-index.actual.XXXXXX); \
	indexed=$$(mktemp /tmp/alchemy-tool-index.indexed.XXXXXX); \
	duplicates=$$(mktemp /tmp/alchemy-tool-index.duplicates.XXXXXX); \
	trap 'rm -f "$$actual" "$$indexed" "$$duplicates"' EXIT; \
	{ find tools -mindepth 2 -maxdepth 2 -name Cargo.toml -print | sed 's#tools/##;s#/Cargo.toml##'; \
	  find tools -mindepth 2 -maxdepth 2 -type f \( -name '*.ts' -o -name '*.js' -o -name '*.py' -o -name '*.sh' \) -print | sed 's#tools/##;s#/[^/]*$$##'; \
	} | sort -u > "$$actual"; \
	awk -F '[()]' '/^## Tooling index/ { inside=1; next } /^## / { inside=0 } inside && /^\| \[/ { sub(/^tools\//, "", $$2); sub(/\/$$/, "", $$2); print $$2 }' AGENTS.md | sort > "$$indexed"; \
	uniq -d "$$indexed" > "$$duplicates"; \
	test ! -s "$$duplicates" || { printf 'tooling index duplicates:\n'; cat "$$duplicates"; exit 1; }; \
	diff -u "$$actual" "$$indexed"; \
	printf 'tooling index ok: %s tools\n' "$$(wc -l < "$$actual" | tr -d ' ')"

# Restore one game from the current commit when a local experiment needs to be discarded.
verified-restore:
	@set -eu; tree=$$(git rev-parse 'HEAD^{tree}'); \
	git checkout "$$tree" -- "games/$(TARGET_GAME_DIR)" "recon/$(TARGET_GAME)"; \
	git diff --cached --name-only -z --diff-filter=A "$$tree" -- "games/$(TARGET_GAME_DIR)" "recon/$(TARGET_GAME)" | xargs -0 -r git rm -q -f --cached; \
	git diff --name-only -z --diff-filter=A "$$tree" -- "games/$(TARGET_GAME_DIR)" "recon/$(TARGET_GAME)" | xargs -0 -r rm -f; \
	printf 'games/%s and recon/%s restored to HEAD tree %s\n' '$(TARGET_GAME_DIR)' '$(TARGET_GAME)' "$$tree"

# Tooling and dashboard behavior are Rust.
# Asset and game source directories carry no scripts.
language-check:
	@set -eu; \
	scripts=$$(git ls-files --cached --others --exclude-standard | grep -E '\.(ts|js|mjs|cjs|py|sh)$$' || true); \
	if [ -n "$$scripts" ]; then printf 'TypeScript, JavaScript, Python, or shell implementation files are not allowed:\n%s\n' "$$scripts"; exit 1; fi; \
	printf 'language gate ok: Rust tooling and dashboard\n'

lint: lint-all-targets

lint-production: standard-check compiler-source-check
	@set -e; git ls-files --cached --others --exclude-standard '*.rs' | while IFS= read -r source; do \
		test ! -f "$$source" || rustfmt --edition 2021 --check "$$source"; \
	done
	$(CHECK) no-asm --target $(TARGET)

lint-all-targets: standard-check compiler-source-check
	@set -e; git ls-files --cached --others --exclude-standard '*.rs' | while IFS= read -r source; do \
		test ! -f "$$source" || rustfmt --edition 2021 --check "$$source"; \
	done
	$(CHECK) no-asm

test: toolchain-check native-format-check lint tooling-index-check tool-tests compiler-source-check
	$(CHECK) publication --self-test
	$(CHECK) publication --tree
	$(CHECK) commit-progress --self-test
	$(CHECK) progress --self-test
	$(CHECK) no-asm --self-test

native-format-check:
	$(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- format --check

review-images-check: source-tracking-check
	$(ASSETS) --review-images out/tbs-en/graphics-review

verify: toolchain-check native-format-check index-sync-check publication-tree-check source-tracking-check review-images-check corpus-check language-check lint-production tooling-index-check \
	strict-tu-check check-owners full-rom-check coverage-check siblings-check

audit: verify test targets \
	correspondence-check progress-report coverage-check

reports: correspondence progress-report coverage

standard-check:
	@set -e; actual=$$($(CHECK) routes --standard | grep -v '^-I' | sort); \
	expected=$$(printf '%s\n' $(GCC296_CFLAGS) | grep -v '^-I' | sort); \
	test "$$actual" = "$$expected" || { printf 'compiler flags differ\nexpected:\n%s\nactual:\n%s\n' "$$expected" "$$actual"; exit 1; }
	@printf 'compiler standard ok\n'

.PHONY: bootstrap compiler-sources compilers toolchain-check verify-clean prepare-inputs

# Every game's registered private inputs are checked by source tracking, so
# both indexed editions are restored whatever TARGET selects. A clone without
# The Lost Age ROM cannot restore that game's inputs; source tracking then
# checks their registration but not their absent bytes.
prepare-inputs:
	$(ASSETS) --extract-missing-sources roms/tbs-en.gba
	$(if $(wildcard roms/tla-en.gba),$(ASSETS) --extract-missing-sources roms/tla-en.gba --target tla-en)

bootstrap:
	$(COMPILER) bootstrap $(if $(BUNDLE),--from "$(BUNDLE)")

toolchain-check:
	$(COMPILER) bootstrap --check

# Check dependencies before cleaning so a missing installation leaves the
# current reports intact. Repeat after cleanup to enforce the storage boundary.
verify-clean: toolchain-check
	$(MAKE) clean
	$(MAKE) toolchain-check
	$(MAKE) verify

compilers:
	$(MAKE) compiler-sources

compiler-sources: compiler-source-check
	sh agscc/build.sh
	$(MAKE) -C agbcc/gcc old -j1

compiler-source-check:
	@set -e; for repo in agbcc agscc; do \
		case "$$repo" in \
		  agbcc) approved=da598c1d918402c42c0c0d7128ba14567f3175e9;; \
		  agscc) approved=f2095030ce7fa3b8991a5b5bdbe32a1860c6fa34;; \
		esac; \
		test "$$(git rev-parse :$$repo)" = "$$approved" || { printf '%s gitlink is not approved\n' "$$repo"; exit 1; }; \
		test "$$(env -u GIT_INDEX_FILE git -C "$$repo" rev-parse HEAD)" = "$$approved" || { printf '%s checkout is not approved\n' "$$repo"; exit 1; }; \
		state=$$(env -u GIT_INDEX_FILE git -C "$$repo" status --porcelain --untracked-files=all -- . ':(exclude,glob)**/.DS_Store'); \
		test -z "$$state" || { printf '%s compiler source is dirty\n' "$$repo"; exit 1; }; \
	done
	@printf 'compiler sources match approved submodules\n'

CLEAN_TREES := out work build builds dist cmatch comparisons compiler-output \
	diffs disassembly dumps m2c objdump reports analysis .cache target

clean-preview:
	@printf 'would remove generated trees (never roms/):\n'; \
	for tree in $(CLEAN_TREES); do \
		if [ -e "$$tree" ]; then printf '  %-22s %s\n' "$$tree" "$$(du -sh "$$tree" | cut -f1)"; fi; \
	done

clean:
	@for tree in $(CLEAN_TREES); do \
		if [ -e "$$tree" ]; then rm -rf -- "$$tree"; fi; \
	done
	@find $(TOOLS) -type d -name target -prune -exec rm -rf -- {} +
	@printf 'generated trees removed; roms/ untouched\n'

# Both games build the licensed agscc submodule; TLA selects -mgs2 at compile time.
.PHONY: compilers-tla
compilers-tla: compilers
