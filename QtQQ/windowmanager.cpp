#include "windowmanager.h"



// ����ģʽ, ����ȫ�־�̬����
Q_GLOBAL_STATIC(WindowManager, theInstance);

WindowManager::WindowManager()
	: QObject(nullptr)
	, m_talkwindowshell(nullptr)
{
}

WindowManager::~WindowManager()
{
}

WindowManager * WindowManager::getInstance()
{
	return theInstance();
}

QWidget * WindowManager::findWindowName(const QString & qsWindowName)
{
	if (m_windowMap.contains(qsWindowName))
	{
		return m_windowMap.value(qsWindowName);
	}
	return nullptr;
}

void WindowManager::deleteWindowName(const QString & qsWindowName)
{
	if (m_windowMap.contains(qsWindowName))
	{
		m_windowMap.remove(qsWindowName);
	}
}

void WindowManager::addWindowName(const QString & qsWindowName, QWidget * qWidget)
{
	if (!m_windowMap.contains(qsWindowName))
	{
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

void WindowManager::addNewTalkWindow(const QString & uid, GroupType groupType, const QString strPeople)
{
	if (m_talkwindowshell == nullptr)
	{
		m_talkwindowshell = new TalkWindowShell;
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject *obj) {
			m_talkwindowshell = nullptr;
		});
	}

	QWidget * widget = findWindowName(uid);
	if (!widget)
	{
		TalkWindow *talkWindow = new TalkWindow(m_talkwindowshell, uid, groupType);
		TalkWindowItem *talkWindowItem = new TalkWindowItem(talkWindow);

		switch (groupType)
		{
		case COMPANY:
		{
			talkWindow->setWindowName(QStringLiteral("��˾Ⱥ"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("��˾Ⱥ"));
		}
			break;
		case PERSONELGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("���²�"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("���²�"));
		}
			break;
		case DEVELOPMENTGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("�з���"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("�з���"));
		}
			break;
		case MARKETGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("�г���"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("�г���"));
		}
			break;
		case PTOP:
		{
			talkWindow->setWindowName(strPeople);
			talkWindowItem->setMsgLabelContent(strPeople);
		}
			break;
		default:
			break;
		}
		m_talkwindowshell->addTalkWindow(talkWindow, talkWindowItem, groupType);
	}
	else
	{
		// ��������б���Ϊѡ��
		QListWidgetItem * item = m_talkwindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);

		// �����Ҳ൱ǰ���촰��
		m_talkwindowshell->setCurrentWidget(widget);
	}


	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
