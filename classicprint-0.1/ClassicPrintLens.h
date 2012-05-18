/*!
** @file	ClassicPrintLens.h
** 
** @brief	Class that manages lens settings
**  
*/  
#ifndef __classicprintlens__h
#define __classicprintlens__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QImage>
#include <QDomElement>
#include <QObject>

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
class ClassicPrintLens : public QObject {
    Q_OBJECT

public:
    //---------------------------------------------------------------------------
    /*!
    ** @brief   Constructor
    **
    */
    ClassicPrintLens();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Destructor
    **
    */
    ~ClassicPrintLens();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Process an image
    **
    ** @param [In] image    Image to process
    **
    ** @return  True/False
    */
    bool    process(QImage& image);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Save configuration to a node
    **
    ** @param [In] parent_node  Node to save to
    **
    ** @return  True/False
    */
    bool    save_node(QDomDocument& doc, QDomElement& parent_node);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Load configuration from a node
    **
    ** @param [In] parent_node  Node to load from
    **
    ** @return  True/False
    */
    bool    load_node(QDomDocument& doc, QDomElement& parent_node);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set the name of the object
    **
    ** @param [In] name     Name of object
    **
    */
    void    setName(QString name);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get the name of the object
    **
    ** @return  Name of the object
    **
    */
    QString     name();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set radius of vignette effect
    **
    ** @param [In] radius   Radius of effect in percent. A lower percent
    **                      accentuates the effect
    **
    */
    void    setRadius(double radius);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get radius of vignette effect
    **
    ** @return  Radius of effect in percent
    **
    */
    double  radius();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set darkening effect of vignette
    **
    ** @param [In] darkness   Darkening effect in percent
    **
    */
    void    setDarkness(double darkness);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get darkening effect of vignette
    **
    ** @return  Darkening effect in percent
    **
    */
    double  darkness();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set lightening effect of lens centre
    **
    ** @param [In] dodge    Lightening effect in percent
    **
    */
    void    setDodge(double dodge);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get lightening effect of lens centre
    **
    ** @return  Lightening effect in percent
    **
    */
    double  dodge();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Enable/disable defocus of lens perimeter
    **
    ** @param [In] enable   True/False
    **
    */
    void    setDefocus(bool enable);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get enable/disable state of defocus
    **
    ** @return  True/False
    **
    */
    bool    defocus();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   operator=
    */
    ClassicPrintLens& operator=(const ClassicPrintLens& lens);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Progress handler
	*/
	static void on_progress(int pr, void* context);

signals:
    void    progress(int percent);

private:
    // Name of object
    QString     m_name;

    // Radius of vignette effect in percent
    double      m_radius;

    // Darkening effect of vignette in percent
    double      m_darkness;

    // Lightening effect of lens centre in percent
    double      m_dodge;

    // Defocusing effect of lens perimeter
    bool        m_defocus;
};


#endif
