#/*****************************************************************************
# * vlyc2 - A Desktop YouTube client
# * Copyright (C) 2013 Orochimarufan <orochimarufan.x3@gmail.com>
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# *****************************************************************************/

# plugin name
SET(PLUGIN_NAME python)

######## Prepare ########
include(../prepare.cmake)

######## Plugin ########
## this is where custom stuff goes!
# Find PythonQt
find_package(PythonQt5_3_QtAll REQUIRED)

include_directories(${PythonQt_INCLUDE_DIRS} ${PythonQt_QtAll_INCLUDE_DIRS})
LIST(APPEND PLUGIN_LIBS ${PythonQt_QtAll_LIBRARIES})

LIST(APPEND PLUGIN_QT ${PythonQt_QtAll_QT})

# Find Python
find_package(Python ${PythonQt_Python} REQUIRED EXACT)
include_directories(${PYTHON_INCLUDE_DIRS})
LIST(APPEND PLUGIN_LIBS ${PYTHON_LIBRARIES})

# Sources
LIST(APPEND PLUGIN_HEADERS
pythonplugin.h
pythonqtdecorator.h
VlycPython.h
PluginLoader.h
LegacyPluginLoader.h
wrap/WrapPlugin.h
wrap/WrapLegacySitePlugin.h
console/PythonQtScriptingConsole.h
)

LIST(APPEND PLUGIN_SOURCES
pythonplugin.cpp
pythonqtdecorator.cpp
VlycPython.cpp
PluginLoader.cpp
LegacyPluginLoader.cpp
wrap/WrapPlugin.cpp
wrap/WrapLegacySitePlugin.cpp
console/PythonQtScriptingConsole.cpp
)

######## Build ########
# Qt
FOREACH(module ${PLUGIN_QT})
	find_package(Qt5${module} REQUIRED)
ENDFOREACH(module)

# Qt stuff
qt5_add_resources(RES pythonplugin.qrc)
LIST(APPEND PLUGIN_SOURCES ${RES})

# Build
add_library(${PLUGIN} SHARED ${PLUGIN_HEADERS} ${PLUGIN_SOURCES})
qt5_use_modules(${PLUGIN} ${PLUGIN_QT})
target_link_libraries(${PLUGIN} ${PLUGIN_LIBS})

IF(WIN32)
	get_target_property(COMPILE_FLAGS ${PLUGIN} COMPILE_FLAGS)
	IF(COMPILE_FLAGS)
		LIST(APPEND COMPILE_FLAGS "-include cmath") #fix '::hypot has not been declared
	ELSE()
		SET(COMPILE_FLAGS "-include cmath")
	ENDIF()
	set_target_properties(${PLUGIN} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
ENDIF()
