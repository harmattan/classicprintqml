/*!
** @file	ClassicPrintProcessing.cpp
**
** @brief	Class that manages processing settings
**
*/

/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "ClassicPrintProcessing.h"
#include "ClassicPrint.h"

#include <QtImageFilter>
#include <QtImageFilterFactory>
#include <QFileInfo>
#include <QTime>
#include <QDir>

#include "ContrastFilter.h"
#include "LevelsFilter.h"
#include "BlendFilter.h"
#include "FrameFilter.h"

#include "utils.h"

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

//---------------------------------------------------------------------------
/*!
** @brief   Constructor
**
*/
ClassicPrintProcessing::ClassicPrintProcessing(ClassicPrint* cp) {
	m_contrast				= 0.0;
	m_colourisation_percent = 0.0;
	m_colourisation.resize(256);
	m_frame_size_percent	= 3.0;
	m_cp = cp;

	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());
}

//---------------------------------------------------------------------------
/*!
** @brief   operator=
*/
ClassicPrintProcessing& ClassicPrintProcessing::operator=(const ClassicPrintProcessing& processing) {
	m_contrast				= processing.m_contrast;
	m_colourisation			= processing.m_colourisation;
	m_colourisation_percent = processing.m_colourisation_percent;
	m_name					= processing.m_name;
	m_frame_size_percent	= processing.m_frame_size_percent;
	m_light_leak			= processing.m_light_leak;
    return *this;
}

//---------------------------------------------------------------------------
/*!
** @brief   Destructor
**
*/
ClassicPrintProcessing::~ClassicPrintProcessing() {
}

