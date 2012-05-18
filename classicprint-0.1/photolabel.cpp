/*!
** @file	photolabel.cpp
** 
** @brief	Label object used to display photo preview
**  
*/  
 
/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "photolabel.h"
#include <QMouseEvent>

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
PhotoLabel::PhotoLabel(QWidget* parent) : QLabel(parent) {
	setMouseTracking(true);
}

//---------------------------------------------------------------------------
/*!
** @brief   Destructor
**
*/
PhotoLabel::~PhotoLabel() {
}

//---------------------------------------------------------------------------
/*!
** @brief   Handle mouse press events
**
*/
void PhotoLabel::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		event->accept();
		emit openPhoto();
	}
}
