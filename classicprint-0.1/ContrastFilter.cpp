/*! 
** @file	ContrastFilter.cpp
** 
** @brief	QtImageFilter to modify contrast
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "ContrastFilter.h"
#include "utils.h"
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

QtImageFilter*
register_contrast_filter() {
        return new ContrastFilter();
}

ContrastFilter::ContrastFilter() {
    m_contrast_percent = 0.0;
}

QImage ContrastFilter::apply(
	const QImage &img,
 	const QRect& clipRect
        ) const {
    int x;
    int y;
    int top = 0;
    int bottom = img.height();
    int left = 0;
    int right = img.width();

    if (!clipRect.isNull()) {
        // If we have a cliprect, set our coordinates to our cliprect
        // and make sure it is within the boundaries of the image
        top = qMax(top, clipRect.top());
        bottom = qMin(bottom, clipRect.bottom());
        left = qMax(left, clipRect.left());
        right = qMin(right, clipRect.right());
    }

    QImage::Format fmt = img.format();
    QImage resultImg = img.convertToFormat(QImage::Format_ARGB32);

	uchar* bits = resultImg.bits();

    for (y = top; y < bottom; y++) {
        for (x = left; x < right; x++) {
			// Read the pixel data
			QRgb rgb = ((uint32_t*)bits)[x];

			int red = qRed(rgb) < 128 ?
						 (2 * scale(qRed(rgb), 0, 255, 64, 192) * qRed(rgb) / 255) :
						 255 - (2 * (255 - scale(qRed(rgb), 0, 255, 64, 192)) * (255 - qRed(rgb)) / 255);
			red = merge_colours(red, qRed(rgb), (int)m_contrast_percent, 100);
			int green = qGreen(rgb) < 128 ?
						 (2 * scale(qGreen(rgb), 0, 255, 64, 192) * qGreen(rgb) / 255) :
						 255 - (2 * (255 - scale(qGreen(rgb), 0, 255, 64, 192)) * (255 - qGreen(rgb)) / 255);
			green = merge_colours(green, qGreen(rgb), (int)m_contrast_percent, 100);
			int blue = qBlue(rgb) < 128 ?
						 (2 * scale(qBlue(rgb), 0, 255, 64, 192) * qBlue(rgb) / 255) :
						 255 - (2 * (255 - scale(qBlue(rgb), 0, 255, 64, 192)) * (255 - qBlue(rgb)) / 255);
			blue = merge_colours(blue, qBlue(rgb), (int)m_contrast_percent, 100);

			((uint32_t*)bits)[x] = qRgb(red, green, blue);
		}
		bits += resultImg.bytesPerLine();
	}
    
    if (resultImg.format() != fmt) {
        resultImg = resultImg.convertToFormat(fmt);
    }
    return resultImg;
}

QString
ContrastFilter::name(
) const {
        return QLatin1String("Contrast");
}

QVariant
ContrastFilter::option(
	int filteroption
) const {
    if (filteroption == ContrastPercent) {
            return QVariant(m_contrast_percent);
    }
    return QVariant();
}

bool
ContrastFilter::setOption(
	int filteroption,
 	const QVariant &value
) {
    if (filteroption == ContrastPercent) {
        m_contrast_percent = value.toDouble();
    }
    return true;
}
	
bool
ContrastFilter::supportsOption(
	int option
) const {
        if (option == ContrastPercent) {
		return true;
	}
	return false;
}

QString
ContrastFilter::description(
) const {
        return QObject::tr("Modify image contrast");
}

