#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network.h"
#include "user.h"
#include "post.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void page();
    void loginButtonClicked();
    void backButtonClicked();
    void imageButtonClicked();
    void loadFriendsTable(User* u);
    void loadSuggestedFriendsTable(User* u);
    void onFriendClicked(int row, int column);
    void onSuggestedFriendClicked(int row, int column);
    void showImage(int id);

private:
    Ui::MainWindow *ui;
    Network ntw;
    User* userlog;
    User* currUser;
    bool logPress = false;
    bool inUser = false;

};
#endif // MAINWINDOW_H
