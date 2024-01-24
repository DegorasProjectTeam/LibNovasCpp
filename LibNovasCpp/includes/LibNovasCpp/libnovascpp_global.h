/*
  Refactorized for be used with modern C++ compilers by Ángel Vera Herrera.
  Royal Observatory of the Spanish Navy.

  Windows DLL export/import directives
*/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// =====================================================================================================================
#if ((defined __WIN32__) || (defined _WIN32)) && (!defined LIBNOVASCPP_STATIC)
#ifdef LIBNOVASCPP_LIBRARY
#define LIBNOVASCPP_EXPORT	__declspec(dllexport)
#else
#define LIBNOVASCPP_EXPORT	__declspec(dllimport)
#endif
#else
                   /* Static libraries or non-Windows needs no special declaration. */
# define LIBNOVASCPP_EXPORT
#endif
// =====================================================================================================================