//---------------------------------------------------------------------------
/*!
** @brief   Process an image
**
** @param [In] image    Image to process
**
** @return  True/False
*/
bool ClassicPrintProcessing::process(QImage& image) {
    // Apply effects
    QtImageFilter* filter;

    emit progress(0);

    filter = QtImageFilterFactory::createImageFilter("Contrast");
    filter->setOption(ContrastFilter::ContrastPercent, m_contrast);
    image = filter->apply(image);
    delete filter;

    emit progress(25);
	/*
    filter = QtImageFilterFactory::createImageFilter("ColourLookup");
    filter->setOption(ColourLookupFilter::ColourLookupPercent, m_colourisation);
    filter->setOption(ColourLookupFilter::ColourLookupFile, ClassicPrintSettings::colour_profile_dir() + "/colour_profile_1.png");
    image = filter->apply(image);
    delete filter;
	*/
	filter = QtImageFilterFactory::createImageFilter("Levels");
	filter->setOption(LevelsFilter::Percent, m_colourisation_percent);
	filter->setOption(QtImageFilter::FilterChannels, "rgb");
	filter->setOption(LevelsFilter::Levels, m_cp->getColourProfile(m_colourisation));
	image = filter->apply(image);
	delete filter;

    emit progress(50);
	// Apply the light leak if the leak file exists
	// If it is set to "Random" then pick a random leak
	QString light_leak(m_light_leak);
	if (m_light_leak == RANDOM_LEAK) {
		QDir leaks(":/classicPrintData/light_leak");
		QStringList filter;
		filter << "*.jpg" << "*.png";
		QStringList files = leaks.entryList(filter);
		// Get a random index into the list. Allow for 1 extra entry to signal
		// no light leak
		if (files.size() > 0) {
			int index = qrand() % (files.size() + 1);
			if (index < files.size()) {
				light_leak = files[index];
			}
			else {
				light_leak.clear();
			}
		}
	}
	if (light_leak.size() > 0) {
		QString leak_name = ":/classicPrintData/light_leak/" + light_leak;
		QFileInfo fi(leak_name);
		if ((m_light_leak.size() > 0) && fi.exists()) {
			filter = QtImageFilterFactory::createImageFilter("Blend");
			filter->setOption(BlendFilter::BlendImage, leak_name);
			image = filter->apply(image);
			delete filter;
		}
	}
    emit progress(75);
    filter = QtImageFilterFactory::createImageFilter("Frame");
	filter->setOption(FrameFilter::FrameSizePercent, m_frame_size_percent);
    image = filter->apply(image);
    delete filter;

    emit progress(100);
    return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Save configuration to a node
**
** @param [In] parent_node  Node to save to
**
** @return  True/False
*/
bool ClassicPrintProcessing::save_node(QDomDocument& doc, QDomElement& parent_node) {
    // Name
    QDomElement name = doc.createElement("Name");
    QDomText name_text = doc.createTextNode(m_name);
    name.appendChild(name_text);
    parent_node.appendChild(name);

	// Contrast
    QDomElement contrast = doc.createElement("Contrast");
    QDomText contrast_text = doc.createTextNode(QString().setNum(m_contrast));
    contrast.appendChild(contrast_text);
    parent_node.appendChild(contrast);

	//  Colourisation
    QDomElement colour = doc.createElement("Colourisation");
	QDomElement percent = doc.createElement("Percent");
	QDomText colour_text = doc.createTextNode(QString().setNum(m_colourisation_percent));
	parent_node.appendChild(colour);
	colour.appendChild(percent);
	percent.appendChild(colour_text);
	QDomElement colours = doc.createElement("Colours");
	QDomText colours_text = doc.createTextNode(m_colourisation);
	colours.appendChild(colours_text);
	colour.appendChild(colours);

	QDomElement leak = doc.createElement("LightLeak");
	QDomText leak_text = doc.createTextNode(m_light_leak);
	leak.appendChild(leak_text);
	parent_node.appendChild(leak);

	QDomElement frame = doc.createElement("FrameSize");
	QDomText frame_text = doc.createTextNode(QString().setNum(m_frame_size_percent));
	frame.appendChild(frame_text);
	parent_node.appendChild(frame);

	return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Load configuration from a node
**
** @param [In] parent_node  Node to load from
**
** @return  True/False
*/
bool ClassicPrintProcessing::load_node(QDomDocument& doc, QDomElement& parent_node) {
	Q_UNUSED(doc)
    // Iterate through all elements
    QDomNode n = parent_node.firstChild();
    while (!n.isNull()) {
        // Try to convert the node to an element.
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if (e.tagName() == "Name") {
                m_name = e.text();
            }
            else if (e.tagName() == "Contrast") {
                m_contrast = e.text().toDouble();
            }
            else if (e.tagName() == "Colourisation") {
				QDomNode n_c = e.firstChild();
				while (!n_c.isNull()) {
					// Try to convert the node to an element.
					QDomElement e_c = n_c.toElement();
					if (!e_c.isNull()) {
						if (e_c.tagName() == "Percent") {
							m_colourisation_percent = e_c.text().toDouble();
						}
						if (e_c.tagName() == "Colours") {
							m_colourisation = e_c.text();
						}
					}
					n_c = n_c.nextSibling();
				}
            }
			else if (e.tagName() == "LightLeak") {
				m_light_leak = e.text();
			}
        }
        n = n.nextSibling();
    }
    return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the name of the object
**
** @param [In] name     Name of object
**
*/
void ClassicPrintProcessing::setName(QString name) {
    m_name = name;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the name of the object
**
** @return  Name of the object
**
*/
QString ClassicPrintProcessing::name() {
    return m_name;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set image contrast in percent
**
** @param [In] contrast Contrast in percent
**
*/
void ClassicPrintProcessing::setContrast(double contrast) {
    m_contrast = contrast;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get image contrast in percent
**
** @return  Image contrast in percent
**
*/
double ClassicPrintProcessing::contrast() {
    return m_contrast;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set colourisation in percent
**
** @param [In] colourisation    Colourisation to apply in percent
**
*/
void ClassicPrintProcessing::setColourisationPercent(double colourisation) {
	m_colourisation_percent = colourisation;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get colourisation in percent
**
** @return  Colourisation in percent
**
*/
double ClassicPrintProcessing::colourisationPercent() {
	return m_colourisation_percent;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set colourisation name
**
** @param [In] colourisation    Colourisation name
**
*/
void ClassicPrintProcessing::setColourisation(const QString& colourisation) {
	m_colourisation = colourisation;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get colourisation name
**
** @return  Colourisation name
**
*/
const QString& ClassicPrintProcessing::colourisation() {
	return m_colourisation;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set light leak file
**
** @param [In] colourisation    light leak filename
**
*/
void ClassicPrintProcessing::setLightLeak(const QString& light_leak) {
	m_light_leak = light_leak;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get light leak file
**
** @return  Light leak filename
**
*/
const QString& ClassicPrintProcessing::lightLeak() {
	return m_light_leak;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get size of frame in percent
**
** @return  Size in percent
**
*/
double ClassicPrintProcessing::frameSizePercent() {
	return m_frame_size_percent;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set size of frame in percent
**
** @param [In] size		Frame size in percent
**
*/
void ClassicPrintProcessing::setFrameSizePercent(double size) {
	m_frame_size_percent = size;
}

