# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=204facee-4981-48db-7cb8-c347a487120b
PACKET_NAME     = GpUnitTests
DEFINES        += GP_UNIT_TESTS_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./..

include($$DIR_LEVEL/../QtGlobalPro.pri)

equals(var_link, "static") {
	CONFIG += staticlib
}

# ----------- Libraries -----------
equals(var_os, "windows") {
	LIBS += -lGpService$$TARGET_POSTFIX
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX
}

equals(var_os, "linux") {
	LIBS += -lGpService$$TARGET_POSTFIX
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	LIBS += -lfmt
}

# ----------- Sources and headers -----------
SOURCES += \
    AppService/GpUnitTestAppCfgDesc.cpp \
    AppService/GpUnitTestAppCfgDescFactory.cpp \
    AppService/GpUnitTestAppCmdArgsDesc.cpp \
    AppService/GpUnitTestAppCmdArgsDescFactory.cpp \
    AppService/GpUnitTestAppMainTask.cpp \
    AppService/GpUnitTestAppMainTaskFactory.cpp \
    GpUnitTestAssert.cpp \
    GpUnitTestGroup.cpp \
    GpUnitTestManager.cpp \
    GpUnitTestRunner.cpp \
    GpUnitTestsLib.cpp \
    Handlers/GpUnitTestHandlerStatistics.cpp \
    Handlers/GpUnitTestLayeredHandler.cpp \
    Handlers/GpUnitTestLogOutHandler.cpp \
    Handlers/GpUnitTestLogOutHandlerFactory.cpp

HEADERS += \
    AppService/GpUnitTestAppCfgDesc.hpp \
    AppService/GpUnitTestAppCfgDescFactory.hpp \
    AppService/GpUnitTestAppCmdArgsDesc.hpp \
    AppService/GpUnitTestAppCmdArgsDescFactory.hpp \
    AppService/GpUnitTestAppMainTask.hpp \
    AppService/GpUnitTestAppMainTaskFactory.hpp \
    GpUnitTest.hpp \
    GpUnitTestAssert.hpp \
    GpUnitTestGroup.hpp \
    GpUnitTestManager.hpp \
    GpUnitTestOps.hpp \
    GpUnitTestRunner.hpp \
    GpUnitTestSuiteGroup.hpp \
    GpUnitTestsLib.hpp \
    GpUnitTests_globals.hpp \
    Handlers/GpUnitTestHandler.hpp \
    Handlers/GpUnitTestHandlerFactory.hpp \
    Handlers/GpUnitTestHandlerStatistics.hpp \
    Handlers/GpUnitTestHandlers.hpp \
    Handlers/GpUnitTestLayeredHandler.hpp \
    Handlers/GpUnitTestLogOutHandler.hpp \
    Handlers/GpUnitTestLogOutHandlerFactory.hpp
