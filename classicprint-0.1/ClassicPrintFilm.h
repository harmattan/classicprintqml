/*!
** @file	ClassicPrintFilm.h
** 
** @brief	Class that manages film settings
**  
*/  
#ifndef __classicprintfilm__h
#define __classicprintfilm__h
 
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
class ClassicPrintFilm : public QObject {
    Q_OBJECT

public:
    //---------------------------------------------------------------------------
    /*!
    ** @brief   Constructor
    **
    */
    ClassicPrintFilm();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Destructor
    **
    */
    ~ClassicPrintFilm();

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
    ** @brief   Set colour temperature in percent. 0% = cold, 100% = warm
    **
    ** @param [In] temperature  Temperature in percent
    **
    */
    void    setTemperature(double temperature);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get colour temperature in percent. 0% = cold, 100% = warm
    **
    ** @return  Temperature in percent
    **
    */
    double  temperature();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set noise in percent
    **
    ** @param [In] noise    Noise value in percent
    **
    */
    void    setNoise(double noise);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get noise value in percent
    **
    ** @return  Noise value in percent
    **
    */
    double  noise();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   operator=
    */
    ClassicPrintFilm& operator=(const ClassicPrintFilm& film);

signals:
    void    progress(int percent);

private:
    // Name of item
    QString     m_name;

    // Colour temperature in percent
    double      m_temperature;

    // Noise value in percent
    double      m_noise;
};


#endif
