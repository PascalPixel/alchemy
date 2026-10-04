# Maintained source is built through ordinary Make and GNU linker rules.
.DEFAULT_GOAL := help
TOOLS := tools
CARGO ?= cargo
export CARGO_TARGET_DIR := $(CURDIR)/tools/out/cargo-target
export PATH := $(CURDIR)/tools/out/binutils/bin:$(PATH)
CARGO_RUN := $(CARGO) run --offline --quiet --release --manifest-path
ALCHEMY ?= $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml --
ALCHEMY_BIN := $(CARGO_TARGET_DIR)/release/alchemy
BUILD := $(ALCHEMY) build
CHECK := $(ALCHEMY) check
TARGET ?= tbs-ja
SHA1 := $(shell { command -v sha1sum || command -v shasum; } 2>/dev/null) -c

# Compiler library members come from the pinned toolchain's own sources/rules.
LIBGCC := tools/out/compiler-runtime/libgcc.a

.PHONY: compiler-runtime worktree
compiler-runtime: toolchain-check compiler-source-check
	$(BUILD) runtime --output $(LIBGCC) \
	    _call_via_rX=gcc/config/arm/lib1funcs.asm \
	    _addsub_df=dp-bit.c \
	    _si_to_df=dp-bit.c \
	    _df_to_si=dp-bit.c \
	    _pack_df=dp-bit.c \
	    _unpack_df=dp-bit.c \
	    _thenan_df=dp-bit.c \
	    _lshrdi3=gcc/libgcc2.c

# A fresh clone builds the library once, from make bootstrap.
$(LIBGCC):
	$(MAKE) compiler-runtime

.PHONY: help bootstrap merge-drivers fetch-dependencies compilers compiler-sources compiler-source-check toolchain-check build-tools
.PHONY: compare compare-tla compare-all build-rom
.PHONY: precommit prepush verify land test test-integration lint-staged language-check
.PHONY: publication-check publication-staged-check coverage coverage-check
.PHONY: progress deps similar worst clean

help:
	@printf '%s\n' \
	  'make bootstrap       fetch locked dependencies and build the approved toolchain' \
	  'make fetch-dependencies  download only the locked Rust dependencies' \
	  'make compare-all     compare linked source and both private ROM compositions' \
	  'make compare-editions  compare all twelve editions, with Japanese as the base' \
	  'make test            the Rust tests' \
	  'make verify          every check, the tests and all twelve editions compared' \
	  'make land            on main, before committing a landing: make verify, then the count published' \
	  'make coverage        measure once: README progress line and the decomp.dev report' \
	  'make progress        report DONE in all six editions from the linker maps of verified builds' \
	  'make deps            map calls and blockers of not-yet-C code into out/reports/deps-*.tsv' \
	  'make similar         the nearest C twins of every not-yet-C function in either game, into out/reports/similar.tsv' \
	  'make worst           rank maintained C by how mechanical it reads; the camelotized share' \
	  'alchemy diff DRAFT.c compare a C draft with the listing it would replace (tools/out/cargo-target/release/alchemy)' \
	  'alchemy adopt ADDR LABEL [NAME]  take an adopted function out of the scaffolding of all six editions'

# Setup may download pinned dependencies; ordinary build commands stay offline.
fetch-dependencies:
	$(CARGO) fetch --locked --manifest-path $(TOOLS)/Cargo.toml

bootstrap: fetch-dependencies
	$(ALCHEMY) bootstrap $(if $(BUNDLE),--from "$(BUNDLE)")
	$(MAKE) $(LIBGCC)
	$(MAKE) merge-drivers

# Git merges scaffolds by ROM bytes and linker scripts by lines
# (.gitattributes); after a merge, alchemy merge-driver links reconciles them.
merge-drivers:
	git config merge.alchemy-scaffold.name "edition scaffold by ROM bytes"
	git config merge.alchemy-scaffold.driver "tools/out/cargo-target/release/alchemy merge-driver scaffold %O %A %B"
	git config merge.alchemy-lines.name "linker script by lines"
	git config merge.alchemy-lines.driver "tools/out/cargo-target/release/alchemy merge-driver lines %O %A %B"

# In a new worktree, before its first build: check out the compiler
# submodules, link the main checkout's ROMs and installed tools, and clone
# release tools and edition builds copy-on-write.
# Private reports, verification and recovery output stay in the main checkout.
MAIN_CHECKOUT = $(shell git rev-parse --path-format=absolute --git-common-dir)/..
worktree:
	bun "$(TOOLS)/alchemy/worktree.ts" "$(MAIN_CHECKOUT)"

toolchain-check:
	$(ALCHEMY) bootstrap --check

compilers: compiler-sources

compiler-sources: compiler-source-check
	sh agscc/build.sh
	$(MAKE) -C agbcc/gcc old -j1
	cd agbcc/gcc_arm && ./configure --target=arm-elf --host=i386-linux-gnu && $(MAKE) cc1 && mv cc1 ../agbcc_arm && rm -f config.bak

