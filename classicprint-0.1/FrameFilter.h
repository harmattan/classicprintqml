	/*! 
** @file	FrameFilter.h
** 
** @brief	QtImageFilter to add a retro style frame to an image
**  
*/  
#ifndef __framefilter__h
#define __framefilter__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QtImageFilter>
#include <QByteArray>
 
/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define REGISTER_FRAME_FILTER 	QtImageFilterFactory::registerImageFilter("Frame", register_frame_filter)
 
/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

QtImageFilter* register_frame_filter();

class FrameFilter : public QtImageFilter {
public:
	enum FrameFilterOption {
		FrameSizePercent
	};

	enum FrameStyle {
		FRAMESTYLE_OLD_PHOTO
	};

	FrameFilter();

	virtual QImage apply(const QImage &img, const QRect& clipRect = QRect()) const;

	virtual QString name() const;

	virtual QVariant option(int filteroption) const;

	virtual bool setOption(int filteroption, const QVariant &value);

	virtual bool supportsOption(int option) const;

	virtual QString description() const;

private:
	
private:
		double				m_frame_size_percent;
};

#endif
