#include "tests/mt_substitute_values_configuration_dlg.h" 
#include <gui/settings/SubstituteValuesConfigurationDlg.cpp>
#include <data/CSettings_XMLKeywords.h>
//#include <QMetaType>
#include <string>

const unsigned int single_timeout(1000); //in s
const unsigned int timeouts_count(5); //in s
//Q_DECLARE_METATYPE(Qt::Orientation)
//Q_DECLARE_METATYPE(QModelIndex)

class CSubstituteValuesConfigurationDlgTemp: public CSubstituteValuesConfigurationDlg
{
	friend class mt_substitute_values_configuration_dlg;
public:
	CSubstituteValuesConfigurationDlgTemp(CSubstituteValuesConfigurationDlgInit init_data)
		: CSubstituteValuesConfigurationDlg(init_data){}
};

void mt_substitute_values_configuration_dlg::createProperiestConfiguration()
{
  char tabConsonants[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 
    'S', 'T', 'V', 'X', 'Z', 'W'};
  int iCount(sizeof(tabConsonants)/sizeof(char));
  consonants_values_set.put(CONSONANTS_COUNT_KEYWORD,iCount);
  for(int i=0;i<iCount;i++)
  {
    consonants_values_set.add(CONSONANTS_KEYWORD,tabConsonants[i]);
  }
}
void mt_substitute_values_configuration_dlg::createDigitsConfiguration()
{
  systemDigitsConfiguration.reset();
  configurations.clear();
  systemDigitsConfiguration.mSystem[0] = std::make_pair('Z','S');
  systemDigitsConfiguration.mSystem[1] = std::make_pair('T','D');
  systemDigitsConfiguration.mSystem[2] = std::make_pair('N',' ');
  systemDigitsConfiguration.mSystem[3] = std::make_pair('M',' ');
  systemDigitsConfiguration.mSystem[4] = std::make_pair('R',' ');
  systemDigitsConfiguration.mSystem[5] = std::make_pair('L',' ');
  systemDigitsConfiguration.mSystem[6] = std::make_pair('J',' ');
  systemDigitsConfiguration.mSystem[7] = std::make_pair('K','G');
  systemDigitsConfiguration.mSystem[8] = std::make_pair('F','W');
  systemDigitsConfiguration.mSystem[9] = std::make_pair('P','B');

  for(SystemMap::const_iterator iter=systemDigitsConfiguration.mSystem.begin();
    iter!=systemDigitsConfiguration.mSystem.end();iter++)
  {
    systemDigitsConfiguration.allConsonants.push_back(iter->first);
    systemDigitsConfiguration.allConsonants.push_back(iter->second);
  }
  systemDigitsConfiguration.createConsonantsDigitsMap();
  systemDigitsConfiguration.strName="basic";
  configurations.push_back(systemDigitsConfiguration);
}

