/*!
** @file	LevelsFilter.h 
** 
** @brief	QtImageFilter to adjust colour levels
**  
*/  
#ifndef __levelsfilter__h
#define __levelsfilter__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QtImageFilter>
#include <QList>
#include <QVariant>
 
/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define REGISTER_LEVELS_FILTER 	QtImageFilterFactory::registerImageFilter("Levels", register_levels_filter)
 
/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/

QtImageFilter* register_levels_filter();

class LevelsFilter : public QtImageFilter {
public:
	enum LevelsFilterOption {
                Levels = UserOption,
                Percent
	};

	LevelsFilter();

	virtual QImage apply(const QImage &img, const QRect& clipRect = QRect()) const;

	virtual QString name() const;

	virtual QVariant option(int filteroption) const;

	virtual bool setOption(int filteroption, const QVariant &value);

	virtual bool supportsOption(int option) const;

	virtual QString description() const;

private:
	bool set_channels(const QString &rgba);

	QString	get_channels() const;

private:
	unsigned char m_red_levels[256];
	unsigned char m_green_levels[256];
	unsigned char m_blue_levels[256];
	bool		m_scale_red;
	bool		m_scale_green;
	bool		m_scale_blue;
	int         m_percent;
};

#endif
