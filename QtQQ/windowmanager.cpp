#include "windowmanager.h"



// 单例模式, 创建全局静态对象
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
			talkWindow->setWindowName(QStringLiteral("公司群"));
			talkWindow->setMsgLabelContent(QStringLiteral("公司群"));
		}
			break;
		case PERSONELGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("人事部"));
			talkWindow->setMsgLabelContent(QStringLiteral("人事部"));
		}
			break;
		case DEVELOPMENTGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("研发部"));
			talkWindow->setMsgLabelContent(QStringLiteral("研发部"));
		}
			break;
		case MARKETGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("市场部"));
			talkWindow->setMsgLabelContent(QStringLiteral("市场部"));
		}
			break;
		case PTOP:
		{
			talkWindow->setWindowName(QStringLiteral("公司群"));
			talkWindow->setMsgLabelContent(QStringLiteral("公司群"));
		}
			break;
		default:
			break;
		}
	}
	else
	{
		m_talkwindowshell->setCurrentWidget(widget);
	}

	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
