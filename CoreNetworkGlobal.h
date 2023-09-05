#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CORENETWORK_LIB)
#  define CORENETWORK_EXPORT Q_DECL_EXPORT
# else
#  define CORENETWORK_EXPORT Q_DECL_IMPORT
# endif
#else
# define CORENETWORK_EXPORT
#endif
