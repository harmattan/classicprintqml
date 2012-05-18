/*!
** @file	ClassicPrintLens.cpp
**
** @brief	Class that manages lens settings
**
*/

/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "ClassicPrintLens.h"
#include <QtImageFilter>
#include <QtImageFilterFactory>
#include <QDomText>

#include "VignetteFilter.h"

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
ClassicPrintLens::ClassicPrintLens() {
    m_radius    = 0.0;
    m_darkness  = 0.0;
    m_dodge     = 0.0;
    m_defocus   = false;
}

//---------------------------------------------------------------------------
/*!
** @brief   operator=
*/
ClassicPrintLens& ClassicPrintLens::operator=(const ClassicPrintLens& lens) {
    m_radius    = lens.m_radius;
    m_darkness  = lens.m_darkness;
    m_dodge     = lens.m_dodge;
    m_defocus   = lens.m_defocus;
    m_name      = lens.m_name;
    return *this;
}

//---------------------------------------------------------------------------
/*!
** @brief   Destructor
**
*/
ClassicPrintLens::~ClassicPrintLens() {
}

//---------------------------------------------------------------------------
/*!
** @brief   Process an image
**
** @param [In] image    Image to process
**
** @return  True/False
*/
bool ClassicPrintLens::process(QImage& image) {
	VignetteFilter* filter = (VignetteFilter*)QtImageFilterFactory::createImageFilter("Vignette");
    if (!filter) {
        return false;
    }
    emit progress(0);
    filter->setOption(VignetteFilter::VignetteRadiusPercent, m_radius);
    filter->setOption(VignetteFilter::VignetteAmountPercent, m_darkness);
    filter->setOption(VignetteFilter::DodgePercent, m_dodge);
    filter->setOption(VignetteFilter::Blur, m_defocus);
	image = filter->apply(image, QRect(), on_progress, this);
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
bool ClassicPrintLens::save_node(QDomDocument& doc, QDomElement& parent_node) {

    // Name
    QDomElement name = doc.createElement("Name");
    QDomText name_text = doc.createTextNode(m_name);
    name.appendChild(name_text);
    parent_node.appendChild(name);

    // Radius
    QDomElement radius = doc.createElement("Radius");
    QDomText radius_text = doc.createTextNode(QString().setNum(m_radius));
    radius.appendChild(radius_text);
    parent_node.appendChild(radius);

    // Darkness
    QDomElement darkness = doc.createElement("Darkness");
    QDomText darkness_text = doc.createTextNode(QString().setNum(m_darkness));
    darkness.appendChild(darkness_text);
    parent_node.appendChild(darkness);

    // Dodge
    QDomElement dodge = doc.createElement("Dodge");
    QDomText dodge_text = doc.createTextNode(QString().setNum(m_dodge));
    dodge.appendChild(dodge_text);
    parent_node.appendChild(dodge);

    // Defocus
    QDomElement defocus = doc.createElement("Defocus");
    QDomText defocus_text = doc.createTextNode(QString().setNum(m_defocus ? 1 : 0));
    defocus.appendChild(defocus_text);
    parent_node.appendChild(defocus);

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
bool ClassicPrintLens::load_node(QDomDocument& doc, QDomElement& parent_node) {
	Q_UNUSED(doc);
    // Iterate through all elements
    QDomNode n = parent_node.firstChild();
    while (!n.isNull()) {
        // Try to convert the node to an element.
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if (e.tagName() == "Name") {
                m_name = e.text();
            }
            else if (e.tagName() == "Radius") {
                m_radius = e.text().toDouble();
            }
            else if (e.tagName() == "Darkness") {
                m_darkness = e.text().toDouble();
            }
            else if (e.tagName() == "Dodge") {
                m_dodge = e.text().toDouble();
            }
            else if (e.tagName() == "Defocus") {
                m_defocus = e.text().toInt() ? true : false;
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
void ClassicPrintLens::setName(QString name) {
    m_name = name;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the name of the object
**
** @return  Name of the object
**
*/
QString ClassicPrintLens::name() {
    return m_name;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set radius of vignette effect
**
** @param [In] radius   Radius of effect in percent. A lower percent
**                      accentuates the effect
**
*/
void ClassicPrintLens::setRadius(double radius) {
    m_radius = radius;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get radius of vignette effect
**
** @return  Radius of effect in percent
**
*/
double ClassicPrintLens::radius() {
    return m_radius;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set darkening effect of vignette
**
** @param [In] darkness   Darkening effect in percent
**
*/
void ClassicPrintLens::setDarkness(double darkness) {
    m_darkness = darkness;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get darkening effect of vignette
**
** @return  Darkening effect in percent
**
*/
double ClassicPrintLens::darkness() {
    return m_darkness;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set lightening effect of lens centre
**
** @param [In] dodge    Lightening effect in percent
**
*/
void ClassicPrintLens::setDodge(double dodge) {
    m_dodge = dodge;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get lightening effect of lens centre
**
** @return  Lightening effect in percent
**
*/
double ClassicPrintLens::dodge() {
    return m_dodge;
}

//---------------------------------------------------------------------------
/*!
** @brief   Enable/disable defocus of lens perimeter
**
** @param [In] enable   True/False
**
*/
void ClassicPrintLens::setDefocus(bool enable) {
    m_defocus = enable;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get enable/disable state of defocus
**
** @return  True/False
**
*/
bool ClassicPrintLens::defocus() {
    return m_defocus;
}

//---------------------------------------------------------------------------
/*!
** @brief   Progress handler
*/
void ClassicPrintLens::on_progress(int pr, void* context) {
	((ClassicPrintLens*)context)->emit progress(pr);
}
