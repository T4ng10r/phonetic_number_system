#ifndef _SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_
#define _SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_
#include <QtGui/QWidget>
#include <boost/scoped_ptr.hpp>

class CSubstituteValuesConfigurationDlgPrivate;

class CSubstituteValuesConfigurationDlg : public QWidget
{
	Q_OBJECT
	friend class SubstituteValuesConfigurationDlgPrivate;
public:
	CSubstituteValuesConfigurationDlg(QWidget * parent = NULL);
	~CSubstituteValuesConfigurationDlg(void);
public slots:
	void onPushButton1ActionTriggered(QAction *);
	void onPushButton2ActionTriggered(QAction *);
protected:
	boost::scoped_ptr<CSubstituteValuesConfigurationDlgPrivate> m_ptrPriv;
};
#endif //_SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_

