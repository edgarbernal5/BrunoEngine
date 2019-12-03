#pragma once

#if TRIO_WINDOWS
#ifdef TRIOAPI_EXPORTS
#define TRIO_API_EXPORT __declspec(dllexport)
#else
#define TRIO_API_EXPORT __declspec(dllimport)
#endif

#else

#define TRIO_API_EXPORT

#endif
