#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QNetworkRequest>
#include <QtNetwork>
#include "dbMeteoPoints.h"
#include "dbArkimet.h"

#ifndef CRIT3DDATE_H
    #include "crit3dDate.h"
#endif
#ifndef GIS_H
    #include "gis.h"
#endif


class Download : public QObject
{
    Q_OBJECT
    public:
        explicit Download(QString dbName, QObject* parent = 0);
        ~Download();
        bool getPointProperties(QStringList datasetList);
        void downloadMetadata(QJsonObject obj);
        bool downloadDailyDataSinglePoint(Crit3DDate dateStart, Crit3DDate dateEnd, QString dataset, QString id, QList<int> variables, bool prec24);
        bool downloadDailyData(Crit3DDate dateStart, Crit3DDate dateEnd, QStringList datasets, QStringList stations, QList<int> variables, bool precSelection);
        bool downloadHourlyData(Crit3DTime dateStartTime, Crit3DTime dateEndTime, QStringList datasets, QStringList stations, QList<int> variables);
        DbArkimet* getDbArkimet();

    private:
        QStringList _datasetsList;
        DbArkimet* _dbMeteo;

        static const QByteArray _authorization;

};

#endif // DOWNLOAD_H
