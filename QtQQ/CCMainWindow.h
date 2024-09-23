#pragma once

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class QTreeWidget
{
public:
	QTreeWidget();
	~QTreeWidget();

private:

};

QTreeWidget::QTreeWidget()
{
}

QTreeWidget::~QTreeWidget()
{
}

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

private:
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *obj, QEvent * event);

private slots:
	void onAppIconClicked(QTreeWidgetItem* item, int column);
	void onItemClicked(QTreeWidgetItem* item);
	void onItemExpanded(QTreeWidgetItem* item);
	void onItemCollapsed(QTreeWidgetItem* item, int column);
	void onItemDoubleClicked(QTreeWidgetItem* item);

private:
    Ui::CCMainWindowClass ui;

};
