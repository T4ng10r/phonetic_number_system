#ifndef _CAF_WINDOW_INCLUDE_
#define _CAF_WINDOW_INCLUDE_

#include <QMutex>
#include <QMainWindow>
#include <boost/scoped_ptr.hpp>

class MainWindowPrivate;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
public Q_SLOTS:
	void onShowSettingsConfigureDialog(bool);
protected:
	void closeEvent(QCloseEvent *);
protected:
	boost::scoped_ptr<MainWindowPrivate> _pimpl;
};
#endif //_CAF_WINDOW_INCLUDE_ 
