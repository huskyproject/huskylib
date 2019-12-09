cmake_minimum_required(VERSION 2.8.11)
PROJECT(husky C)
set(husky_VERSION 1.9.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libs" ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-g -DDEBUG -D_DEBUG)
  set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "My multi config types" FORCE)
else(CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DNDEBUG)
  set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "My multi config types" FORCE)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

if (MSVC)
  if (BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-D_DLL -D_MAKE_DLL)
  else()
    set(CompilerFlags
      CMAKE_CXX_FLAGS
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_RELEASE
    )
    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
  endif()
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../out/lib")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../out/bin")
  ADD_DEFINITIONS(-DWIN32 -D_WINDOWS -D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
else ()
  ADD_DEFINITIONS(-Wall -DUNIX)
endif (MSVC)

message("-------------------------")
message("System            : ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
message("Compiler          : ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message("Prefix            : ${CMAKE_INSTALL_PREFIX} (run \"cmake . -DCMAKE_INSTALL_PREFIX:PATH=/other\" for other prefix)")
message("Build shared      : ${BUILD_SHARED_LIBS}")
message("Build type        : ${CMAKE_CONFIGURATION_TYPES}")
message("-------------------------")

SET(husky_SOURCES 
  src/adcase.c
  src/calendar.c
  src/cmpfname.c
  src/crc.c
  src/cvtdate.c
  src/dirlayer.c
  src/fexist.c
  src/ffind.c
  src/flush.c
  src/ftnaddr.c
  src/genmsgid.c
  src/genverst.c
  src/getfree.c
  src/ioutil.c
  src/locking.c
  src/log.c
  src/mapfile.c
  src/memory.c
  src/parsenn.c
  src/patmat.c
  src/qksort.c
  src/recode.c
  src/setfsize.c
  src/strext.c
  src/strftime.c
  src/tdelay.c
  src/temp.c
#  src/tlog.c
  src/tree.c
  src/version.c
  src/xstr.c
)
SET(husky_HEADERS 
  huskylib/BCD.h
  huskylib/BCO.h
  huskylib/BCW.h
  huskylib/BEOS5.h
  huskylib/BORLANDC.h
  huskylib/calendar.h
  huskylib/compiler.h
  huskylib/crc.h
  huskylib/cvtdate.h
  huskylib/dirlayer.h
  huskylib/DJGPP.h
  huskylib/EMX.h
  huskylib/fexist.h
  huskylib/ffind.h
  huskylib/ftnaddr.h
  huskylib/HIGHC.h
  huskylib/huskyext.h
  huskylib/huskylib.h
  huskylib/IBMC_OS2.h
  huskylib/locking.h
  huskylib/log.h
  huskylib/memory.h
  huskylib/MINGW32.h
  huskylib/MSC.h
  huskylib/MSVC.h
  huskylib/parsenn.h
  huskylib/pstdint.h
  huskylib/recode.h
  huskylib/SASC.h
  huskylib/strext.h
  huskylib/syslogp.h
  huskylib/temp.h
  huskylib/tree.h
  huskylib/typesize.h
  huskylib/UNIX.h
  huskylib/unused.h
  huskylib/vixie.h
  huskylib/WATCOMC.h
  huskylib/WCD.h
  huskylib/WCO.h
  huskylib/WCW.h
  huskylib/WCX.h
  huskylib/xstr.h
)

include_directories(huskylib)
ADD_LIBRARY(husky ${husky_SOURCES} ${husky_HEADERS})
set_target_properties(husky PROPERTIES VERSION ${husky_VERSION})
set_target_properties(husky PROPERTIES PUBLIC_HEADER "${husky_HEADERS}")
ADD_EXECUTABLE(gnmsgid src/gnmsgid.c)
target_link_libraries(gnmsgid husky)

if (NOT MSVC)
  INSTALL(TARGETS husky gnmsgid
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION include/huskylib
  )
  set(CPACK_GENERATOR ${CPACK_GENERATOR} TGZ)
endif (NOT MSVC)
set (CPACK_PACKAGE_VERSION_MAJOR "1")
set (CPACK_PACKAGE_VERSION_MINOR "9")
file(READ cvsdate.h CPACK_PACKAGE_VERSION_PATCH LIMIT 10 OFFSET 17)
string(SUBSTRING ${CPACK_PACKAGE_VERSION_PATCH} 0 10 CPACK_PACKAGE_VERSION_PATCH)
string(REGEX REPLACE "-" "" CPACK_PACKAGE_VERSION_PATCH ${CPACK_PACKAGE_VERSION_PATCH})
INCLUDE(CPack)
