/*! 
** @file	NoiseFilter.h
** 
** @brief	QtImageFilter to add noise to image
**  
*/  
#ifndef __noisefilter__h
#define __noisefilter__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QtImageFilter>
#include <QImage>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define REGISTER_NOISE_FILTER 	QtImageFilterFactory::registerImageFilter("Noise", register_noise_filter)
 
/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

QtImageFilter* register_noise_filter();

class NoiseFilter : public QtImageFilter {
public:
    enum NoiseFilterOption {
            NoisePercent = UserOption
    };
        NoiseFilter();

	virtual QImage apply(const QImage &img, const QRect& clipRect = QRect()) const;

	virtual QString name() const;

	virtual QVariant option(int filteroption) const;

	virtual bool setOption(int filteroption, const QVariant &value);

	virtual bool supportsOption(int option) const;

	virtual QString description() const;

private:
        double		m_noise_percent;
        QImage          m_noise_image;
};

#endif
