/*! 
** @file	VignetteFilter.h
** 
** @brief	QtImageFilter to adjust colour levels
**  
*/  
#ifndef __vignettefilter__h
#define __vignettefilter__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QtImageFilter>
 
/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define REGISTER_VIGNETTE_FILTER 	QtImageFilterFactory::registerImageFilter("Vignette", register_vignette_filter)
 
/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

QtImageFilter* register_vignette_filter();

class VignetteFilter : public QtImageFilter {
public:
    enum LevelsFilterOption {
            VignetteRadiusPercent = UserOption,
            VignetteAmountPercent,
            DodgePercent,
            Blur
    };
        VignetteFilter();

	virtual QImage apply(const QImage &img, const QRect& clipRect = QRect()) const;
	virtual QImage apply(const QImage &img, const QRect& clipRect,
						 void (*progress)(int, void*), void* context) const;

	virtual QString name() const;

	virtual QVariant option(int filteroption) const;

	virtual bool setOption(int filteroption, const QVariant &value);

	virtual bool supportsOption(int option) const;

	virtual QString description() const;

private:
		int
        process_colour(
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
        ) const;


private:
		double			m_vignette_radius_percent;
        double          m_vignette_amount_percent;
        double          m_dodge_percent;
        bool            m_blur;
};

#endif