compiler-source-check:
	@set -e; for repo in agbcc agscc; do \
	  case "$$repo" in \
	    agbcc) approved=da598c1d918402c42c0c0d7128ba14567f3175e9;; \
	    agscc) approved=6baba9179b16f18e5667994fb4e70617561cbadc;; \
	  esac; \
	  test "$$(git rev-parse :$$repo)" = "$$approved" || { printf '%s gitlink is not approved\n' "$$repo"; exit 1; }; \
	  test "$$(env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git -C "$$repo" rev-parse HEAD)" = "$$approved" || { printf '%s checkout is not approved\n' "$$repo"; exit 1; }; \
	  state=$$(env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git -C "$$repo" status --porcelain --untracked-files=all -- . ':(exclude,glob)**/.DS_Store'); \
	  test -z "$$state" || { printf '%s compiler source is dirty\n' "$$repo"; exit 1; }; \
	done
	@printf 'compiler sources match approved submodules\n'

build-tools:
	$(CARGO) build --offline --release --workspace --manifest-path $(TOOLS)/Cargo.toml

compare:
	$(BUILD) rom --target tbs-ja
	@grep -F ' out/tbs-ja/' rom.sha1 | $(SHA1) -

compare-tla:
	$(BUILD) rom --target tla-ja
	@grep -F ' out/tla-ja/' rom.sha1 | $(SHA1) -

compare-all:
	@$(MAKE) --no-print-directory -j2 compare compare-tla

# The other ten editions link through their scaffold under recon/<game>/<lang>,
# as pret's early builds linked a version through baserom.gba. Each edition
# builds into its own out/<edition>, so they build concurrently, one per CPU.
EDITIONS := tbs-en tbs-de tbs-es tbs-fr tbs-it tla-en tla-de tla-es tla-fr tla-it
COMPARE_EDITIONS := $(addprefix compare-,$(EDITIONS))
CPUS := $(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
.PHONY: compare-editions compare-other-editions compare-tbs-ja compare-tla-ja $(COMPARE_EDITIONS)
compare-editions:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(MAKE) --no-print-directory -j$(CPUS) ALCHEMY=$(ALCHEMY_BIN) compare compare-tla $(COMPARE_EDITIONS)
compare-other-editions:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(MAKE) --no-print-directory -j$(CPUS) ALCHEMY=$(ALCHEMY_BIN) $(COMPARE_EDITIONS)
compare-tbs-ja: compare
compare-tla-ja: compare-tla
$(COMPARE_EDITIONS): compare-%:
	$(BUILD) rom --target $*
	@grep -F ' out/$*/' rom.sha1 | $(SHA1) -

build-rom:
	$(BUILD) rom --target $(TARGET)

precommit:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify --pre-commit

prepush:
	$(ALCHEMY) verify --pre-push

# Every check over the whole tree, the tests and all twelve editions, at once.
verify:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify

# On main, before committing a landing: make verify, then one measurement that
# writes README's progress line, staged for the commit, and the decomp.dev
# report; pre-push uploads it for the hosted reporting Action.
land:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify --land

# The checks AGENTS.md names, each runnable alone.
publication-check:
	$(CHECK) publication --tree
	$(CHECK) layout

publication-staged-check:
	$(CHECK) publication --staged
	$(CHECK) layout

# no-asm (S2): every C source as each game's Japanese and English builds
# preprocess it.
lint-staged:
	$(CHECK) no-asm --target tbs-ja --target tla-ja --target tbs-en --target tla-en

language-check:
	@set -eu; scripts=$$(git ls-files --cached --others --exclude-standard | grep -E '\.(js|mjs|cjs|py|sh)$$' || true); \
	test -z "$$scripts" || { printf 'Use Rust or Bun TypeScript for tooling:\n%s\n' "$$scripts"; exit 1; }

test:
	$(CARGO) test --offline --quiet --release --workspace --manifest-path $(TOOLS)/Cargo.toml

test-integration: toolchain-check
	$(CARGO) test --offline --quiet --release --workspace --manifest-path $(TOOLS)/Cargo.toml -- --ignored

coverage:
	$(CHECK) coverage --write

# Source and build defaults are Japanese. Published coverage/progress count
# each game's six editions together: the English build gives the bytes, and
# an edition earns those of the objects its own verified build links.
coverage-check:
	$(CHECK) coverage --check

progress:
	$(CHECK) progress

# A report for people only: the build and the count never read it. The listings
# under recon/<game>/raw are the English builds' disassembly, so it reads those.
deps:
	$(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- deps --build out/tbs-en --build out/tla-en \
	    --out-dir out/reports $(DEPS_FLAGS)

# Each not-yet-C function's nearest C twins across both games, for ports.
# A report for people only.
similar:
	$(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- similar --build out/tbs-en --build out/tla-en \
	    --out out/reports/similar.tsv $(SIMILAR_FLAGS)

# Maintained C ranked by how mechanical it reads, worst first: what to
# rewrite in Camelot's style next. A report for people only.
worst:
	$(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- worst --out out/reports/worst.tsv

clean:
	@rm -rf out
