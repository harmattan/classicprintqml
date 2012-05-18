/*! 
** @file	ContrastFilter.h
** 
** @brief	QtImageFilter to add noise to image
**  
*/  
#ifndef __ContrastFilter__h
#define __ContrastFilter__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QtImageFilter>
#include <QImage>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define REGISTER_CONTRAST_FILTER 	QtImageFilterFactory::registerImageFilter("Contrast", register_contrast_filter)
 
/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

QtImageFilter* register_contrast_filter();

class ContrastFilter : public QtImageFilter {
public:
    enum ContrastFilterOption {
            ContrastPercent = UserOption
    };
        ContrastFilter();

	virtual QImage apply(const QImage &img, const QRect& clipRect = QRect()) const;

	virtual QString name() const;

	virtual QVariant option(int filteroption) const;

	virtual bool setOption(int filteroption, const QVariant &value);

	virtual bool supportsOption(int option) const;

	virtual QString description() const;

private:
        double		m_contrast_percent;
};

#endif
