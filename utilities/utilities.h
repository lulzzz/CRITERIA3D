#ifndef UTILITIES_H
#define UTILITIES_H

    class Crit3DDate;
    class Crit3DTime;
    class QDate;
    class QDateTime;
    class QVariant;
    class QString;

    Crit3DDate getCrit3DDate(const QDate &myDate);
    Crit3DTime getCrit3DTime(const QDateTime &myTime);
    Crit3DTime getCrit3DTime(const QDate& t, int hour);

    QDate getQDate(const Crit3DDate &myDate);
    QDateTime getQDateTime(const Crit3DTime &myCrit3DTime);
    int decadeFromDate(QDate date);
    int getSeasonFromDate(QDate date);

    bool getValue(QVariant myRs, int* myValue);
    bool getValue(QVariant myRs, float* myValue);
    bool getValue(QVariant myRs, double* myValue);
    bool getValue(QVariant myRs, QDate* myValue);
    bool getValue(QVariant myRs, QDateTime* myValue);
    bool getValue(QVariant myRs, QString* myValue);

    QString getPath(QString filePath);
    QString getFileName(QString filePath);



#endif // UTILITIES_H
