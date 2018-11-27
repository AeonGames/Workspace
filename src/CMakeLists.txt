# Copyright (C) 2018 Rodrigo Jose Hernandez Cordoba
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

find_package(Qt5 COMPONENTS Core Gui Widgets)
if(WIN32)
  find_package(Qt5 COMPONENTS WinExtras)
endif(WIN32)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
                    ${Qt5Widgets_INCLUDE_DIRS})

set(WORKSPACE_QT_HEADERS WindowSelector.h WindowModel.h)

set(WORKSPACE_HEADERS Workspace.h PlatformDependent.h ${WORKSPACE_QT_HEADERS})

set(WORKSPACE_SOURCES Workspace.cpp WindowSelector.cpp WindowModel.cpp PlatformDependent.cpp)

set(WORKSPACE_UI ui/WindowSelector.ui)

if(WIN32)
  include_directories(${Qt5WinExtras_INCLUDE_DIRS})
  list(APPEND WORKSPACE_SOURCES ${WORKSPACE_SOURCES} Workspace.rc)
endif(WIN32)

qt5_wrap_cpp(WORKSPACE_MOCS ${WORKSPACE_QT_HEADERS})
qt5_wrap_ui(WORKSPACE_UI_HEADERS ${WORKSPACE_UI})
qt5_add_resources(WORKSPACE_RESOURCES Workspace.qrc)

add_executable(Workspace
               ${WORKSPACE_SOURCES}
               ${WORKSPACE_MOCS}
               ${WORKSPACE_UI_HEADERS}
               ${WORKSPACE_HEADERS}
               ${WORKSPACE_RESOURCES})

if(UNIX OR MINGW OR MSYS)
  set_target_properties(Workspace PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()

if(MSVC)
  set_target_properties(Workspace
                        PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_WARNINGS")
endif()

target_link_libraries(Workspace
                      ${Qt5Core_LIBRARIES}
                      ${Qt5Gui_LIBRARIES}
                      ${Qt5Widgets_LIBRARIES}
                      ${Qt5WinExtras_LIBRARIES}
                      ${CMAKE_DL_LIBS}
                      ${CMAKE_THREAD_LIBS_INIT})
