ifneq ($(VERBOSE),1)
Q    := @
NQMAKE := $(MAKE) --no-print-directory
MAKE := $(Q)$(MAKE) --no-print-directory
NULL := 2>/dev/null
endif

all: debug
ci: lint debug release test

help:
	$(Q)echo "      release: Run a release build"
	$(Q)echo "        debug: Run a debug build"
	$(Q)echo "         lint: Lint check all source-code"
	$(Q)echo "         test: Build and run tests"
	$(Q)echo "        clean: Clean all build output"
	$(Q)echo "rebuild_cache: Rebuild all CMake caches"

rebuild_cache: build/Debug build/Release
	$(MAKE) -C build/Debug rebuild_cache
	$(MAKE) -C build/Release rebuild_cache

debug: build/Debug
	$(Q)echo "[DEBUG]"
	$(MAKE) -C $^

release: build/Release
	$(Q)echo "[RELEASE]"
	$(MAKE) -C $^

lint: build/Debug
	$(Q)echo "[LINT]"
	$(MAKE) -C build/Debug lint

analyze: 
	scan-build -o build/Analyze --status-bugs make debug

test: build/Debug
	$(MAKE) -C build/Debug test

build/Debug:
	$(Q)mkdir -p $@
	$(Q)cd $@ && cmake -DCMAKE_BUILD_TYPE=Debug ../../

build/Release:
	$(Q)mkdir -p $@
	$(Q)cd $@ && cmake -Wno-dev -DCMAKE_BUILD_TYPE=Release ../../

clean:
	@echo "[CLEAN]"
	@rm -Rf build

.PHONY: clean
