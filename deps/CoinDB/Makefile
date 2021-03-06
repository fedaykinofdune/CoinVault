ODB_DB = \
    -DDATABASE_SQLITE

CXX_FLAGS += -Wall
ifdef DEBUG
    CXX_FLAGS += -g
else
    CXX_FLAGS += -O3
endif

LOCAL_SYSROOT = ../../sysroot

CLI = ../cli

INCLUDE_PATH += \
    -Isrc \
    -I$(CLI)/src

ODB_INCLUDE_PATH += \
    -Isrc

ifneq ($(wildcard $(LOCAL_SYSROOT)/include),)
    INCLUDE_PATH += -I$(LOCAL_SYSROOT)/include
    ODB_INCLUDE_PATH += -I$(LOCAL_SYSROOT)/include
endif

LIB_PATH += \
    -Llib

ifneq ($(wildcard $(LOCAL_SYSROOT)/lib),)
    LIB_PATH += -L$(LOCAL_SYSROOT)/lib
endif

ifndef OS
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Linux)
        OS = linux
    else ifeq ($(UNAME_S), Darwin)
        OS = osx
    endif
endif

ifeq ($(OS), linux)
    ifndef SYSROOT
        SYSROOT = /usr/local
    endif

    CXX = g++
    CC = gcc
    CXX_FLAGS += -Wno-unknown-pragmas -std=c++0x -DBOOST_SYSTEM_NOEXCEPT=""
    ARCHIVER = ar

    PLATFORM_LIBS += \
        -lpthread

else ifeq ($(OS), mingw64)
    ifndef SYSROOT
        SYSROOT = /usr/x86_64-w64-mingw32
    endif

    CXX =  x86_64-w64-mingw32-g++
    CC =  x86_64-w64-mingw32-gcc
    CXX_FLAGS += -Wno-unknown-pragmas -Wno-strict-aliasing -std=c++0x -DBOOST_SYSTEM_NOEXCEPT=""
    ARCHIVER = x86_64-w64-mingw32-ar

    INCLUDE_PATH += -I$(SYSROOT)/include

    # ODB_INCLUDE_PATH must not contain windows system headers
    ODB_INCLUDE_PATH += -I$(SYSROOT)/local/include

    EXE_EXT = .exe

    BOOST_THREAD_SUFFIX = _win32

    PLATFORM_LIBS += \
        -static-libgcc -static-libstdc++ \
        -lgdi32 \
        -lws2_32 \
        -lmswsock

else ifeq ($(OS), osx)
    ifndef SYSROOT
        SYSROOT = /usr/local
    endif

    CXX = clang++
    CC = clang
    CXX_FLAGS += -Wno-unknown-pragmas -Wno-unneeded-internal-declaration -std=c++11 -stdlib=libc++ -DBOOST_THREAD_DONT_USE_CHRONO -DMAC_OS_X_VERSION_MIN_REQUIRED=MAC_OS_X_VERSION_10_6 -mmacosx-version-min=10.7
    ARCHIVER = ar

    INCLUDE_PATH += -I$(SYSROOT)/include
    ODB_INCLUDE_PATH += -I$(SYSROOT)/include

else ifneq ($(MAKECMDGOALS), clean)
    $(error OS must be set to linux, mingw64, or osx)
endif

# Detect boost library filename suffix
ifneq ($(wildcard $(SYSROOT)/lib/libboost_system-mt.*),)
    BOOST_SUFFIX = -mt
else ifneq ($(wildcard $(SYSROOT)/lib/libboost_system-mt-s.*),)
    BOOST_SUFFIX = -mt-s
endif

LIBS = \
    -lCoinDB \
    -lCoinQ \
    -lCoinCore \
    -llogger \
    -lboost_system$(BOOST_SUFFIX) \
    -lboost_filesystem$(BOOST_SUFFIX) \
    -lboost_regex$(BOOST_SUFFIX) \
    -lboost_thread$(BOOST_THREAD_SUFFIX)$(BOOST_SUFFIX) \
    -lboost_serialization$(BOOST_SUFFIX) \
    -lcrypto \
    -lodb-sqlite \
    -lodb

OBJS = \
    obj/Schema-odb.o \
    obj/Schema.o \
    obj/Vault.o \
    obj/SynchedVault.o

TOOLS = \
    tools/build/coindb$(EXE_EXT)

TESTS = \
    tests/build/SynchedVaultTest$(EXE_EXT)

all: lib tools tests

lib: lib/libCoinDB.a

tools: $(TOOLS) 

tests: $(TESTS)

lib/libCoinDB.a: $(OBJS)
	$(ARCHIVER) rcs $@ $^

#
# odb compiled dependencies
#
odb/Schema-odb.hxx: src/Schema.h
	odb --output-dir odb/ --std c++11 -d sqlite --generate-query --generate-schema $(ODB_INCLUDE_PATH) $<

odb/Schema-odb.cxx: odb/Schema-odb.hxx

#
# odb generated schema object
#
obj/Schema-odb.o: odb/Schema-odb.cxx odb/Schema-odb.hxx src/Schema.h
	$(CXX) $(CXX_FLAGS) $(ODB_DB) $(INCLUDE_PATH) -c $< -o $@

#
# schema classes
#
obj/Schema.o: src/Schema.cpp src/Schema.h
	$(CXX) $(CXX_FLAGS) $(ODB_DB) $(INCLUDE_PATH) -c $< -o $@

#
# vault class
#
obj/Vault.o: src/Vault.cpp src/Vault.h src/VaultExceptions.h src/SigningRequest.h src/Schema.h src/Database.h odb/Schema-odb.hxx
	$(CXX) $(CXX_FLAGS) $(ODB_DB) $(INCLUDE_PATH) -c $< -o $@

#
# synched vault class
#
obj/SynchedVault.o: src/SynchedVault.cpp src/SynchedVault.h src/VaultExceptions.h src/SigningRequest.h src/Schema.h src/Database.h odb/Schema-odb.hxx
	$(CXX) $(CXX_FLAGS) $(ODB_DB) $(INCLUDE_PATH) -c $< -o $@

#
# coindb command line tool
#
tools/build/coindb$(EXE_EXT): tools/src/coindb.cpp tools/src/formatting.h lib/libCoinDB.a
	$(CXX) $(CXX_FLAGS) $(ODB_DB) $(INCLUDE_PATH) $< -o $@ $(LIB_PATH) $(LIBS) $(PLATFORM_LIBS)

#
# SynchedVault unit test
#
tests/build/SynchedVaultTest$(EXE_EXT): tests/src/SynchedVaultTest.cpp lib/libCoinDB.a
	$(CXX) $(CXX_FLAGS) $(ODB_DB) $(INCLUDE_PATH) $< -o $@ $(LIB_PATH) $(LIBS) $(PLATFORM_LIBS)

install: install_lib install_tools

install_lib:
	-mkdir -p $(SYSROOT)/include/CoinDB
	-rsync -u src/*.h $(SYSROOT)/include/CoinDB/
	-mkdir -p $(SYSROOT)/lib
	-rsync -u lib/libCoinDB.a $(SYSROOT)/lib/

install_tools:
	-mkdir -p $(SYSROOT)/bin
	-rsync -u $(TOOLS) $(SYSROOT)/bin/

remove: remove_lib remove_tools

remove_lib:
	-rm -rf $(SYSROOT)/include/CoinDB
	-rm $(SYSROOT)/lib/libCoinDB.a

remove_tools:
	-rm $(SYSROOT)/bin/coindb$(EXE_EXT)

clean:
	-rm -f obj/*.o odb/*-odb.* lib/*.a tools/build/*
