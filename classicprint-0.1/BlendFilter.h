/*! 
** @file	BlendFilter.h
** 
** @brief	QtImageFilter to blend an image on disk with the current image
**  
*/  
#ifndef __blendfilter__h
#define __blendfilter__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QtImageFilter>
#include <QImage>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define REGISTER_BLEND_FILTER 	QtImageFilterFactory::registerImageFilter("Blend", register_blend_filter)
 
/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

QtImageFilter* register_blend_filter();

class BlendFilter : public QtImageFilter {
public:
    enum BlendFilterOption {
            BlendImage = UserOption
    };
        BlendFilter();

	virtual QImage apply(const QImage &img, const QRect& clipRect = QRect()) const;

	virtual QString name() const;

	virtual QVariant option(int filteroption) const;

	virtual bool setOption(int filteroption, const QVariant &value);

	virtual bool supportsOption(int option) const;

	virtual QString description() const;

private:
        QImage          m_blend_image;
        QString         m_blend_filename;
};

#endif
