#pragma once

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class QTreeWidgetItem;

extern QString gLoginEmployeeID;

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = nullptr);
	virtual ~CCMainWindow();

public:
	void initTimer();								// ��ʼ����ʱ��
	void initControl();
	void setUserName(const QString &userName);		// �����û���
	void setLevelPixmap(int level);					// ���õȼ�
	void setHeadPixmap(const QString &headPath);	// ����ͷ��
	void setStatusMenuIcon(const QString &statusPath);	// ����״̬
	// ���Ӧ�ò���(appͼƬ·��,app���ֶ�����)
	QWidget *addOtherAppExtension(const QString &appPath, const QString appName);
	void initContactTree();

private:
	void updateSearchStyle();	// ����������ʽ
	void addGroupDeps(QTreeWidgetItem *pRootGroupItem, int depID);

private:
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *obj, QEvent * event);
	void mousePressEvent(QMouseEvent *event);

private slots:
	void onAppIconClicked();
	void onItemClicked(QTreeWidgetItem* item, int column);
	void onItemExpanded(QTreeWidgetItem* item);
	void onItemCollapsed(QTreeWidgetItem* item);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
    Ui::CCMainWindowClass ui;
	QMap<QTreeWidgetItem*, QString> m_groupMap;		// ���з���ķ�����

};
