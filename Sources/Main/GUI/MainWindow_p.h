#ifndef _CAF_WINDOW_PRIVATE_INCLUDE_
#define _CAF_WINDOW_PRIVATE_INCLUDE_

#include <GUI/MainWindow.h>
#include <QtGui/QBoxLayout>
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QMenuBar>
#include <QtGui/QLabel>
#include <QtGui/QTableView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeView>
#include <QtCore/QSignalMapper>
#include <boost/scoped_ptr.hpp>

enum { MaxRecentFiles = 5 };

class CSubstituteValuesConfigurationDlg;

class CMainWindowPrivate
{
public:
	CMainWindowPrivate(CMainWindow * ptrPublic);
	~CMainWindowPrivate();
protected:
	void setupUI();
	void setupActions();
	void setConnections();
protected:
	QWidget *					m_ptCentralWidget;
	QVBoxLayout *				m_ptVLayout;
	QToolBar *					m_ptToolBar;

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	QAction *					m_actionNewConf;
	QAction *					m_actionOpenConf;
	QAction *					m_actionSaveConf;
	QAction *					m_actionSaveAsConf;
	QAction *					m_actionRefreshConf;
	QAction *					m_actionProxySettings;
	//////////////////////////////////////////////////////////////////////////
	QMenu *						m_recentFilesMenu;
	QAction *					m_separatorAct;
	QAction *					m_aRecentFileActs[MaxRecentFiles];
	//////////////////////////////////////////////////////////////////////////
	QMutex						m_DataThreadTerminatedMutex;
	void *						ptrDataThread;
	CMainWindow *				m_ptrPublic;
	boost::scoped_ptr<CSubstituteValuesConfigurationDlg>	m_ptrSubstituteConfiguration;
};
#endif //_CAF_WINDOW_PRIVATE_INCLUDE_ 
