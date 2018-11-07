#ifndef INTERPOLATIONDIALOG_H
#define INTERPOLATIONDIALOG_H

#ifndef INTERPOLATIONSETTINGS_H
    #include "interpolationSettings.h"
#endif

#ifndef DBMETEOPOINTS_H
    #include "dbMeteoPoints.h"
#endif

#include <QSettings>
#include <QDialog>
#include <QtWidgets>
#include <QPushButton>


class InterpolationDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit InterpolationDialog(QSettings *settings, Crit3DInterpolationSettings *myInterpolationSetting);

        QComboBox algorithmEdit;
        QLineEdit minRegressionR2Edit;
        QCheckBox* lapseRateCodeEdit;
        QCheckBox* thermalInversionEdit;
        QCheckBox* optimalDetrendingEdit;
        QCheckBox* topographicDistanceEdit;
        QCheckBox* useDewPointEdit;
        QComboBox gridAggregationMethodEdit;
        QVector <QCheckBox*> proxy;

        void writeInterpolationSettings();
        void accept();

    private:
        QSettings* _paramSettings;
        Crit3DInterpolationSettings *_interpolationSettings;

};

class ProxyDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ProxyDialog(QSettings *settings,
                             Crit3DInterpolationSettings *myInterpolationSettings,
                             Crit3DInterpolationSettings *myQualityInterpolationSettings,
                             Crit3DMeteoPointsDbHandler *myMeteoPointsHandler);

        QComboBox _proxyCombo;
        QComboBox _field;
        QComboBox _table;
        QLineEdit _proxyGridName;

        void changedProxy();
        void changedTable();
        void getGridFile();
        void redrawProxies();
        void addProxy();
        void deleteProxy();
        void writeProxies();
        void saveProxies();
        void saveProxy();
        bool checkProxies(std::string *error);
        void accept();

    private:
        QSettings* _paramSettings;
        Crit3DInterpolationSettings *_interpolationSettings;
        Crit3DInterpolationSettings *_qualityInterpolationSettings;
        Crit3DMeteoPointsDbHandler *_meteoPointsHandler;
        std::vector <Crit3DProxy> _proxy;

};

#endif // INTERPOLATIONDIALOG_H
