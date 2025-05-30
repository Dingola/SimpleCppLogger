#ifdef SIMPLECPPLOGGER_USE_DLL
#ifdef SIMPLECPPLOGGER_BUILDING_PROJECT
#define SIMPLECPPLOGGER_API __declspec(dllexport)
#else
#define SIMPLECPPLOGGER_API __declspec(dllimport)
#endif
#else
#define SIMPLECPPLOGGER_API
#endif
