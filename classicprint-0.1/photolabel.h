/*!
** @file	photolabel.h
** 
** @brief	Label object used to display photo preview
**  
*/  
#ifndef __photolabel__h
#define __photolabel__h
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include <QLabel>

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
class PhotoLabel : public QLabel {
    Q_OBJECT

public:
    //---------------------------------------------------------------------------
    /*!
    ** @brief   Constructor
    **
    */
	PhotoLabel(QWidget* parent = NULL);

    //---------------------------------------------------------------------------
    /*!
    ** @brief   Destructor
    **
    */
	~PhotoLabel();

	//---------------------------------------------------------------------------
	/*!
	** @brief   Handle mouse press events
	**
	*/
	virtual void mousePressEvent(QMouseEvent* event);

signals:
	void openPhoto();

};


#endif
