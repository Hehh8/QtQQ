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
	void initTimer();								// 初始化计时器
	void initControl();
	void setUserName(const QString &userName);		// 设置用户名
	void setLevelPixmap(int level);					// 设置等级
	void setHeadPixmap(const QString &headPath);	// 设置头像
	void setStatusMenuIcon(const QString &statusPath);	// 设置状态
	// 添加应用部件(app图片路径,app布局对象名)
	QWidget *addOtherAppExtension(const QString &appPath, const QString appName);
	void initContactTree();

private:
	void updateSearchStyle();	// 更新搜索样式

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
