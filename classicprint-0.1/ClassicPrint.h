/*!
** @file	ClassicPrint.h
** 
** @brief	Main class for managing classic prints
**  
*/  
#ifndef __classicprint__h
#define __classicprint__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QMap>
#include <QImage>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QMap>

/*--------------------------------------------------------------------------- 
** Defines and Macros 
*/

/*--------------------------------------------------------------------------- 
** Typedefs 
*/ 
class ClassicPrintFilm;
class ClassicPrintLens;
class ClassicPrintProcessing;
 
/*--------------------------------------------------------------------------- 
** Local function prototypes 
*/ 
 
/*--------------------------------------------------------------------------- 
** Data 
*/
class ClassicPrint : public QObject {
    Q_OBJECT

public:
    //---------------------------------------------------------------------------
    /*!
    ** @brief   Constructor
    **
    */
    ClassicPrint();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Destructor
    **
    */
    ~ClassicPrint();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Process a photo
    **
    ** @param[In] photo     Photo to process
    ** @param[In] width     Width of output image. Set to 0 to use original width
    ** @param[In] height    Height of output image. Set to 0 to use original height
    ** @param[out] processed On return contains processed photo
    **
    ** @return True/False
    */
    bool    process(const QImage& photo, int width, int height, QImage& processed);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Save configuration to a file
    **
    ** @param[In] filename  Filename to save to
    **
    ** @return True/False
    */
    bool    save(QString filename);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Load configuration from a file
    **
    ** @param[In] filename  Filename to load from
    **
    ** @return True/False
    */
    bool    load(QString filename);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Add a lens configuration to the camera
    **
    ** @param[In] lens      Lens to add. This class takes ownership of the object
    **
    */
    void    addLens(ClassicPrintLens* lens);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Add a film configuration to the camera
    **
    ** @param[In] film      Film to add. This class takes ownership of the object
    **
    */
    void    addFilm(ClassicPrintFilm* film);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Add a processing type configuration to the camera
    **
    ** @param[In] process   Process to add. This class takes ownership of the object
    **
    */
    void    addProcessing(ClassicPrintProcessing* process);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Delete a lens configuration from the camera
    **
    ** @param[In] name      Name of lens
    **
    */
    void    deleteLens(const QString& name);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Delete a film configuration from the camera
    **
    ** @param[In] name      Name of film
    **
    */
    void    deleteFilm(const QString& name);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Delete a processing type configuration from the camera
    **
    ** @param[In] name      Name of process
    **
    */
    void    deleteProcessing(const QString& name);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get a lens object
    **
    ** @param[In] index     Index of object to get
    ** @param[Out] name     Contains name of object on return
    ** @param[Out] lens     Contains pointer to object on return
    **
    ** @return      TRUE if index is in range else FALSE
    **
    */
    bool    getLens(int index, QString& name, ClassicPrintLens** lens);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get a film object
    **
    ** @param[In] index     Index of object to get
    ** @param[Out] name     Contains name of object on return
    ** @param[Out] lens     Contains pointer to object on return
    **
    ** @return      TRUE if index is in range else FALSE
    **
    */
    bool    getFilm(int index, QString& name, ClassicPrintFilm** film);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get a processing object
    **
    ** @param[In] index     Index of object to get
    ** @param[Out] name     Contains name of object on return
    ** @param[Out] lens     Contains pointer to object on return
    **
    ** @return      TRUE if index is in range else FALSE
    **
    */
    bool    getProcessing(int index, QString& name, ClassicPrintProcessing** processing);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set the current lens
    **
    ** @param[In] name      Name of lens to set as current
    **
    ** @return      True/False
    */
    bool    setCurrentLens(const QString& name);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set the current film
    **
    ** @param[In] name      Name of film to set as current
    **
    ** @return      True/False
    */
    bool    setCurrentFilm(const QString& name);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Set the current processing
    **
    ** @param[In] name      Name of processing to set as current
    **
    ** @return      True/False
    */
    bool    setCurrentProcessing(const QString& name);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get the current lens
    **
    ** @return  Lens or NULL if not set
    */
    ClassicPrintLens* getCurrentLens();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get the current film
    **
    ** @return  Film or NULL if not set
    */
    ClassicPrintFilm* getCurrentFilm();

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Get the current processing
    **
    ** @return  Processing or NULL if not set
    */
    ClassicPrintProcessing* getCurrentProcessing();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get a colour profile by name
	**
	** @return  Colour profile. If the name is not found then a default
	**			colour profile is returned
	*/
	QList<QVariant> getColourProfile(const QString& name);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Load colour profiles from configuration
	*/
	bool loadColourProfiles();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get a colour profile name by index
	**
	** @param[out] profile_name		On return contains the profile name
	**
	** @return  True if successful or false for out of range
	*/
	bool getColourProfileIndex(int index, QString& profile_name);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Set the last folder that was used to load a photo
	**
	** @param[out] folder	Folder that contains the last opened image
	*/
	void setPhotoFolder(const QString& folder);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get the last folder that was used to load a photo
	**
	** @return	Folder that contains the last opened image
	*/
	const QString& photoFolder();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Set the last folder that was used to save
	**
	** @param[out] folder	Folder that contains the last saved image
	*/
	void setSaveFolder(const QString& folder);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get the last folder that was used to save
	**
	** @return	Folder that contains the last saved image
	*/
	const QString& saveFolder();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Set the image width used on last save
	**
	** @param[out] width	Image width
	*/
	void setSaveWidth(int width);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get the image width used on last save
	**
	** @return	Image width
	*/
	int saveWidth();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Set the image height used on last save
	**
	** @param[out] height	Image height
	*/
	void setSaveHeight(int height);

	//---------------------------------------------------------------------------
	/*!
	** @brief   Get the image height used on last save
	**
	** @return	Image height
	*/
	int saveHeight();

        /* Initializes all filters */
        static void init();

signals:
    void    progress(int percent);

private slots:
    void    progress_lens(int percent);
    void    progress_film(int percent);
    void    progress_processing(int percent);

private:
    QMap<QString, ClassicPrintLens*>        m_lenses;
    QMap<QString, ClassicPrintFilm*>        m_films;
    QMap<QString, ClassicPrintProcessing*>  m_processes;

    ClassicPrintLens*                       m_current_lens;
    ClassicPrintFilm*                       m_current_film;
    ClassicPrintProcessing*                 m_current_processing;

	QMap<QString, QList<QVariant> >			m_colour_profiles;

	QString									m_photo_folder;
	QString									m_save_folder;
	int										m_save_width;
	int										m_save_height;
};


#endif
