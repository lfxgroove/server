# This code is part of MaNGOS. Contributor & Copyright details are in AUTHORS/THANKS.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

if(NOT ACE_USE_EXTERNAL)
  add_subdirectory(ACE_wrappers)
endif()

add_subdirectory(recastnavigation)
add_subdirectory(src)
add_subdirectory(libmpq)
add_subdirectory(lualib)

# This is done to generate the config.h file for libmpq
# Kind of a cludge..
include(ExternalProject)
ExternalProject_Add(mympqtmp
  PREFIX "./"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/dep/libmpq/"
  INSTALL_DIR "${CMAKE_BINARY_DIR}/lib"
  # This is for the CONFIGURE_COMMAND to have the right working dir
  BINARY_DIR "${CMAKE_SOURCE_DIR}/dep/libmpq/"
  CONFIGURE_COMMAND "${CMAKE_SOURCE_DIR}/dep/libmpq/autogen.sh"
  BUILD_COMMAND "${CMAKE_SOURCE_DIR}/dep/libmpq/configure"
  INSTALL_COMMAND ""
)
