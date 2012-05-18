/*! 
** @file	BlendFilter.cpp
** 
** @brief	QtImageFilter to blend an image on disk with the current image
**
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "BlendFilter.h"
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
register_blend_filter() {
        return new BlendFilter();
}

BlendFilter::BlendFilter() {
}

QImage BlendFilter::apply(
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

    // Resize the blend image to match the source image
    QImage resized_image(m_blend_image.scaled(img.width(), img.height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	uchar* bits = resultImg.bits();
	const uchar* bits_blend = resized_image.bits();

	for (y = top; y < bottom; y++) {
        for (x = left; x < right; x++) {
			// Read the pixel data
			QRgb rgb_image = ((uint32_t*)bits)[x];
			QRgb rgb_blend = ((uint32_t*)bits_blend)[x];

            int red = qRed(rgb_image) + qRed(rgb_blend) -
                         (qRed(rgb_blend) * qRed(rgb_image) / 255.0);
            int green = qGreen(rgb_image) + qGreen(rgb_blend) -
                         (qGreen(rgb_blend) * qGreen(rgb_image) / 255.0);
            int blue = qBlue(rgb_image) + qBlue(rgb_blend) -
                         (qBlue(rgb_blend) * qBlue(rgb_image) / 255.0);

			((uint32_t*)bits)[x] = qRgb(red, green, blue);
		}
		bits += resultImg.bytesPerLine();
		bits_blend += resized_image.bytesPerLine();
	}
    
    if (resultImg.format() != fmt) {
        resultImg = resultImg.convertToFormat(fmt);
    }
    return resultImg;
}

QString
BlendFilter::name(
) const {
        return QLatin1String("Blend");
}

QVariant
BlendFilter::option(
	int filteroption
) const {
    if (filteroption == BlendImage) {
            return QVariant(m_blend_filename);
    }
    return QVariant();
}

bool
BlendFilter::setOption(
	int filteroption,
 	const QVariant &value
) {
    if (filteroption == BlendImage) {
        m_blend_filename = value.toString();
        m_blend_image.load(m_blend_filename);
        if (m_blend_image.isNull()) {
            return false;
        }
    }
    return true;
}
	
bool
BlendFilter::supportsOption(
	int option
) const {
        if (option == BlendImage) {
		return true;
	}
	return false;
}

QString
BlendFilter::description(
) const {
        return QObject::tr("Blend an image on disk with the current image");
}

