#ifndef WINDOW_CHANNELS_H
#define WINDOW_CHANNELS_H

#include <QDialog>
#include <QListWidgetItem>
#include <project/channel.h>

namespace Ui {
class WindowChannels;
}

class WindowChannels : public QDialog
{
    Q_OBJECT

public:
    WindowChannels(QList<Channel> channels = QList<Channel>(), QWidget *parent = 0);
    ~WindowChannels();

    QList<Channel> getChannels() const;

private:
    void setChannelsCount(int count);

private slots:
    void on_channelsCount_valueChanged(int arg1);
    void on_ok_clicked();
    void on_cancel_clicked();

private:
    Ui::WindowChannels *ui;
    int m_channelsCount;
    QList<Channel> m_channels;
    QList<QListWidgetItem*> m_items;
};

#endif // WINDOW_CHANNELS_H
