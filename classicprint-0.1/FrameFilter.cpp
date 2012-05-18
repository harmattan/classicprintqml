/*! 
** @file	FrameFilter.cpp
** 
** @brief	QtImageFilter to adjust colour levels
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "FrameFilter.h"
#include <QPainter>
#include "NoiseFilter.h"
 
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
register_frame_filter() {
	return new FrameFilter();
}

FrameFilter::FrameFilter() {
	m_frame_size_percent = 5;
}

QImage FrameFilter::apply(
	const QImage &img,
 	const QRect& clipRect
) const {
    int top = 0;
    int bottom = img.height();
    int left = 0;
    int right = img.width();

	int frame_width = (int)((double)img.width() * m_frame_size_percent / 100.0);

    if (!clipRect.isNull()) {
        // If we have a cliprect, set our coordinates to our cliprect
        // and make sure it is within the boundaries of the image
        top = qMax(top, clipRect.top());
        bottom = qMin(bottom, clipRect.bottom());
        left = qMax(left, clipRect.left());
        right = qMin(right, clipRect.right());
    }

    QImage::Format fmt = img.format();

    // Create a destination image including the frame
    QImage resultImg(QSize(img.width() + 2 * frame_width, img.height() + 2 * frame_width), QImage::Format_ARGB32);

	// Draw the solid frame colour
	QPainter* painter = new QPainter(&resultImg);
	painter->fillRect(0, 0, resultImg.width(), resultImg.height(), QColor(229, 217, 203));
	delete painter;

	// Add noise to it
	NoiseFilter noise_filter;
	noise_filter.setOption(NoiseFilter::NoisePercent, 60);
	resultImg = noise_filter.apply(resultImg);

	// Put the image in the centre
	painter = new QPainter(&resultImg);
	painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->drawImage(frame_width, frame_width, img);
    delete painter;

    if (resultImg.format() != fmt) {
        resultImg = resultImg.convertToFormat(fmt);
    }
    return resultImg;
}

QString
FrameFilter::name(
) const {
	return QLatin1String("Frame");
}

QVariant
FrameFilter::option(
	int filteroption
) const {
	if (filteroption == FrameSizePercent) {
		return QVariant((int)m_frame_size_percent);
	}
	return QVariant();
}

bool
FrameFilter::setOption(
	int filteroption,
 	const QVariant &value
) {
	if (filteroption == FrameSizePercent) {
		m_frame_size_percent = (FrameStyle)value.toDouble();
	}
	return true;
}
	
bool
FrameFilter::supportsOption(
	int option
) const {
	if (option == FrameSizePercent) {
		return true;
	}
	return false;
}

QString
FrameFilter::description(
) const {
	return QObject::tr("Add a retro style frame to an image");
}

