/*!
** @file	ClassicPrintProcessing.h
** 
** @brief	Class that manages processing settings
**  
*/  
#ifndef __classicprintprocessing__h
#define __classicprintprocessing__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QImage>
#include <QDomElement>
#include <QObject>
#include <QByteArray>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/
#define RANDOM_LEAK		"Random"

/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
class ClassicPrint;
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/
class ClassicPrintProcessing : public QObject {
    Q_OBJECT

public:
    //---------------------------------------------------------------------------
    /*!
    ** @brief   Constructor
    **
    */
	ClassicPrintProcessing(ClassicPrint* cp);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Destructor
    **
    */
    ~ClassicPrintProcessing();

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
    ** @brief   Set image contrast in percent
    **
    ** @param [In] contrast Contrast in percent
    **
    */
    void    setContrast(double contrast);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get image contrast in percent
    **
    ** @return  Image contrast in percent
    **
    */
    double  contrast();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set colourisation in percent
    **
    ** @param [In] colourisation    Colourisation to apply in percent
    **
    */
	void    setColourisationPercent(double colourisation);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get colourisation in percent
    **
    ** @return  Colourisation in percent
    **
    */
	double  colourisationPercent();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Set colourisation name
	**
	** @param [In] colourisation    Colourisation name
	**
	*/
	void    setColourisation(const QString& colourisation);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get colourisation name
	**
	** @return  Colourisation name
	**
	*/
	const QString& colourisation();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Set light leak file
	**
	** @param [In] colourisation    light leak filename
	**
	*/
	void    setLightLeak(const QString& light_leak);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get light leak file
	**
	** @return  Light leak filename
	**
	*/
	const QString& lightLeak();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get size of frame in percent
	**
	** @return  Size in percent
	**
	*/
	double  frameSizePercent();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Set size of frame in percent
	**
	** @param [In] size		Frame size in percent
	**
	*/
	void    setFrameSizePercent(double size);

	//---------------------------------------------------------------------------
    /*!
    ** @brief   operator=
    */
    ClassicPrintProcessing& operator=(const ClassicPrintProcessing& processing);

signals:
    void    progress(int percent);

private:
    // Name of object
    QString     m_name;

    // Contrast in percent
    double      m_contrast;

    // Colourisation in percent
	double      m_colourisation_percent;

	// Colour profile configuration
	QString		m_colourisation;

	// Light leak filename
	QString		m_light_leak;

	// Frame size as percent of image size
	double		m_frame_size_percent;

	// Parent class
	ClassicPrint*	m_cp;
};


#endif
