/*! 
** @file	VignetteFilter.cpp
** 
** @brief	QtImageFilter to create or fix vignette lens effects
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "VignetteFilter.h"
#include <math.h>
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
static const int matrix[9] = { 1, 2, 1, 2, 2, 2, 1, 2, 1 };
static const int normalise = 1 + 2 + 1 + 2 + 2 + 2 + 1 + 2 + 1;

QtImageFilter*
register_vignette_filter() {
	return new VignetteFilter();
}

VignetteFilter::VignetteFilter() {
    m_vignette_radius_percent = 30.0;
    m_vignette_amount_percent = 80.0;
    m_dodge_percent           = 50.0;
    m_blur                    = false;
}

int
VignetteFilter::process_colour(
		int		colour,
		int		cnv_pixel,
		int		x,
		int		y,
		int		centre_x,
		int		centre_y,
		int		vignette_radius,
		int		vignette_amount_percent,
		int		dodge_percent,
		int		image_diag_dist_to_centre
) const {
    // Find the distance of this pixel from the centre
	int x_sq = sq(x - centre_x);
	int y_sq = sq(y - centre_y);
	int dist_centre = (int)sqrt(x_sq + y_sq);

    // Apply any bluring
    if (m_blur && (dist_centre > vignette_radius)) {
        colour = scale(dist_centre, vignette_radius, image_diag_dist_to_centre, colour, cnv_pixel);
    }

    // Scale the colour based on distance from radius
    if (dist_centre > vignette_radius) {
		colour -= qBound(0, scale(dist_centre, vignette_radius,
									 image_diag_dist_to_centre, 0,
									 (int)vignette_amount_percent / 3),
									 (int)vignette_amount_percent / 3);
    }

	int bright_colour = qBound(0, 2 * colour, 255);
	int contrast_colour = qBound(0, 2 * colour - 255, 255);
	int vignette_distance = scale(dist_centre, vignette_radius, image_diag_dist_to_centre, 0, 100);
	int vig_dist_sq = sq(vignette_distance) / 100;

    // If distance > vignette radius then mix pixel value with vignette value. This effectively
    // makes dark colours darker but has less effect on light colours
    // If inside the vignette radius then mix the pixel value with double the colour value scaled
    // by the distance from the vignette radius (multiplied by amount factor)
    if (dist_centre > vignette_radius) {
		colour = merge_colours(contrast_colour, colour, scale(vig_dist_sq, 0, 100, 0, (int)vignette_amount_percent), 100);
    }
	else {
		if (vignette_radius > 0) {
			colour = merge_colours(bright_colour, colour, scale(sq(vignette_radius - dist_centre) / image_diag_dist_to_centre,
													   0, vignette_radius, 0, (int)dodge_percent), 100);
		}
		else {
			colour = bright_colour;
		}
    }
    return colour;
}

QImage VignetteFilter::apply(
	const QImage &img,
	const QRect& clipRect
) const {
	return apply(img, clipRect, NULL, NULL);
}

QImage VignetteFilter::apply(
	const QImage &img,
	const QRect& clipRect,
	void (*progress)(int, void*), void* context
) const {
    int x;
    int y;
    int top = 0;
    int bottom = img.height();
    int left = 0;
    int right = img.width();
    int centre_x = right / 2;
    int centre_y = bottom / 2;
	int percent = -1;

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

	int image_diag_dist_to_centre = (int)sqrt(sq(img.width()) + sq(img.height())) / 2;
	int vignette_radius = scale((int)m_vignette_radius_percent, 0, 100, 0, image_diag_dist_to_centre);

	uchar* bits = resultImg.bits();

	for (y = top; y < bottom; y++) {
		int this_percent = y * 100 / bottom;
		if (progress && (percent != this_percent)) {
			percent = this_percent;
			progress(percent, context);
		}
        for (x = left; x < right; x++) {
			// Read the pixel data
			QRgb rgb = ((uint32_t*)bits)[x];
			QRgb cnv_pixel = rgb;

            // If blurring is to be applied then convolve the pixel
            if (m_blur) {
                cnv_pixel = convolvePixel(resultImg, x, y, matrix, 3, 3, normalise, 0);
            }

			int red = process_colour(qRed(rgb), qRed(cnv_pixel), x, y, centre_x, centre_y, vignette_radius, m_vignette_amount_percent,
                                        m_dodge_percent, image_diag_dist_to_centre);
			int green = process_colour(qGreen(rgb), qGreen(cnv_pixel), x, y, centre_x, centre_y, vignette_radius, m_vignette_amount_percent,
                                        m_dodge_percent, image_diag_dist_to_centre);
			int blue = process_colour(qBlue(rgb), qBlue(cnv_pixel), x, y, centre_x, centre_y, vignette_radius, m_vignette_amount_percent,
                                        m_dodge_percent, image_diag_dist_to_centre);

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
VignetteFilter::name(
) const {
	return QLatin1String("Vignette");
}

QVariant
VignetteFilter::option(
	int filteroption
) const {
    if (filteroption == VignetteRadiusPercent) {
        return QVariant(m_vignette_radius_percent);
    }
    else if (filteroption == VignetteAmountPercent) {
        return QVariant(m_vignette_amount_percent);
    }
    else if (filteroption == DodgePercent) {
        return QVariant(m_dodge_percent);
    }
    else if (filteroption == Blur) {
        return QVariant(m_blur);
    }
    return QVariant();
}

bool
VignetteFilter::setOption(
	int filteroption,
 	const QVariant &value
) {
    if (filteroption == VignetteRadiusPercent) {
        m_vignette_radius_percent = value.toDouble();
    }
    else if (filteroption == VignetteAmountPercent) {
        m_vignette_amount_percent = value.toDouble();
    }
    else if (filteroption == DodgePercent) {
        m_dodge_percent = value.toDouble();
    }
    else if (filteroption == Blur) {
        m_blur = value.toBool();
    }
    return true;
}
	
bool
VignetteFilter::supportsOption(
	int option
) const {
        if ((option == VignetteRadiusPercent) ||
            (option == VignetteAmountPercent) ||
            (option == DodgePercent) ||
            (option == Blur)) {
		return true;
	}
	return false;
}

QString
VignetteFilter::description(
) const {
	return QObject::tr("Applies or fixes a vignette effect");
}

