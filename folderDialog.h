#include "shlobj.h"

// Macros for interface casts
#ifdef __cplusplus
#define IID_PPV_ARG(IType, ppType) IID_##IType, reinterpret_cast(static_cast(ppType))
#else
#define IID_PPV_ARG(IType, ppType) &IID_##IType, (void**)(ppType)
#endif
