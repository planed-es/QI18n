#ifndef QI18N_GLOBAL_H
#define QI18N_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QI18N_LIBRARY)
#  define QI18N_EXPORT Q_DECL_EXPORT
#else
#  define QI18N_EXPORT Q_DECL_IMPORT
#endif

#endif // QI18N_GLOBAL_H