mt_substitute_values_configuration_dlg::mt_substitute_values_configuration_dlg()
{
  createProperiestConfiguration();
  createDigitsConfiguration();
}
void mt_substitute_values_configuration_dlg::init()
{
  CSubstituteValuesConfigurationDlgInit init_data = 
  { consonants_values_set, configurations, "basic" };

  dialog.reset(new CSubstituteValuesConfigurationDlgTemp(init_data));
}
void mt_substitute_values_configuration_dlg::cleanup()
{
	dialog.reset();  
}
void mt_substitute_values_configuration_dlg::test_NoneInFirstMenusColumnSelected()
{
  CSubstituteValuesConfigurationDlgInit init_data = 
  { consonants_values_set, configurations, "" };

  dialog.reset(new CSubstituteValuesConfigurationDlgTemp(init_data));
	//check if in all FIRST Menus None is selected
	for(unsigned int index=0;index<dialog->_pimpl->m_ptrDigitsEntries.size();index++)
	{
		EntryLine & entry = dialog->_pimpl->m_ptrDigitsEntries[index];
		QVERIFY2(entry.m_ptrConsonantsActions1[0]->isChecked()==true,QString("Entry line for %1 row").arg(index).toLatin1());
		//QCOMPARE(entry.m_ptrConsonantsActions1[0]->isChecked(),true);
		QVERIFY2(entry.m_ptrConsonantButton1->isEnabled()==true,QString("Entry line for %1 row").arg(index).toLatin1());
		QVERIFY2(entry.m_ptrConsonantButton2->isEnabled()==false,QString("Entry line for %1 row").arg(index).toLatin1());
	}

}
void mt_substitute_values_configuration_dlg::test_OtherMenusEntriesDisabled()
{
	unsigned int iDigitEntry(1);
	unsigned int iConsonantEntry(2);
	//choose one of consonants for digit '1' 
	EntryLine & entry = dialog->_pimpl->m_ptrDigitsEntries[iDigitEntry];
	//choose C (3rd after None and B
	entry.m_ptrConsonantsActions1[iConsonantEntry]->trigger();
	//wait till all signal/Q_SLOTS are triggered

	//check if in others digits this Action is deactivated
	for(unsigned int index=0;index<dialog->_pimpl->m_ptrDigitsEntries.size();index++)
	{
		if (index==iDigitEntry)
			continue;
		EntryLine & entry = dialog->_pimpl->m_ptrDigitsEntries[index];
		QCOMPARE(entry.m_ptrConsonantsActions1[iConsonantEntry]->isEnabled(),false);
		QCOMPARE(entry.m_ptrConsonantsActions2[iConsonantEntry]->isEnabled(),false);
	}
}
void mt_substitute_values_configuration_dlg::test_SecondMenusDisabled()
{
	unsigned int iDigitEntry(1);
	unsigned int iConsonantEntry(2);
	//choose one of consonants for digit '1'
	EntryLine & entry = dialog->_pimpl->m_ptrDigitsEntries[iDigitEntry];
	//choose C (3rd after None and B
	entry.m_ptrConsonantsActions1[iConsonantEntry]->trigger();
	QCOMPARE(entry.m_ptrConsonantButton2->isEnabled(),true);
	entry.m_ptrConsonantsActions1[0]->trigger();
	QCOMPARE(entry.m_ptrConsonantButton2->isEnabled(),false);
}
void mt_substitute_values_configuration_dlg::test_SelectNoneConfiguration()
{
	int iCount =0;
	QSignalSpy spy(dialog.get(), SIGNAL(set_selected_consonant_system(const QString &)));
	dialog->_pimpl->m_ptrSystemsCombo->setCurrentIndex(0);
	while (spy.count() == 0)
	{
		QTest::qWait(single_timeout);
		++iCount;
		if (iCount>timeouts_count)
		{
			QVERIFY2(false,"Timeout waiting for set_selected_consonant_system signal");
			return;
		}
	}
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(0).toString(), QString(""));
	for(unsigned int index=0;index<dialog->_pimpl->m_ptrDigitsEntries.size();index++)
	{
		EntryLine & entry = dialog->_pimpl->m_ptrDigitsEntries[index];
		QVERIFY2(entry.m_ptrConsonantsActions1[0]->isChecked()==true,QString("Entry line for %1 row").arg(index).toLatin1());
		QVERIFY2(entry.m_ptrConsonantButton1->isEnabled()==true,QString("Entry line for %1 row").arg(index).toLatin1());
		QVERIFY2(entry.m_ptrConsonantButton2->isEnabled()==false,QString("Entry line for %1 row").arg(index).toLatin1());
	}
}
void mt_substitute_values_configuration_dlg::test_select_none_existing_system()
{
	int iCount =0;
	QSignalSpy spy(dialog.get(), SIGNAL(set_selected_consonant_system(const QString &)));
  dialog->onSystemsActvivated_changeCurrentDigitsSystem("wieloryb");
	while (spy.count() == 0)
	{
		QTest::qWait(single_timeout);
		++iCount;
		if (iCount>timeouts_count)
		{
			QVERIFY2(false,"Timeout waiting for set_selected_consonant_system signal");
			return;
		}
	}
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(0).toString(), QString(""));
}
