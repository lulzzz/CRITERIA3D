/*!
    \copyright 2016 Fausto Tomei, Gabriele Antolini,
    Alberto Pistocchi, Marco Bittelli, Antonio Volta, Laura Costantini

    This file is part of CRITERIA3D.
    CRITERIA3D has been developed under contract issued by A.R.P.A. Emilia-Romagna

    CRITERIA3D is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CRITERIA3D is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CRITERIA3D.  If not, see <http://www.gnu.org/licenses/>.

    Contacts:
    fausto.tomei@gmail.com
    ftomei@arpae.it
*/

#include <math.h>
#include <malloc.h>
#include "commonConstants.h"
#include "furtherMathFunctions.h"
#include "basicMath.h"
#include "statistics.h"

namespace statistics
{

    float rootMeanSquareError(float *measured , float *simulated , int nrData)
    {
        float sigma=0;
        for (int i=0; i< nrData; i++)
        {
            sigma += powf(measured[i]-simulated[i],2);
        }
        sigma /= nrData;
        sigma = sqrtf(sigma);
        return sigma;
    }

    float coefficientOfVariation(float *measured , float *simulated , int nrData)
    {
        float sigma=0;
        float measuredMean = 0;
        float coefVar;
        for (int i=0; i< nrData; i++)
        {
            sigma += powf(measured[i]-simulated[i],2);
            measuredMean += measured[i];
        }
        sigma /= nrData;
        measuredMean /= nrData;
        sigma = sqrtf(sigma);
        coefVar = sigma / measuredMean ;
        return coefVar;
    }

    float weighedMean(float *data , float *weights, int nrData)
    {
        float mean=0 ;
        float weightsSum=0 ;
        for (int i = 0 ; i<nrData;i++) weightsSum += weights[i];
        if ((weightsSum< 0.99) || (weightsSum > 1.01)) return -8888 ;
        for (int i = 0 ; i<nrData;i++) mean += weights[i]*data[i];
        return mean ;
    }

    void linearRegression( float* x,  float* y, long nrItems, bool zeroIntercept, float* y_intercept, float* mySlope, float* r2)
    {
       double SUMx = 0;         /*!< sum of x values */
       double SUMy = 0;         /*!< sum of y values */
       double SUMxy = 0;        /*!< sum of x * y */
       double SUMxx = 0;        /*!< sum of x^2 */
       double AVGy = 0;         /*!< mean of y */
       double AVGx = 0;         /*!< mean of x */
       double dy = 0;           /*!< squared of the discrepancies */
       double SUM_dy = 0;       /*!< sum of squared of the discrepancies */
       double SUMres = 0;       /*!< sum of squared residue */
       double res = 0;          /*!< residue squared */

       long nrValidItems = 0;

       *mySlope = 0;             /*!< slope of regression line */
       *y_intercept = 0;         /*!< y intercept of regression line */
       *r2 = 0;                  /*!< coefficient of determination */

       /*! calculate various sums */
       for (int i = 0; i < nrItems; i++)
           if ((x[i] != NODATA) && (y[i] != NODATA))
           {
                nrValidItems++;
                SUMx += x[i];
                SUMy += y[i];
                SUMxy += x[i] * y[i];
                SUMxx += x[i] * x[i];
           }

        /*! means of x and y */
        AVGy = SUMy / nrValidItems;
        AVGx = SUMx / nrValidItems;

        if (!zeroIntercept)
        {
            *mySlope = float((SUMxy - SUMx * SUMy / nrValidItems) / (SUMxx - SUMx * SUMx / nrValidItems));
            *y_intercept = float(AVGy - *mySlope * AVGx);
        }
        else
        {
            *mySlope = float(SUMxy / SUMxx);
            *y_intercept = 0.;
        }

        /*! calculate squared residues and their sum */
        for (int i = 0; i < nrItems; i++)
           if ((x[i] != NODATA) && (y[i] != NODATA))
           {
              /*! sum of squared of the discrepancies */
              dy = y[i] - (*y_intercept + *mySlope * x[i]);
              dy *= dy;
              SUM_dy += dy;

              /*! sum of squared residues */
              res = y[i] - AVGy;
              res *= res;
              SUMres += res;
           }

        /*! calculate r^2 (coefficient of determination) */
        *r2 = float((SUMres - SUM_dy) / SUMres);
    }

    /*! Variance */
    float variance(float *myList, int nrList)
    {
        float myMean,myDiff,squareDiff;
        int i, nrValidValues;

        if (nrList <= 1) return NODATA;

        myMean = mean(myList,nrList);

        squareDiff = 0;
        nrValidValues = 0;
        for (i = 0;i<nrList;i++)
        {
            if (myList[i]!= NODATA)
            {
                myDiff = (myList[i] - myMean);
                squareDiff += powf(myDiff, 2);
                nrValidValues++;
            }
        }
        if (nrValidValues > 1)
            return (squareDiff / (nrValidValues - 1));
        else
            return NODATA;
    }

    float mean(float *myList, int nrList)
    {
        float sum=0;
        int i, nrValidValues;
        if (nrList < 1) return NODATA;
        nrValidValues = 0;
        for (i = 0;i<nrList;i++)
        {
            if (myList[i]!= NODATA)
            {
                sum += myList[i];
                nrValidValues++;
            }
        }
        if (nrValidValues > 0)
            return (sum/(float)(nrValidValues));
        else
            return NODATA;
    }

