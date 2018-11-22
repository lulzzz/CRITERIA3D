#include "interpolationCmd.h"
#include "interpolation.h"
#include "formInfo.h"
#include "gis.h"


bool loadProxyGrids(Crit3DInterpolationSettings* mySettings)
{
    std::string* myError = new std::string();
    Crit3DProxy *myProxy;
    gis::Crit3DRasterGrid *myGrid;

    for (int i=0; i<mySettings->getProxyNr(); i++)
    {
        myProxy = mySettings->getProxy(i);
        myGrid = myProxy->getGrid();

        if (mySettings->getSelectedCombination().getValue(i) || myProxy->getForQualityControl())
        {
            if (myGrid == nullptr)
            {
                if (getProxyPragaName(myProxy->getName()) != height)
                {
                    myGrid = new gis::Crit3DRasterGrid();
                    if (!gis::readEsriGrid(myProxy->getGridName(), myGrid, myError))
                        return false;
                }
            }
        }
    }

    return true;
}

bool interpolationRaster(std::vector <Crit3DInterpolationDataPoint> &myPoints, Crit3DInterpolationSettings* mySettings,
                        gis::Crit3DRasterGrid* myGrid, const gis::Crit3DRasterGrid& myDTM, meteoVariable myVar, bool showInfo)
{
    if (! myGrid->initializeGrid(myDTM))
        return false;

    FormInfo myInfo;
    int infoStep;
    QString infoStr;

    if (showInfo)
    {
        infoStr = "Interpolation on DEM...";
        infoStep = myInfo.start(infoStr, myGrid->header->nrRows);
    }

    float myX, myY;

    for (long myRow = 0; myRow < myGrid->header->nrRows ; myRow++)
    {
        if (showInfo)
            myInfo.setValue(myRow);

        for (long myCol = 0; myCol < myGrid->header->nrCols; myCol++)
        {
            gis::getUtmXYFromRowColSinglePrecision(*myGrid, myRow, myCol, &myX, &myY);
            float myZ = myDTM.value[myRow][myCol];
            if (myZ != myGrid->header->flag)
                myGrid->value[myRow][myCol] = interpolate(myPoints, mySettings, myVar, myX, myY, myZ, getProxyValuesXY(myX, myY, mySettings), true);
        }
    }

    if (showInfo) myInfo.close();

    if (! gis::updateMinMaxRasterGrid(myGrid))
        return false;

    return true;
}
