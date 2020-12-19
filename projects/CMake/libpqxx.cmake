# Libpqxx (dll or MSVC lib)
if (ARES_LIBPQXX_LIB)
  set(ARES_POSTGRES_LIBS ${ARES_POSTGRES_LIBS} ${ARES_LIBPQXX_LIB})
else()
  if (WIN32)
    if (MINGW)
      message("Compiling on Windows with MINGW, but ARES_LIBPQXX_LIB is not specified. Relying on package system. If linking to libpqxx.dll fails, specify it through ARES_LIBPQXX_LIB variable explicitly")
      set(NEED_FIND_PACKAGE_LIBPQXX_LIB "yes")
    elseif (MSVC)
      message(FATAL_ERROR "Compiling on Windows with MSVC, but ARES_LIBPQXX_LIB is not specified. Assuming no package system is available. Specify ARES_LIBPQXX_LIB exlicitly (should point to libpq library)")
    endif()
  else()
    # message("Compiling on Non-Windows platform, ARES_LIBPQXX_LIB is not specified. Will rely on package system")
    set(NEED_FIND_PACKAGE_LIBPQXX_LIB "yes")
  endif()
endif()

# Libpqxx include headers
if (ARES_LIBPQXX_INCLUDE_DIRS)
  set(ARES_POSTGRES_INCLUDE_DIRS ${ARES_POSTGRES_INCLUDE_DIRS} ${ARES_LIBPQXX_INCLUDE_DIRS})
else()
  if (WIN32)
    if (MINGW)
      message("Compiling on Windows with MINGW, but ARES_LIBPQXX_INCLUDE_DIRS is not specified. Relying on package system. If including libpqxx fails, specify it through ARES_LIBPQXX_INCLUDE_DIRS variable explicitly")
      set(NEED_FIND_PACKAGE_LIBPQXX_INCLUDE_DIRS "yes")
    elseif (MSVC)
      message(FATAL_ERROR "Compiling on Windows with MSVC, but ARES_LIBPQXX_INCLUDE_DIRS is not specified. Assuming no package system is available. Specify ARES_LIBPQXX_INCLUDE_DIRS exlicitly")
    endif()
  else()
    # message("Compiling on Non-Windows platform, ARES_LIBPQXX_INCLUDE_DIRS is not specified. Will rely on package system")
    set(NEED_FIND_PACKAGE_LIBPQXX_INCLUDE_DIRS "yes")
  endif()
endif()

# Invoke FindPackage PostgreSQL if needed
if (NEED_FIND_PACKAGE_LIBPQXX_LIB OR NEED_FIND_PACKAGE_LIBPQXX_INCLUDE_DIRS)
  # message("FindPqxx is not implemented, falling back to default library name 'pqxx'")
  set(PQXX_LIBRARIES "pqxx")
  if (NEED_FIND_PACKAGE_LIBPQXX_LIB)
    message("Setting default pqxx ${PQXX_LIBRARIES}")
    set(ARES_POSTGRES_LIBS ${ARES_POSTGRES_LIBS} ${PQXX_LIBRARIES})
  endif()
  if (NEED_FIND_PACKAGE_LIBPQXX_INCLUDE_DIRS)
    set(ARES_POSTGRES_INCLUDE_DIRS ${ARES_POSTGRES_INCLUDE_DIRS} ${PQXX_INCLUDE_DIRECTORIES})
  endif()
endif()

