/*!
** @file	utils.cpp
** 
** @brief	Utility functions
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "utils.h"
#include <QVariant>
#include <stdint.h>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
 
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
scale(double value, double src_min, double src_max, double dst_min, double dst_max) {
    return dst_min + (dst_max - dst_min) * ((value - src_min) / (src_max - src_min));
}

int
scale(int value, int src_min, int src_max, int dst_min, int dst_max) {
	return dst_min + ((dst_max - dst_min) * ((value - src_min)) / (src_max - src_min));
}

double
merge_colours(double val1, double val2, double min, double max) {
    return val1 * min / max + val2 * (max - min) / max;
}

int
merge_colours(int val1, int val2, int min, int max) {
	return val1 * min / max + val2 * (max - min) / max;
}

QRgb
convolvePixel(const QImage &img, int x, int y,
              const int *kernelMatrix,
              int kernelRows, int kernelColumns,
              int divisor, int bias)
{
    int i;
    int j;
    int rows = kernelRows;
    int cols = kernelColumns;
	int width = img.width();
	int height = img.height();
	int bytes_per_line = img.bytesPerLine();

	const uchar* bits = img.bits();

    int c_offset = x - kernelColumns / 2;
    int r_offset = y - kernelRows / 2;

    int sumRed   = 0;
    int sumGreen = 0;
    int sumBlue  = 0;
    int sumAlpha = 0;

    for (i = 0; i < cols; i++) {
        for (j = 0; j < rows; j++) {
            int srcpix_x = c_offset + j;
            int srcpix_y = r_offset + i;

            if (srcpix_x < 0) {
                    srcpix_x = -srcpix_x;
					srcpix_x %= width;
			}else if (srcpix_x >= width) {
					srcpix_x %= width;
					srcpix_x = width - 1 - srcpix_x;
            }

            if (srcpix_y < 0) {
                    srcpix_y = -srcpix_y;
					srcpix_y %= height;
			}else if (srcpix_y >= height) {
					srcpix_y %= height;
					srcpix_y = height - 1 - srcpix_y;
            }

			QRgb rgb = ((uint32_t*)(bits + srcpix_y * bytes_per_line))[srcpix_x];
            int weight = kernelMatrix[i * rows + j];
            sumRed   += qRed(rgb)   * weight;
            sumGreen += qGreen(rgb) * weight;
            sumBlue  += qBlue(rgb)  * weight;
            sumAlpha += qAlpha(rgb) * weight;
        }
    }

    if (divisor) sumRed/=divisor;
	sumRed = qBound(0, sumRed + bias, 255);

    if (divisor) sumGreen/=divisor;
    sumGreen = qBound(0, sumGreen + bias, 255);

    if (divisor) sumBlue/=divisor;
    sumBlue = qBound(0, sumBlue + bias, 255);

    if (divisor) sumAlpha/=divisor;
    sumAlpha = qBound(0, sumAlpha + bias, 255);

    return qRgba(sumRed, sumGreen, sumBlue, sumAlpha);
}

QString
byteArrayToString(const QByteArray& ba) {
	QString	ret;
	for (int i = 0; i < ba.size(); ++i) {
		if (i > 0) {
			ret += ", ";
		}
		ret += QVariant(ba[i]).toString();
	}
	return ret;
}

QByteArray
stringToByteArray(const QString& str) {
	QByteArray ret;
	QString cp(str);

	while (cp.size() > 0) {
		bool got_comma = false;
		for (int i = 0; i < cp.size(); ++i) {
			if (cp[i] == ',') {
				ret.push_back((char)cp.mid(0, i).toInt());
				cp = cp.mid(i + 2);
				got_comma = true;
				break;
			}
		}
		if (!got_comma) {
			// Last entry
			ret.push_back((char)cp.toInt());
			cp.clear();
		}
	}
	return ret;
}

