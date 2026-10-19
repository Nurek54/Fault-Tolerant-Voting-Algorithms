cmake_minimum_required(VERSION 3.20)
project(Fault_Tolerant_Voting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VOTING_CORE
        src/voting/Voting.cpp
        src/signal/DataGenerator.cpp
        src/io/CsvWriter.cpp)

add_executable(voting src/main.cpp ${VOTING_CORE})
target_include_directories(voting PRIVATE src)

# Ocena glosowania wzgledem prawdziwego sygnalu (tabele i dane do wykresow w README)
add_executable(experiment tools/experiment.cpp ${VOTING_CORE})
target_include_directories(experiment PRIVATE src)

# MinGW: libstdc++ i libgcc wbudowane w exe, a libwinpthread-1.dll kopiowany z kompilatora
# obok exe, zeby Windows nie wzial starszej wersji z innego MinGW w PATH.
# (Pelne -static nie dziala z MinGW dolaczonym do CLion: brak __intrinsic_setjmpex)
if (MINGW)
    get_filename_component(MINGW_BIN "${CMAKE_CXX_COMPILER}" DIRECTORY)
    foreach (target voting experiment)
        target_link_options(${target} PRIVATE -static-libgcc -static-libstdc++)
        if (EXISTS "${MINGW_BIN}/libwinpthread-1.dll")
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN}/libwinpthread-1.dll" $<TARGET_FILE_DIR:${target}>)
        endif ()
    endforeach ()
endif ()
