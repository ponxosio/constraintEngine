#ifndef CONSTRAINTSENGINELIBRARY_GLOBAL_H
#define CONSTRAINTSENGINELIBRARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONSTRAINTSENGINELIBRARY_LIBRARY)
#  define PROLOGEXECUTOR_EXPORT Q_DECL_EXPORT
#  define PROLOGTRANSLATIONSTACK_EXPORT Q_DECL_EXPORT
#else
#  define PROLOGEXECUTOR_EXPORT Q_DECL_IMPORT
#  define PROLOGTRANSLATIONSTACK_EXPORT Q_DECL_IMPORT
#endif

#endif // CONSTRAINTSENGINELIBRARY_GLOBAL_H
