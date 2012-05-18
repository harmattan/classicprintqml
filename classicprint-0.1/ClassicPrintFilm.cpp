/*!
** @file	ClassicPrintFilm.cpp
**
** @brief	Class that manages film settings
**
*/

/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "ClassicPrintFilm.h"
#include "NoiseFilter.h"
#include "LevelsFilter.h"
#include "utils.h"

#include <QtImageFilter>
#include <QtImageFilterFactory>

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
ClassicPrintFilm::ClassicPrintFilm() {
    m_temperature = 0.0;
    m_noise = 0.0;
}

//---------------------------------------------------------------------------
/*!
** @brief   operator=
*/
ClassicPrintFilm& ClassicPrintFilm::operator=(const ClassicPrintFilm& film) {
    m_temperature = film.m_temperature;
    m_noise = film.m_noise;
    m_name = film.m_name;
    return *this;
}

//---------------------------------------------------------------------------
/*!
** @brief   Destructor
**
*/
ClassicPrintFilm::~ClassicPrintFilm() {
}

//---------------------------------------------------------------------------
/*!
** @brief   Process an image
**
** @param [In] image    Image to process
**
** @return  True/False
*/
bool ClassicPrintFilm::process(QImage& image) {
    QtImageFilter* filter;
	QList<QVariant> levels;

    int green_level = (22 * m_temperature / 100) - 11;
    int blue_level = -((64 * m_temperature / 100) - 32);

    emit progress(0);

    for (int i = 0; i < 256; ++i) {
		levels.push_back(QVariant((qlonglong)qRgb(0,
												  qBound(0, i + green_level, 255),
												  qBound(0, i + blue_level, 255))));
    }

    filter = QtImageFilterFactory::createImageFilter("Levels");
	filter->setOption(QtImageFilter::FilterChannels, "gb");
	filter->setOption(LevelsFilter::Levels, levels);
    image = filter->apply(image);

    emit progress(66);
    filter = QtImageFilterFactory::createImageFilter("Noise");
    filter->setOption(NoiseFilter::NoisePercent, m_noise);
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
bool ClassicPrintFilm::save_node(QDomDocument& doc, QDomElement& parent_node) {
    // Name
    QDomElement name = doc.createElement("Name");
    QDomText name_text = doc.createTextNode(m_name);
    name.appendChild(name_text);
    parent_node.appendChild(name);

    // Temperature
    QDomElement temperature = doc.createElement("Temperature");
    QDomText temperature_text = doc.createTextNode(QString().setNum(m_temperature));
    temperature.appendChild(temperature_text);
    parent_node.appendChild(temperature);

    // Noise
    QDomElement noise = doc.createElement("Noise");
    QDomText noise_text = doc.createTextNode(QString().setNum(m_noise));
    noise.appendChild(noise_text);
    parent_node.appendChild(noise);

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
bool ClassicPrintFilm::load_node(QDomDocument& doc, QDomElement& parent_node) {
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
            else if (e.tagName() == "Temperature") {
                m_temperature = e.text().toDouble();
            }
            else if (e.tagName() == "Noise") {
                m_noise = e.text().toDouble();
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
void ClassicPrintFilm::setName(QString name) {
    m_name = name;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the name of the object
**
** @return  Name of the object
**
*/
QString ClassicPrintFilm::name() {
    return m_name;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set colour temperature in percent. 0% = cold, 100% = warm
**
** @param [In] temperature  Temperature in percent
**
*/
void ClassicPrintFilm::setTemperature(double temperature) {
    m_temperature = temperature;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get colour temperature in percent. 0% = cold, 100% = warm
**
** @return  Temperature in percent
**
*/
double ClassicPrintFilm::temperature() {
    return m_temperature;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set noise in percent
**
** @param [In] noise    Noise value in percent
**
*/
void ClassicPrintFilm::setNoise(double noise) {
    m_noise = noise;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get noise value in percent
**
** @return  Noise value in percent
**
*/
double ClassicPrintFilm::noise() {
    return m_noise;
}
