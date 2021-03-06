/*!
    \file rasterObject.h

    \abstract draw raster in MapGraphics widget

    This file is part of CRITERIA-3D distribution.

    CRITERIA-3D has been developed by A.R.P.A.E. Emilia-Romagna.

    \copyright
    CRITERIA-3D is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    CRITERIA-3D is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with CRITERIA-3D.  If not, see <http://www.gnu.org/licenses/>.

    \authors
    Fausto Tomei ftomei@arpae.it
    Gabriele Antolini gantolini@arpae.it
    Laura Costantini laura.costantini0@gmail.com
*/


#ifndef RASTEROBJECT_H
#define RASTEROBJECT_H

#include "MapGraphics_global.h"
#include "MapGraphicsObject.h"
#include "MapGraphicsView.h"
#include "colorlegend.h"
#include "map.h"


struct RowCol
{
    unsigned int row;
    unsigned int col;
};


/*!
 * \brief The RasterObject class
 */
class RasterObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    gis::Crit3DRasterGrid *currentRaster;

    /*!
     * \brief RasterObject constructor
     * \param view a MapGraphicsView pointer
     * \param parent MapGraphicsObject
     */
    explicit RasterObject(MapGraphicsView* view, MapGraphicsObject *parent = nullptr);

    /*!
     * \brief boundingRect pure-virtual from MapGraphicsObject
     * Defines the outer bounds of the item as a rectangle; all painting must be restricted to inside an item's bounding rect.
     * \return the bounding rect QRectF
     */
    QRectF boundingRect() const;

    /*!
     * \brief paint pure-virtual from MapGraphicsObject
     * \param painter a QPainter pointer
     * \param option a QStyleOptionGraphicsItem pointer
     * \param widget a QWidget pointer
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void clean();
    void updateCenter();
    void setDrawing(bool value);
    void setDrawBorders(bool value);
    void setColorLegend(ColorLegend* myLegend);
    bool initializeUTM(gis::Crit3DRasterGrid* myRaster, const gis::Crit3DGisSettings& gisSettings, bool isGrid_);
    bool initializeLatLon(gis::Crit3DRasterGrid* myRaster, const gis::Crit3DGisSettings& gisSettings,
                          const gis::Crit3DGridHeader& latLonHeader, bool isGrid_);
    float getRasterMaxSize();
    gis::Crit3DGeoPoint* getRasterCenter();
    void setCurrentRaster(gis::Crit3DRasterGrid* rasterPointer);

protected:
    //virtual from MapGraphicsObject

private:
    MapGraphicsView* _view;
    gis::Crit3DGeoMap* geoMap;
    ColorLegend* legend;
    bool isDrawing;
    bool drawBorder;
    RowCol **matrix;
    gis::Crit3DGridHeader latLonHeader;
    bool isLatLon;
    bool isGrid;
    int utmZone;

    void freeIndexesMatrix();
    void initializeIndexesMatrix();

    bool setMapResolution();
    bool drawRaster(gis::Crit3DRasterGrid *myRaster, QPainter* myPainter, bool drawBorder);
};


#endif // RASTEROBJECT_H
