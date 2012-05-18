/*!
** @file	ClassicPrint.cpp
**
** @brief	Main class for managing classic prints
**
*/

/*--------------------------------------------------------------------------- 
** Includes 
*/
#include "ClassicPrint.h"
#include "ClassicPrintFilm.h"
#include "ClassicPrintLens.h"
#include "ClassicPrintProcessing.h"
#include "utils.h"

/* Filters for ClassicPrint::init() */
#include "LevelsFilter.h"
#include "VignetteFilter.h"
#include "FrameFilter.h"
#include "NoiseFilter.h"
#include "ContrastFilter.h"
#include "ColourLookupFilter.h"
#include "BlendFilter.h"

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QDebug>

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
ClassicPrint::ClassicPrint() {
    m_current_lens = NULL;
    m_current_film = NULL;
    m_current_processing = NULL;
	m_save_width = 0;
	m_save_height = 0;

	// Load the colour profiles
	loadColourProfiles();
}

//---------------------------------------------------------------------------
/*!
** @brief   Destructor
**
*/
ClassicPrint::~ClassicPrint() {
    delete m_current_lens;
    delete m_current_film;
    delete m_current_processing;
}

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
bool ClassicPrint::process(const QImage& photo, int width, int height, QImage& processed) {
    if (!m_current_lens || !m_current_film || !m_current_processing) {
        return false;
    }
    // See if we have to scale the image
    if ((width > 0) && (height > 0)) {
        processed = photo.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    else {
        processed = photo;
    }

    connect(m_current_lens, SIGNAL(progress(int)), this, SLOT(progress_lens(int)));
    connect(m_current_film, SIGNAL(progress(int)), this, SLOT(progress_film(int)));
    connect(m_current_processing, SIGNAL(progress(int)), this, SLOT(progress_processing(int)));

    // Apply the lens first of all
    if (!m_current_lens->process(processed)) {
        qDebug() << "lens failed";
        return false;
    }
    // Then the film
    if (!m_current_film->process(processed)) {
        qDebug() << "film failed";
        return false;
    }
    // And finally the processing
    if (!m_current_processing->process(processed)) {
        qDebug() << "processing failed";
        return false;
    }

    disconnect(m_current_lens, SIGNAL(progress(int)), this, SLOT(progress_lens(int)));
    disconnect(m_current_film, SIGNAL(progress(int)), this, SLOT(progress_film(int)));
    disconnect(m_current_processing, SIGNAL(progress(int)), this, SLOT(progress_processing(int)));

    return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Save configuration to a file
**
** @param[In] filename  Filename to save to
**
** @return True/False
*/
bool ClassicPrint::save(QString filename) {
    QDomDocument    doc("ClassicPrint");

    // Create the root element
    QDomElement     root = doc.createElement("ClassicPrint");
    doc.appendChild(root);

	// Save general config
	if (m_photo_folder.size() > 0) {
		QDomElement	photo_dir = doc.createElement("LastPhotoFolder");
		QDomText	photo_text = doc.createTextNode(m_photo_folder);
		photo_dir.appendChild(photo_text);
		root.appendChild(photo_dir);
	}
	if (m_save_folder.size() > 0) {
		QDomElement	save_dir = doc.createElement("LastSaveFolder");
		QDomText	save_text = doc.createTextNode(m_save_folder);
		save_dir.appendChild(save_text);
		root.appendChild(save_dir);
	}
	if ((m_save_width > 0) && (m_save_height > 0)) {
		QDomElement	save_width = doc.createElement("LastSaveWidth");
		QDomText	save_text = doc.createTextNode(QVariant(m_save_width).toString());
		save_width.appendChild(save_text);
		root.appendChild(save_width);

		QDomElement	save_height = doc.createElement("LastSaveHeight");
		save_text = doc.createTextNode(QVariant(m_save_height).toString());
		save_height.appendChild(save_text);
		root.appendChild(save_height);
	}

    // Save the list of lenses
    QDomElement    lenses = doc.createElement("Lenses");
    root.appendChild(lenses);
    QMap<QString, ClassicPrintLens*>::iterator itlens;
    for (itlens = m_lenses.begin(); itlens != m_lenses.end(); ++itlens) {
        QDomElement lens = doc.createElement("lens");
        lenses.appendChild(lens);
        itlens.value()->save_node(doc, lens);
    }

    // Then the list of films
    QDomElement    films = doc.createElement("Films");
    root.appendChild(films);
    QMap<QString, ClassicPrintFilm*>::iterator itfilm;
    for (itfilm = m_films.begin(); itfilm != m_films.end(); ++itfilm) {
        QDomElement film = doc.createElement("film");
        films.appendChild(film);
        itfilm.value()->save_node(doc, film);
    }

    // And the list of processes
    QDomElement    processes = doc.createElement("Processes");
    root.appendChild(processes);
    QMap<QString, ClassicPrintProcessing*>::iterator itproc;
    for (itproc = m_processes.begin(); itproc != m_processes.end(); ++itproc) {
        QDomElement process = doc.createElement("Processing");
        processes.appendChild(process);
        itproc.value()->save_node(doc, process);
    }

    // Save the current lens, process and film
    if (m_current_lens) {
        QDomElement lens = doc.createElement("CurrentLens");
        root.appendChild(lens);
        m_current_lens->save_node(doc, lens);
    }
    if (m_current_film) {
        QDomElement film = doc.createElement("CurrentFilm");
        root.appendChild(film);
        m_current_film->save_node(doc, film);
    }
    if (m_current_processing) {
        QDomElement process = doc.createElement("CurrentProcessing");
        root.appendChild(process);
        m_current_processing->save_node(doc, process);
    }

    // Write it to the file
    QString xml(doc.toString());
    QFile file(filename);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        return false;
    }
    file.write(xml.toUtf8());

    file.close();

    return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Load configuration from a file
**
** @param[In] filename  Filename to load from
**
** @return True/False
*/
bool ClassicPrint::load(QString filename) {
    QDomDocument doc("ClassicPrint");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    if (!doc.setContent(&file)) {
        return false;
    }

    // Get the root element
    QDomElement root = doc.documentElement();
    if (root.tagName() != "ClassicPrint") {
        return false;
    }
    // Iterate through all elements
    QDomNode n = root.firstChild();
    while (!n.isNull()) {
        // Try to convert the node to an element.
        QDomElement e = n.toElement();
        if(!e.isNull()) {
			if (e.tagName() == "LastPhotoFolder") {
				m_photo_folder = e.text();
			}
			else if (e.tagName() == "LastSaveFolder") {
				m_save_folder = e.text();
			}
			else if (e.tagName() == "LastSaveWidth") {
				m_save_width = e.text().toInt();
			}
			else if (e.tagName() == "LastSaveHeight") {
				m_save_height = e.text().toInt();
			}
			else if (e.tagName() == "Lenses") {
                QDomNode child = e.firstChild();
                while (!child.isNull()) {
                    // Try to convert the node to an element.
                    QDomElement child_elem = child.toElement();
                    if(!child_elem.isNull()) {
                        ClassicPrintLens* lens = new ClassicPrintLens();
                        if (lens->load_node(doc, child_elem)) {
                            addLens(lens);
                        }
                    }
                    child = child.nextSibling();
                }
            }
            else if (e.tagName() == "Films") {
                QDomNode child = e.firstChild();
                while (!child.isNull()) {
                    // Try to convert the node to an element.
                    QDomElement child_elem = child.toElement();
                    if(!child_elem.isNull()) {
                        ClassicPrintFilm* film = new ClassicPrintFilm();
                        if (film->load_node(doc, child_elem)) {
                            addFilm(film);
                        }
                    }
                    child = child.nextSibling();
                }
            }
            else if (e.tagName() == "Processes") {
                QDomNode child = e.firstChild();
                while (!child.isNull()) {
                    // Try to convert the node to an element.
                    QDomElement child_elem = child.toElement();
                    if(!child_elem.isNull()) {
						ClassicPrintProcessing* proc = new ClassicPrintProcessing(this);
                        if (proc->load_node(doc, child_elem)) {
                            addProcessing(proc);
                        }
                    }
                    child = child.nextSibling();
                }
            }
            else if (e.tagName() == "CurrentLens") {
                delete m_current_lens;
                m_current_lens = new ClassicPrintLens();
                if (!m_current_lens->load_node(doc, e)) {
                    delete m_current_lens;
                    m_current_lens = NULL;
                }
            }
            else if (e.tagName() == "CurrentFilm") {
                delete m_current_film;
                m_current_film = new ClassicPrintFilm();
                if (!m_current_film->load_node(doc, e)) {
                    delete m_current_film;
                    m_current_film = NULL;
                }
            }
            else if (e.tagName() == "CurrentProcessing") {
                delete m_current_processing;
				m_current_processing = new ClassicPrintProcessing(this);
                if (!m_current_processing->load_node(doc, e)) {
                    delete m_current_processing;
                    m_current_processing = NULL;
                }
            }
        }
        n = n.nextSibling();
    }

	// If any of the current settings are missing then pick the first saved setting
	if (!m_current_lens && (m_lenses.size() > 0)) {
		setCurrentLens(m_lenses.begin().value()->name());
	}
	if (!m_current_film && (m_films.size() > 0)) {
		setCurrentFilm(m_films.begin().value()->name());
	}
	if (!m_current_processing && (m_processes.size() > 0)) {
		setCurrentProcessing(m_processes.begin().value()->name());
	}

    return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Add a lens configuration to the camera
**
** @param[In] lens      Lens to add. This class takes ownership of the object
**
*/
void ClassicPrint::addLens(ClassicPrintLens* lens) {
    // Delete any previous setting
    QMap<QString, ClassicPrintLens*>::iterator it = m_lenses.find(lens->name());
    if (it != m_lenses.end()) {
        delete it.value();
        m_lenses.erase(it);
    }
    // Store this one
    m_lenses[lens->name()] = lens;
}

//---------------------------------------------------------------------------
/*!
** @brief   Add a film configuration to the camera
**
** @param[In] film      Film to add. This class takes ownership of the object
**
*/
void ClassicPrint::addFilm(ClassicPrintFilm* film) {
    // Delete any previous setting
    QMap<QString, ClassicPrintFilm*>::iterator it = m_films.find(film->name());
    if (it != m_films.end()) {
        delete it.value();
        m_films.erase(it);
    }
    // Store this one
    m_films[film->name()] = film;
}

//---------------------------------------------------------------------------
/*!
** @brief   Add a processing type configuration to the camera
**
** @param[In] process   Process to add. This class takes ownership of the object
**
*/
void ClassicPrint::addProcessing(ClassicPrintProcessing* process) {
    // Delete any previous setting
    QMap<QString, ClassicPrintProcessing*>::iterator it = m_processes.find(process->name());
    if (it != m_processes.end()) {
        delete it.value();
        m_processes.erase(it);
    }
    // Store this one
    m_processes[process->name()] = process;
}

//---------------------------------------------------------------------------
/*!
** @brief   Delete a lens configuration from the camera
**
** @param[In] name      Name of lens
**
*/
void ClassicPrint::deleteLens(const QString& name) {
    QMap<QString, ClassicPrintLens*>::iterator it = m_lenses.find(name);
    if (it != m_lenses.end()) {
        delete it.value();
        m_lenses.erase(it);
    }
}

//---------------------------------------------------------------------------
/*!
** @brief   Delete a film configuration from the camera
**
** @param[In] name      Name of film
**
*/
void ClassicPrint::deleteFilm(const QString& name) {
    QMap<QString, ClassicPrintFilm*>::iterator it = m_films.find(name);
    if (it != m_films.end()) {
        delete it.value();
        m_films.erase(it);
    }
}

//---------------------------------------------------------------------------
/*!
** @brief   Delete a processing type configuration from the camera
**
** @param[In] name      Name of process
**
*/
void ClassicPrint::deleteProcessing(const QString& name) {
    QMap<QString, ClassicPrintProcessing*>::iterator it = m_processes.find(name);
    if (it != m_processes.end()) {
        delete it.value();
        m_processes.erase(it);
    }
}

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
bool ClassicPrint::getLens(int index, QString& name, ClassicPrintLens** lens) {
    QMap<QString, ClassicPrintLens*>::iterator it = m_lenses.begin();
    if (it == m_lenses.end()) {
        return false;
    }
    while (index > 0) {
        ++it;
        if (it == m_lenses.end()) {
            return false;
        }
        --index;
    }
    name = it.key();
    *lens = it.value();
    return true;
}

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
bool ClassicPrint::getFilm(int index, QString& name, ClassicPrintFilm** film) {
    QMap<QString, ClassicPrintFilm*>::iterator it = m_films.begin();
    if (it == m_films.end()) {
        return false;
    }
    while (index > 0) {
        ++it;
        if (it == m_films.end()) {
            return false;
        }
        --index;
    }
    name = it.key();
    *film = it.value();
    return true;
}

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
bool ClassicPrint::getProcessing(int index, QString& name, ClassicPrintProcessing** processing) {
    QMap<QString, ClassicPrintProcessing*>::iterator it = m_processes.begin();
    if (it == m_processes.end()) {
        return false;
    }
    while (index > 0) {
        ++it;
        if (it == m_processes.end()) {
            return false;
        }
        --index;
    }
    name = it.key();
    *processing = it.value();
    return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the current lens
**
** @param[In] name      Name of lens to set as current
**
** @return      True/False
*/
bool ClassicPrint::setCurrentLens(const QString& name) {
    QMap<QString, ClassicPrintLens*>::iterator it = m_lenses.find(name);
    if (it != m_lenses.end()) {
        delete m_current_lens;
        m_current_lens = new ClassicPrintLens();
        *m_current_lens = *it.value();
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the current film
**
** @param[In] name      Name of film to set as current
**
** @return      True/False
*/
bool ClassicPrint::setCurrentFilm(const QString& name) {
    QMap<QString, ClassicPrintFilm*>::iterator it = m_films.find(name);
    if (it != m_films.end()) {
        delete m_current_film;
        m_current_film = new ClassicPrintFilm();
        *m_current_film = *it.value();
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the current processing
**
** @param[In] name      Name of processing to set as current
**
** @return      True/False
*/
bool ClassicPrint::setCurrentProcessing(const QString& name) {
    QMap<QString, ClassicPrintProcessing*>::iterator it = m_processes.find(name);
    if (it != m_processes.end()) {
        delete m_current_processing;
		m_current_processing = new ClassicPrintProcessing(this);
        *m_current_processing = *it.value();
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the current lens
**
** @return  Lens or NULL if not set
*/
ClassicPrintLens* ClassicPrint::getCurrentLens() {
    return m_current_lens;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the current film
**
** @return  Film or NULL if not set
*/
ClassicPrintFilm* ClassicPrint::getCurrentFilm() {
    return m_current_film;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the current processing
**
** @return  Processing or NULL if not set
*/
ClassicPrintProcessing* ClassicPrint::getCurrentProcessing() {
    return m_current_processing;
}

// Progress handling functions. The various functions are weighted
// depending on average how long they take
void ClassicPrint::progress_lens(int percent) {
    emit progress(percent / 8);
}

void ClassicPrint::progress_film(int percent) {
    emit progress(percent / 8 + (100 / 8));
}

void ClassicPrint::progress_processing(int percent) {
    emit progress(percent / 2 + 50);
}

//---------------------------------------------------------------------------
/*!
** @brief   Get a colour profile by name
**
** @return  Colour profile. If the name is not found then a default
**			colour profile is returned
*/
QList<QVariant> ClassicPrint::getColourProfile(const QString& name) {
	QList<QVariant> result;

	QMap< QString, QList< QVariant > >::iterator it;
	it = m_colour_profiles.find(name);
	if (it != m_colour_profiles.end()) {
		return it.value();
	}

	for (int i = 0; i < 256; ++i) {
		result.push_back(QVariant((qlonglong)qRgb(i, i, i)));
	}
	return result;
}

//---------------------------------------------------------------------------
/*!
** @brief   Load colour profiles from configuration
*/
bool ClassicPrint::loadColourProfiles() {
	QDomDocument doc("ColourLevels");
	QFile file(":/classicPrintData/colour_profile/colour_profiles.xml");
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}
	if (!doc.setContent(&file)) {
		return false;
	}

	// Get the root element
	QDomElement root = doc.documentElement();
	if (root.tagName() != "ColourLevels") {
		return false;
	}
	// Iterate through all elements
	QDomNode n = root.firstChild();
	while (!n.isNull()) {
		// Try to convert the node to an element.
		QDomElement e = n.toElement();
		if (!e.isNull()) {
			if (e.tagName() == "Levels") {
				QString			name;
				QList<QVariant> levels;
				QString			red;
				QString			green;
				QString			blue;

				QDomNode child = e.firstChild();
				while (!child.isNull()) {
					// Try to convert the node to an element.
					QDomElement child_elem = child.toElement();
					if (!child_elem.isNull()) {
						if (child_elem.tagName() == "Name") {
							name = child_elem.text();
						}
						else if (child_elem.tagName() == "Red") {
							red = child_elem.text();
						}
						else if (child_elem.tagName() == "Green") {
							green = child_elem.text();
						}
						else if (child_elem.tagName() == "Blue") {
							blue = child_elem.text();
						}
					}
					child = child.nextSibling();
				}
				if (!name.isNull() && !red.isNull() && !green.isNull() && !blue.isNull()) {
					// Convert the strings to byte arrays and then on to the list
					QByteArray ba_red, ba_green, ba_blue;
					ba_red = stringToByteArray(red);
					ba_green = stringToByteArray(green);
					ba_blue = stringToByteArray(blue);
					if ((ba_red.size() == 256) &&
						(ba_green.size() == 256) &&
						(ba_blue.size() == 256)) {
						// Still OK if here
						for (int i = 0; i < 256; ++i) {
							levels.push_back((qlonglong)qRgb(ba_red[i], ba_green[i], ba_blue[i]));
						}
						m_colour_profiles[name] = levels;
					}
				}
			}
		}
		n = n.nextSibling();
	}

	return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get a colour profile name by index
**
** @param[out] profile_name		On return contains the profile name
**
** @return  True if successful or false for out of range
*/
bool ClassicPrint::getColourProfileIndex(int index, QString& profile_name) {
	QMap< QString, QList< QVariant > >::iterator it = m_colour_profiles.begin();
	while (index > 0) {
		++it;
		if (it == m_colour_profiles.end()) {
			break;
		}
		--index;
	}
	if (it == m_colour_profiles.end()) {
		return false;
	}
	profile_name = it.key();
	return true;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the last folder that was used to load a photo
**
** @param[out] folder	Folder that contains the last opened image
*/
void ClassicPrint::setPhotoFolder(const QString& folder) {
	m_photo_folder = folder;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the last folder that was used to load a photo
**
** @return	Folder that contains the last opened image
*/
const QString& ClassicPrint::photoFolder() {
	return m_photo_folder;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the last folder that was used to save
**
** @param[out] folder	Folder that contains the last saved image
*/
void ClassicPrint::setSaveFolder(const QString& folder) {
	m_save_folder = folder;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the last folder that was used to save
**
** @return	Folder that contains the last saved image
*/
const QString& ClassicPrint::saveFolder() {
	return m_save_folder;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the image width used on last save
**
** @param[out] width	Image width
*/
void ClassicPrint::setSaveWidth(int width) {
	m_save_width = width;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the image width used on last save
**
** @return	Image width
*/
int ClassicPrint::saveWidth() {
	return m_save_width;
}

//---------------------------------------------------------------------------
/*!
** @brief   Set the image height used on last save
**
** @param[out] height	Image height
*/
void ClassicPrint::setSaveHeight(int height) {
	m_save_height = height;
}

//---------------------------------------------------------------------------
/*!
** @brief   Get the image height used on last save
**
** @return	Image height
*/
int ClassicPrint::saveHeight() {
	return m_save_height;
}

void ClassicPrint::init() {
        REGISTER_LEVELS_FILTER;
        REGISTER_VIGNETTE_FILTER;
        REGISTER_FRAME_FILTER;
        REGISTER_NOISE_FILTER;
        REGISTER_CONTRAST_FILTER;
        REGISTER_COLOUR_LOOKUP_FILTER;
        REGISTER_BLEND_FILTER;
}

