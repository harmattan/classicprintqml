/*! 
** @file	ColourLookupFilter.h
** 
** @brief	QtImageFilter which modifies colours based on a lookup table
**  
*/  
#ifndef __ColourLookupFilter__h
#define __ColourLookupFilter__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QtImageFilter>
#include <QImage>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define REGISTER_COLOUR_LOOKUP_FILTER 	QtImageFilterFactory::registerImageFilter("ColourLookup", register_colourlookup_filter)
 
/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

QtImageFilter* register_colourlookup_filter();

class ColourLookupFilter : public QtImageFilter {
public:
    enum ColourLookupFilterOption {
            ColourLookupPercent = UserOption,
            ColourLookupFile,
            ColourLookupIndex
    };
        ColourLookupFilter();

	virtual QImage apply(const QImage &img, const QRect& clipRect = QRect()) const;

	virtual QString name() const;

	virtual QVariant option(int filteroption) const;

	virtual bool setOption(int filteroption, const QVariant &value);

	virtual bool supportsOption(int option) const;

	virtual QString description() const;

private:
        double		m_colour_lookup_percent;
        QString         m_colour_lookup_file;
        QImage          m_colour_lookup_image;
        int             m_colour_lookup_index;
};

#endif
