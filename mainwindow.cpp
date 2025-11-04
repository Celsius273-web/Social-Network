#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <QPixmap>
#include <QFile>
#include <QMessageBox>
#include "network.h"
#include "user.h"
#include "post.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ntw.readUsers("users.txt");
    ntw.readPosts("posts.txt");
    ntw.readImages("images.txt");
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::loginButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::backButtonClicked);
    connect(ui->imageButton, &QPushButton::clicked, this, &MainWindow::imageButtonClicked);
    ui->profileLabel->setVisible(false);
    ui->fSuggest->setVisible(false);//suggest friends
    ui->backButton->setVisible(false);
    ui->fList->setVisible(false);
    ui->labelLabel->setVisible(false);
    ui->imageLin->setVisible(false);
    ui->imageButton->setVisible(false);
    ui->label->setVisible(false);
    connect(ui->fList, &QTableWidget::cellClicked, this, &MainWindow::onFriendClicked);
    connect(ui->fSuggest, &QTableWidget::cellClicked, this, &MainWindow::onSuggestedFriendClicked);
    ntw.readImages("images.txt");


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::loginButtonClicked(){

    logPress = true;
    std::string s = ui->loginLineEdit->text().toStdString();
    if (ntw.getId(s) != -1){
        userlog = ntw.getUser(ntw.getId(s));
        currUser = ntw.getUser(ntw.getId(s));
        inUser = true;
    } else {
        logPress = false;
    }
    page();

}
void MainWindow::backButtonClicked(){
    currUser = userlog;
    page();
}
void MainWindow::imageButtonClicked(){
    std::string s = ui->imageLin->text().toStdString();
    std::cout << s << std::endl;
    ntw.addImage(s, currUser->getId());
    page();
}
void MainWindow::showImage(int id){
    std::string s = ntw.getLinkfromList(id);
    QString imagePath = QString::fromStdString(s);

    // Check if the file exists
    if (QFile::exists(imagePath)) {
        // Load the image
        QPixmap pixmap(imagePath);

        // Set the pixmap to QLabel
        if (!pixmap.isNull()) {
            ui->label->setPixmap(pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio));
        } else {
            QMessageBox::warning(this, "Error", "Failed to load image.");
        }
    } else {
        QMessageBox::warning(this, "Error", "File does not exist at the specified path.");
    }

}
void MainWindow::loadFriendsTable(User* u) {
    ui->fList->setRowCount(0);

    const std::set<int>& friends = u->getFriends();
    ui->fList->setRowCount(friends.size() + 1);

    ui->fList->setColumnCount(1);

    QTableWidgetItem* title = new QTableWidgetItem(QString::fromStdString("Friends"));
    ui->fList->setItem(0, 0, title);
    int row = 1;
    for (int friendId : friends) {
        User* friendUser = ntw.getUser(friendId);
        if (!friendUser) continue;
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(friendUser->getName()));

        ui->fList->setItem(row, 0, nameItem);
        ++row;
    }

    ui->fList->resizeColumnsToContents();
}

void MainWindow::loadSuggestedFriendsTable(User* u) {
    ui->fSuggest->setRowCount(0);
    ui->fSuggest->setColumnCount(1);


    int score;
    const std::vector<int> sfriends = ntw.suggestFriends(u->getId(), score);
    ui->fSuggest->setRowCount(sfriends.size() + 1);

    QTableWidgetItem* title = new QTableWidgetItem(QString::fromStdString("Suggested Friends"));
    ui->fSuggest->setItem(0, 0, title);

    int row = 1;
    for (int friendId : sfriends) {
        User* friendUser = ntw.getUser(friendId);
        if (!friendUser) continue;

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(friendUser->getName()));
        ui->fSuggest->setItem(row, 0, nameItem);
        ++row;
    }

    ui->fSuggest->resizeColumnsToContents();
}

void MainWindow::onFriendClicked(int row, int column) {
    QTableWidgetItem* item = ui->fList->item(row, column);
    if (item) {
        QString friendName = item->text();
        User* friendUser = ntw.getUser(ntw.getId(friendName.toStdString()));
        if (friendUser) {
            currUser = friendUser;
            std::cout << "Selected friend: " << currUser->getName() << std::endl;
            std::cout << "Friend User Info: " << " ID: " << friendUser->getId() << " Name: " << friendUser->getName() <<
            " Year: " << friendUser->getYear() << std::endl;
            page();

        }
    }
}
void MainWindow::onSuggestedFriendClicked(int row, int column) {
    QTableWidgetItem* item = ui->fSuggest->item(row, column);
    if (item) {
        QString friendName = item->text();
        User* friendUser = ntw.getUser(ntw.getId(friendName.toStdString()));
        if (friendUser) {
            ntw.addConnection(friendUser->getName(), userlog->getName());
            if (userlog->isFriend(friendUser->getId()) == true){
                std::cout << "true baby" << std::endl;
            }
            page();

        }
    }
}



void MainWindow::page(){
    ui->label->clear();
    if (logPress == true){

    }

    if (logPress == false){
        inUser = false;
        ui->loginLabel->setVisible(true);
        ui->loginButton->setVisible(true);
        ui->loginLineEdit->setVisible(true);
        ui->backButton->setVisible(false);
        ui->fList->setVisible(false);
        ui->fSuggest->setVisible(false);
        ui->backButton->setVisible(false);
        ui->imageButton->setVisible(false);
        ui->imageLin->setVisible(false);
        ui->label->setVisible(false);
    } if (inUser == true){
        ui->loginLabel->setVisible(false);
        ui->loginButton->setVisible(false);
        ui->loginLineEdit->setVisible(false);
        ui->label->setVisible(true);

        if (currUser->getId() == userlog->getId()){
            ui->profileLabel->setText(QString::fromStdString("My Profile"));
            ui->labelLabel->setText(QString::fromStdString(currUser->getPostsString(5, false)));//posts

        } else {
            ui->labelLabel->setText(QString::fromStdString(currUser->getPostsString(5, true)));//posts
            ui->profileLabel->setText(QString::fromStdString(currUser->getName()) + "'s Profile");
        }
        loadFriendsTable(currUser);
        loadSuggestedFriendsTable(currUser);
        ui->profileLabel->setVisible(true);
        ui->fSuggest->setVisible(true);//suggest friends
        ui->backButton->setVisible(true);
        ui->fList->setVisible(true);
        ui->labelLabel->setVisible(true);
        ui->imageLin->setVisible(true);
        ui->imageButton->setVisible(true);
        showImage(currUser->getId());
        ntw.writeUsers("users.txt");
        ntw.writeImages("images.txt");


    }


}
//"/Users/school/Desktop/CS62/HW8/download.jpg"


