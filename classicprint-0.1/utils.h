/*!
** @file	utils.h
** 
** @brief	Utility functions
**  
*/  
#ifndef __utils__h
#define __utils__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QRgb>
#include <QImage>
#include <QString>
#include <QByteArray>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define sq(x) ((x) * (x))

/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

double
scale(double value, double src_min, double src_max, double dst_min, double dst_max);

int
scale(int value, int src_min, int src_max, int dst_min, int dst_max);

double
merge_colours(double val1, double val2, double min, double max);

int
merge_colours(int val1, int val2, int min, int max);

QRgb
convolvePixel(const QImage &img, int x, int y,
              const int *kernelMatrix,
              int kernelRows, int kernelColumns,
              int divisor, int bias);

QString
byteArrayToString(const QByteArray& ba);

QByteArray
stringToByteArray(const QString& str);

#endif
