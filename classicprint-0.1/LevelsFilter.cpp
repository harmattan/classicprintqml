/*! 
** @file	LevelsFilter.cpp
** 
** @brief	QtImageFilter to adjust colour levels
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "LevelsFilter.h"
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
register_levels_filter() {
	return new LevelsFilter();
}

LevelsFilter::LevelsFilter() {
	m_scale_red 	= false;
	m_scale_green 	= false;
	m_scale_blue 	= false;
	m_percent       = 100;
	memset(m_red_levels, 0, sizeof(unsigned char) * 256);
	memset(m_green_levels, 0, sizeof(unsigned char) * 256);
	memset(m_blue_levels, 0, sizeof(unsigned char) * 256);
}

QImage LevelsFilter::apply(
	const QImage &img,
 	const QRect& clipRect
) const {
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

    int x;
    int y;

	uchar* bits = resultImg.bits();

	for (y = top; y < bottom; y++) {
		for (x = left; x < right; x++) {
			// Read the pixel data
			QRgb rgb = ((uint32_t*)bits)[x];

			// Read from the level lookup arrays
			int red = m_red_levels[qRed(rgb)];
			int green = m_green_levels[qGreen(rgb)];
			int blue = m_blue_levels[qBlue(rgb)];

			// Apply to the selected channels
			red = m_scale_red ? red : qRed(rgb);
			green = m_scale_green ? green : qGreen(rgb);
			blue = m_scale_blue ? blue : qBlue(rgb);

			// Merge back with the main image
			red = merge_colours(red, qRed(rgb), m_percent, 100);
			green = merge_colours(green, qGreen(rgb), m_percent, 100);
			blue = merge_colours(blue, qBlue(rgb), m_percent, 100);

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
LevelsFilter::name(
) const {
	return QLatin1String("Levels");
}

QVariant
LevelsFilter::option(
	int filteroption
) const {
	if (filteroption == Levels) {
		QList<QVariant> levels;
		for (int i = 0; i < 256; ++i) {
			levels.push_back((unsigned int)qRgb(m_red_levels[i], m_green_levels[i], m_blue_levels[i]));
		}
		return QVariant(levels);
	}
	if (filteroption == QtImageFilter::FilterChannels) {
            return get_channels();
	}
	if (filteroption == Percent) {
		return QVariant(m_percent);
	}
	return QVariant();
}

bool
LevelsFilter::setOption(
	int filteroption,
 	const QVariant &value
) {
	if (filteroption == Levels) {
		QList<QVariant> levels = value.toList();
		if (levels.size() == 256) {
			for (int i = 0; i < 256; ++i) {
				unsigned int rgb = levels[i].toUInt();
				m_red_levels[i] = qRed(rgb);
				m_green_levels[i] = qGreen(rgb);
				m_blue_levels[i] = qBlue(rgb);
			}
		}
	}
	if (filteroption == QtImageFilter::FilterChannels) {
		return set_channels(value.toString());
	}
	if (filteroption == Percent) {
		m_percent = value.toInt();
	}
	return true;
}
	
bool
LevelsFilter::supportsOption(
	int option
) const {
	if ((option == Levels) ||
            (option == QtImageFilter::FilterChannels) ||
            (option == Percent)) {
            return true;
	}
	return false;
}

QString
LevelsFilter::description(
) const {
	return QObject::tr("Adjust colour levels within an image");
}

bool
LevelsFilter::set_channels(
	const QString &rgba
) {
    bool bOK 		= true;
	m_scale_red 	= false;
	m_scale_green 	= false;
	m_scale_blue 	= false;
    int rgbaCount[4] = {0,0,0,0};
	
    // We don't want to accept multiple number of channels (e.g. "rrgbaaa")
    // (That might break the interface for future changes)
    int i;
    for (i = 0; i < rgba.length() && bOK; i++) {
        int index = -1;
        switch (rgba.at(i).toLatin1()) {
            case 'r':
            index = 0;  
            break;
            case 'g':
            index = 1;
            break;
            case 'b':
            index = 2;
            break;
            case 'a':
            index = 3;
            break;
        }
        if (index != -1 && ++rgbaCount[index] > 1) bOK = false;
    }
    if (bOK)
    {
        if (rgba.contains(QChar('r'))) m_scale_red = true;
        if (rgba.contains(QChar('g'))) m_scale_green = true;
        if (rgba.contains(QChar('b'))) m_scale_blue = true;
    }
    return bOK;
}

QString
LevelsFilter::get_channels() const {
    QByteArray chan;
    if (m_scale_red)
        chan.append('r');
    if (m_scale_green)
        chan.append('g');
    if (m_scale_blue)
        chan.append('b');
    return chan;
}
