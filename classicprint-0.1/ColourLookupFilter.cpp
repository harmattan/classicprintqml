/*! 
** @file	ColourLookupFilter.cpp
** 
** @brief	QtImageFilter to lookup and merge colours from a data file
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "ColourLookupFilter.h"
#include "utils.h"

#if 1
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#endif

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
register_colourlookup_filter() {
        return new ColourLookupFilter();
}

ColourLookupFilter::ColourLookupFilter() {
    m_colour_lookup_percent = 0.0;
    m_colour_lookup_index = 0;
}

QImage ColourLookupFilter::apply(
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

#if 0
	{
		QDomDocument    doc("ColourLevels");
		// Create the root element
		QDomElement     root = doc.createElement("ColourLevels");
		QDomElement levels;
		doc.appendChild(root);

		for (y = 0; y < 21; ++y) {
			QDomElement colour;
			QString lst;

			if ((y % 3) == 0) {
				levels = doc.createElement("Levels");
				root.appendChild(levels);

				QDomElement name = doc.createElement("Name");
				QDomText name_text = doc.createTextNode(QString("Profile " + QVariant(y / 3 + 1).toString()));
				levels.appendChild(name);
				name.appendChild(name_text);

				colour = doc.createElement("Red");
				levels.appendChild(colour);
			}
			else if ((y % 3) == 1) {
				colour = doc.createElement("Green");
				levels.appendChild(colour);
			}
			else {
				colour = doc.createElement("Blue");
				levels.appendChild(colour);
			}

			for (x = 0; x < 256; ++x) {
				QRgb rgb = m_colour_lookup_image.pixel(x, y);
				if (x > 0) {
					lst += ", ";
				}
				if ((y % 3) == 0) {
					lst += QVariant(qRed(rgb)).toString();
				}
				else if ((y % 3) == 1) {
					lst += QVariant(qGreen(rgb)).toString();
				}
				else {
					lst += QVariant(qBlue(rgb)).toString();
				}
			}
			QDomText text = doc.createTextNode(lst);
			colour.appendChild(text);
		}
		// Write it to the file
		QString xml(doc.toString());
		QFile file("/scratchbox/users/roy/home/user/src/ClassicPrint/config/colour_profile/colour_profiles.xml");
		if (file.open(QIODevice::ReadWrite)) {
			file.write(xml.toUtf8());
			file.close();
		}
	}
#endif

    for (y = top; y < bottom; y++) {
        for (x = left; x < right; x++) {
            QRgb rgb = resultImg.pixel(x, y);
            int red = qRed(m_colour_lookup_image.pixel(qRed(rgb), 0 + 3 * m_colour_lookup_index));
            int green = qGreen(m_colour_lookup_image.pixel(qGreen(rgb), 1 + 3 * m_colour_lookup_index));
            int blue = qBlue(m_colour_lookup_image.pixel(qBlue(rgb), 2 + 3 * m_colour_lookup_index));
			red = merge_colours(red, qRed(rgb), (int)m_colour_lookup_percent, 100);
			green = merge_colours(green, qGreen(rgb), (int)m_colour_lookup_percent, 100);
			blue = merge_colours(blue, qBlue(rgb), (int)m_colour_lookup_percent, 100);

            resultImg.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    
    if (resultImg.format() != fmt) {
        resultImg = resultImg.convertToFormat(fmt);
    }
    return resultImg;
}

QString
ColourLookupFilter::name(
) const {
        return QLatin1String("ColourLookup");
}

QVariant
ColourLookupFilter::option(
	int filteroption
) const {
    if (filteroption == ColourLookupPercent) {
        return QVariant(m_colour_lookup_percent);
    }
    else if (filteroption == ColourLookupFile) {
        return QVariant(m_colour_lookup_file);
    }
    else if (filteroption == ColourLookupIndex) {
        return QVariant(m_colour_lookup_index);
    }
    return QVariant();
}

bool
ColourLookupFilter::setOption(
	int filteroption,
 	const QVariant &value
) {
    if (filteroption == ColourLookupPercent) {
        m_colour_lookup_percent = value.toDouble();
    }
    else if (filteroption == ColourLookupFile) {
        m_colour_lookup_file = value.toString();
        m_colour_lookup_image.load(m_colour_lookup_file);
    }
    else if (filteroption == ColourLookupIndex) {
        m_colour_lookup_index = value.toInt();
    }
    return true;
}
	
bool
ColourLookupFilter::supportsOption(
	int option
) const {
        if ((option == ColourLookupPercent) ||
            (option == ColourLookupFile) ||
            (option == ColourLookupIndex)) {
		return true;
	}
	return false;
}

QString
ColourLookupFilter::description(
) const {
        return QObject::tr("Merge colours from a lookup table");
}

