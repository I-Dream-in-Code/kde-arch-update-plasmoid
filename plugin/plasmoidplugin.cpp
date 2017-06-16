#include "plasmoidplugin.h"
#include "systemCalls.h"

#include <QtQml>
#include <QDebug>

void PlasmoidPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.private.archUpdate"));
    
    qmlRegisterType<systemCalls>(uri, 1, 0, "SystemCalls");
}
