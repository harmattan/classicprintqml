/*! 
** @file	NoiseFilter.cpp
** 
** @brief	QtImageFilter to add noise to an image
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "NoiseFilter.h"
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
register_noise_filter() {
        return new NoiseFilter();
}

NoiseFilter::NoiseFilter() {
    m_noise_percent = 0.0;
    m_noise_image.load(":/classicPrintData/noise/noise.jpg");
}

QImage NoiseFilter::apply(
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

	int noise_width = m_noise_image.width();
	int noise_height = m_noise_image.height();

	uchar* bits = resultImg.bits();
	const uchar* bits_noise = m_noise_image.bits();

    for (y = top; y < bottom; y++) {
		// Noise image wraps around if it is smaller than the main image
		int noise_y = y % noise_height;
		bits_noise = m_noise_image.bits() + m_noise_image.bytesPerLine() * noise_y;
		for (x = left; x < right; x++) {
            int noise_x = x % noise_width;

			// Get a pointer to the image data
			QRgb rgb = ((uint32_t*)bits)[x];
			QRgb rgb_noise = ((uint32_t*)bits_noise)[noise_x];

			// Merge the noise image with the main image
			int red = qBound(0, merge_colours(qRed(rgb_noise), 128, (int)m_noise_percent, 100), 255);
			int green = qBound(0, merge_colours(qGreen(rgb_noise), 128, (int)m_noise_percent, 100), 255);
			int blue = qBound(0, merge_colours(qBlue(rgb_noise), 128, (int)m_noise_percent, 100), 255);

			red = (qRed(rgb) < 128) ?
				  (2 * qRed(rgb) * red / 255) :
				  (255 - (2 * (255 - qRed(rgb)) * (255 - red) / 255));
			green = (qGreen(rgb) < 128) ?
				  (2 * qGreen(rgb) * green / 255) :
				  (255 - (2 * (255 - qGreen(rgb)) * (255 - green) / 255));
			blue = (qBlue(rgb) < 128) ?
				  (2 * qBlue(rgb) * blue / 255) :
				  (255 - (2 * (255 - qBlue(rgb)) * (255 - blue) / 255));

			red = qBound(0, red, 255);
			green = qBound(0, green, 255);
			blue = qBound(0, blue, 255);

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
NoiseFilter::name(
) const {
        return QLatin1String("Noise");
}

QVariant
NoiseFilter::option(
	int filteroption
) const {
    if (filteroption == NoisePercent) {
            return QVariant(m_noise_percent);
    }
    return QVariant();
}

bool
NoiseFilter::setOption(
	int filteroption,
 	const QVariant &value
) {
    if (filteroption == NoisePercent) {
        m_noise_percent = value.toDouble();
    }
    return true;
}
	
bool
NoiseFilter::supportsOption(
	int option
) const {
        if (option == NoisePercent) {
		return true;
	}
	return false;
}

QString
NoiseFilter::description(
) const {
        return QObject::tr("Add noise to an image");
}