    float standardDeviation(float *myList, int nrList)
    {
        return sqrtf(variance(myList,nrList));
    }
    /*! covariance */
    float covariance(float *myList1, int nrList1,float *myList2, int nrList2)
    {
        float myMean1,myMean2,myDiff1,myDiff2,prodDiff;
        int i, nrValidValues;

        if (nrList1 <= 1) return NODATA;
        if (nrList2 <= 1) return NODATA;
        if (nrList2 != nrList1) return NODATA;

        myMean1 = mean(myList1,nrList1);
        myMean2 = mean(myList2,nrList2);
        prodDiff = 0;
        nrValidValues = 0;
        for (i = 0;i<nrList1;i++)
        {
            if ((myList1[i]!= NODATA)&&myList2[i]!=NODATA)
            {
                myDiff1 = (myList1[i] - myMean1);
                myDiff2 = (myList2[i] - myMean2);
                prodDiff += myDiff1*myDiff2;
                nrValidValues++;
            }
        }
        return (prodDiff / (nrValidValues - 1));
    }

    float coefficientPearson(float *myList1, int nrList1,float *myList2, int nrList2)
    {
        return (covariance(myList1,nrList1,myList2,nrList2)/(standardDeviation(myList1,nrList1)*standardDeviation(myList2,nrList2)));
    }

    float** covariancesMatrix(int nrRowCol, float**myLists,int nrLists)
    {
        float** c = (float**)calloc(nrRowCol, sizeof(float*));
        for(int i = 0;i<nrRowCol;i++)
        {
            c[i] = (float*)calloc(nrRowCol, sizeof(float));
        }
        for(int i = 0;i<nrRowCol;i++)
        {
            c[i][i]= variance(myLists[i],nrLists);
            for(int j = i+1;j<nrRowCol;j++)
            {
                c[i][j]= covariance(myLists[i],nrLists,myLists[j],nrLists);
                c[j][i]=c[i][j];
            }

        }
        return c;
    }

    void correlationsMatrix(int nrRowCol, float**myLists,int nrLists, float** c)
    {
        // input: myLists matrix
        // output: c matrix
        for(int i = 0;i<nrRowCol;i++)
        {
            c[i][i]=1.;
            for(int j = i+1;j<nrRowCol;j++)
            {
                c[i][j]= covariance(myLists[i],nrLists,myLists[j],nrLists);
                if (c[i][j] != 0) c[i][j] /= sqrtf(variance(myLists[i],nrLists)*variance(myLists[j],nrLists));
                c[j][i] = c[i][j];
            }

        }
    }

    float ERF(float x, float accuracy) // error function
    {
        return (float)(2*pow(PI,-0.5)*integration::qsimp(errorFunctionPrimitive,0.,x,accuracy));
    }

    float ERFC(float x, float accuracy) // error function
    {
        return (float)(1. - ERF(x,accuracy));
    }
    float inverseERF(float value, float accuracy)
    {

        if (value >=1 || value <= -1)
        {
            return PARAMETER_ERROR;
        }
        float rootLeft,rootRight;
        float root;
        float absoluteValue;
        absoluteValue = fabs(value);


        if (value == 0)
        {
            return 0.;
        }
        else if (value  > 0)
        {
            float leftBound, rightBound;
            leftBound = 0.;
            rightBound = 100.;
            rootLeft = 0.;
            rootRight = ERF(rightBound,accuracy);
            do
            {
                root = ERF((rightBound+leftBound)/2,accuracy);
                if (root < value)
                {
                    leftBound = (rightBound+leftBound)/2;
                }
                else
                {
                    rightBound = (rightBound+leftBound)/2;
                }
            } while(fabs(leftBound - rightBound) > accuracy);

            return (rightBound+leftBound)/2;
        }
        else
        {
            float leftBound, rightBound;
            leftBound = -100.;
            rightBound = 0.;
            rootLeft = 0.;
            rootRight = ERF(rightBound,accuracy);
            do
            {
                root = ERF((rightBound+leftBound)/2,accuracy);
                if (root < value)
                {
                    leftBound = (rightBound+leftBound)/2;
                }
                else
                {
                    rightBound = (rightBound+leftBound)/2;
                }
            } while(fabs(leftBound - rightBound) > accuracy);

            return (rightBound+leftBound)/2;
        }



    }

    float inverseERFC(float value, float accuracy)
    {

        if (value >=2 || value <= 0)
        {
            return PARAMETER_ERROR;
        }


        float rootLeft,rootRight;
        float root;
        float absoluteValue;
        absoluteValue = fabs(value);

        if (value == 1)
        {
            return 0. ;
        }
        else if (value  < 1)
        {
            float leftBound, rightBound;
            leftBound = 0.;
            rightBound = 100.;
            rootLeft = 1.;
            rootRight = ERFC(rightBound,accuracy);
            do
            {
                root = ERFC((rightBound+leftBound)/2,accuracy);
                if (root > value)
                {
                    leftBound = (rightBound+leftBound)/2;
                }
                else
                {
                    rightBound = (rightBound+leftBound)/2;
                }
            } while(fabs(leftBound - rightBound) > accuracy);
            return (rightBound+leftBound)/2;
        }
        else
        {
            float leftBound, rightBound;
            leftBound = -100.;
            rightBound = 0.;
            rootLeft = ERFC(rightBound,accuracy);
            rootRight = 1.;
            do
            {
                root = ERFC((rightBound+leftBound)/2,accuracy);
                if (root > value)
                {
                    leftBound = (rightBound+leftBound)/2;
                }
                else
                {
                    rightBound = (rightBound+leftBound)/2;
                }
            } while(fabs(leftBound - rightBound) > accuracy);
            return (rightBound+leftBound)/2;
        }


    }





}
